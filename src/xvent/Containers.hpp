#pragma once

#include <mutex>

#include <unordered_map>
#include <vector>

namespace xvent {

template <typename T>
class AtomicVector {
public:
    explicit AtomicVector()
        : m_mutex(std::make_shared<std::mutex>()) {
    }

    AtomicVector(const AtomicVector<T>& rhs)
        : m_data(rhs.m_data)
        , m_mutex(std::make_shared<std::mutex>()) {
    }

    AtomicVector<T> operator==(const AtomicVector<T>& rhs) {
        if (*this == rhs)
            return *this;

        m_data = rhs.data;
        m_mutex = std::make_shared<std::mutex>();

        return *this;
    }

    auto lock() {
        return std::lock_guard{ *m_mutex };
    }

    const auto begin() const {
        return m_data.cbegin();
    }

    const auto end() const {
        return m_data.cend();
    }

    auto begin() {
        return m_data.begin();
    }

    auto end() {
        return m_data.end();
    }

    void clear() {
        std::lock_guard guard{ *m_mutex };
        m_data.clear();
    }

	void clearUnsafe() {
		m_data.clear();
	}

    void push_back(const T& value) {
        std::lock_guard guard{ *m_mutex };
        m_data.push_back(value);
    }

    template <class InputIt>
    void insert(std::vector<T>::iterator pos, InputIt first, InputIt last) {
        std::lock_guard guard{ *m_mutex };
        m_data.insert(pos, first, last);
    }

    T& operator[](int pos) {
        std::lock_guard guard{ *m_mutex };
        return m_data[pos];
    }

    T operator[](int pos) const {
        std::lock_guard guard{ *m_mutex };
        return m_data[pos];
    }

    auto size() const {
        std::lock_guard guard{ *m_mutex };
        return m_data.size();
    }

    auto front() {
        std::lock_guard guard{ *m_mutex };
        return m_data.front();
    }

private:
    std::shared_ptr<std::mutex> m_mutex = std::make_shared<std::mutex>();
    std::vector<T> m_data;
};

template <typename K, typename T>
class AtomicUnorderedMap {
public:
    explicit AtomicUnorderedMap()
        : m_mutex(std::make_shared<std::mutex>()) {
    }

    AtomicUnorderedMap(const AtomicUnorderedMap<K, T>& rhs)
        : m_data(rhs.m_data)
        , m_mutex(std::make_shared<std::mutex>()) {
    }

    AtomicUnorderedMap<K, T> operator==(const AtomicUnorderedMap<K, T>& rhs) {
        if (*this == rhs)
            return *this;

        m_data = rhs.m_data;
        m_mutex = std::make_shared<std::mutex>();

        return *this;
    }

    auto lock() {
        return std::lock_guard{ *m_mutex };
    }

    auto begin() {
        return m_data.begin();
    }

    auto end() {
        return m_data.end();
    }

    bool contains(const K& key) const {
        std::lock_guard guard{ *m_mutex };
        return m_data.contains(key);
    }

    void clear() {
        std::lock_guard guard{ *m_mutex };
        m_data.clear();
    }

    T& operator[](const K& key) {
        std::lock_guard guard{ *m_mutex };
        return m_data[key];
    }

    T& operator[](K&& key) {
        std::lock_guard guard{ *m_mutex };
        return m_data[key];
    }

    void erase(const K& key) {
        std::lock_guard guard{ *m_mutex };
        m_data.erase(key);
    }

    auto size() const {
        std::lock_guard guard{ *m_mutex };
        return m_data.size();
    }

private:
    std::shared_ptr<std::mutex> m_mutex;
    std::unordered_map<K, T> m_data;
};
}
