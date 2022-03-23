#include <iostream>
#include <cstddef>
#include <iterator>
#include <memory>

template<size_t N>
struct StackStorage {
    alignas(std::max_align_t) uint8_t data[N];
    uint8_t* map[N];
    uint8_t* begin = data;

    StackStorage() = default;

    StackStorage(const StackStorage<N>&) = delete;
    StackStorage<N>& operator=(const StackStorage<N>&) = delete;
    
    size_t pos() {
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

    StackAllocator();

    StackAllocator(StackStorage<N>& storage): storage(&storage) {}

    template<class T_>
    StackAllocator(const StackAllocator<T_, N>& source): storage(source.storage) {}

    template<class T_>
    StackAllocator<T, N>& operator=(const StackAllocator<T_, N>& source) {
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

    void deallocate(T* ptr, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            (ptr + i)->~T();
        }
        if (ptr + n == reinterpret_cast<T*>(storage->begin)) {
            storage->begin -= n * sizeof(T);
            uint8_t* new_begin;
            while ((new_begin = storage->map[storage->pos()])) {
                storage->map[storage->pos()] == nullptr;
                storage->begin = new_begin;
            }
        } else {
            storage->map[reinterpret_cast<uint8_t*>(ptr) - storage->data + n] = reinterpret_cast<uint8_t*>(ptr);
        }
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

        void swap(Node& right) {
            std::swap(prev, right.prev);
            std::swap(next, right.next);
            std::swap(value, right.value);
        }
    };
public:
    template<class Node_t, class T_>
    class base_iterator: public std::iterator<std::bidirectional_iterator_tag, T> {
    private:
        Node_t *node;

    public:
        base_iterator(Node_t* node) noexcept: node(node) {}

        base_iterator(const base_iterator<Node, T>& source) noexcept: node(source.node) {}

        base_iterator(const base_iterator<const Node, const T>& source) noexcept: node(source.node) {}

        void swap(base_iterator<Node_t, T_>& right) noexcept {
            std::swap(node, right.node);
        }

        base_iterator<Node_t, T_>& operator=(const base_iterator<Node_t, T_>& source) noexcept {
            auto tmp = source;
            swap(tmp);
            return *this;
        }

        bool operator==(const base_iterator<Node_t, T_>& right) const noexcept {
            return node == right.node;
        }

        bool operator!=(const base_iterator<Node_t, T_>& right) const noexcept {
            return !(*this == right);
        }

        base_iterator<Node_t, T_>& operator++() noexcept {
            node = node->next;
            return *this;
        }

        base_iterator<Node_t, T_> operator++(int) noexcept {
            auto res = *this;
            ++*this;
            return res;
        }

        base_iterator<Node_t, T_>& operator--() noexcept {
            node = node->prev;
            return *this;
        }

        base_iterator<Node_t, T_> operator--(int) noexcept {
            auto res = *this;
            --*this;
            return res;
        }

        base_iterator<Node_t, T_>& operator+=(ssize_t val) noexcept {
            for (size_t i = 0; i < val; ++i)
                ++(*this);
            return *this;
        }

        base_iterator<Node_t, T_> operator+(ssize_t val) const noexcept {
            auto res = *this;
            res += val;
            return res;
        }

        base_iterator<Node_t, T_>& operator-=(ssize_t val) noexcept {
            for (size_t i = 0; i < val; ++i)
                --(*this);
            return *this;
        }

        base_iterator<Node_t, T_> operator-(ssize_t val) const noexcept {
            auto res = *this;
            res -= val;
            return res;
        }

        size_t operator-(const base_iterator<Node_t, T_>& right) const noexcept {
            size_t res = 0;
            for (auto it = right; *it != node; ++res, ++it)
            return res;
        }

        T_& operator*() noexcept {
            return *(node->value);
//            return dynamic_cast<
//                std::conditional<std::is_same<Node_t, const BaseNode>::value, Node*, const Node*>>(node)->value;
        }

        const T_& operator*() const noexcept {
            return *(node->value);
//            return dynamic_cast<
//                std::conditional<std::is_same<Node_t, const BaseNode>::value, Node*, const Node*>>(node)->value;
        }

        T_* operator->() noexcept {
            return &(this->operator*());
        }

        const T_* operator->() const noexcept {
            return &(this->operator*());
        }
        friend std::conditional_t<std::is_same<Node_t,
                                  const Node>::value, base_iterator<Node, T>,
                                  base_iterator<const Node, const T>>;
        friend List<T, Allocator>;
    };

    using iterator = base_iterator<Node, T>;
    using const_iterator = base_iterator<const Node, const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
//    using AllocatorBaseNode = typename std::allocator_traits<Allocator>::template rebind<BaseNode>::other;
    using AllocTraitsT = std::allocator_traits<Allocator>;
    using AllocatorNode = typename AllocTraitsT::template rebind_alloc<Node>;
//    using BaseAllocTraits = std::allocator_traits<AllocatorBaseNode>;
    using AllocTraitsNode = std::allocator_traits<AllocatorNode>;
    size_t size_ = 0;
//    AllocatorBaseNode base_alloc;
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

    void insert(const iterator& it, const T& value) {
        auto new_node = AllocTraitsNode::allocate(alloc_node, 1);
        try {
            AllocTraitsNode::construct(alloc_node, new_node, Node()); // TODO: Does value copy?
        } catch (...) {
            AllocTraitsNode::deallocate(alloc_node, new_node, 1);
            throw;
        }
        new_node->value = AllocTraitsT::allocate(alloc, 1);
        try {
            AllocTraitsT::construct(alloc, new_node->value, value);
        } catch (...) {
            AllocTraitsT::deallocate(alloc, new_node->value, 1);
            AllocTraitsNode::deallocate(alloc_node, new_node, 1);
            throw;
        }
        new_node->next = it.node;
        new_node->prev = it.node->prev;
        it.node->prev->next = new_node;
        it.node->prev = new_node;
        ++size_;
    }

    void push_back(const T& value) {
        insert(end(), value);
    }

    void push_front(const T& value) {
        insert(end() + 1, value);
    }

    void erase(const iterator& it) noexcept {
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
        for (size_t i = 0; i < size_; ++i) {
            pop_back();
        }
    }

public:
    ~List() noexcept {
        delete_list();
        AllocTraitsNode::destroy(alloc_node, end_);
        AllocTraitsNode::deallocate(alloc_node, end_, 1);
    }

    List(const Allocator& alloc = Allocator()): alloc(alloc) {
        end_ = AllocTraitsNode::allocate(alloc_node, 1);
        AllocTraitsNode::construct(alloc_node, end_, Node{end_, end_, nullptr});
    }

    List(size_t count, const T& value = T(), const Allocator& alloc = Allocator()): List(alloc) {
        size_ = count;
        for (size_t i = 0; i < count; ++i) {
            try {
                push_back(value);
            } catch (...) {
                delete_list();
            }
        }
    }

    List(size_t count, const Allocator& alloc): List(count, T(), alloc) {} // TODO: making odd copy?

    List(const List& source): List(source.alloc) {
        size_ = source.size_;
        for (auto it = source.begin(); it != source.end(); ++it) {
            try {
                push_back(*it);
            } catch (...) {
                delete_list();
            }
        }
    }

    void swap(List& right) noexcept {
        std::swap(alloc, right.alloc);
        std::swap(size_, right.size_);
        end_->swap(*(right.end_));
    }

    List& operator=(const List& source) {
        auto tmp = source;
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