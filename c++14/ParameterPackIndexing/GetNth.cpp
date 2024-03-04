#include <type_traits>
#include <iostream>

template <std::size_t I, typename T, typename ...Ts>
struct nth_element_impl {
  using type = typename nth_element_impl<I-1, Ts...>::type;
};

template <typename T, typename ...Ts>
struct nth_element_impl<0, T, Ts...> {
  using type = T;
};

template <std::size_t I, typename ...Ts>
using nth_element = typename nth_element_impl<I, Ts...>::type;

template <std::size_t I>
struct NThElement {
  template <typename T, typename ...Ts>
#if __cpp_decltype_auto
  constexpr decltype(auto) operator()(T&&, Ts&&...args) const {
#else
  constexpr auto operator()(T&&, Ts&&...args) const -> decltype(NThElement<I-1>{}(std::forward<decltype(args)>(args)...)) {
#endif
    return NThElement<I-1>{}(std::forward<decltype(args)>(args)...);
  }
};

template <>
struct NThElement<0> {
  template <typename T, typename ...Ts>
#if __cpp_decltype_auto
  constexpr decltype(auto) operator()(T&& arg, Ts&&...) const {
#else
  constexpr auto operator()(T&& arg, Ts&&...) const -> decltype(arg) {
#endif
    return std::forward<decltype(arg)>(arg);
  }
};

template <std::size_t I, typename ...Ts>
#if __cpp_decltype_auto
constexpr decltype(auto) getNThElement(Ts&&...args) {
#else
constexpr auto getNThElement(Ts&&...args) -> decltype(NThElement<I>{}(std::forward<decltype(args)>(args)...)) {
#endif
  return NThElement<I>{}(std::forward<decltype(args)>(args)...);
}


#if __cpp_generic_lambdas >= 201707L && !__cpp_pack_indexing
template<std::size_t>
struct dummy { constexpr dummy(auto&&){}; };
#endif // __cpp_pack_indexing 

#if __cpp_decltype_auto && __cpp_generic_lambdas >= 201707L
template <std::size_t N, typename... Args>
constexpr decltype(auto) nth_element(Args&&... args) {
#if __cpp_pack_indexing
    return std::forward<Args...[N]>(args...[N]);
#elif __cpp_generic_lambdas
    return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> decltype(auto) {
        return [](dummy<Is> ..., auto&& arg, auto&&...) -> decltype(auto) {
            return std::forward<decltype(arg)>(arg);
        }(std::forward<Args>(args)...);
    }(std::make_index_sequence<N>());
#endif // __cpp_pack_indexing
}
#endif

int main()
{
  class A {};
  class B {};
  class C {};
  using one_t = std::integral_constant<int, 1>;
  using two_t = std::integral_constant<int, 2>;
  using three_t = std::integral_constant<int, 3>;
#if __cpp_pack_indexing
    std::cout << "using C++-26 pack indexing\n";
#elif __cpp_generic_lambdas >= 201707L
    std::cout << "using C++-20 lambda with explicit template parameter list\n";
#elif __cpp_decltype_auto
    std::cout << "using C++14 return type deduction for normal function with decltype(auto)\n";
#endif // __cpp_pack_indexing

#if __cpp_pack_indexing || __cpp_generic_lambdas >= 201707L
    static_assert(nth_element<0>(1, 2, 3) == 1);
    static_assert(nth_element<1>(1, 2, 3) == 2);
    static_assert(nth_element<2>(1, 2, 3) == 3);
    
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(nth_element<1>(A{}, B{}, C{}))>, B>);
#else
    static_assert(std::is_same<nth_element<0, A, B, C>, A>::value, "");
    static_assert(std::is_same<nth_element<1, A, B, C>, B>::value, "");
    static_assert(std::is_same<nth_element<2, A, B, C>, C>::value, "");
    static_assert(nth_element<0, one_t, two_t, three_t>::value == 1, "");
    static_assert(nth_element<1, one_t, two_t, three_t>::value == 2, "");
    static_assert(nth_element<2, one_t, two_t, three_t>::value == 3, "");
    static_assert(getNThElement<0>(1, 2, 3) == 1, "");
#endif
    return 0;
}
