#include <iostream>
#include <cstddef>
#include <iterator>
#include <memory>

template<size_t N>
struct StackStorage {
    alignas(std::max_align_t) uint8_t data[N];
    uint8_t* map[N];
    uint8_t* begin = data;

    StackStorage() noexcept {
        for (size_t i = 0; i < N; ++i)
            map[i] = nullptr;
    };

    StackStorage(const StackStorage<N>&) = delete;
    StackStorage<N>& operator=(const StackStorage<N>&) = delete;
    
    size_t pos() const noexcept {
        return begin - data;
    }
};

template<typename T, size_t N>
class StackAllocator {
public:
    StackStorage<N>* storage;

public:
    using value_type = T;
    using type = T;

    StackAllocator() noexcept;

    StackAllocator(StackStorage<N>& storage) noexcept: storage(&storage) {}

    template<class T_>
    StackAllocator(const StackAllocator<T_, N>& source) noexcept: storage(source.storage) {}

    template<class T_>
    StackAllocator<T, N>& operator=(const StackAllocator<T_, N>& source) noexcept {
        storage = source.storage;
    }

    T* allocate(size_t n) {
        auto delta = (alignof(T) - (storage->pos()) % alignof(T)) % alignof(T);
        storage->map[storage->pos() + delta] = storage->map[storage->pos()];
        storage->map[storage->pos()] = nullptr;
        storage->begin += delta;
        auto res = reinterpret_cast<T*>(storage->begin);
        storage->begin += n * sizeof(T);
        return res;
    }

    void deallocate(T*& ptr, size_t n) noexcept {
        if (ptr + n == reinterpret_cast<T*>(storage->begin)) {
            storage->begin -= n * sizeof(T);
            uint8_t* new_begin;
            while ((new_begin = storage->map[storage->pos()])) {
                storage->map[storage->pos()] = nullptr;
                storage->begin = new_begin;
            }
        } else {
            storage->map[reinterpret_cast<uint8_t*>(ptr) - storage->data + n] = reinterpret_cast<uint8_t*>(ptr);
        }
        ptr = nullptr;
    }

    template<class U>
    struct rebind {
        using other = StackAllocator<U, N>;
    };
};

template<class T, class U, size_t N>
bool operator==(const StackAllocator<T, N>& left, const StackAllocator<U, N>& right) {
    return left.storage == right.storage;
}

template<class T, class U, size_t N>
bool operator!=(const StackAllocator<T, N>& left, const StackAllocator<U, N>& right) {
    return !(left == right);
}