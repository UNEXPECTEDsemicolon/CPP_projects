void swap(Node& right) {
    prev->next = &right;
    next->prev = &right;
    right.prev->next = this;
    right.next->prev = this;
    std::swap(prev, right.prev);
    std::swap(next, right.next);
    std::swap(value, right.value);
}