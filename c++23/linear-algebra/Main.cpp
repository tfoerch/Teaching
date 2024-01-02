#include <ranges>
#include <algorithm>
#include <iostream>

void print(std::ranges::viewable_range auto&& v, std::string_view separator = " ")
{
  for (auto&& x : v)
  {
    std::cout << x << separator;
  }
}

void print2D(std::ranges::viewable_range auto&& v)
{
  for (auto&& x : v)
  {
    std::cout << '[';
    print(x);
    std::cout << "]\n";
  }
}

struct InnerProductFtor
{
  template<std::ranges::input_range R>
  constexpr auto operator()(R&& r1, R&& r2) const
  {
    return
      std::ranges::fold_left(std::views::zip_transform(std::multiplies{}, r1, r2), 0., std::plus{});
  }
};

inline constexpr InnerProductFtor inner_product;

struct ExtractNthRow
{
  template<std::ranges::input_range R>
  constexpr auto operator()(R&& r, unsigned int rowNumber, unsigned int width) const
  {
    return
      ( r | std::views::drop(rowNumber) | std::views::stride(width) );
  }
};

inline constexpr ExtractNthRow extract_nth_row;


struct TransposeFtor
{
  template<std::ranges::input_range R>
  constexpr auto operator()(R&& r) const
  {
    auto flat = r | std::views::join;
    print(flat);
    std::cout  << '\n';
    int height = std::ranges::distance(r);
    int width = std::ranges::distance(flat) / height;
//    auto inner = [&flat, width](int i)
//    {
//      auto newRow = flat | std::views::drop(i) | std::views::stride(width);
//      print(newRow);
//      return newRow;
//    };
    return
      ( std::views::iota(0,width) |
        std::views::transform([&flat, width](int i)
                              {
                                return ( flat | std::views::take(i) );
//                                  ( flat |
//                                    std::views::drop(i) |
//                                    std::views::stride(width) );
                              }) );
#if 0
    auto transposed = flat | std::views::chunk(height);
    auto transposedWithIndex =
      std::views::zip(std::views::iota(0, width), transposed);
    print2D(transposed);
    std::cout  << '\n';
    return
      ( std::views::zip(std::views::iota(0, width),
                        ( flat | std::views::chunk(height) ) ) |
        std::views::transform([&r, width](auto&& rowWithIndex)
                              {
                                auto&& [rowIndex, row] = rowWithIndex;
                                std::cout << rowIndex << " : ";
                                print(row);
                                std::cout << '\n';
                                int colIndex = 0;
                                auto rowPosIter = std::ranges::begin(row);
                                auto origRowIter = std::ranges::begin(r);
                                while (rowPosIter != std::ranges::end(row) &&
                                       origRowIter != std::ranges::end(r))
                                {
                                  if (rowIndex != colIndex)
                                  {
                                    const auto& origRow = *origRowIter;
                                    auto origRowPosIter = std::ranges::begin(origRow);
                                    if (rowIndex > 0 &&
                                        origRowPosIter != std::ranges::end(origRow))
                                    {
                                      std::ranges::advance(origRowPosIter, rowIndex);
                                    }
                                    if (origRowPosIter != std::ranges::end(origRow))
                                    {
                                      auto& elem = *rowPosIter;
                                      elem = *origRowPosIter;
                                    }
                                  }
                                  ++rowPosIter;
                                  ++origRowIter;
                                  ++colIndex;
                                }
                                return row;
                              }) );
#endif
  }
};

inline constexpr TransposeFtor transpose;

int main()
{
  std::initializer_list<double> vec1 = { 0.3, 1.2, -0.7 };
  std::initializer_list<double> vec2 = { 1.1, 0.2, 1 };
  auto zipped = std::views::zip(vec1, vec2);
  for (std::pair<const double&, const double&> entry : zipped)
  {
    std::cout << entry.first << ' ' << entry.second << '\n';
  }
  auto products = std::views::zip_transform(std::multiplies{}, vec1, vec2);
  print(products);
  std::cout  << '\n';
  double innerProduct = inner_product(vec1, vec2);
  std::cout << innerProduct << '\n';
//  double inner_product =
//    std::ranges::fold_left(std::views::zip_transform(std::multiplies{}, vec1, vec2), 0., std::plus{});
//  std::cout << inner_product << '\n';

//  std::initializer_list<std::initializer_list<double>> matrix1 =
//    { { 3., 1. },
//      { -1., 0. },
//      { 1., -2. } };
  std::initializer_list<std::initializer_list<double>> matrix2 =
    { { 0., 1., -1. },
      { 1., 0., 2. } };
  print2D(matrix2);
  std::cout  << '\n';

#if 0
  auto transpose = [](auto rng)
  {
    auto flat = rng | std::views::join;
    int height = std::ranges::distance(rng);
    int width = std::ranges::distance(flat) / height;
    auto inner = [&flat, width](int i)
    {
      return flat | std::views::drop(i) | std::views::stride(width);
    };
    return std::views::iota(0,width) | std::views::transform(inner);
  };
#endif

//  auto firstTranspRow = get_nth_row(matrix2, 0, 3);
//  auto firstTranspRow = extract_nth_row((matrix2 | std::views::join), 0, 3);
//  auto firstTranspRow = matrix2 | std::views::join | std::views::drop(0) | std::views::stride(3);
//  print(firstTranspRow);
  const auto transp_matrix2 = transpose(matrix2);
  std::cout  << '\n';
  print2D(transp_matrix2);
//  for (const auto& row : transp_matrix2)
//  {
//    std::cout << '[';
//    for (const auto& elem : row)
//    {
//      std::cout << elem << ", ";
//    }
//    std::cout << "]\n";
//  }
}
