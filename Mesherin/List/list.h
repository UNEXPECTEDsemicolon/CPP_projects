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


template<class T, class Allocator = std::allocator<T>>
class List {
private:
    struct Node {
        Node* prev = this;
        Node* next = this;
        T* value = nullptr;

        Node() = default;
        Node(const Node&) = default;
        Node& operator=(const Node&) = default;
    };
public:
    template<class T_>
    class base_iterator: public std::iterator<std::bidirectional_iterator_tag, T_> {
    private:
        Node *node;

    public:
        base_iterator(Node* node) noexcept: node(node) {}

        template<class T__, typename = std::enable_if_t<std::is_same<T__, T>::value && std::is_same<T_, const T>::value>>
        base_iterator(const base_iterator<T__>& source) noexcept: node(source.node) {};

        template<class T__>
        base_iterator(const base_iterator<T_>& source) noexcept: node(source.node) {};

        void swap(base_iterator<T_>& right) noexcept {
            std::swap(node, right.node);
        }

        template<class T__, typename = std::enable_if_t<std::is_same<T__, T>::value && std::is_same<T_, const T>::value>>
        base_iterator<T_>& operator=(const base_iterator<T__>& source) noexcept {
            auto tmp = source;
            swap(tmp);
            return *this;
        }

        template<class T__>
        base_iterator<T_>& operator=(const base_iterator<T_>& source) noexcept {
            auto tmp = source;
            swap(tmp);
            return *this;
        }

        bool operator==(const base_iterator<T_>& right) const noexcept {
            return node == right.node;
        }

        bool operator!=(const base_iterator<T_>& right) const noexcept {
            return !(*this == right);
        }

        base_iterator<T_>& operator++() noexcept {
            node = node->next;
            return *this;
        }

        base_iterator<T_> operator++(int) noexcept {
            auto res = *this;
            ++*this;
            return res;
        }

        base_iterator<T_>& operator--() noexcept {
            node = node->prev;
            return *this;
        }

        base_iterator<T_> operator--(int) noexcept {
            auto res = *this;
            --*this;
            return res;
        }

        base_iterator<T_>& operator+=(size_t val) noexcept {
            for (size_t i = 0; i < val; ++i)
                ++(*this);
            return *this;
        }

        base_iterator<T_> operator+(size_t val) const noexcept {
            auto res = *this;
            res += val;
            return res;
        }

        base_iterator<T_>& operator-=(size_t val) noexcept {
            for (size_t i = 0; i < val; ++i)
                --(*this);
            return *this;
        }

        base_iterator<T_> operator-(size_t val) const noexcept {
            auto res = *this;
            res -= val;
            return res;
        }

        size_t operator-(const base_iterator<T_>& right) const noexcept {
            size_t res = 0;
            for (auto it = right; *it != node; ++res, ++it)
            return res;
        }

        T_& operator*() noexcept {
            return *(node->value);
        }

        const T_& operator*() const noexcept {
            return *(node->value);
        }

        T_* operator->() noexcept {
            return &(this->operator*());
        }

        const T_* operator->() const noexcept {
            return &(this->operator*());
        }
        friend std::conditional_t<std::is_same<T_,const T>::value,
                                  base_iterator<T>,
                                  base_iterator<const T>>;
        friend List<T, Allocator>;
    };

    using iterator = base_iterator<T>;
    using const_iterator = base_iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    using AllocTraitsT = std::allocator_traits<Allocator>;
    using AllocatorNode = typename AllocTraitsT::template rebind_alloc<Node>;
    using AllocTraitsNode = std::allocator_traits<AllocatorNode>;
    size_t size_ = 0;
    Allocator alloc;
    AllocatorNode alloc_node = alloc;
    Node* end_ = nullptr;

public:
    iterator begin() noexcept {
        return iterator(end_->next);
    }
    const_iterator cbegin() const noexcept {
        return const_iterator(end_->next);
    }
    const_iterator begin() const noexcept {
        return cbegin();
    }

    iterator end() noexcept {
        return iterator(end_);
    }
    const_iterator cend() const noexcept {
        return const_iterator(end_);
    }
    const_iterator end() const noexcept {
        return cend();
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }
    const_reverse_iterator rbegin() const noexcept {
        return crbegin();
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }
    const_reverse_iterator rend() const noexcept {
        return crend();
    }

private:
    template<typename... Args>
    void emplace_insert(const const_iterator& it, Args&... args) {
        auto new_node = AllocTraitsNode::allocate(alloc_node, 1);
        try {
            AllocTraitsNode::construct(alloc_node, new_node, Node());
        } catch (...) {
            AllocTraitsNode::deallocate(alloc_node, new_node, 1);
            throw;
        }
        new_node->value = AllocTraitsT::allocate(alloc, 1);
        try {
            AllocTraitsT::construct(alloc, new_node->value, args...);
        } catch (...) {
            AllocTraitsT::deallocate(alloc, new_node->value, 1);
            AllocTraitsNode::destroy(alloc_node, new_node);
            AllocTraitsNode::deallocate(alloc_node, new_node, 1);
            throw;
        }
        if (it.node == nullptr) {
            throw 0;
        }
        new_node->next = it.node;
        new_node->prev = it.node->prev;
        it.node->prev->next = new_node;
        it.node->prev = new_node;
        ++size_;
    }

public:
    void insert(const const_iterator& it, const T& value) {
        emplace_insert(it, value);
    }

private:
    template<typename... Args>
    void emplace_push_back(Args&... args) {
        emplace_insert(end(), args...);
    }

    template<typename... Args>
    void emplace_push_front(Args&... args) {
        emplace_insert(end() + 1, args...);
    }

public:
    void push_back(const T& value) {
        insert(end(), value);
    }

    void push_front(const T& value) {
        insert(end() + 1, value);
    }

    void erase(const const_iterator& it) noexcept {
        it.node->prev->next = it.node->next;
        it.node->next->prev = it.node->prev;
        AllocTraitsT::destroy(alloc, it.node->value);
        AllocTraitsT::deallocate(alloc, it.node->value, 1);
        AllocTraitsNode::destroy(alloc_node, it.node);
        AllocTraitsNode::deallocate(alloc_node, it.node, 1);
        --size_;
    }

    void pop_back() noexcept {
        erase(end() - 1);
    }

    void pop_front() noexcept {
        erase(end() + 1);
    }

private:
    void delete_list() noexcept {
        if (end_ == nullptr) return;
        auto cpy_size_ = size_;
        for (size_t i = 0; i < cpy_size_; ++i) {
            pop_back();
        }
        AllocTraitsNode::destroy(alloc_node, end_);
        AllocTraitsNode::deallocate(alloc_node, end_, 1);
        end_ = nullptr;
    }

public:
    ~List() noexcept {
        delete_list();
    }

    List(const Allocator& alloc = Allocator()): alloc(alloc) {
        end_ = AllocTraitsNode::allocate(alloc_node, 1);
        AllocTraitsNode::construct(alloc_node, end_, Node{end_, end_, nullptr});
    }

    List(size_t count, const T& value, const Allocator& alloc = Allocator()): List(alloc) {
        for (size_t i = 0; i < count; ++i) {
            try {
                push_back(value);
            } catch (...) {
                delete_list();
            }
        }
    }

    List(size_t count, const Allocator& alloc = Allocator()): List(alloc) {
        for (size_t i = 0; i < count; ++i) {
            try {
                emplace_push_back();
            } catch (...) {
                delete_list();
            }
        }
    }

private:
    List(const List& source, bool pocca)
        : List(pocca ? Allocator(source.alloc) : AllocTraitsT::select_on_container_copy_construction(source.alloc)) {
        for (auto it = source.begin(); it != source.end(); ++it) {
            try {
                push_back(*it);
            } catch (...) {
                delete_list();
            }
        }
    }

public:
    List(const List& source): List(source, false) {}

    void swap(List& right) noexcept {
        std::swap(alloc, right.alloc);
        std::swap(alloc_node, right.alloc_node);
        std::swap(size_, right.size_);
        std::swap(end_, right.end_);
    }

    List& operator=(const List& source) {
        auto tmp = List<T, Allocator>(source, AllocTraitsT::propagate_on_container_copy_assignment::value);
        swap(tmp);
        return *this;
    }

    const Allocator& get_allocator() const noexcept {
        return alloc;
    }

    size_t size() const noexcept {
        return size_;
    }
};