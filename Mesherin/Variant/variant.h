#include <iostream>
#include <new>
#include <variant>
//#include <string>

//namespace my {
    template <typename T, typename... Types>
    struct get_index_by_type {
        static const size_t value = 0;
    };

    template <typename T, typename Head, typename... Tail>
    struct get_index_by_type<T, Head, Tail...> {
        static const size_t value = std::is_same<T, Head>::value
            ? 0 : get_index_by_type<T, Tail...>::value + 1;
    };

    template <typename T, typename... Types>
    inline constexpr size_t get_index_by_type_v = get_index_by_type<T, Types...>::value;

    template <size_t index, typename Head, typename... Tail>
    struct type_by_index {
        using type = typename type_by_index<index - 1, Tail...>::type;
    };

    template <typename Head, typename... Tail>
    struct type_by_index<0, Head, Tail...> {
        using type = Head;
    };

    template <size_t index, typename... Types>
    using type_by_index_t = typename type_by_index<index, Types...>::type;

    template<typename... Types>
    class Variant;

    template<typename T, typename... Types>
    struct VariantAlternative {
        using Derived = Variant<Types...>;
        static const size_t index = get_index_by_type<T, Types...>::value;

        auto get_this_ptr() noexcept {
            return static_cast<Derived*>(this);
        }

        VariantAlternative() {}

        template <typename T_>
        void init_by_value(T_&& value) {
            if constexpr (std::is_same<std::remove_const_t<T_>, T_>::value) {
                auto this_ptr = get_this_ptr();
                this_ptr->storage.template put<index, T>(std::forward<T_>(value));
                this_ptr->current = index;
                this_ptr->is_empty = false;
            }
        }

        VariantAlternative(const T& value) {
            init_by_value(value);
        }

        VariantAlternative(T&& value) {
            init_by_value(std::move(value));
        }

        template <typename T_, typename A, typename... B>
        struct constructible_check {
            static const bool value = (!std::is_same<A, T_>::value && std::is_constructible<T_, A>::value)
                    || constructible_check<T_, B...>::value;
        };

        template <typename T_, typename A>
        struct constructible_check<T_, A> {
            static const bool value = !std::is_same<A, T_>::value && std::is_constructible<T_, A>::value;
        };

        template <typename T_, typename = std::enable_if_t<std::is_constructible<T, T_>::value
                && std::is_same<T, std::remove_const_t<T>>::value
                && !constructible_check<T_, Types...>::value>>
        VariantAlternative(T_&& value) {
            init_by_value(std::forward<T_>(value));
        }

        VariantAlternative& operator=(const VariantAlternative&) = delete;
        VariantAlternative& operator=(VariantAlternative&&) = delete;

    private:
        template <typename T_>
        auto& assign_operator(T_&& value, bool) {
            auto this_ptr = get_this_ptr();
            if (this_ptr->current == index)
                this_ptr->storage.template get<index>() = value;
            else {
                auto tmp = Derived(std::forward<T_>(value));
                this_ptr->swap(tmp);
            }
            return *this_ptr;
        }

    public:
        auto& operator=(const T& value) {
            return assign_operator(value, true);
        }

        auto& operator=(T&& value) {
            return assign_operator(std::move(value), true);
        }

        template <typename T_, std::enable_if_t<std::is_constructible<T, T_>::value
                && std::is_same<T, std::remove_const_t<T>>::value
                && !constructible_check<T_, Types...>::value>>
        auto& operator=(T_&& value) {
            return assign_operator(std::forward<T_>(value), true);
        }
    };


    template<typename... Types>
    class Variant: private VariantAlternative<Types, Types...>... {
    private:
        template<typename T, typename... Ts>
        friend struct VariantAlternative;

        template<size_t N, typename... Ts>
        friend auto& get(Variant<Ts...>&);

        template<typename... Ts>
        union VariadicUnion {
            void swap(VariadicUnion&, size_t) noexcept {}

            template <typename VarUnion>
            void copy_VarUnion(VarUnion&&, size_t) {}

            template<size_t index>
            auto& get() {}

            template<size_t N, typename T, typename... Args>
            T& put(Args&&...) {}

            void destroy(size_t) noexcept {}

            ~VariadicUnion() {}
        };

        template<typename Head, typename... Tail>
        union VariadicUnion<Head, Tail...> {
            using NonconstHead = std::remove_const_t<Head>;
            NonconstHead head;
            VariadicUnion<Tail...> tail;

            VariadicUnion() {}

            void swap(VariadicUnion& right, size_t right_current = 0) noexcept {
                if (right_current == 0) {
                    /*if constexpr (std::is_same<std::remove_const_t<Head>, Head>::value) {*/
                        std::swap(head, right.head);
                    /*}*/
                } else
                    tail.swap(right.tail, right_current - 1);
            }

//            template <typename SrcHead>
//            void create_head(SrcHead&& src_head) {
//
//                new (std::launder(&(head))) Head(std::forward<SrcHead>(src_head));
//            }

            template <typename VarUnion>
            void copy_VarUnion(VarUnion&& source, size_t source_current) {
                if (source_current == 0) {
                    /*if constexpr (std::is_same<std::remove_const_t<Head>, Head>::value) {*/
                        new (std::launder(&(head))) NonconstHead(std::forward<VarUnion>(source).head);
//                        create_head(std::forward<VarUnion>(source).head);
                    /*}*/
                } else
                    tail.copy_VarUnion(std::forward<VarUnion>(source).tail, source_current - 1);
            }

            template<size_t N, typename T, typename... Args>
            T& put(Args&&... args) {
                if constexpr (N == 0/* && !std::is_same<const T, T>::value*/) {
                    new (std::launder(&head)) T(std::forward<Args>(args)...);
                    return head;
                } else {
                    return tail.template put<N-1, T>(std::forward<Args>(args)...);
                }
            }

            template<size_t index>
            type_by_index_t<index, Head, Tail...>& get() noexcept { // TODO: move-versions?
                if constexpr (index == 0)
                    return head;
                else
                    return tail.template get<index - 1>();
            }

            template<size_t index>
            const type_by_index_t<index, Head, Tail...>& get() const noexcept {
                if constexpr (index == 0)
                    return head;
                else
                    return tail.template get<index - 1>();
            }

            void destroy(size_t index) noexcept {
                if (index == 0)
                    head.~NonconstHead();
                else
                    tail.destroy(index - 1);
            }

            ~VariadicUnion() {}
        };

        VariadicUnion<Types...> storage;
        size_t current;
        bool is_empty = true;

    public:
        void swap(Variant& right) noexcept {
            storage.swap(right.storage, right.current);
            std::swap(current, right.current);
            std::swap(is_empty, right.is_empty);
        }

        Variant(const Variant& source): current(source.current), is_empty(source.current) {
            storage.copy_VarUnion(source.storage, source.current);
        }

        Variant(Variant&& source): current(source.current), is_empty(source.current) {
            storage.copy_VarUnion(std::move(source).storage, source.current);
        }

//        using VariantAlternative<Types, Types...>::operator=...;
        using VariantAlternative<Types, Types...>::VariantAlternative...;

        Variant& operator=(const Variant& source) {
            auto tmp = Variant(source);
            swap(tmp);
            return *this;
        }

        Variant& operator=(Variant&& source) {
            auto tmp = Variant(std::move(source));
            swap(tmp);
            return *this;
        }

        ~Variant() {
            if (!is_empty)
                storage.destroy(current);
        }

        size_t index() const {
            return current;
        }

        template <size_t index, typename... Args/*,
                typename = std::enable_if_t<std::is_constructible<type_by_index_t<index, Types...>, Args...>::value>*/>
        decltype(auto) emplace(Args&&... args) {
            if (!is_empty) {
                storage.destroy(current);
                is_empty = true;
            }
            return storage.template put<index, type_by_index_t<index, Types...>>(std::forward<Args>(args)...);
        }

        template <size_t index, typename U, typename... Args/*,
                typename = std::enable_if_t<std::is_constructible<type_by_index_t<index, Types...>,
                        std::initializer_list<U>, Args...>::value>*/>
        decltype(auto) emplace(std::initializer_list<U> il, Args&&... args) {
            if (!is_empty) {
                storage.destroy(current);
                is_empty = true;
            }
            return storage.template put<index, type_by_index_t<index, Types...>>(il, std::forward<Args>(args)...);
        }

        template <typename T, typename... Args/*,
                typename = std::enable_if_t<std::is_constructible<T, Args...>::value>*/>
        T& emplace(Args&&... args) {
            return emplace<get_index_by_type_v<T>, Args...>(std::forward<Args>(args)...);
        }

        template <typename T, typename U, typename... Args/*,
                typename = std::enable_if_t<std::is_constructible<T, std::initializer_list<U>, Args...>::value>*/>
        T& emplace(std::initializer_list<U> il, Args&&... args) {
            return emplace<get_index_by_type_v<T>>(il, std::forward<Args>(args)...);
        }

        bool valueless_by_exception() const noexcept {
            return is_empty;
        }
    };

    template<typename T, typename... Types>
    bool holds_alternative(const Variant<Types...>& v)  {
        return get_index_by_type_v<T, Types...> == v.index();
    }

    template <size_t N, typename... Types>
    auto& get(Variant<Types...>& v) {
        if (N != v.index())
            throw std::bad_variant_access();
        return v.storage.template get<N>();
    }

    template <size_t N, typename... Types>
    const auto& get(const Variant<Types...>& v) {
        if (N != v.index())
            throw std::bad_variant_access();
        return v.storage.template get<N>();
    }

    template <size_t N, typename... Types>
    auto&& get(Variant<Types...>&& v) {
        if (N != v.index())
            throw std::bad_variant_access();
        return std::move(v).storage.template get<N>();
    }

    template <size_t N, typename... Types>
    const auto&& get(const Variant<Types...>&& v) {
        if (N != v.index())
            throw std::bad_variant_access();
        return std::move(v).storage.template get<N>();
    }

    template <typename T, typename... Types>
    T& get(Variant<Types...>& v) {
        return get<get_index_by_type_v<T, Types...>>(v);
    }

    template <typename T, typename... Types>
    const T& get(const Variant<Types...>& v) {
        return get<get_index_by_type_v<T, Types...>>(v);
    }

    template <typename T, typename... Types>
    T&& get(Variant<Types...>&& v) {
        return get<get_index_by_type_v<T, Types...>>(std::move(v));
    }

    template <typename T, typename... Types>
    const T&& get(const Variant<Types...>&& v) {
        return get<get_index_by_type_v<T, Types...>>(std::move(v));
    }
//}
