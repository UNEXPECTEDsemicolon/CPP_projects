#include <iostream>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>
#include <cmath>


template<size_t N>
struct StackStorage {
    alignas(std::max_align_t) uint8_t data[N];
    uint8_t* begin = data;

    StackStorage() noexcept {};

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

    StackAllocator() = delete;

    StackAllocator(StackStorage<N>& storage) noexcept: storage(&storage) {}

    template<class T_>
    StackAllocator(const StackAllocator<T_, N>& source) noexcept: storage(source.storage) {}

    template<class T_>
    StackAllocator(StackAllocator<T_, N>&& source) noexcept: storage(source.storage) {
        source.storage = nullptr;
    }

    template<class T_>
    StackAllocator<T, N>& operator=(const StackAllocator<T_, N>& source) noexcept {
        storage = source.storage;
    }

    T* allocate(size_t n) {
        auto delta = (alignof(T) - (storage->pos()) % alignof(T)) % alignof(T);
        storage->begin += delta;
        auto res = reinterpret_cast<T*>(storage->begin);
        storage->begin += n * sizeof(T);
        return res;
    }

    void deallocate(T*& ptr, size_t n) noexcept {
        if (ptr + n == reinterpret_cast<T*>(storage->begin))
            storage->begin -= n * sizeof(T);
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

        Node(Node* const& prev, Node* const& next, T* const& value) noexcept:
                prev(prev), next(next), value(value) {}

        Node(const Node&) = default;
        //        Node(Node&&) noexcept = default;

        Node(Node&& source) noexcept: Node(source) { // TODO: test default move constructor
            source.prev = nullptr;
            source.next = nullptr;
            source.value = nullptr;
        }

        Node& operator=(const Node&) = default;
        //        Node& operator=(Node&&) noexcept = default;

        Node& operator=(Node&& source) noexcept {
            *this = source;
            source.prev = nullptr;
            source.next = nullptr;
            source.value = nullptr;
            return *this;
        }


    };

public:
    template<class T_>
    class base_iterator : public std::iterator<std::bidirectional_iterator_tag, T_> {
    private:
        Node* node;

        base_iterator(Node* node) noexcept: node(node) {}

    public: // TODO ?

        template<class T__, typename = std::enable_if_t<
                std::is_same<T__, T>::value && std::is_same<T_, const T>::value>>
        base_iterator(const base_iterator<T__>& source) noexcept: node(source.node) {};

        template<class T__>
        base_iterator(const base_iterator<T_>& source) noexcept: node(source.node) {};

        void swap(base_iterator<T_>& right) noexcept {
            std::swap(node, right.node);
        }

        template<class T__, typename = std::enable_if_t<
                std::is_same<T__, T>::value && std::is_same<T_, const T>::value>>
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

        template<typename T__>
        bool operator==(const base_iterator<T__>& right) const noexcept {
            return node == right.node;
        }
        template<typename T__>
        bool operator!=(const base_iterator<T__>& right) const noexcept {
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

        friend std::conditional_t<std::is_same<T_, const T>::value,
                base_iterator<T>,
                base_iterator<const T>>;
        friend List<T, Allocator>;
    };

public:
    using iterator = base_iterator<T>;
    using const_iterator = base_iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    using AllocTraitsT = std::allocator_traits<Allocator>;
    using AllocatorNode = typename AllocTraitsT::template rebind_alloc<Node>;
    using AllocTraitsNode = std::allocator_traits<AllocatorNode>;
    Allocator alloc;
    AllocatorNode alloc_node = alloc;
    Node* end_ = nullptr;
    size_t size_ = 0;

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

    template<typename... Args>
    void emplace(const const_iterator& it, Args&&... args) {
        auto new_node = AllocTraitsNode::allocate(alloc_node, 1);
        try {
            AllocTraitsNode::construct(alloc_node, new_node, Node());
        } catch (...) {
            AllocTraitsNode::deallocate(alloc_node, new_node, 1);
            throw;
        }
        new_node->value = AllocTraitsT::allocate(alloc, 1);
        try {
            AllocTraitsT::construct(alloc, new_node->value, std::forward<Args>(args)...);
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

    void insert(const const_iterator& it, const T& value) {
        emplace(it, value);
    }
    void insert(const const_iterator& it, T&& value) noexcept {
        emplace(it, std::move(value));
    }

    template<typename InputIt>
    void insert(const const_iterator& it, InputIt first, InputIt last) {
        auto begin_ = it - 1;
        for (auto iit = first; iit != last; ++iit) {
            try {
                insert(it, *iit);
            } catch (...) {
                for (auto rit = it - 1; rit != begin_; --rit)
                    erase(rit);
                throw;
            }
        }
    }

    template<typename... Args>
    void emplace_push_back(Args&& ... args) {
        emplace(end(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void emplace_push_front(Args&& ... args) {
        emplace(end() + 1, std::forward<Args>(args)...);
    }

    void push_back(const T& value) {
        emplace_push_back(value);
    }
    void push_back(T&& value) noexcept {
        emplace_push_back(std::move(value));
    }

    void push_front(const T& value) {
        emplace_push_front(value);
    }
    void push_front(T&& value) noexcept {
        emplace_push_front(std::move(value));
    }

    void erase(const const_iterator& it) noexcept {
        if (it == end()) return;
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

    List(const Allocator& alloc = Allocator()) : alloc(alloc) {
        end_ = AllocTraitsNode::allocate(alloc_node, 1);
        AllocTraitsNode::construct(alloc_node, end_, Node(end_, end_, nullptr));
    }

    List(size_t count, const T& value, const Allocator& alloc = Allocator()) : List(alloc) {
        for (size_t i = 0; i < count; ++i) {
            try {
                push_back(value);
            } catch (...) {
                delete_list();
                throw;
            }
        }
    }

    List(size_t count, const Allocator& alloc = Allocator()) : List(alloc) {
        for (size_t i = 0; i < count; ++i) {
            try {
                emplace_push_back();
            } catch (...) {
                delete_list();
                throw;
            }
        }
    }

    template<typename InputIt>
    List(InputIt first, InputIt last, const Allocator& allocator): List(alloc) {
        try {
            insert(end(), first, last);
        } catch (...) {
            delete_list();
            throw;
        }
    }

private:
    List(const List& source, const Allocator& alloc)
            : List(alloc) {
        for (auto it = source.begin(); it != source.end(); ++it) {
            try {
                push_back(*it);
            } catch (...) {
                delete_list();
            }
        }
    }

    template<typename Allocator_, typename AllocatorNode_>
    List(List&& source, Allocator_&& alloc, AllocatorNode_&& alloc_node) noexcept:
            alloc(std::forward<Allocator_>(alloc)),
            alloc_node(std::forward<AllocatorNode_>(alloc_node)),
            end_(source.end_),
            size_(source.size_) {
        source.end_ = nullptr;
        source.size_ = 0;
    }

public:
    List(const List& source) : List(source, AllocTraitsT::select_on_container_copy_construction(source.alloc)) {}

    List(List&& source) noexcept: List(std::move(source), std::move(source.alloc), std::move(source.alloc_node)) {}

    void swap(List& right) noexcept {
        std::swap(alloc, right.alloc);
        std::swap(alloc_node, right.alloc_node);
        std::swap(size_, right.size_);
        std::swap(end_, right.end_);
    }

    List& operator=(const List& source) {
        auto tmp = List<T, Allocator>(source, AllocTraitsT::propagate_on_container_copy_assignment::value ? Allocator(
                source.alloc) : source.alloc);
        swap(tmp);
        return *this;
    }

    List& operator=(List&& source) noexcept {
        auto tmp = List<T, Allocator>(std::move(source),
                                      AllocTraitsT::propagate_on_container_move_assignment::value
                                      ? Allocator(source.alloc) : std::move(source.alloc),
                                      AllocTraitsT::propagate_on_container_move_assignment::value
                                      ? AllocatorNode(source.alloc_node) : std::move(source.alloc_node));
        swap(tmp);
        return *this;
    }

    const Allocator& get_allocator() const noexcept {
        return alloc;
    }

    size_t size() const noexcept {
        return size_;
    }

    void move_node(iterator source, iterator dest) noexcept {
        auto src_node = source.node;
        auto dst_node = dest.node;
        src_node->next->prev = src_node->prev;
        src_node->prev->next = src_node->next;
        src_node->prev = dst_node->prev;
        src_node->prev->next = src_node;
        src_node->next = dst_node;
        dst_node->prev = src_node;
    }
};




////#include "list.h"
//#include "../List/list.h"

template<class Key,
        class Value,
        class Hash = std::hash<Key>,
        class Equal = std::equal_to<Key>,
        class Allocator = std::allocator<std::pair<const Key, Value>>>
class UnorderedMap {
public:
    using NodeType = std::pair<const Key, Value>;
//private:
    static const size_t DEFAULT_BUCKET_SIZE = 10;
    size_t bucket_count = DEFAULT_BUCKET_SIZE;
    float max_load_factor_ = 0.7;
    Hash hasher = Hash();
    Equal equaler = Equal();
    Allocator alloc = Allocator();
    using DataList = List<NodeType, Allocator>;
    DataList data = DataList(alloc);
    using TableCell = std::pair<typename DataList::iterator, typename DataList::iterator>;
    using AllocTraits = std::allocator_traits<Allocator>;
    using Table = std::vector<TableCell, typename AllocTraits::template rebind_alloc<TableCell>>;
    Table hash_table = Table(bucket_count, {data.end(), data.end()}, alloc);

    size_t get_idx(size_t hash) const noexcept {
        return hash % bucket_count;
    }

public:
//    class iterator : public DataList::iterator {
//    public:
//        iterator(const typename DataList::iterator& it): DataList::iterator::iterator(it) {}
//        iterator(const iterator& it): DataList::iterator::iterator(it) {}
//    };
//
//    class const_iterator :  public DataList::const_iterator {
//    public:
//        const_iterator(const typename DataList::iterator& it): DataList::const_iterator::const_iterator(it) {}
//        const_iterator(const typename DataList::const_iterator& it): DataList::const_iterator::const_iterator(it) {}
//        const_iterator(const iterator& it): DataList::const_iterator::const_iterator(it) {}
//        const_iterator(const const_iterator& it): DataList::const_iterator::const_iterator(it) {}
//    };

    using iterator = typename DataList::iterator;
    using const_iterator = typename DataList::const_iterator;

    iterator begin() noexcept {
        return data.begin();
    }
    const_iterator cbegin() const noexcept {
        return data.cbegin();
    }
    const_iterator begin() const noexcept {
        return cbegin();
    }

    iterator end() noexcept {
        return data.end();
    }
    const_iterator cend() const noexcept {
        return data.cend();
    }
    const_iterator end() const noexcept {
        return cend();
    }

    size_t size() const noexcept {
        return data.size();
    }

    float load_factor() const noexcept {
        return float(size()) / bucket_count;
    }

    float max_load_factor() const noexcept {
        return max_load_factor_;
    }

    void max_load_factor(float new_mlf) noexcept {
        max_load_factor_ = new_mlf;
    }

    size_t max_size() {
        return AllocTraits::max_size(alloc);
    }

    void easy_rehash(size_t count) {
        auto new_table = Table(count, {data.end(), data.end()}, alloc);
        for (auto it = data.begin(); it != data.end();) {
            auto old_it = it++;
            auto cur_hash = get_idx(hasher(old_it->first));
            if (new_table[cur_hash].first == end()) {
                new_table[cur_hash].first = new_table[cur_hash].second = old_it;
            } else {
                data.move_node(old_it, new_table[cur_hash].second + 1);
            }
        }
        bucket_count = count;
        hash_table = std::move(new_table);
    }

    void bad_rehash(size_t count) {
        using HashList = List<size_t, typename AllocTraits::template rebind_alloc<size_t>>;
        using TableHashCell = std::pair<typename HashList::iterator, typename HashList::iterator>;
        auto hash_list = HashList(alloc);
        auto new_table_hash = std::vector<TableHashCell,
                typename AllocTraits::template rebind_alloc<TableHashCell>>
                (count, {hash_list.end(), hash_list.end()}, alloc);
        for (auto it = data.begin(); it != data.end(); ++it) {
            hash_list.push_back(get_idx(hasher(it->first)));
        }
        auto new_table = Table(count, {data.end(), data.end()}, alloc);
        auto hash_it = hash_list.begin();
        for (auto it = data.begin(); it != data.end();) {
            auto old_it = it++;
            auto old_hash_it = hash_it++;
            auto cur_hash = *old_hash_it;
            if (new_table[cur_hash].first == end()) {
                new_table[cur_hash].first = new_table[cur_hash].second = old_it;
                new_table_hash[cur_hash].first = new_table_hash[cur_hash].second = old_hash_it;
            } else {
                data.move_node(old_it, new_table[cur_hash].second + 1);
                hash_list.move_node(old_hash_it, new_table_hash[cur_hash].second + 1);
            }
        }
        bucket_count = count;
        hash_table = std::move(new_table);
    }

    void rehash(size_t count) {
//        if constexpr (noexcept(hasher()))
//            easy_rehash(count);
//        else
        bad_rehash(count);
    }

    void reserve(size_t count) {
        if (count > size())
            rehash(std::ceil(count / max_load_factor_));
    }
private:
    iterator find(const Key& key, size_t cur_hash) {
        auto begin_ = hash_table[cur_hash].first;
        auto end_ = hash_table[cur_hash].second + 1;
        for (auto it = begin_; it != end() && it != end_; ++it) {
            if (equaler(it->first, key))
                return it;
        }
        return end();
    }
    const_iterator find(const Key& key, size_t cur_hash) const {
        auto begin_ = hash_table[cur_hash].first;
        auto end_ = hash_table[cur_hash].second + 1;
        for (auto it = begin_; it != end() && it != end_; ++it) {
            if (equaler(it->first, key))
                return it;
        }
        return end();
    }

public:
    iterator find(const Key& key) {
        return find(key, get_idx(hasher(key)));
    }
    const_iterator find(const Key& key) const {
        return find(key, get_idx(hasher(key)));
    }
private:
    template<class T>
    std::pair<iterator, bool> insert_(T&& pair) {
        if (load_factor() >= max_load_factor_)
            rehash(bucket_count * 2);
        auto cur_hash = get_idx(hasher(pair.first));
        auto pos = find(pair.first, cur_hash);
        if (pos != end())
            return {pos, false};
        if (hash_table[cur_hash].second != end()) {
            data.insert(hash_table[cur_hash].second + 1, std::forward<T>(pair));
            ++(hash_table[cur_hash].second);
        } else {
            data.push_back(std::forward<T>(pair));
            hash_table[cur_hash].first = --(hash_table[cur_hash].second); // TODO: check!
        }
        return {hash_table[cur_hash].second, true};
    }

public:
    std::pair<iterator, bool> insert(const NodeType& pair) {
        return insert_(pair);
    }

    std::pair<iterator, bool> insert(NodeType&& pair) {
        return insert_(std::move(pair));
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it)
            insert(*it);
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&& ... args) {
        if (load_factor() >= max_load_factor_)
            rehash(bucket_count * 2);
        data.emplace(data.begin(), std::forward<Args>(args)...); // TODO: template?
        auto& cur_key = data.begin()->first;
        auto cur_hash = get_idx(hasher(cur_key));
        auto pos = find(cur_key, cur_hash);
        if (pos != end()) {
            data.pop_front();
            return {pos, false};
        }
        if (hash_table[cur_hash].second != end()) {
            data.move_node(data.begin(), hash_table[cur_hash].second + 1);
            ++(hash_table[cur_hash].second);
        } else {
            data.move_node(data.begin(), end());
            hash_table[cur_hash].first = --(hash_table[cur_hash].second); // TODO: check!
        }
        return {hash_table[cur_hash].second, true};
    }

    Value& operator[](const Key& key) {
        auto pos = find(key);
        if (pos != end())
            return pos->second;
        return insert({Key(key), Value()}).first->second;
    }
    Value& operator[](Key&& key) {
        auto pos = find(key);
        if (pos != end())
            return pos->second;
        return insert({std::move(key), Value()}).first->second;
    }

    Value& at(const Key& key) {
        auto pos = find(key);
        if (pos != end())
            return pos->second;
        else
            throw std::range_error("The element for this key does not exist!");
    }
    const Value& at(const Key& key) const {
        auto pos = find(key);
        if (pos != end())
            return pos->second;
        else
            throw std::range_error("The element for this key does not exist!");
    }

    void erase(const const_iterator& it) noexcept {
        data.erase(it);
    }

    void erase(const const_iterator& first, const const_iterator& last) noexcept {
        for (auto it = first; it != last; ++it)
            data.erase(it);
    }

    void erase(const Key& key) {
        erase(find(key));
    }

    UnorderedMap(size_t bucket_count = DEFAULT_BUCKET_SIZE) : bucket_count(bucket_count) {}

    UnorderedMap(const Allocator& alloc) : alloc(alloc) {}

private:
    UnorderedMap(const UnorderedMap& source, const Allocator& alloc) :
            bucket_count(source.bucket_count),
            hasher(source.hasher),
            equaler(source.equaler),
            alloc(alloc),
            hash_table(source.hash_table) {
        for (size_t i = 0; i < hash_table.size(); ++i) {
            if (source.hash_table[i].first != source.end()) {
                hash_table[i].first = data.end() - 1;
                data.insert(data.end(), source.hash_table[i].first, source.hash_table[i].second + 1);
                hash_table[i].second = data.end() - 1;
            }
        }
    }

    template<typename Allocator_>
    UnorderedMap(UnorderedMap&& source, Allocator_&& alloc):
            bucket_count(source.bucket_count),
            hasher(std::move(source.hasher)),
            equaler(std::move(source.equaler)),
            alloc(std::forward<Allocator_>(alloc)), // TODO: data's allocs moves?
            data(std::move(source.data)),
            hash_table(std::move(source.hash_table)) {}

public:
    UnorderedMap(const UnorderedMap& source) :
            UnorderedMap(source, AllocTraits::select_on_container_copy_construction(source.alloc)) {}

    UnorderedMap(UnorderedMap&& source) : UnorderedMap(std::move(source), std::move(source.alloc)) {}

    void swap(UnorderedMap& right) noexcept {
        std::swap(bucket_count, right.bucket_count);
        std::swap(hasher, right.hasher);
        std::swap(equaler, right.equaler);
        std::swap(alloc, right.alloc);
        data.swap(right.data);
        std::swap(hash_table, right.hash_table);
    }

    UnorderedMap& operator=(const UnorderedMap& source) {
        auto tmp = UnorderedMap(source, AllocTraits::propagate_on_container_copy_assignment::value
                                        ? Allocator(source.alloc) : source.alloc);
        swap(tmp);
        return *this;
    }

    UnorderedMap& operator=(UnorderedMap&& source) {
        auto tmp = UnorderedMap(std::move(source), AllocTraits::propagate_on_container_move_assignment::value
                                                   ? Allocator(source.alloc) : std::move(source.alloc));
        swap(tmp);
        return *this;
    }
};
