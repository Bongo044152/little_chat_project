// impl for client.hpp
#include "server/server.hpp"

#include <sstream>
#include <cstring>

#ifdef _WIN32

//------------------------------------------------------------------------------
// ServerSocket Implementation
//------------------------------------------------------------------------------

ServerSocket::ServerSocket(SOCKET connect_socket,
                        std::shared_ptr<std::condition_variable> cv,
                        int message_buffer_len)
    : ConnectSocket_(connect_socket),
        cv_(std::move(cv)),
        message_buffer_len_(message_buffer_len)
{
    if (message_buffer_len_ > 10240) {
        throw std::invalid_argument("message buffer size must be <= 10240");
    }
    if (ConnectSocket_ == INVALID_SOCKET) {
        throw std::invalid_argument("Invalid SOCKET provided");
    }
    if (!cv_) {
        throw std::invalid_argument("Condition variable pointer is null");
    }

    state.store(State::Connection);

    // Launch receive thread immediately
    recv_thread_ = std::thread([this]{ this->_recv_func_async(); });
}

ServerSocket::~ServerSocket()
{
    _shutdown();  // Ensure thread and socket are closed on destruction
}

void ServerSocket::send_message(const std::string &message) const
{
    if (state.load() != State::Connection) return;
    if (message.size() > message_buffer_len_) {
        // TODO: logging here
        throw std::runtime_error("message too large!");
    }

    std::vector<char> buffer(message_buffer_len_, 0);
    std::memcpy(buffer.data(), message.data(), message.size());

    int iResult = send(ConnectSocket_, buffer.data(), message_buffer_len_, 0);
    if (iResult == SOCKET_ERROR) {
        std::stringstream oss;
        oss << "[Error] send failed with error: " << WSAGetLastError();
        // TODO: logging here
        throw std::runtime_error(oss.str());
    }
}

void ServerSocket::_shutdown()
{
    if (state.load() == State::DisConnection) return;
    state.store(State::DisConnection);
    if (ConnectSocket_ != INVALID_SOCKET) {
        closesocket(ConnectSocket_);        // force shutdown recv function
        ConnectSocket_ = INVALID_SOCKET;    // Prevent misuse
    }
    if (recv_thread_.joinable()) {
        recv_thread_.join();
    }
}

void ServerSocket::_recv_func_async()
{
    while (state.load() == State::Connection) {
        std::vector<char> buffer(message_buffer_len_);
        int iResult = recv(ConnectSocket_, buffer.data(), message_buffer_len_, 0);

        if (iResult > 0) {
            // TODO: handle incoming event here
            // ! dummy for test
            send_message(buffer.data());
        }
        else if (iResult == 0) {
            // Connection closed by client
            // TODO: handle graceful disconnect here
            // ? use callback to notify Server of disconnection ?
        }
        else {
            int err = WSAGetLastError();
            if (state.load() != State::Connection) break;
            std::stringstream oss;
            oss << "[Error] recv failed with error: " << err;
            // TODO: logging here
        }
    }

    // Clean-up after loop exit
    // 教授提供意見: 讓 thread 自己 stop 順便 dispose 會不會好處理一點?
    state.store(State::DisConnection);
    if (ConnectSocket_ != INVALID_SOCKET) {
        closesocket(ConnectSocket_);
        ConnectSocket_ = INVALID_SOCKET;    // Prevent misuse
    }
    cv_->notify_one();  // Notify Server that a slot is now free ( notify Server::_aceept )
}

//------------------------------------------------------------------------------
// Server Implementation
//------------------------------------------------------------------------------

Server::Server(const std::string &server_ip,
            const std::string &server_port,
            int max_connections,
            int message_buffer_len)
    : server_ip_(server_ip),
    server_port_(server_port),
    max_connections_(max_connections),
    message_buffer_len_(message_buffer_len)
{
    if (_init()) {
        throw std::runtime_error("Initialization failed");
    }
}

Server::~Server()
{
    shutdown();  // Ensure clean shutdown
}

void Server::run()
{
    // avoid called twice
    if (is_run_called || is_shutdown_called) return;
    is_run_called = true;

    // add thread
    accept_thread_ = std::thread([this]{ _accept(); });
}

void Server::shutdown()
{
    // avoid called twice
    if (is_shutdown_called && !is_run_called) return;
    is_shutdown_called = true;

    stop_.store(true);
    cv_->notify_all();

    if (ListenSocket_ != INVALID_SOCKET) {
        closesocket(ListenSocket_);
        ListenSocket_ = INVALID_SOCKET; // Prevent misuse
    }

    // Shutdown each client handler
    for (auto &ptr : ConnectSockets_) {
        ptr->_shutdown();
    }
    ConnectSockets_.clear();

    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }

    WSACleanup();
}

const ServerSocket& Server::get_server_sock(size_t i) const
{
    if (i >= ConnectSockets_.size()) {
        throw std::out_of_range("Client index out of range");
    }
    return *ConnectSockets_[i];
}

void Server::_accept()
{
    std::unique_lock<std::mutex> lock(mtu_);
    while (!stop_.load()) {
        // Block when at capacity until a client disconnects
        if (ConnectSockets_.size() >= static_cast<size_t>(max_connections_)) {
            cv_->wait(lock, [this]{
                if (stop_.load()) return true;
                for (const auto &ptr : ConnectSockets_) {
                    if (ptr->get_state() == ServerSocket::State::DisConnection) {
                        return true;
                    }
                }
                return ConnectSockets_.size() < static_cast<size_t>(max_connections_);
            });
        }
        if (stop_.load()) return;

        SOCKET ClientSocket = accept(ListenSocket_, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            // TODO: logging here
            continue;
        }

        auto server_sock = std::make_unique<ServerSocket>(ClientSocket, cv_, message_buffer_len_);

        bool reused = false;
        for (auto &ptr : ConnectSockets_) {
            if (ptr->get_state() == ServerSocket::State::DisConnection) {
                ptr = std::move(server_sock);
                reused = true;
                break;
            }
        }
        if (!reused) {
            ConnectSockets_.push_back(std::move(server_sock));
        }
    }
}

bool Server::_init()
{
    // Enable UTF-8 support for console I/O
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    struct addrinfo *result = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP socket
    hints.ai_protocol = IPPROTO_TCP;    // TCP protocol
    hints.ai_flags = AI_PASSIVE;        // For binding

    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData_);
    if (iResult != 0) {
        // TODO: logging here
        return true;
    }

    iResult = getaddrinfo(server_ip_.c_str(), server_port_.c_str(), &hints, &result);
    if (iResult != 0) {
        // TODO: logging here
        return true;
    }

    ListenSocket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket_ == INVALID_SOCKET) {
        // TODO: logging here
        freeaddrinfo(result);
        return true;
    }

    iResult = bind(ListenSocket_, result->ai_addr, static_cast<int>(result->ai_addrlen));
    if (iResult == SOCKET_ERROR) {
        // TODO: logging here
        freeaddrinfo(result);
        return true;
    }
    freeaddrinfo(result);

    iResult = listen(ListenSocket_, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        // TODO: logging here
        return true;
    }

    // TODO: logging here (e.g., server start info)
    return false;
}

#endif // _WIN32
