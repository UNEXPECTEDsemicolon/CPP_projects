#pragma once

#include <iostream>
#include <utility>
#include <iterator>

template <typename T>
class Vector {
 private:
  T* body_ = nullptr;
  size_t sz_ = 0;
  size_t cap_ = 0;

  static T* CreateArr(size_t size) {
    return reinterpret_cast<T*>(new uint8_t[size * sizeof(T)]);
  }

  static void DeleteArr(T* arr) noexcept {
    delete[] reinterpret_cast<uint8_t*>(arr);
  }

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;

  size_t Size() const noexcept {
    return sz_;
  }

  bool Empty() const noexcept {
    return sz_ == 0;
  }

  size_t Capacity() const noexcept {
    return cap_;
  }

 private:
  void ShrinkOrExpand(size_t new_cap, bool expand = true) {
    if (expand && new_cap <= cap_ || !expand && new_cap >= cap_) {
      return;
    }
    auto new_body = CreateArr(new_cap);
    for (size_t i = 0; i < sz_; ++i) {
      try {
        new (new_body + i) T(std::move(body_[i]));
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (new_body + j)->~T();
        }
        DeleteArr(new_body);
        throw;
      }
    }
    for (size_t i = 0; i < sz_; ++i) {
      (body_ + i)->~T();
    }
    DeleteArr(body_);
    cap_ = new_cap;
    body_ = new_body;
  }

 public:
  void Reserve(size_t new_cap) {
    ShrinkOrExpand(new_cap);
  }

  void ShrinkToFit() {
    ShrinkOrExpand(sz_, false);
  }

 public:
  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    auto temp = T(std::forward<Args>(args)...);
    if (sz_ >= cap_) {
      Reserve(sz_ * 2);
    }
    new (body_ + sz_) T(std::move(temp));
    ++sz_;
  }
  void PushBack(const T& value) {
    EmplaceBack(value);
  }
  void PushBack(T&& value) {
    EmplaceBack(std::move(value));
  }

  void PopBack() {
    if (sz_) {
      (body_ + sz_--)->~T();
    }
  }

 private:
  template <typename... Args>
  void ResizePrivate(size_t new_size, Args&&... args) {
    while (new_size < sz_) {
      PopBack();
    }
    if (new_size == sz_) {
      return;
    }
    Reserve(new_size);
    for (size_t i = sz_; i < new_size; ++i) {
      EmplaceBack(std::forward<Args>(args)...);
    }
  }

 public:
  void Resize(size_t new_size) {
    ResizePrivate(new_size);
  }

  void Resize(size_t new_size, const T& value) {
    ResizePrivate(new_size, value);
  }

  template <class TT>
  class BaseIterator : public std::iterator<std::random_access_iterator_tag, TT> {
   private:
    TT* ptr_;

   public:
    explicit BaseIterator(TT* ptr) noexcept : ptr_(ptr) {
    }

    template <class TTT, typename = std::enable_if_t<std::is_same<TTT, T>::value && std::is_same<TT, const T>::value>>
    explicit BaseIterator(const BaseIterator<TTT>& source) noexcept : ptr_(source.ptr_) {
    }

    template <class TTT>
    explicit BaseIterator(const BaseIterator<TT>& source) noexcept : ptr_(source.ptr_) {
    }

    void Swap(BaseIterator<TT>& right) noexcept {
      std::swap(ptr_, right.ptr_);
    }

    template <class TTT, typename = std::enable_if_t<std::is_same<TTT, T>::value && std::is_same<TT, const T>::value>>
    BaseIterator<TT>& operator=(const BaseIterator<TTT>& source) noexcept {
      auto tmp = source;
      Swap(tmp);
      return *this;
    }

    template <class TTT>
    BaseIterator<TT>& operator=(const BaseIterator<TT>& source) noexcept {
      auto tmp = source;
      Swap(tmp);
      return *this;
    }

    bool operator==(const BaseIterator<TT>& right) const noexcept {
      return ptr_ == right.ptr_;
    }

    bool operator!=(const BaseIterator<TT>& right) const noexcept {
      return !(*this == right);
    }

    BaseIterator<TT>& operator++() noexcept {
      ++ptr_;
      return *this;
    }

    BaseIterator<TT> operator++(int)noexcept {
      auto res = *this;
      ++*this;
      return res;
    }

    BaseIterator<TT>& operator--() noexcept {
      --ptr_;
      return *this;
    }

    BaseIterator<TT> operator--(int)noexcept {
      auto res = *this;
      --*this;
      return res;
    }

    BaseIterator<TT>& operator+=(size_t val) noexcept {
      ptr_ += val;
      return *this;
    }

    BaseIterator<TT> operator+(size_t val) const noexcept {
      auto res = *this;
      res += val;
      return res;
    }

    BaseIterator<TT>& operator-=(size_t val) noexcept {
      ptr_ -= val;
      return *this;
    }

    BaseIterator<TT> operator-(size_t val) const noexcept {
      auto res = *this;
      res -= val;
      return res;
    }

    size_t operator-(const BaseIterator<TT>& right) const noexcept {
      return right.ptr_ - ptr_;
    }

    TT& operator*() noexcept {
      return *ptr_;
    }

    const TT& operator*() const noexcept {
      return *ptr_;
    }

    TT* operator->() noexcept {
      return ptr_;
    }

    const TT* operator->() const noexcept {
      return ptr_;
    }
    friend std::conditional_t<std::is_same<TT, const T>::value, BaseIterator<T>, BaseIterator<const T>>;
  };

  using Iterator = BaseIterator<T>;
  using ConstIterator = BaseIterator<const T>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Iterator begin() noexcept {  // NOLINT
    return Iterator(body_);
  }
  ConstIterator cbegin() const noexcept {  // NOLINT
    return ConstIterator(body_);
  }
  ConstIterator begin() const noexcept {  // NOLINT
    return cbegin();
  }

  Iterator end() noexcept {  // NOLINT
    return Iterator(body_ + sz_);
  }
  ConstIterator cend() const noexcept {  // NOLINT
    return ConstIterator(body_ + sz_);
  }
  ConstIterator end() const noexcept {  // NOLINT
    return cend();
  }

  ReverseIterator rbegin() noexcept {  // NOLINT
    return ReverseIterator(end());
  }
  ConstReverseIterator crbegin() const noexcept {  // NOLINT
    return ConstReverseIterator(cend());
  }
  ConstReverseIterator rbegin() const noexcept {  // NOLINT
    return crbegin();
  }

  ReverseIterator rend() noexcept {  // NOLINT
    return ReverseIterator(begin());
  }
  ConstReverseIterator crend() const noexcept {  // NOLINT
    return ConstReverseIterator(cbegin());
  }
  ConstReverseIterator rend() const noexcept {  // NOLINT
    return crend();
  }

  void Clear() noexcept {
    while (sz_) {
      PopBack();
    }
  }

  ~Vector() {
    Clear();
  }

  Vector() noexcept = default;

 private:
  class Special {
  };

  template <typename... Args>
  Vector(size_t size, Special, Args&&... args) : body_(CreateArr(size)), cap_(size) {
    for (size_t i = 0; i < size; ++i) {
      EmplaceBack(std::forward<Args>(args)...);
    }
  }

 public:
  explicit Vector(size_t size) : Vector(size, Special()) {
  }
  Vector(size_t size, const T& value) : Vector(size, Special(), value) {
  }

  template <class Iterator,
            class = std::enable_if_t<std::is_base_of<
                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>::value>>
  Vector(Iterator first, Iterator last) {
    for (auto it = first; it != last; ++it) {
      try {
        PushBack(*it);
      } catch (...) {
        Clear();
        throw;
      }
    }
  }

  Vector(const std::initializer_list<T>& list) : body_(CreateArr(list.size())), cap_(list.size()) {
    for (auto elem : list) {
      try {
        PushBack(elem);
      } catch (...) {
        Clear();
        throw;
      }
    }
  }

  Vector(const Vector& source) : sz_(source.sz_) {
    Reserve(source.sz_);
    for (size_t i = 0; i < source.sz_; ++i) {
      try {
        PushBack(source[i]);
      } catch (...) {
        Clear();
        throw;
      }
    }
  }

  Vector(Vector&& source) noexcept : body_(source.body_), sz_(source.sz_), cap_(source.cap_) {
    source.body_ = nullptr;
    source.sz_ = 0;
    source.cap_ = 0;
  }

  void Swap(Vector& right) noexcept {
    std::swap(body_, right.body_);
    std::swap(sz_, right.sz_);
    std::swap(cap_, right.cap_);
  }

  Vector& operator=(const Vector& source) {
    auto tmp = Vector(source);
    Swap(tmp);
    return *this;
  }

  Vector& operator=(Vector&& source) noexcept {
    auto tmp = Vector(std::move(source));
    Swap(tmp);
    return *this;
  }

  T& operator[](size_t index) noexcept {
    return body_[index];
  }
  const T& operator[](size_t index) const noexcept {
    return body_[index];
  }

  T& At(size_t index) {
    if (index >= sz_) {
      throw std::out_of_range("Vector index out of range");
    }
    return operator[](index);
  }
  const T& At(size_t index) const {
    if (index >= sz_) {
      throw std::out_of_range("Vector index out of range");
    }
    return operator[](index);
  }

  T& Front() noexcept {
    return body_[0];
  }
  const T& Front() const noexcept {
    return body_[0];
  }

  T& Back() noexcept {
    return body_[sz_ - 1];
  }
  const T& Back() const noexcept {
    return body_[sz_ - 1];
  }

  T* Data() noexcept {
    return body_;
  }
  const T* Data() const noexcept {
    return body_;
  }
};

template <typename T>
bool operator==(const Vector<T>& v1, const Vector<T>& v2) noexcept {
  return std::equal(v1.begin(), v1.end(), v2.begin());
}

template <typename T>
bool operator!=(const Vector<T>& v1, const Vector<T>& v2) noexcept {
  return !(v1 == v2);
}

template <typename T>
bool operator<(const Vector<T>& v1, const Vector<T>& v2) noexcept {
  return std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
}

template <typename T>
bool operator<=(const Vector<T>& v1, const Vector<T>& v2) noexcept {
  return v1 < v2 || v1 == v2;
}

template <typename T>
bool operator>(const Vector<T>& v1, const Vector<T>& v2) noexcept {
  return !(v1 <= v2);
}

template <typename T>
bool operator>=(const Vector<T>& v1, const Vector<T>& v2) noexcept {
  return !(v1 < v2);
}