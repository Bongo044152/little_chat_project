// impl for client.hpp
#include "client.hpp"

#ifdef _WIN32

#include <iostream>
#include <sstream>
#include <cstring>

// Implementation of CilentSocket methods

CilentSocket::CilentSocket(const std::string &server_ip,
                            const std::string &server_port,
                            int message_buffer_len = 1024)
    : server_ip_(server_ip), server_port_(server_port), message_buffer_len_(message_buffer_len)
{
    // invalid configuration
    if (message_buffer_len_ > 10240) {
        throw std::runtime_error("message_buffer_len_ must be less than 10240");
    }
    if (!_init()) {
        throw std::runtime_error("CilentSocket initialization failed");
    }
}

CilentSocket::~CilentSocket() {
    if (recv_thread_.joinable())
        stop();
}

void CilentSocket::send_message(const std::string &message) {
    // message payload check
    if (static_cast<int>(message.size()) > message_buffer_len_) {
        throw std::runtime_error("message too large!");
    }

    // Prepare fixed-size buffer (padded with zeros)
    std::vector<char> buffer(message_buffer_len_);
    std::memcpy(buffer.data(), message.data(), message.size());

    // send message via socket
    int iResult = send(ConnectSocket_, buffer.data(), message_buffer_len_, 0);
    if (iResult == SOCKET_ERROR) {
        std::stringstream oss;
        oss << "[Error] send failed with error: " << WSAGetLastError();
        throw std::runtime_error(oss.str());
    }
}

Queue<std::string>& CilentSocket::get_message_queue() {
    return q_;
}

void CilentSocket::register_after_send(std::function<void()> cb) {
    after_send_callbacks_.push_back(std::move(cb));
}

void CilentSocket::register_after_receive(std::function<void(const std::string&)> cb) {
    after_receive_callbacks_.push_back(std::move(cb));
}

void CilentSocket::run() {
    // Launch background receive thread
    recv_thread_ = std::thread([this]{ this->_recv_func_async(); });
}

void CilentSocket::stop() {
    // signal thread to stop
    stop_.store(true);
    
    // close the socket to unblock recv
    if (ConnectSocket_ != INVALID_SOCKET) {
        closesocket(ConnectSocket_);
    }

    // notify queue no more data
    q_.shutdown();

    // wait for thread to finish
    if (recv_thread_.joinable())
        recv_thread_.join();
    
    // cleanup Winsock
    WSACleanup();
}

void CilentSocket::_recv_func_async() {
    try {
        while (!stop_.load()) {
            // vector auto-manages char buffer
            std::vector<char> buffer(message_buffer_len_);
            int iResult = recv(ConnectSocket_, buffer.data(), message_buffer_len_, 0);

            if (iResult > 0) {
                // push received data into queue
                q_.push(std::string(buffer.data(), iResult));
                std::string message = buffer.data();
                for(size_t i=0; i<after_receive_callbacks_.size(); ++i) {
                    after_receive_callbacks_[i](message);
                }
            }
            else if (iResult == 0) {
                // connection closed gracefully
                q_.push("[Info] Connection closed by server");
                break;
            }
            else {
                int err = WSAGetLastError();
                if (stop_.load()) break;

                std::stringstream oss;
                oss << "[Error] recv failed with error: " << err;
                q_.push(oss.str()); // cannot throw in thread
                break;
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        q_.push(std::string("[Error] ") + e.what());
    }
}

bool CilentSocket::_init() {
    // enable UTF-8 for console output (Chinese/emoji support)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        return false;
    }

    // setup address hints
    struct addrinfo hints{};
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo *result = nullptr;
    // Resolve server address and port
    iResult = getaddrinfo(server_ip_.c_str(), server_port_.c_str(), &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return false;
    }

    // Create socket
    ConnectSocket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnectSocket_ == INVALID_SOCKET) {
        std::cerr << "socket creation failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    // Connect to server
    iResult = connect(ConnectSocket_, result->ai_addr, static_cast<int>(result->ai_addrlen));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket_);
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    freeaddrinfo(result);
    return true;
}

#endif // _WIN32