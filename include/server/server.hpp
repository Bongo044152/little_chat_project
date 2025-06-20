/**
 * @file server.hpp / server.cpp
 * @brief Implements a simple threaded TCP server and per-client socket handler.
 *
 * @note Need to link against Ws2_32.lib on Windows
 */

#pragma once

#ifdef _WIN32

#include <string>
#include <vector>
#include <memory>               // for std::shared_ptr, std::unique_ptr
#include <stdexcept>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

// windows 的技術債
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN    // Exclude rarely-used Windows headers to reduce compile time
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

/**
 * @class ServerSocket
 * @brief Manages communication with a single connected client socket.
 * 
 * Encapsulates the receive thread, send operations, and state management.
 */
class ServerSocket
{
public:
    /**
     * @enum State
     * @brief Connection state of this socket thread.
     */
    enum class State {
        Connection,    ///< Client is connected and messages can be sent/received
        DisConnection  ///< Client has disconnected, thread should stop
    };

    /**
     * @brief Construct a ServerSocket for an accepted raw socket.
     * @param connect_socket Underlying SOCKET returned by accept().
     * @param cv Shared condition variable to notify the Server when this client disconnects.
     * @param message_buffer_len Maximum buffer length for send/recv operations (default 1024).
     * @throws std::invalid_argument if parameters are invalid.
     */
    ServerSocket(SOCKET connect_socket,
                std::shared_ptr<std::condition_variable> cv,
                int message_buffer_len = 1024);

    /**
     * @brief Clean up resources, signal thread shutdown.
     */
    ~ServerSocket();

    /**
     * @brief Send a fixed-size message to the connected client.
     * @param message The payload to send; will be zero-padded or truncated to buffer size.
     * @throws std::runtime_error if send fails or message is too large.
     */
    void send_message(const std::string &message) const;

    /// @name Accessors
    ///@{
    const std::string& get_username() const { return username_; }
    State get_state() const { return state.load(); }
    ///@}

    // -- disable copy trait -- //
    ServerSocket(const ServerSocket &) = delete;
    ServerSocket& operator=(const ServerSocket &) = delete;

    // -- disable move trait -- //
    ServerSocket(ServerSocket &&) = delete;
    ServerSocket& operator=(ServerSocket &&) = delete;

    friend class Server;

private:
    std::atomic<State> state{State::Connection};  ///< state flag for thread control (MT-safe)
    int message_buffer_len_;                      ///< Maximum payload size for send/recv
    std::string username_;                        ///< Placeholder username (unused currently)

    SOCKET ConnectSocket_{INVALID_SOCKET};        ///< Underlying WinSock socket
    std::thread recv_thread_;                     ///< Worker thread for receiving events
    std::shared_ptr<std::condition_variable> cv_; ///< Notify Server when disconnect occurs

    /**
     * @brief Internal receive loop running in a separate thread.
     * Blocks on recv(), handles incoming data and disconnect events.
     * TODO: handle incoming events and dispatch callbacks here.
     */
    void _recv_func_async();

    /**
     * @brief Cleanly shut down this socket and join the receive thread.
     * Not to be called by external users; used by Server for cleanup.
     */
    void _shutdown();
};


/**
 * @class Server
 * @brief Listens for incoming connections and dispatches them to a pool of ServerSocket handlers.
 */
class Server
{
public:
    /**
     * @brief Construct the listening server.
     * @param server_ip IP address to bind (e.g., "0.0.0.0").
     * @param server_port Port number as string (e.g., "5090").
     * @param max_connections Maximum concurrent clients (default 3).
     * @param message_buffer_len Buffer size for client messaging (default 1024).
     */
    Server(const std::string &server_ip,
            const std::string &server_port,
            int max_connections = 3,
            int message_buffer_len = 1024);

    /**
     * @brief Clean up server, shutdown threads and sockets.
     */
    ~Server();

    /**
     * @brief Start accepting connections. Should be called once.
     * 
     * @note should be called manually, otherwise thread will not be started.
     */
    void run();

    /**
     * @brief Signal shutdown and wait for cleanup. Safe to call multiple times.
     */
    void shutdown();

    // -- getter -- //

    /**
     * @brief Access a connected ServerSocket by index.
     * @param i Index in the client list.
     * @return Reference to the ServerSocket.
     * @throws std::out_of_range if i is invalid.
     */
    const ServerSocket& get_server_sock(size_t i) const;

    // -- disable copy trait -- //
    Server(const Server &) = delete;
    Server& operator=(const Server &) = delete;

    // -- disable move trait -- //
    Server(Server &&) = delete;
    Server& operator=(Server &&) = delete;

private:
    std::atomic<bool> stop_{false};   ///< Flag to stop the accept loop (MT-safe)

    std::string server_ip_;           ///< Listening IP
    std::string server_port_;         ///< Listening port
    int message_buffer_len_;          ///< Buffer size for send/recv
    int max_connections_;             ///< Maximum simultaneous clients

    std::thread accept_thread_;       ///< Thread running _accept()
    std::shared_ptr<std::condition_variable> cv_{std::make_shared<std::condition_variable>()};
    std::mutex mtu_;                  ///< Mutex for condition_variable waits
    bool is_run_called{false};        ///< Prevent multiple run() calls
    bool is_shutdown_called{false};   ///< Prevent multiple shutdown calls

    WSADATA wsaData_;                 ///< WinSock initialization data
    SOCKET ListenSocket_{INVALID_SOCKET};  ///< Listening socket handle
    std::vector<std::unique_ptr<ServerSocket>> ConnectSockets_; ///< Active client handlers

    /**
     * @brief Accept loop for incoming connections and manage client slots.
     * Blocks when max_connections_ reached until a client disconnects.
     */
    void _accept();

    /**
     * @brief Initialize WinSock, address info, bind, and listen.
     * @return true on error, false on success.
     */
    bool _init();
};

#endif // _WIN32
