#include <utility>
#include <iostream>

// #define __cpp_pack_indexing 1 // work-around until feature testing is also working
#if !__cpp_pack_indexing
template<std::size_t>
struct dummy { constexpr dummy(auto&&){}; };
#endif // __cpp_pack_indexing 

template <std::size_t N, typename... Args>
constexpr decltype(auto) nth_element(Args&&... args) {
#if __cpp_pack_indexing
    return std::forward<Args...[N]>(args...[N]);
#else
    return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> decltype(auto) {
        return [](dummy<Is> ..., auto&& arg, auto&&...) -> decltype(auto) {
            return std::forward<decltype(arg)>(arg);
        }(std::forward<Args>(args)...);
    }(std::make_index_sequence<N>());
#endif // __cpp_pack_indexing
}

int main()
{
#if __cpp_pack_indexing
    std::cout << "using C++-26 pack indexing\n";
#else
    std::cout << "using C++-20 lambda with explicit template parameter list\n";
#endif // __cpp_pack_indexing

    static_assert(nth_element<0>(1, 2, 3) == 1);
    static_assert(nth_element<1>(1, 2, 3) == 2);
    static_assert(nth_element<2>(1, 2, 3) == 3);
    
    class A {};
    class B {};
    class C {};
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(nth_element<1>(A{}, B{}, C{}))>, B>);
    return 0;
}
