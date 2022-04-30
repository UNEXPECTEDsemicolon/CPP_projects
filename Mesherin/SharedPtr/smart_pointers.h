#include <iostream>
#include <memory>

//template<typename T>
//class SharedPtr;

template <typename T>
class WeakPtr;

template <typename T, typename Alloc, typename... Args>
auto allocateShared(Alloc&& alloc, Args&&... args);

template<typename T, typename... Args>
auto makeShared(Args&&... args);

struct BaseControlBlock {
    size_t shared_count;
    size_t weak_count;

    BaseControlBlock(size_t sc, size_t wc) noexcept: shared_count(sc), weak_count(wc) {}

    virtual void* get_ptr() {
        std::cerr << "WRONG!!!\n";
        return nullptr;
    };

    virtual void destroy_object() {
        std::cerr << "WRONG!!!\n";
    };

    virtual void deallocate() {
        std::cerr << "WRONG!!!\n";
    }

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
        using AllocCB = typename std::allocator_traits<Alloc>::template rebind_alloc<ControlBlockDirect>;
        AllocCB allocCB = alloc;
        std::allocator_traits<AllocCB>::deallocate(allocCB, this, 1); // TODO: rebind alloc
        alloc.~Alloc();
    }

    ~ControlBlockDirect() {
        if (ptr) destroy_object();
    }
};

template<typename T_, typename Alloc>
struct ControlBlockShared: BaseControlBlock {
    T_ object;
    Alloc alloc;

    ControlBlockShared(const T_& object, size_t sc, size_t wc, Alloc&& alloc)
    : BaseControlBlock(sc, wc), object(object), alloc(std::move(alloc)) {}

    void* get_ptr() override {
        return &object;
    }

    void deallocate() override {
        using AllocCB = typename std::allocator_traits<Alloc>::template rebind_alloc<ControlBlockShared>;
        AllocCB allocCB = alloc;
        std::allocator_traits<AllocCB>::deallocate(allocCB, this, 1); // TODO: destroy alloc ?
        alloc.~Alloc();
    }

    void destroy_object() override {
        object.~T_();
    }
};

template<typename T>
class SharedPtr {
private:
    template<typename Alloc, typename... Args>
    friend auto allocateShared(Alloc&& alloc, Args&&... args);

    template<typename... Args>
    friend auto makeShared(Args&&... args);

//    template<typename T_>
//    template<typename std::enable_if_t<std::is_base_of<T, T_>::value && !std::is_same<T, T_>::value, T_>>
//    friend class SharedPtr<T>;

//    template<typename T_, typename std::enable_if_t<std::is_base_of<T, T_>::value && !std::is_same<T, T_>::value>>
    friend WeakPtr<T>;

    template<typename T_>
    template<typename T__, typename>
    friend WeakPtr<T_>::WeakPtr(const SharedPtr<T__>&);

    BaseControlBlock* cb;
    T* ptr;

    SharedPtr(BaseControlBlock* cb): cb(cb), ptr(static_cast<T*>(cb->get_ptr())) {
        ++cb->shared_count;
    }

public:
    template<typename T_ = T,
             typename Deleter = std::default_delete<T_>,
             typename Alloc = std::allocator<T_>,
             typename = std::enable_if_t<std::is_base_of<std::remove_reference<T>, std::remove_reference<T_>>::value>>
    SharedPtr(T_* ptr = nullptr, const Deleter& deleter = Deleter(), const Alloc& alloc = Alloc()) {
        using AllocCB = typename std::allocator_traits<Alloc>::template rebind_alloc<
                /*template*/ ControlBlockDirect<T_, Deleter, Alloc>>; // TODO: make substruct of SharedPtr to avoid code copying
        AllocCB allocCB = alloc;
        using AllocTraitsCB = std::allocator_traits<AllocCB>;
        auto new_cb = AllocTraitsCB::allocate(allocCB, 1);
        AllocTraitsCB::construct(allocCB, new_cb, ptr, 0, 0, deleter, std::move(allocCB));
        cb = new_cb;
        this->ptr = static_cast<T*>(cb->get_ptr());
    }

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    SharedPtr(const SharedPtr<T_>& source): cb(source.cb), ptr(source.ptr) {
        ++cb->shared_count;
    }

    template<typename T_>
    template<typename T__, typename>
    friend SharedPtr<T_>::SharedPtr(const SharedPtr<T__>&);

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
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

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    SharedPtr& operator=(const SharedPtr<T_>& source) {
        auto tmp = SharedPtr<T>(source); // TODO: T or T_ ?
        swap(tmp);
        return *this;
    }

    template<typename T_>
    template<typename T__, typename>
    friend SharedPtr<T_>& SharedPtr<T_>::operator=(const SharedPtr<T__>&);

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    SharedPtr& operator=(SharedPtr<T_>&& source) {
        auto tmp = SharedPtr<T>(std::move(source)); // TODO: T or T_ ?
        swap(tmp);
        return *this;
    }

    template<typename T_>
    template<typename T__, typename>
    friend SharedPtr<T_>& SharedPtr<T_>::operator=(SharedPtr<T__>&&);

    auto use_count() const {
        return cb->shared_count;
    }

    template<typename T_ = T, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    void reset(T_* c_ptr = nullptr) {
        *this = c_ptr;
    }

    T* get() const { // TODO: constness?
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
        if (cb->weak_count == 0 && cb->shared_count == 0) {
            cb->~BaseControlBlock();
        } else if (cb->shared_count == 0) {
            cb->destroy_object();
        }
    }
};

//template<typename T>
//template<typename T_>
//SharedPtr<T>::SharedPtr(const SharedPtr<T_>& source): SharedPtr(source, true) {}

template <typename T>
class WeakPtr {
    BaseControlBlock* cb;
    T* ptr;

public:
    //TODO: make friends

    WeakPtr() = default;

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    WeakPtr(const SharedPtr<T_>& shpr): cb(shpr.cb), ptr(shpr.ptr) {
        ++cb->weak_count;
    }

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    WeakPtr(const WeakPtr<T_>& source): cb(source.cb), ptr(source.ptr) {
        ++cb->weak_count;
    }

    template<typename T_>
    template<typename T__, typename>
    friend WeakPtr<T_>::WeakPtr(const WeakPtr<T__>&);

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
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

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    WeakPtr& operator=(const SharedPtr<T_>& shpr) {
        auto tmp = WeakPtr<T>(shpr);
        swap(tmp);
        return *this;
    }

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    WeakPtr& operator=(const WeakPtr<T_>& source) {
        auto tmp = WeakPtr<T>(source);
        swap(tmp);
        return *this;
    }

    template<typename T_>
    template<typename T__, typename>
    friend WeakPtr<T_>& WeakPtr<T_>::operator=(const WeakPtr<T__>&);

    template<typename T_, typename = std::enable_if_t<std::is_base_of<T, T_>::value>>
    WeakPtr& operator=(WeakPtr<T_>&& source) {
        auto tmp = WeakPtr<T>(std::move(source));
        swap(tmp);
        return *this;
    }

    template<typename T_>
    template<typename T__, typename>
    friend WeakPtr<T_>& WeakPtr<T_>::operator=(WeakPtr<T__>&&);

    bool expired() const {
        return cb && cb->shared_count == 0;
    }

    auto lock() {
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
auto allocateShared(Alloc&& alloc, Args&&... args) {
    using AllocCB = typename std::allocator_traits<Alloc>::template rebind_alloc<
            ControlBlockShared<T, Alloc>>; // TODO: make substruct of SharedPtr to avoid code copying
    AllocCB allocCB = alloc;
    using AllocTraitsCB = std::allocator_traits<AllocCB>;
    auto result = AllocTraitsCB::allocate(allocCB, 1);
    AllocTraitsCB::construct(allocCB, result, T(std::forward<Args>(args)...), 0, 0, std::move(allocCB));
    return SharedPtr<T>(result);
//    return SharedPtr<T>(new SharedPtr<T>::ControlBlockShared<Alloc>(T(std::forward<Args>(args)...), 1, 0, std::forward<Alloc>(alloc)));
}

template<typename T, typename... Args>
auto makeShared(Args&&... args) {
    return allocateShared<T>(std::allocator<T>(), std::forward<Args>(args)...);
}
