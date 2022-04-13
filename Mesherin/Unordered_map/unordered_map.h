#include <iostream>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>
//#include "list.h"
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
    Hash hasher = Hash();
    Equal equaler = Equal();
    Allocator alloc = Allocator();
    using DataList = List<NodeType, Allocator>;
    DataList data = DataList(alloc);
    using TableCell = std::pair<typename DataList::iterator, typename DataList::iterator>;
    using AllocTraits = std::allocator_traits<Allocator>;
    using Table = std::vector<TableCell, typename AllocTraits::template rebind_alloc<TableCell>>;
    Table hash_table = Table(bucket_count, {data.end(), data.end()}, alloc);

    size_t get_idx(size_t hash) {
        return hash % bucket_count;
    }

public:
    using iterator = typename DataList::iterator;
    using const_iterator = typename DataList::const_iterator;
    using reverse_iterator = typename DataList::reverse_iterator;
    using const_reverse_iterator = typename DataList::const_reverse_iterator;

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

    reverse_iterator rbegin() noexcept {
        return data.rbegin();
    }
    const_reverse_iterator crbegin() const noexcept {
        return data.crbegin();
    }
    const_reverse_iterator rbegin() const noexcept {
        return crbegin();
    }

    reverse_iterator rend() noexcept {
        return data.rend();
    }
    const_reverse_iterator crend() const noexcept {
        return data.crend();
    }
    const_reverse_iterator rend() const noexcept {
        return crend();
    }

    size_t size() const noexcept {
        return data.size();
    }

private:
    iterator find(const Key& key, size_t cur_hash) noexcept {
        auto begin_ = hash_table[cur_hash].first;
        auto end_ = hash_table[cur_hash].second + 1;
        for (auto it = begin_; it != end() && it != end_; ++it) {
            if (equaler(it->first, key))
                return it;
        } return end();
    }

public:
    iterator find(const Key& key) noexcept {
        return find(key, get_idx(hasher(key)));
    }
private:
    template<class T>
    std::pair<iterator, bool> insert_(T&& pair) {
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
        return insert_(pair);
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it)
            insert(*it);
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        data.emplace(data.begin(), std::forward<Args>(args)...); // TODO: template?
        auto cur_key = data.begin()->first;
        auto cur_hash = get_idx(hasher(cur_key));
        auto pos = find(cur_key, cur_hash);
        if (pos != end()) {
            data.pop_front();
            return {pos, false};
        } if (hash_table[cur_hash].second != end()) {
            data.move_node(data.begin(), hash_table[cur_hash].second + 1);
            ++(hash_table[cur_hash].second);
        } else {
            data.move_node(data.begin(), end());
            hash_table[cur_hash].first = --(hash_table[cur_hash].second); // TODO: check!
        }
        return {hash_table[cur_hash].second, true};
    }

    void erase(const const_iterator& it) noexcept {
        data.erase(it);
    }

    void erase(const const_iterator& first, const const_iterator& last) noexcept {
        for (auto it = first; it != last; ++it)
            data.erase(it);
    }

    UnorderedMap(size_t bucket_count = DEFAULT_BUCKET_SIZE): bucket_count(bucket_count) {}

    UnorderedMap(const Allocator& alloc): alloc(alloc) {}

private:
    UnorderedMap(const UnorderedMap& source, const Allocator& alloc):
        bucket_count(source.bucket_count),
        hasher(source.hasher),
        equaler(source.equaler),
        alloc(alloc),
        hash_table(source.hash_table) {
        for (size_t i = 0; i < hash_table.size(); ++i) {
            if (source.hash_table[i].first != source.end()) {
                hash_table[i].first = end() - 1;
                data.insert(end(), source.hash_table[i].first, source.hash_table[i].second + 1);
                hash_table[i].second = end() - 1;
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
    UnorderedMap(const UnorderedMap& source):
        UnorderedMap(source, AllocTraits::select_on_container_copy_construction(source.alloc)) {}

    UnorderedMap(UnorderedMap&& source): UnorderedMap(std::move(source), std::move(source.alloc)) {}

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
