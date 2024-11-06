#include <utility>
#include <iostream>

// c++-20 code from https://stackoverflow.com/questions/75133287/c-parameter-pack-expansion-with-concepts
namespace {
template<std::size_t>
struct Dummy { constexpr Dummy(auto&&){}; };// NOLINT(bugprone-forwarding-reference-overload)

#if __cpp_generic_lambdas >= 201707L
template <std::size_t N, typename... Args>
constexpr auto getNthElement(Args&&... args)-> decltype(auto) {
  return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> decltype(auto) {
    return [](Dummy<Is> ..., auto&& arg, auto&&...) -> decltype(auto) {
      return std::forward<decltype(arg)>(arg);
    }(std::forward<Args>(args)...);
  }(std::make_index_sequence<N>());
}
#endif // __cpp_generic_lambdas
} // namespace

auto main() -> int {
#if __cpp_generic_lambdas >= 201707L
    std::cout << "using C++-20 lambda with explicit template parameter list\n";

    static_assert(getNthElement<0>(1, 2, 3) == 1);
    static_assert(getNthElement<1>(1, 2, 3) == 2);
    static_assert(getNthElement<2>(1, 2, 3) == 3);
    
    class A {};
    class B {};
    class C {};
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(getNthElement<0>(A{}, B{}, C{}))>, A>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(getNthElement<1>(A{}, B{}, C{}))>, B>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(getNthElement<2>(A{}, B{}, C{}))>, C>);
    return 0;
#endif // __cpp_generic_lambdas
}
