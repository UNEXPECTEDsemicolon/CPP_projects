#include <unordered_map>

template<class Key,
         class Value,
         class Hash = std::hash<Key>,
         class Equal = std::equal_to<Key>,
         class Allocator = std::allocator<std::pair<const Key, Value>>>
using UnorderedMap = std::unordered_map<Key, Value, Hash, Equal, Allocator>;

#define NodeType value_type
