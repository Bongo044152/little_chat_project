// queue for MT-Safe
#pragma once

// -- headers for thread -- //
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

// queue
#include <queue>

// optional
#include <optional>

// queue for MT-safe
/**
 * @brief Thread-safe queue version
 * 
 * NOTE: Always call shutdown() or rely on destructor to wake up waiting threads.
 */
template<typename T>
class Queue
{
public:
    // -- constructor and destructor -- //

    // Construct a new Queue object.
    Queue() = default;

    // Destruct the Queue object and ensure waiting threads are awakened. ( for safe quit )
    /// NOTE: Calls shutdown() to notify all waiting threads.
    ~Queue() { shutdown(); }

    // -- copy trait -- //

    // Disable copy construction to prevent copying of mutex and condition variable.
    Queue(const Queue &other) = delete;

    // -- move trait -- //

    // Disable move construction due to mutex.
    Queue(Queue&&) = delete;

    // Disable move assignment due to mutex.
    Queue& operator=(Queue&&) = delete;

    // -- basic getter method -- //

    /**
     * @brief Pop a value from the queue, blocking until an element is available or shutdown is called.
     *
     * @param[out] out_ Reference to store the popped value.
     * @return true if a value was popped, false if shutdown be called.
     */
    bool pop(T &out_);

    /**
     * @brief Pop a value from the queue, blocking up to the specified timeout.
     *
     * @tparam Rep_  Representation type for duration (e.g., int).
     * @tparam Period_ Period type for duration (e.g., std::milli).
     * @param timeout_  Maximum duration to wait for an element.
     * @param[out] out_ Reference to store the popped value if available.
     * @return true if a value was popped within timeout, false on timeout or shutdown.
     */
    template<typename Rep_, typename Period_>
    bool pop_for(const std::chrono::duration<Rep_, Period_> &timeout_, T &out_);

    /**
     * @brief Try to pop a value without blocking.
     *
     * @param[out] out_ Reference to store the popped value if available.
     * @return true if a value was popped, false if queue is empty or shutdown.
     */
    bool try_pop(T &out_);

    /**
     * @brief Push a new value into the queue and notify one waiting thread.
     *
     * @param item_ The value to be pushed into the queue.
     * 
     * NOTE: Not blocking; always fast path.
     */
    void push(const T &item_);

    // -- shutdown -- //

    /**
     * @brief Signal all waiting threads to wake up and stop processing.
     *
     * NOTE: You should call this function for exit the thread gracefully.
     */
    void shutdown();

private:
    std::mutex mtx_;               // muti-thread lock : prevent race condition
    std::queue<T> my_queue_;       // queue from standard library
    std::condition_variable cv_;   // for conditional wait behavior
    bool stop_ = false;            // flag for safe quit
};

// -- impl part -- //

template <typename T>
inline bool Queue<T>::pop(T &out_)
{
    std::unique_lock<std::mutex> lk(mtx_);
    cv_.wait(lk, [this]{ return stop_ || !my_queue_.empty(); });

    if (stop_) return false; // for safe quit

    out_ = std::move(my_queue_.front());
    my_queue_.pop();
    return true;
}

template <typename T>
template<typename Rep_, typename Period_>
inline bool Queue<T>::pop_for(const std::chrono::duration<Rep_, Period_> &timeout_, T &out_)
{
    std::unique_lock<std::mutex> lk(mtx_);
    if (!cv_.wait_for(lk, timeout_, [this] { return stop_ || !my_queue_.empty(); }))
        return false;  // timeout
    
    if (stop_) return false; // for safe quit

    out_ = std::move(my_queue_.front());
    my_queue_.pop();
    return true;
}

template <typename T>
inline bool Queue<T>::try_pop(T &out_)
{
    std::lock_guard<std::mutex> lk(mtx_);
    if (stop_ || my_queue_.empty()) return false; // for safe quit
    out_ = std::move(my_queue_.front());
    my_queue_.pop();
    return true;
}

template <typename T>
inline void Queue<T>::push(const T &item_)
{
    std::lock_guard<std::mutex> lock(mtx_);
    my_queue_.push(item_);
    cv_.notify_one();
}

template <typename T>
inline void Queue<T>::shutdown()
{
    std::lock_guard<std::mutex> lk(mtx_);
    stop_ = true;
    cv_.notify_all();
}