// g++-13 --std=c++23 -o ./ExpressionTemplateAsRangeAdaptors.gcc ExpressionTemplateAsRangeAdaptors.cpp; clang++-16 -std=c++2b -o ./ExpressionTemplateAsRangeAdaptors.clang ExpressionTemplateAsRangeAdaptors.cpp
#include <ranges>
#include <iostream>
#include <array>
#include <vector>
#include <cassert>
#include <type_traits>
#include <complex>


// grammar https://en.wikipedia.org/wiki/LR_parser
//rule0: Expression → Sums eof
//rule1: Sums → Sums + Products
//rule2: Sums → Products
//rule3: Products → Products * Value
//rule4: Products → Value
//rule5: Value → int
//rule6: Value → id
//
// VecProduct ::= Scalar * VecProduct
// VecProduct ::= VecValue



template <typename T>
concept ComplexArithmetic = requires(T t)
{
  { std::real(t) } -> std::floating_point;
  { std::imag(t) } -> std::floating_point;
};

template < typename T>
concept ScalarArithmetic =
  ( std::floating_point<T> || ComplexArithmetic<T> );

template < typename R>
concept RankOneArithmeticRange =
  ( std::ranges::input_range<R> &&
    ScalarArithmetic<decltype(*std::ranges::begin(std::declval<R&>()))> &&
    ScalarArithmetic<decltype(*std::ranges::end(std::declval<R&>()))> );


template <RankOneArithmeticRange VecExpression>
struct VectorExpr : VecExpression
{ using VecExpression::VecExpression; };

template <ScalarArithmetic Scalar, RankOneArithmeticRange ScalarVecProductExpr>
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

template <ScalarArithmetic Scalar, RankOneArithmeticRange ScalarVecProductExpr>
using ScalarVecProduct = VectorExpr<ScalarVecProductImpl<Scalar, ScalarVecProductExpr>>;

template <ScalarArithmetic Scalar, RankOneArithmeticRange ScalarVecProductExpr>
constexpr ScalarVecProduct<Scalar, ScalarVecProductExpr> operator*(const Scalar& left, const ScalarVecProductExpr& right)
{
  return
    ScalarVecProduct<Scalar, ScalarVecProductExpr>(left, right);
}

template <RankOneArithmeticRange SumExprLeft, RankOneArithmeticRange SumExprRight>
class VectorSumImpl
{
public:
  using left_value_type = typename SumExprLeft::value_type;
  using left_iterator_t = typename SumExprLeft::iterator;
  using right_value_type = typename SumExprRight::value_type;
  using right_iterator_t = typename SumExprRight::iterator;
  using value_type = decltype(std::declval<left_value_type>() + std::declval<right_value_type>());
  class VecSumIterator;
  using iterator_t = VecSumIterator;
  constexpr VectorSumImpl(const SumExprLeft& left, const SumExprRight& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr auto operator[](size_t i) const { return m_left[i] + m_right[i]; }
  constexpr size_t size() const
  {
    assert(m_left.size() == m_right.size());
    return m_left.size();
  }
private:
  const SumExprLeft&   m_left;
  const SumExprRight&  m_right;
};

template <RankOneArithmeticRange SumExprLeft, RankOneArithmeticRange SumExprRight>
using VectorSum = VectorExpr<VectorSumImpl<SumExprLeft, SumExprRight>>;

template <RankOneArithmeticRange SumExprLeft, RankOneArithmeticRange SumExprRight>
constexpr VectorSum<SumExprLeft, SumExprRight> operator+(const SumExprLeft& left, const SumExprRight& right)
{
  return
    VectorSum<SumExprLeft, SumExprRight>(left, right);
}

#if 0
template<std::ranges::input_range R> requires std::ranges::view<R>
class custom_take_view : public std::ranges::view_interface<custom_take_view<R>>
{
private:
  R                                         base_ {};
  std::iter_difference_t<std::ranges::iterator_t<R>> count_ {};
  std::ranges::iterator_t<R>                         iter_ {std::begin(base_)};
public:
  custom_take_view() = default;

  constexpr custom_take_view(R base, std::iter_difference_t<std::ranges::iterator_t<R>> count)
  : base_(base)
  , count_(count)
  , iter_(std::begin(base_))
  {}

  constexpr R base() const &
  { return base_; }
  constexpr R base() &&
  { return std::move(base_); }

  constexpr auto begin() const
  { return iter_; }
  constexpr auto end() const
  { return std::next(iter_, count_); }

  constexpr auto size() const requires std::ranges::sized_range<const R>
  {
    const auto s = std::ranges::size(base_);
    const auto c = static_cast<decltype(s)>(count_);
    return s < c ? 0 : s - c;
  }
};

template<class R>
custom_take_view(R&& base, std::iter_difference_t<std::ranges::iterator_t<R>>)
-> custom_take_view<std::ranges::views::all_t<R>>;



template <RankOneArithmeticRangeIF VecExpression>
class VecProductExpr
{
public:
//  using value_type = typename VecExpression::value_type;
  constexpr auto begin() const
  { return static_cast<const VecExpression&>(*this).begin(); }
  constexpr auto end() const
  { return static_cast<const VecExpression&>(*this).end(); }
  constexpr auto operator[](size_t i) const requires std::ranges::random_access_range<const VecExpression>
  { return static_cast<const VecExpression&>(*this)[i]; }
  constexpr size_t size() const requires std::ranges::sized_range<const VecExpression>
  { return static_cast<const VecExpression*>(this)->size(); }
};

template <ScalarArithmetic Scalar, RankOneArithmeticRange ScalarVecProductExpr>
class ScalarVecProduct : public VecProductExpr<ScalarVecProduct<Scalar, ScalarVecProductExpr>> // rule3
{
public:
  using value_type = typename ScalarVecProductExpr::value_type;
  using iterator_t = typename ScalarVecProductExpr::iterator;
  constexpr ScalarVecProduct(const Scalar& left, const ScalarVecProductExpr& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr auto begin() const
  { return m_right.begin(); }
  constexpr auto end() const
  { return m_right.end(); }
//  constexpr auto begin()
//  { return m_right.begin(); }
//  constexpr auto end()
//  { return m_right.end(); }
  constexpr auto operator[](size_t i) const
  {
    std::cout << "operator[" << i << "]" << std::endl;
    auto iter = std::ranges::next(begin(), i, end());
    std::cout << "operator[" << i << "] before assert" << std::endl;
    assert(iter != end());
    std::cout << "operator[" << i << "] after assert" << std::endl;
   return (m_left * *iter);
  }
  constexpr size_t size() const
  { return m_right.size(); }

private:
  const Scalar&   m_left;
  const ScalarVecProductExpr&  m_right;
};

template <RankOneArithmeticRange VecValue>
class TerminalVecProduct : public VecProductExpr<TerminalVecProduct<VecValue>> // rule4
{
public:
  TerminalVecProduct(const VecValue&  vectorValue)
  : m_vectorValue(vectorValue)
  {}
  constexpr auto operator[](size_t i) const
  {
    auto iter = std::ranges::next(m_vectorValue.begin(), i, m_vectorValue.end());
    assert(iter != m_vectorValue.end());
    return *iter;
  }
  constexpr size_t size() const
  { return m_vectorValue.size(); }
private:
  const VecValue&  m_vectorValue;
};

template <ScalarArithmetic Scalar, RankOneArithmeticRange ScalarVecProductExpr>
constexpr ScalarVecProduct<Scalar, ScalarVecProductExpr> operator*(const Scalar& left, const ScalarVecProductExpr& right)
{
  return
    ScalarVecProduct<Scalar, ScalarVecProductExpr>(static_cast<const Scalar&>(left), static_cast<const ScalarVecProductExpr&>(right));
}

template<class VecExpression>
class VecSumExpr // non-terminal symbol Sums
{
public:
  constexpr auto operator[](size_t i) const
  { return static_cast<const VecExpression&>(*this)[i]; }
  constexpr size_t size() const
  { return static_cast<const VecExpression*>(this)->size(); }
};

template <RankOneArithmeticRange ScalarVecProductExpr>
class UnaryVecSum : public VecSumExpr<UnaryVecSum<ScalarVecProductExpr>> // rule2
{
public:
  UnaryVecSum(const ScalarVecProductExpr&  product)
  : m_product(product)
  {}
  constexpr auto operator[](size_t i) const
  {
    auto iter = std::ranges::next(m_product.begin(), i, m_product.end());
    assert(iter != m_product.end());
    return *iter;
  }
  constexpr size_t size() const
  { return m_product.size(); }
private:
  const ScalarVecProductExpr&  m_product;
};

template <RankOneArithmeticRange SumExpr, RankOneArithmeticRange ScalarVecProductExpr>
class BinaryVecSum : public VecSumExpr<BinaryVecSum<SumExpr, ScalarVecProductExpr>> // rule1
{
public:
  constexpr BinaryVecSum(const SumExpr& left, const ScalarVecProductExpr& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr auto operator[](size_t i) const { return m_left[i] + m_right[i]; }
  constexpr size_t size() const
  {
    assert(m_left.size() == m_right.size());
    return m_left.size();
  }
private:
  const SumExpr&      m_left;
  const ScalarVecProductExpr&  m_right;
};

template <typename SumExpr, RankOneArithmeticRange ScalarVecProductExpr>
constexpr BinaryVecSum<SumExpr, ScalarVecProductExpr> operator+(const SumExpr& left, const ScalarVecProductExpr& right)
{
  return
    BinaryVecSum<SumExpr, ScalarVecProductExpr>(static_cast<const SumExpr&>(left), static_cast<const ScalarVecProductExpr&>(right));
}
#endif

int main()
{
  using namespace std::complex_literals;

  ScalarArithmetic auto scVal1 = 2.3;
  std::initializer_list<double> vec1 = { 1.2, 0.3, 5.6 };
  static_assert(RankOneArithmeticRange<decltype(vec1)>);
  double scVal2 = -1.2;
  ScalarArithmetic auto scVal3 = 1.2 + 0.5i;
  std::vector<double> vec2 = { 2.4, -1.3, 0.6 };
  static_assert(RankOneArithmeticRange<decltype(vec2)>);
  static_assert(RankOneArithmeticRange<ScalarVecProduct<double, std::initializer_list<double> >>);
  static_assert(RankOneArithmeticRange<decltype(scVal1 * vec1)>);
  static_assert(RankOneArithmeticRange<decltype(ScalarVecProduct<decltype(scVal1), decltype(vec1)>(scVal1, vec1))>);
  static_assert(RankOneArithmeticRange<decltype(vec1 + vec2)>);
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
  const auto scProd3 = scVal1 * vec1 + scVal3 * vec2;
  const size_t scProd3Size = scProd3.size();
  assert(scProd3Size == 3);
  for (size_t i = 0; i < scProd3Size; ++i)
  {
    std::cout << scProd3[i] << std::endl;
  }
#endif
}


