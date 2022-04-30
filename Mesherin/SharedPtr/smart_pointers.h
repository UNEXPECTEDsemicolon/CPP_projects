#include <iostream>
#include <memory>


template <typename T>
class WeakPtr;

template <typename X, typename Y>
using is_familiar = std::enable_if_t< std::is_same<X, Y>::value || std::is_base_of<X, Y>::value>;

struct BaseControlBlock {
    size_t shared_count;
    size_t weak_count;

    BaseControlBlock(size_t sc, size_t wc) noexcept: shared_count(sc), weak_count(wc) {}

    virtual void* get_ptr() { return nullptr; };

    virtual void destroy_object() {};

    virtual void deallocate() {}

    virtual ~BaseControlBlock() = default;
};

template<typename T_, typename Deleter, typename Alloc>
struct ControlBlockDirect: BaseControlBlock {
    T_* ptr;
    Deleter deleter;
    Alloc alloc;

    ControlBlockDirect(T_* ptr, size_t sc, size_t wc, const Deleter& deleter, Alloc&& alloc)
    : BaseControlBlock(sc, wc), ptr(ptr), deleter(deleter), alloc(std::move(alloc)) {}

    void* get_ptr() override {
        return ptr;
    }

    void destroy_object() override {
        deleter(ptr);
        ptr = nullptr;
    }

    void deallocate() override {
        typename std::allocator_traits<Alloc>::template rebind_alloc<ControlBlockDirect>(alloc).deallocate(this, 1);
        alloc.~Alloc();
    }

    ~ControlBlockDirect() {
        if (ptr) destroy_object();
    }
};

template<typename T, typename Alloc>
struct ControlBlockShared: BaseControlBlock {
    T object;
    Alloc alloc;

    template<typename... Args>
    ControlBlockShared(size_t sc, size_t wc, Alloc&& alloc, Args&&... args)
        : BaseControlBlock(sc, wc), object(std::forward<Args>(args)...), alloc(std::move(alloc)) {}

    void* get_ptr() override {
        return &object;
    }

    void deallocate() override {
        typename std::allocator_traits<Alloc>::template rebind_alloc<ControlBlockShared>(alloc).deallocate(this, 1);
        alloc.~Alloc();
    }

    void destroy_object() override {
        std::allocator_traits<Alloc>::destroy(alloc, &object);
    }
};

template<typename T>
class SharedPtr {
private:
    template<typename T_, typename Alloc, typename... Args>
    friend SharedPtr<T_> allocateShared(const Alloc& alloc, Args&&... args);

    template<typename T_, typename... Args>
    friend SharedPtr<T_> makeShared(Args&&... args);

    friend WeakPtr<T>;

    template<typename T_>
    template<typename T__, typename>
    friend WeakPtr<T_>::WeakPtr(const SharedPtr<T__>&);

    BaseControlBlock* cb = nullptr;
    T* ptr = nullptr;

    explicit SharedPtr(BaseControlBlock* cb): cb(cb), ptr(static_cast<T*>(cb->get_ptr())) {
        ++cb->shared_count;
    }

public:
    SharedPtr(std::nullptr_t = nullptr) {}

    template<typename T_ = T,
             typename Deleter = std::default_delete<T_>,
             typename Alloc = std::allocator<T_>,
             typename = is_familiar<T, T_>>
    SharedPtr(T_* ptr, const Deleter& deleter = Deleter(), const Alloc& alloc = Alloc()) {
        using AllocCB = typename std::allocator_traits<Alloc>::
                                    template rebind_alloc<ControlBlockDirect<T_, Deleter, Alloc>>;
        AllocCB allocCB = alloc;
        using AllocTraitsCB = std::allocator_traits<AllocCB>;
        auto new_cb = AllocTraitsCB::allocate(allocCB, 1);
//        AllocTraitsCB::construct(allocCB, new_cb, ptr, 1, 0, deleter, std::move(allocCB));
        ::new ((void*)new_cb) ControlBlockDirect<T_, Deleter, Alloc>(ptr, 1, 0, deleter, std::move(allocCB));
        cb = new_cb;
        this->ptr = static_cast<T*>(cb->get_ptr());
    }

    SharedPtr(const SharedPtr<T>& source): cb(source.cb), ptr(source.ptr) {
        if (cb) ++(cb->shared_count);
    }

    template<typename T_, typename = is_familiar<T, T_>>
    SharedPtr(const SharedPtr<T_>& source): cb(source.cb), ptr(source.ptr) {
        if (cb) ++(cb->shared_count);
    }

    template<typename T_>
    template<typename T__, typename>
    friend SharedPtr<T_>::SharedPtr(const SharedPtr<T__>&);

    template<typename T_, typename = is_familiar<T, T_>>
    SharedPtr(SharedPtr<T_>&& source): cb(source.cb), ptr(source.ptr) {
        source.ptr = nullptr;
        source.cb = nullptr;
    }

    template<typename T_>
    template<typename T__, typename>
    friend SharedPtr<T_>::SharedPtr(SharedPtr<T__>&&);

    void swap(SharedPtr<T>& right) {
        std::swap(cb, right.cb);
        std::swap(ptr, right.ptr);
    }

    SharedPtr& operator=(const SharedPtr<T>& source) {
        auto tmp = SharedPtr<T>(source);
        swap(tmp);
        return *this;
    }

    template<typename T_, typename = is_familiar<T, T_>>
    SharedPtr& operator=(const SharedPtr<T_>& source) {
        auto tmp = SharedPtr<T>(source);
        swap(tmp);
        return *this;
    }

    SharedPtr& operator=(SharedPtr<T>&& source) {
        auto tmp = SharedPtr<T>(std::move(source));
        swap(tmp);
        return *this;
    }

    template<typename T_, typename = is_familiar<T, T_>>
    SharedPtr& operator=(SharedPtr<T_>&& source) {
        auto tmp = SharedPtr<T>(std::move(source));
        swap(tmp);
        return *this;
    }

    auto use_count() const {
        return cb ? cb->shared_count : 0;
    }

    void reset() {
        *this = SharedPtr<T>();
    }

    template<typename T_>
    void reset(T_* c_ptr) {
        *this = c_ptr;
    }

    T* get() const {
        return ptr;
    }

    T& operator*() {
        return *ptr;
    }
    const T& operator*() const {
        return *ptr;
    }

    T* operator->() noexcept {
        return ptr;
    }
    const T* operator->() const noexcept {
        return ptr;
    }

    ~SharedPtr() {
        if (!cb) return;
        --cb->shared_count;
        if (cb->shared_count == 0) {
            cb->destroy_object();
            if (cb->weak_count == 0)
                cb->deallocate();
        }
    }
};


template <typename T>
class WeakPtr {
    BaseControlBlock* cb = nullptr;
    T* ptr = nullptr;

public:
    WeakPtr() = default;

    WeakPtr(const SharedPtr<T>& shpr): cb(shpr.cb), ptr(shpr.ptr) {
        if (cb) ++cb->weak_count;
    }

    template<typename T_ = T, typename = is_familiar<T, T_>>
    WeakPtr(const SharedPtr<T_>& shpr): cb(shpr.cb), ptr(shpr.ptr) {
        if (cb) ++cb->weak_count;
    }

    WeakPtr(const WeakPtr<T>& source): cb(source.cb), ptr(source.ptr) {
        if (cb) ++cb->weak_count;
    }

    template<typename T_ = T, typename = is_familiar<T, T_>>
    WeakPtr(const WeakPtr<T_>& source): cb(source.cb), ptr(source.ptr) {
        if (cb) ++cb->weak_count;
    }

    template<typename T_>
    template<typename T__, typename>
    friend WeakPtr<T_>::WeakPtr(const WeakPtr<T__>&);

    WeakPtr(WeakPtr<T>&& source): cb(source.cb), ptr(source.ptr) {
        source.cb = nullptr;
        source.ptr = nullptr;
    }

    template<typename T_ = T, typename = is_familiar<T, T_>>
    WeakPtr(WeakPtr<T_>&& source): cb(source.cb), ptr(source.ptr) {
        source.cb = nullptr;
        source.ptr = nullptr;
    }

    template<typename T_>
    template<typename T__, typename>
    friend WeakPtr<T_>::WeakPtr(WeakPtr<T__>&&);

    void swap(WeakPtr<T>& right) {
        std::swap(cb, right.cb);
        std::swap(ptr, right.ptr);
    }

    template<typename T_>
    WeakPtr& operator=(const SharedPtr<T_>& shpr) {
        auto tmp = WeakPtr<T>(shpr);
        swap(tmp);
        return *this;
    }

    template<typename T_>
    WeakPtr& operator=(const WeakPtr<T_>& source) {
        auto tmp = WeakPtr<T>(source);
        swap(tmp);
        return *this;
    }

    template<typename T_>
    WeakPtr& operator=(WeakPtr<T_>&& source) {
        auto tmp = WeakPtr<T>(std::move(source));
        swap(tmp);
        return *this;
    }

    auto use_count() const {
        return cb ? cb->shared_count : 0;
    }

    bool expired() const {
        return cb && cb->shared_count == 0;
    }

    auto lock() const {
        return !expired() ? SharedPtr<T>(cb) : SharedPtr<T>();
    }

    ~WeakPtr() {
        if (!cb) return;
        --cb->weak_count;
        if (cb->weak_count == 0 && cb->shared_count == 0) {
            cb->deallocate();
        }
    }
};

template <typename T, typename Alloc, typename... Args>
SharedPtr<T> allocateShared(const Alloc& alloc, Args&&... args) {
    using AllocCB = typename std::allocator_traits<Alloc>::template rebind_alloc<ControlBlockShared<T, Alloc>>;
    AllocCB allocCB = alloc;
    using AllocTraitsCB = std::allocator_traits<AllocCB>;
    auto result = AllocTraitsCB::allocate(allocCB, 1);
    AllocTraitsCB::construct(allocCB, result, 0, 0, std::move(allocCB), std::forward<Args>(args)...);
    return SharedPtr<T>(result);
}

template<typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) {
    return allocateShared<T>(std::allocator<T>(), std::forward<Args>(args)...);
}
