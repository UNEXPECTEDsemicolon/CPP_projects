#include <vector>
#include <stdexcept>
#include <iterator>
#include <type_traits>
#include <iostream>

template<class T>
class deque {
private:
    static const ssize_t ARR_SIZE = 1 << 10;
//    static const ssize_t ARR_SIZE = 3;
    ssize_t size_ = 0;
    std::vector<T*> body;

    struct Index {
        ssize_t x = 0;
        ssize_t y = 0;

        ssize_t meta_ind() const noexcept {
            return x * ARR_SIZE + y;
        }

        Index(ssize_t x, ssize_t y) noexcept: x(x), y(y) {}
        Index(ssize_t y = 0) noexcept: y(y % ARR_SIZE), x(y / ARR_SIZE) {}
        Index(const Index& right) noexcept: x(right.x), y(right.y) {}

        void swap(Index& right) noexcept {
            std::swap(x, right.x);
            std::swap(y, right.y);
        }

        Index& operator=(const Index& right) noexcept {
            auto tmp = right;
            swap(tmp);
            return *this;
        }

        ssize_t& operator[](ssize_t ind) noexcept {
            return (ind ? y : x);
        }

        const ssize_t& operator[](ssize_t ind) const noexcept {
            return (ind ? y : x);
        }

        explicit operator ssize_t() const noexcept {
            return meta_ind();
        }

        bool operator<(const Index& right) const noexcept {
            return meta_ind() < right.meta_ind();
        }

        bool operator<=(const Index& right) const noexcept {
            return meta_ind() <= right.meta_ind();
        }

        bool operator>(const Index& right) const noexcept {
            return meta_ind() > right.meta_ind();
        }

        bool operator>=(const Index& right) const noexcept {
            return meta_ind() >= right.meta_ind();
        }

        bool operator==(const Index& right) const noexcept {
            return (x == right.x) && (y == right.y);
        }

        bool operator!=(const Index& right) const noexcept {
            return !(*this == right);
        }

        Index& operator+=(const Index& right) noexcept {
            auto sum = meta_ind() + right.meta_ind();
            x = sum / ARR_SIZE;
            y = sum % ARR_SIZE;
            return *this;
        }

        Index& operator++() noexcept {
            return *this += 1;
        }

        Index operator++(int) noexcept {
            auto res = *this;
            ++*this;
            return res;
        }

        Index& operator-=(const Index& right) noexcept {
            auto sum = meta_ind() - right.meta_ind();
            x = sum / ARR_SIZE;
            y = sum % ARR_SIZE;
            return *this;
        }

        Index& operator--() noexcept {
            return *this -= 1;
        }

        Index operator--(int) noexcept {
            auto res = *this;
            --*this;
            return res;
        }

        Index operator+(const Index& right) const noexcept {
            auto res = *this;
            return res += right;
        }

        Index operator-(const Index& right) const noexcept {
            auto res = *this;
            return res -= right;
        }
    };

    T& body_(const Index& ind) noexcept {
        return body[ind[0]][ind[1]];
    }

    const T& body_(const Index& ind) const noexcept {
        return body[ind[0]][ind[1]];
    }

    Index begin_ = {0, 0};
    Index end_ = {0, 0};
    
    static T* get_new_arr() {
        return reinterpret_cast<T*>(new uint8_t [ARR_SIZE * sizeof(T)]);
    }

    void delete_body() noexcept {
        for (ssize_t i = 0; i < body.size(); ++i)
            if (body[i] != nullptr) {
                for (ssize_t j = 0; j < ARR_SIZE && begin_ <= Index(i, j) && Index(i, j) < end_; ++j)
                    body[i][j].~T();
                delete[] reinterpret_cast<uint8_t*>(body[i]);
                body[i] = nullptr;
            }
    }

    void safety_assign(const Index& i, const T& val) {
        try {
            if (body[i[0]] == nullptr)
                body[i[0]] = get_new_arr();
            new(&body_(i)) T(val);
        } catch (...) {
            end_ = i;
            delete_body();
            throw;
        }
    }

    void resize(ssize_t new_cap) {
        auto new_body = std::vector<T *>(new_cap, nullptr);
        auto new_begin = new_cap / 3;
        auto new_end = new_begin + size_;
        for (ssize_t i = 0; i < size_; ++i) {
            new_body[new_begin + i] = body[(begin_)[0] + i];
        }
        begin_ = new_begin;
        end_ = new_end;
        body = new_body;
    }

public:
    template<class deque_t, class T_>
    class base_iterator: public std::iterator<std::random_access_iterator_tag, T>, public Index {
    private:
        deque_t *origin;
        using Index::operator+;
        using Index::operator+=;
        using Index::operator-=;
        using Index::operator-;

    public:
        base_iterator(const Index& ind, deque_t* origin) noexcept: Index::Index(ind), origin(origin) {}

        base_iterator(const base_iterator<deque<T>, T>& source) noexcept
            : Index::Index(source), origin(source.origin) {}

        base_iterator(const base_iterator<const deque<T>, const T>& source) noexcept
            : Index::Index(source), origin(source.origin) {}

        void swap(base_iterator<deque_t, T_>& right) noexcept {
            std::swap(origin, right.origin);
            Index::swap(right);
        }

        base_iterator<deque_t, T_>& operator=(const base_iterator<deque_t, T_>& source) noexcept {
            auto tmp = source;
            swap(tmp);
            return *this;
        }

        base_iterator<deque_t, T_>& operator++() noexcept {
            Index::operator++();
            return *this;
        }

        base_iterator<deque_t, T_> operator++(int) noexcept {
            auto res = *this;
            ++*this;
            return res;
        }

        base_iterator<deque_t, T_>& operator--() noexcept {
            Index::operator--();
            return *this;
        }

        base_iterator<deque_t, T_> operator--(int) noexcept {
            auto res = *this;
            --*this;
            return res;
        }

        base_iterator<deque_t, T_>& operator+=(ssize_t val) noexcept {
            Index::operator+=(val);
            return *this;
        }

        base_iterator<deque_t, T_> operator+(ssize_t val) const noexcept {
            auto res = *this;
            res += val;
            return res;
        }

        base_iterator<deque_t, T_>& operator-=(ssize_t val) noexcept {
            return *this += (-val);
        }

        base_iterator<deque_t, T_> operator-(ssize_t val) const noexcept {
            return *this + (-val);
        }

        ssize_t operator-(const base_iterator<deque_t, T_>& right) const noexcept {
            return ssize_t(Index::operator-(right));
        }

        T_& operator*() noexcept {
            return origin->body_(*this);
        }

        const T_& operator*() const noexcept {
            return origin->body_(*this);
        }

        T_* operator->() noexcept {
            return &(this->operator*());
        }

        const T_* operator->() const noexcept {
            return &(this->operator*());
        }

        friend std::conditional_t<std::is_same_v<deque_t, const deque<T>>, base_iterator<deque<T>, T>, base_iterator<const deque<T>, const T>>;
    };

    using iterator = base_iterator<deque<T>, T>;
    using const_iterator = base_iterator<const deque<T>, const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    deque(ssize_t size = 10 * ARR_SIZE): size_(size), body(std::vector<T*>(3 * (size_ / ARR_SIZE + 1), nullptr)),
                                         begin_(body.size() / 3, 0), end_(begin_ + size) {}

    deque(ssize_t size, const T& value): deque(size) {
        for (auto i = begin_; i != end_; ++i) {
            safety_assign(i, value);
        }
    }

    deque(const deque<T>& source): begin_(source.begin_), end_(source.end_),
                                   size_(source.size_), body(source.body.size(), nullptr) {
        for (auto i = source.begin_; i != source.end_; ++i) {
            safety_assign(i, source.body_(i));
        }
    }

    void swap(deque<T>& right) noexcept {
        std::swap(size_, right.size_);
        body.swap(right.body);
        begin_.swap(right.begin_);
        end_.swap(right.end_);
    }
    
    deque<T>& operator=(const deque<T>& source) {
        auto tmp = source;
        swap(tmp);
        return *this;
    }

    ~deque() {
        delete_body();
    }

    ssize_t size() const noexcept {
        return size_;
    }

    iterator begin() noexcept {
        return iterator(begin_, this);
    }
    const_iterator cbegin() const noexcept {
        return const_iterator(begin_, this);
    }
    const_iterator begin() const noexcept {
        return cbegin();
    }

    iterator end() noexcept {
        return iterator(end_, this);
    }
    const_iterator cend() const noexcept {
        return const_iterator(end_, this);
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

    T& operator[](ssize_t index) noexcept {
        auto temp = begin_ + index;
        return body_(begin_ + index);
    }

    const T& operator[] (ssize_t index) const noexcept {
        return body_(begin_ + index);
    }

    T& at(ssize_t index) {
        if (index >= size() || index < 0) throw(std::out_of_range("Deque index out of range"));
        return (*this)[index];
    }

    const T& at(ssize_t index) const {
        if (index >= size() || index < 0) throw(std::out_of_range("Deque index out of range"));
        return (*this)[index];
    }

    void push_back(const T& val) {
        if (end_[0] >= body.size())
            resize(size_ * 3);
        if (body[end_[0]] == nullptr)
            body[end_[0]] = get_new_arr();
        new(&body_(end_++)) T(val);
        ++size_;
    }

    void push_front(const T& val) {
        if (begin_[0] == 0)
            resize(size_ * 3);
        if (body[begin_[0] - 1] == nullptr)
            body[begin_[0] - 1] = get_new_arr();
        new(&body_(--begin_)) T(val);
        ++size_;
    }

    void pop_back() {
        if (size_ == 0)
            throw std::out_of_range("Deque is empty");
        if (size_ < body.size() / 6)
            resize(size_ / 3);
        body_(--end_).~T();
        --size_;
    }

    void pop_front() {
        if (size_ == 0)
            throw std::out_of_range("Deque is empty");
        if (size_ < body.size() / 6)
            resize(size_ / 3);
        body_(begin_++).~T();
        --size_;
    }

    void insert(const const_iterator& pos, const T& val) {
        push_back(val);
        for (auto it = end() - 1; it != pos; --it)
            std::swap(*it, *(it - 1));
    }

    void erase(const const_iterator& pos) {
        for (auto it = begin() + (pos - cbegin()) + 1; it != end(); ++it)
            std::swap(*(it - 1), *it);
        try {
            pop_back();
        } catch (...) {
            for (auto it = end() - 1; it != pos; --it)
                std::swap(*it, *(it - 1));
            throw;
        }
    }
};
