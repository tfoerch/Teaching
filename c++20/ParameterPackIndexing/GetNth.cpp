#include <utility>

// c++-20 code from https://stackoverflow.com/questions/75133287/c-parameter-pack-expansion-with-concepts
template<std::size_t>
struct dummy { constexpr dummy(auto&&){}; };

template <std::size_t N, typename... Args>
constexpr decltype(auto) nth_element(Args&&... args) {
  return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> decltype(auto) {
    return [](dummy<Is> ..., auto&& arg, auto&&...) -> decltype(auto) {
      return std::forward<decltype(arg)>(arg);
    }(std::forward<Args>(args)...);
  }(std::make_index_sequence<N>());
}

int main()
{
  static_assert(nth_element<0>(1, 2, 3) == 1);
  static_assert(nth_element<1>(1, 2, 3) == 2);
  static_assert(nth_element<2>(1, 2, 3) == 3);
    
  class A {};
  class B {};
  class C {};
  static_assert(std::is_same_v<std::remove_cvref_t<decltype(nth_element<0>(A{}, B{}, C{}))>, A>);
  static_assert(std::is_same_v<std::remove_cvref_t<decltype(nth_element<1>(A{}, B{}, C{}))>, B>);
  static_assert(std::is_same_v<std::remove_cvref_t<decltype(nth_element<2>(A{}, B{}, C{}))>, C>);
  return 0;
}
