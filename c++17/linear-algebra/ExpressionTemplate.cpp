// g++-12 --std=c++20 -o ./ExpressionTemplate.gcc ExpressionTemplate.cpp; clang++-15 --std=c++20  -o ./ExpressionTemplate.clang ExpressionTemplate.cpp
#include <iostream>
#include <array>

template <typename Expr>
class VecExpr
{
public:
  constexpr double operator[](size_t i) const
  { return static_cast<const Expr&>(*this)[i]; }
};

template <std::size_t  dimension>
class Vector : public VecExpr<Vector<dimension>>
{
  std::array<double, dimension> m_elements;
public:
  template<typename ... Types,
           typename std::enable_if<sizeof...(Types) == dimension, int>::type = 0>
  constexpr explicit Vector(Types ... s) : m_elements {static_cast<double>(s)... }
  {}
  constexpr Vector(const std::initializer_list<double>& init)
  : m_elements(conv2Arr(init))
  {}
  constexpr double  operator[](size_t i) const
  { return m_elements[i]; }
  constexpr double& operator[](size_t i)
  { return m_elements[i]; }
private:
  constexpr static std::array<double, dimension> conv2Arr(const std::initializer_list<double>& init)
  {
    std::array<double, dimension> result;
    auto iter = init.begin();
    for (size_t i = 0; i < dimension && iter != init.end(); ++i, ++iter)
    {
      result[i] = *iter;
    }
    return result;
  }
//  template <typename Expr>
//  Vector(const VecExpr<Expr>& expr)
//  : m_elements()
//  {
//    for (size_t i = 0; i < dimension; ++i)
//    {
//      m_elements[i] = expr[i];
//    }
//  }
};

template <typename Expr1, typename Expr2>
class VecSum : public VecExpr<VecSum<Expr1, Expr2> >
{
  const Expr1& m_left;
  const Expr2& m_right;
public:
  constexpr VecSum(const Expr1& left, const Expr2& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr double operator[](size_t i) const { return m_left[i] + m_right[i]; }
};

template <typename Expr1, typename Expr2>
constexpr VecSum<Expr1, Expr2> operator+(const Expr1& left, const Expr2& right)
{
  return
    VecSum<Expr1, Expr2>(static_cast<const Expr1&>(left), static_cast<const Expr2&>(right));
}

int main()
{
  static constexpr std::size_t dim = 3;
  constexpr Vector<dim> v0( 0.3, 3.4, 1.7 );
  constexpr Vector<dim> v1 = { 1.2, 0.3, 5.6 };
  constexpr Vector<dim> v2 = { 2.4, -1.3, 0.6 };
  constexpr Vector<dim> v3 = { 1.4, 2.1, -0.3 };
  const auto sum = v1 + v2 + v3;
//  static_assert(sum[0] == 5, "something wrong");
//  static_assert(sum[1] == 1.1, "something wrong");
//  static_assert(sum[2] == 5.9, "something wrong");
  for (size_t i = 0; i < dim; ++i)
  {
    std::cout << sum[i] << std::endl;
  }
}
