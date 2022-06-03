#include <iostream>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>
#include <cmath>

#include "../List/list.h"


template<class Key,
        class Value,
        class Hash = std::hash<Key>,
        class Equal = std::equal_to<Key>,
        class Allocator = std::allocator<std::pair<const Key, Value>>>
class UnorderedMap {
public:
    using NodeType = std::pair<const Key, Value>;
private:
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
        return hash % hash_table.size();
    }

public:
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
        return float(size()) / hash_table.size();
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
//        bad_rehash(count);
        easy_rehash(count);
    }

    void reserve(size_t count) {
        if (count > size())
            rehash(std::ceil(count / max_load_factor_));
    }
private:
    iterator find(const Key& key, size_t cur_hash) { // 1428572
        auto begin_ = hash_table[cur_hash].first;
        if (begin_ == end())
            return end();
        auto end_ = hash_table[cur_hash].second;
        ++end_;
        for (auto it = begin_; it != end() && it != end_; ++it) {
            if (equaler(it->first, key))
                return it;
        }
        return end();
    }
    const_iterator find(const Key& key, size_t cur_hash) const {
        auto begin_ = hash_table[cur_hash].first;
        if (begin_ == end())
            return end();
        auto end_ = hash_table[cur_hash].second;
        ++end_;
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
            rehash(hash_table.size() * 2);
        auto cur_hash = get_idx(hasher(pair.first));
        auto pos = find(pair.first, cur_hash);
        if (pos != end())
            return {pos, false};
        if (hash_table[cur_hash].second != end()) {
            data.insert(hash_table[cur_hash].second + 1, std::forward<T>(pair));
            ++(hash_table[cur_hash].second);
        } else {
            data.push_back(std::forward<T>(pair));
            hash_table[cur_hash].first = --(hash_table[cur_hash].second);
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

    template<typename _Pair, typename = typename std::enable_if_t<std::is_constructible<NodeType&&, _Pair&&>::value>>
    std::pair<iterator, bool> insert(_Pair&& pair) {
        return insert_(std::forward<_Pair>(pair));
    }

    template<class InputIt>
    void insert(const InputIt& first, const InputIt& last) {
        for (auto it = first; it != last; ++it)
            insert(*it);
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&& ... args) {
        if (load_factor() >= max_load_factor_)
            rehash(hash_table.size() * 2);
        data.emplace(data.begin(), std::forward<Args>(args)...);
        try {
            auto& cur_key = data.begin()->first;
            auto cur_hash = get_idx(hasher(cur_key));
            auto pos = find(cur_key, cur_hash);
            if (pos != end() && pos != begin()) {
                data.pop_front();
                return {pos, false};
            }
            if (hash_table[cur_hash].second != end()) {
                data.move_node(data.begin(), ++(hash_table[cur_hash].second));
                --(hash_table[cur_hash].second);
            } else {
                data.move_node(data.begin(), end());
                hash_table[cur_hash].first = --(hash_table[cur_hash].second);
            }
            return {hash_table[cur_hash].second, true};
        } catch (...) {
            data.pop_front();
            throw;
        }
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
        if (it == end()) return;
        auto cur_hash = get_idx(hasher(it->first));
        if (hash_table[cur_hash].second == hash_table[cur_hash].first)
            hash_table[cur_hash].second = hash_table[cur_hash].first = end();
        else if (hash_table[cur_hash].second == it)
            --(hash_table[cur_hash].second);
        else if (hash_table[cur_hash].first == it)
            ++(hash_table[cur_hash].first);
        data.erase(it);
    }

    void erase(const const_iterator& first, const const_iterator& last) noexcept {
        for (auto it = first; it != last;)
            erase(it++);
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
            alloc(std::forward<Allocator_>(alloc)),
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
