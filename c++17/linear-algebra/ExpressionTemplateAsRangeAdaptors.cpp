// g++-13 --std=c++23 -o ./ExpressionTemplateAsRangeAdaptors.gcc ExpressionTemplateAsRangeAdaptors.cpp; clang++-16 -std=c++2b -o ./ExpressionTemplateAsRangeAdaptors.clang ExpressionTemplateAsRangeAdaptors.cpp
#include <ranges>
#include <iostream>
#include <array>
#include <vector>
#include <cassert>
#include <type_traits>
#include <algorithm>
#include <complex>
#include <valgrind/callgrind.h>


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
    std::same_as<decltype(*std::ranges::begin(std::declval<R&>())), decltype(*std::ranges::end(std::declval<R&>()))> &&
    ScalarArithmetic<std::remove_cvref_t<decltype(*std::ranges::begin(std::declval<R&>()))>> &&
    ScalarArithmetic<std::remove_cvref_t<decltype(*std::ranges::end(std::declval<R&>()))>> );


template <RankOneArithmeticRange VecExpression>
struct VectorExpr : VecExpression
{ using VecExpression::VecExpression;
  using VecExpression::begin;
  using VecExpression::end; };

template <ScalarArithmetic Scalar, RankOneArithmeticRange ScalarVecProductExpr>
class ScalarVecProductImpl
{
public:
  using right_value_type = typename ScalarVecProductExpr::value_type;
  using right_const_iterator_t = typename ScalarVecProductExpr::const_iterator;
  using value_type = decltype(std::declval<Scalar>() + std::declval<right_value_type>());
  class VecProdIterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = ScalarVecProductImpl::value_type;
    using pointer           = const value_type*;  // or also value_type*
    using reference         = const value_type&;  // or also value_type&
    VecProdIterator(const Scalar&          left, const right_const_iterator_t&  rightIter)
    : m_left(left),
      m_rightIter(rightIter)
    {}
    VecProdIterator() = default;
    reference operator*() const { static value_type tmp{}; tmp = (m_left * *m_rightIter); return tmp; }
    pointer operator->() { static value_type tmp{}; tmp = (m_left * *m_rightIter); return &tmp; }

    // Prefix increment
    VecProdIterator& operator++() { m_rightIter++; return *this; }

    // Postfix increment
    VecProdIterator operator++(int) { VecProdIterator tmp = *this; ++(*this); return tmp; }
    bool operator==(const VecProdIterator&  other) const
    {
      return
        ( m_rightIter == other.m_rightIter );
    }
  private:
    Scalar                 m_left;
    right_const_iterator_t m_rightIter;
  };
  using const_iterator = VecProdIterator;
  constexpr ScalarVecProductImpl(const Scalar& left, const ScalarVecProductExpr& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr auto begin() const
  { return VecProdIterator(m_left, m_right.begin()); }
  constexpr auto end() const
  { return VecProdIterator(m_left, m_right.end()); }
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
  using left_const_iterator_t = typename SumExprLeft::const_iterator;
  using right_value_type = typename SumExprRight::value_type;
  using right_const_iterator_t = typename SumExprRight::const_iterator;
  using value_type = decltype(std::declval<left_value_type>() + std::declval<right_value_type>());
  class VecSumIterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = VectorSumImpl::value_type;
    using pointer           = const value_type*;  // or also value_type*
    using reference         = const value_type&;  // or also value_type&
    VecSumIterator(const left_const_iterator_t& leftIter, const right_const_iterator_t&  rightIter)
    : m_leftIter(leftIter),
      m_rightIter(rightIter)
    {}
    VecSumIterator() = default;
    reference operator*() const { static value_type tmp{}; tmp = (*m_leftIter + *m_rightIter); return tmp; }
    pointer operator->() { static value_type tmp{}; tmp = (*m_leftIter + *m_rightIter); return &tmp; }

    // Prefix increment
    VecSumIterator& operator++() { m_leftIter++; m_rightIter++; return *this; }

    // Postfix increment
    VecSumIterator operator++(int) { VecSumIterator tmp = *this; ++(*this); return tmp; }
    bool operator==(const VecSumIterator&  other) const
    {
      return
        ( m_leftIter == other.m_leftIter &&
          m_rightIter == other.m_rightIter );
    }
  private:
    left_const_iterator_t  m_leftIter;
    right_const_iterator_t m_rightIter;
  };
  using const_iterator = VecSumIterator;

  constexpr VectorSumImpl(const SumExprLeft& left, const SumExprRight& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr auto begin() const
  { return VecSumIterator(m_left.begin(), m_right.begin()); }
  constexpr auto end() const
  { return VecSumIterator(m_left.end(), m_right.end()); }
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


int main()
{
  using namespace std::complex_literals;

  ScalarArithmetic auto scVal1 = 2.3;
  std::initializer_list<double> vec1 = { 1.2, 0.3, 5.6 };
  static_assert(RankOneArithmeticRange<decltype(vec1)>);
  const ScalarArithmetic auto& scVal1Ref = scVal1;
  static_assert(ScalarArithmetic<std::remove_cvref_t<decltype(scVal1Ref)>>);
  double scVal2 = -1.2;
  //ScalarArithmetic auto scVal3 = 1.2 + 0.5i;
  std::vector<double> vec2 = { 2.4, -1.3, 0.6 };
  std::initializer_list<double> exp_sumOfScalProd =
    { scVal1 * *vec1.begin() + scVal2 * vec2[0],
      scVal1 * *std::next(vec1.begin()) + scVal2 * vec2[1],
      scVal1 * *std::next(vec1.begin(), 2) + scVal2 * vec2[2] };
  static_assert(RankOneArithmeticRange<decltype(vec2)>);
  static_assert(RankOneArithmeticRange<ScalarVecProduct<double, std::initializer_list<double> >>);
  static_assert(RankOneArithmeticRange<decltype(scVal1 * vec1)>);
  static_assert(RankOneArithmeticRange<decltype(ScalarVecProduct<decltype(scVal1), decltype(vec1)>(scVal1, vec1))>);
  VectorSum<std::initializer_list<double>, std::vector<double>>  vSum(vec1, vec2);

  static_assert(RankOneArithmeticRange<decltype(vec1 + vec2)>);
  CALLGRIND_START_INSTRUMENTATION;
  CALLGRIND_TOGGLE_COLLECT;
  const auto scProd1 = scVal1 * vec1 + scVal2 * vec2;
  assert(std::ranges::equal(scProd1, exp_sumOfScalProd));
  CALLGRIND_TOGGLE_COLLECT;
  CALLGRIND_STOP_INSTRUMENTATION;

  assert(std::ranges::equal(exp_sumOfScalProd, scProd1));
  auto iter = scProd1.begin();
  const auto endIter = scProd1.end();
  assert(iter != endIter);
  assert(*iter == (2.3 * 1.2 + -1.2 * 2.4));
  iter++;
  assert(iter != endIter);
  assert(*iter == (2.3 * 0.3 + -1.2 * -1.3));
  iter++;
  assert(iter != endIter);
  assert(*iter == (2.3 * 5.6 + -1.2 * 0.6));
  iter++;
  assert(iter == endIter);
#if 0
  assert(std::distance(scProd1.begin(), scProd1.end()) == 3);
  for (const auto& coord : scProd1)
  {
    std::cout << coord << '\n';
  }
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


