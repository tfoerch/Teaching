// g++-13 --std=c++23 -o ./ExpressionTemplateAsRangeAdaptors.gcc ExpressionTemplateAsRangeAdaptors.cpp; clang++-16 -std=c++2b -o ./ExpressionTemplateAsRangeAdaptors.clang ExpressionTemplateAsRangeAdaptors.cpp
#include <ranges>
#include <vector>
#include <cassert>
#include <type_traits>
#include <iostream>

//template <template<class Arg> class Base>
//concept CRTPBaseArg = std::derived_from<Arg, Base<Arg>>;

template < typename R>
concept RankOneArithmeticRangeIF =
  ( std::floating_point<std::remove_cvref_t<decltype(*std::ranges::begin(std::declval<R&>()))>> &&
    std::floating_point<std::remove_cvref_t<decltype(*std::ranges::end(std::declval<R&>()))>> );

template < typename R>
concept RankOneArithmeticRange =
  ( std::ranges::input_range<R> &&
    RankOneArithmeticRangeIF<R> );


template <RankOneArithmeticRange VecExpression>
struct VecProductExpr : VecExpression
{ using VecExpression::VecExpression; };

template <std::floating_point Scalar, RankOneArithmeticRange ScalarVecProductExpr>
class ScalarVecProductImpl
{
public:
  using value_type = typename ScalarVecProductExpr::value_type;
  using iterator_t = typename ScalarVecProductExpr::iterator;
  constexpr ScalarVecProductImpl(const Scalar& left, const ScalarVecProductExpr& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr auto begin() const
  { return m_right.begin(); }
  constexpr auto end() const
  { return m_right.end(); }
  constexpr auto operator[](size_t i) const
  {
    auto iter = std::ranges::next(begin(), i, end());
   return (m_left * *iter);
  }
  constexpr size_t size() const
  { return m_right.size(); }

private:
  const Scalar&   m_left;
  const ScalarVecProductExpr&  m_right;
};

template <std::floating_point Scalar, RankOneArithmeticRange ScalarVecProductExpr>
using ScalarVecProduct = VecProductExpr<ScalarVecProductImpl<Scalar, ScalarVecProductExpr>>;

template <std::floating_point Scalar, RankOneArithmeticRange ScalarVecProductExpr>
constexpr ScalarVecProduct<Scalar, ScalarVecProductExpr> operator*(const Scalar& left, const ScalarVecProductExpr& right)
{
  return
    ScalarVecProduct<Scalar, ScalarVecProductExpr>(left, right);
}

int main()
{
  std::floating_point auto scVal1 = 2.3;
  std::initializer_list<double> vec1 = { 1.2, 0.3, 5.6 };
  static_assert(RankOneArithmeticRange<decltype(vec1)>);
  static_assert(RankOneArithmeticRange<ScalarVecProduct<double, std::initializer_list<double> >>);
  static_assert(RankOneArithmeticRange<decltype(scVal1 * vec1)>);
  static_assert(std::is_same_v<VecProductExpr<ScalarVecProductImpl<double, std::initializer_list<double>>>, decltype(scVal1 * vec1)>);
  const RankOneArithmeticRange auto& scProd = scVal1 * vec1;
  for (const auto& coord : scProd)
  {
    std::cout << coord << '\n';
  }
  // static_assert(RankOneArithmeticRange<decltype(ScalarVecProduct<decltype(scVal1), decltype(vec1)>(static_cast<const decltype(scVal1)&>(scVal1), static_cast<const decltype(vec1)&>(vec1)))>);
#if 0
  const auto scProd1 = scVal1 * vec1 + scVal2 * vec2;
  assert(scProd1.size() == 3);
  for (size_t i = 0; i < scProd1.size(); ++i)
  {
    std::cout << scProd1[i] << std::endl;
  }
  const auto scProd2 = scVal1 * vec1 + vec2;
  assert(scProd2.size() == 3);
  for (size_t i = 0; i < scProd2.size(); ++i)
  {
    std::cout << scProd2[i] << std::endl;
  }
#endif
}


