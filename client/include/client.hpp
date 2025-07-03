#pragma once

/**
 * @file cilent.hpp
 * @brief Declaration of the CilentSocket class for Windows-based socket client
 * communication. Provides asynchronous receive functionality and thread-safe
 * message queuing.
 *
 * @note  Need to link with Ws2_32.lib, Mswsock.lib, AdvApi32.lib
 */

// only support for Windows ( Linux may be add later )
#ifdef _WIN32

// -- Standard headers -- //
#include <string>
#include <vector>

#include <atomic>
#include <thread>

#include <functional>

#include "thread_safe_queue.hpp"  // thread-safe Queue<T>

// windows 的技術債
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // WIN32_LEAN_AND_MEAN

// Windows-specific headers
#include <winsock2.h>
#include <ws2tcpip.h>

/**
 * @class CilentSocket
 * @brief A simple Windows-based TCP client socket with asynchronous receive
 * thread.
 *
 * Manages a background thread to receive messages into a thread-safe queue.
 * Allows sending fixed-size messages and clean shutdown.
 *
 * @note You must manually call run() after construction to start the receive
 * thread.
 */
class CilentSocket
{
public:
    // -- constructor and destructor -- //

    /**
     * @brief Constructor
     * @param server_ip       IPv4 or IPv6 address of the server
     * @param server_port     Service name or port number as string
     * @param message_buffer_len  Fixed buffer size for send/recv (max 10240)
     * @throws std::runtime_error if buffer size > 10240 or socket init fails
     */
    CilentSocket(const std::string &server_ip,
                 const std::string &server_port,
                 int message_buffer_len);

    /**
     * @brief Destructor: ensures clean shutdown of the receive thread and
     * socket.
     */
    ~CilentSocket();

    // -- thread controller -- //

    /**
     * @brief Start the background receive thread.
     */
    void run();

    /**
     * @brief Stop the receive thread, close socket, cleanup Winsock.
     */
    void stop();

    // -- send and get message -- //

    /**
     * @brief Send a message to the server.
     * @param message  Payload to send (padded or truncated to buffer size)
     * @throws std::runtime_error on send failure or if message too large
     */
    void send_message(const std::string &message);

    /**
     * @brief Access the thread-safe queue of received messages.
     * @return Reference to the internal Queue<std::string>
     */
    Queue<std::string> &get_message_queue();

    // -- Register various callbacks -- //

    /// @brief Register a callback to be called after sending a message.
    /// @param cb : callback function
    void register_after_send(std::function<void()> cb);


    /// @brief Register a callback to be called after receiving a message.
    /// @param cb : callback function
    void register_after_receive(std::function<void(const std::string &)> cb);

    // getter ( function vector )

    std::vector<std::function<void()>> &get_after_send_functions()
    {
        return after_send_callbacks_;
    }
    std::vector<std::function<void(const std::string &)>> &
    get_after_recive_functions()
    {
        return after_receive_callbacks_;
    }

    // -- disable copy and move trait -- //

    CilentSocket(const CilentSocket &) = delete;
    CilentSocket &operator=(const CilentSocket &) = delete;
    CilentSocket(CilentSocket &&) = delete;
    CilentSocket &operator=(CilentSocket &&) = delete;

private:
    // -- member variables -- //

    // variable ( MT-safe )
    Queue<std::string> q_;           // Queue for incoming messages ( MT-safe )
    std::atomic<bool> stop_{false};  // bool for stopping the thread ( MT-safe )

    // Server connection parameters
    std::string server_ip_;    // server's ip
    std::string server_port_;  // server's open port
    int message_buffer_len_;   // message/string buffer size ( both send and
                               // receive )

    // Receive thread handle
    std::thread recv_thread_;

    // Winsock data and socket handle
    WSADATA wsaData_;
    SOCKET ConnectSocket_{INVALID_SOCKET};

    // callback function
    std::vector<std::function<void()>> after_send_callbacks_;
    std::vector<std::function<void(const std::string &)>>
        after_receive_callbacks_;
    // std::vector<std::function<void(const std::string&)>>
    // before_send_callbacks_; std::vector<std::function<void()>>
    // before_receive_callbacks_;

    // -- helper function -- //

    /**
     * @brief Internal initialization: Sets up Winsock, resolves address,
     * connects socket.
     * @return true on success, false on failure (check via exceptions or logs)
     */
    bool _init();

    /**
     * @brief Background thread function: receives data and pushes into queue.
     */
    void _recv_func_async();
};

#endif  // _WIN32