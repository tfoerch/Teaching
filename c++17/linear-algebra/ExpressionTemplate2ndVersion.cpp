// g++-12 --std=c++20 -o ./ExpressionTemplate.gcc ExpressionTemplate.cpp; clang++-15 --std=c++20  -o ./ExpressionTemplate.clang ExpressionTemplate.cpp
#include <iostream>
#include <array>
#include <vector>
#include <cassert>
#include <type_traits>

template<class T, class U, class Callable>
struct BinaryContainerExpression
{
    template<class Func>
    BinaryContainerExpression(const T& _left, const U& _right, Func&& _callable) :
    left_{&_left},
    right_{&_right},
    callable_{std::forward<Func>(_callable)}
    {
        assert(_left.size() == _right.size());
    }

    auto operator[](size_t index) const
    {
        return callable_((*left_)[index], (*right_)[index]);
    }

    size_t size() const
    {
        return left_->size();
    }

    const T* left_= nullptr;
    const U* right_= nullptr;
    Callable callable_;
};
template<class T, class U, class Func>
BinaryContainerExpression(const T&, const U&, Func&&) -> BinaryContainerExpression<T, U, Func>;


int main()
{
    std::vector<int> left_vals{1, 2, 3};
    std::vector<int> right_vals{1, 1, 1};
    auto add = [](int lhs, int rhs){return lhs + rhs;};
    auto add_vectors = BinaryContainerExpression(left_vals, right_vals, add);

    // print 2 3 4
    for(size_t i = 0; i < add_vectors.size(); ++i)
        std::cout << add_vectors[i] << std::endl;
}
