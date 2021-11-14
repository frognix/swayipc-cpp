#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace swayipc {

template <class T>
class safe_queue {
public:
    safe_queue() = default;
    safe_queue(safe_queue&& other) {
        std::unique_lock lock(other.m_qlock);
        m_queue = std::move(other.m_queue);
    }
    safe_queue& operator=(safe_queue&& other) {
        if (this != &other) {
            std::unique_lock lock(other.m_qlock);
            m_queue = std::move(other.m_queue);
        }
        return *this;
    }
    safe_queue(const safe_queue&) = delete;
    safe_queue& operator=(const safe_queue&) = delete;
    bool empty() const;
    void push(T value);
    T pop();
private:
    bool unsafe_empty() const;

    std::queue<T> m_queue;
    mutable std::mutex m_qlock;
    mutable std::condition_variable m_qcheck;
};

template <class T>
bool safe_queue<T>::empty() const {
    std::unique_lock lock(m_qlock);
    return unsafe_empty();
}

template <class T>
void safe_queue<T>::push(T value) {
    std::unique_lock lock(m_qlock);
    m_queue.push(value);
    m_qcheck.notify_one();
}

template <class T>
T safe_queue<T>::pop() {
    std::unique_lock lock(m_qlock);
    while(unsafe_empty())
        m_qcheck.wait(lock);

    auto tmp = m_queue.front();
    m_queue.pop();
    return tmp;
}

template <class T>
bool safe_queue<T>::unsafe_empty() const {
    return m_queue.empty();
}

}
