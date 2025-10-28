#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class BoundedBuffer {
public:
    explicit BoundedBuffer(size_t capacity) : capacity_(capacity) {}

    void push_from_buffer1(const T& item) {
        std::unique_lock<std::mutex> lock(mx);
        not_full_.wait(lock, [this]() { return buffer.size() < capacity_; });
        buffer.push(item);
        not_empty_.notify_one();
    }

    void push_from_buffer2(const T& item) {
        std::unique_lock<std::mutex> lock(mx);
        not_full_.wait(lock, [this]() { return buffer.size() < capacity_; });
        buffer.push(item);
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mx);
        not_empty_.wait(lock, [this]() { return !buffer.empty(); });
        T item = buffer.front();
        buffer.pop();
        not_full_.notify_one();
        return item;
    }

private:
    std::queue<T> buffer;
    size_t capacity_;

    std::mutex mx;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

int main() {
    BoundedBuffer<int> buffer(5);
    return 0;
}
