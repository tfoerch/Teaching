// g++-13 --std=c++23 -o ./ExpressionTemplate3rdVersion.gcc ExpressionTemplate3rdVersion.cpp; clang++-15 --std=c++20  -o ./ExpressionTemplate3rdVersion.clang ExpressionTemplate3rdVersion.cpp
#include <ranges>
#include <iostream>
#include <array>
#include <vector>
#include <cassert>
#include <type_traits>


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
struct vector_t : private T {
    using T::begin, T::end; /*...*/
};
static_assert(std::ranges::range< vector_t<std::vector<int>> >);

template <typename T> struct scalar_t { T t{}; /* no begin/end */ };

template<class Expression>
class VecProduct // non-terminal symbol Products
{
public:
  constexpr double operator[](size_t i) const
  { return static_cast<const Expression&>(*this)[i]; }
  constexpr size_t size() const
  { return static_cast<const Expression*>(this)->size(); }
};

template <std::ranges::random_access_range VecValue>
class TerminaVecProduct : public VecProduct<TerminaVecProduct<VecValue>> // rule4
{
public:
  TerminaVecProduct(const VecValue&  vectorValue)
  : m_vectorValue(vectorValue)
  {}
  constexpr double operator[](size_t i) const
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

template <typename Scalar, typename ScarVecProductExpr>
class ScarVecProduct : public VecProduct<ScarVecProduct<Scalar, ScarVecProductExpr>> // rule3
{
public:
  constexpr ScarVecProduct(const Scalar& left, const ScarVecProductExpr& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr double operator[](size_t i) const
  {
    auto iter = std::ranges::next(m_right.begin(), i, m_right.end());
    assert(iter != m_right.end());
    return (m_left * *iter);
  }
  constexpr size_t size() const
  { return m_right.size(); }

private:
  const Scalar&   m_left;
  const ScarVecProductExpr&  m_right;
};

template <typename Scalar, typename ScarVecProductExpr>
constexpr ScarVecProduct<Scalar, ScarVecProductExpr> operator*(const Scalar& left, const ScarVecProductExpr& right)
{
  return
    ScarVecProduct<Scalar, ScarVecProductExpr>(static_cast<const Scalar&>(left), static_cast<const ScarVecProductExpr&>(right));
}

template<class Expression>
class VecSum // non-terminal symbol Sums
{
public:
  constexpr double operator[](size_t i) const
  { return static_cast<const Expression&>(*this)[i]; }
  constexpr size_t size() const
  { return static_cast<const Expression*>(this)->size(); }
};

template <class ScarVecProductExpr>
class UnaryVecSum : public VecSum<UnaryVecSum<ScarVecProductExpr>> // rule2
{
public:
  UnaryVecSum(const ScarVecProductExpr&  product)
  : m_product(product)
  {}
  constexpr double  operator[](size_t i) const
  {
    auto iter = std::ranges::next(m_product.begin(), i, m_product.end());
    assert(iter != m_product.end());
    return *iter;
  }
  constexpr size_t size() const
  { return m_product.size(); }
private:
  const ScarVecProductExpr&  m_product;
};

template <typename SumExpr, typename ScarVecProductExpr>
class BinaryVecSum : public VecSum<BinaryVecSum<SumExpr, ScarVecProductExpr>> // rule1
{
public:
  constexpr BinaryVecSum(const SumExpr& left, const ScarVecProductExpr& right)
  : m_left(left),
    m_right(right)
  {}
  constexpr double operator[](size_t i) const { return m_left[i] + m_right[i]; }
  constexpr size_t size() const
  {
    assert(m_left.size() == m_right.size());
    return m_left.size();
  }
private:
  const SumExpr&      m_left;
  const ScarVecProductExpr&  m_right;
};

template <typename SumExpr, typename ScarVecProductExpr>
constexpr BinaryVecSum<SumExpr, ScarVecProductExpr> operator+(const SumExpr& left, const ScarVecProductExpr& right)
{
  return
      BinaryVecSum<SumExpr, ScarVecProductExpr>(static_cast<const SumExpr&>(left), static_cast<const ScarVecProductExpr&>(right));
}

int main()
{
  double scVal1 = 2.3;
  std::initializer_list<double> vec1 = { 1.2, 0.3, 5.6 };
  double scVal2 = -1.2;
  std::vector<double> vec2 = { 2.4, -1.3, 0.6 };
  const auto scProd = scVal1 * vec1 + scVal2 * vec2;
  assert(scProd.size() == 3);
  for (size_t i = 0; i < scProd.size(); ++i)
  {
    std::cout << scProd[i] << std::endl;
  }
}


