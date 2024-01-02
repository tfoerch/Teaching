// g++-12 --std=c++98 -o ./Inheritance-c++98.gcc Inheritance-c++98.cpp; clang++-14 --std=c++98  -o ./Inheritance-c++98.clang Inheritance-c++98.cpp
#include <vector>
#include <iostream>
#include <utility>
#include <numeric>
#include <cstdlib>
#include <time.h>
#include <valgrind/callgrind.h>

class Canvas;

const std::size_t numDimensions = 2;
typedef double Coordinate2D[numDimensions];

template <typename T, std::size_t  N>
T* assign(T const  (&source)[N],
          T        (&destination)[N]);


class Shape
{
public:
  virtual ~Shape() {};
  inline void draw(Canvas&  canvas) const
  { doDraw(canvas); }
  inline std::pair<Coordinate2D, Coordinate2D> getSurroundingRectangleCorners() const
  { return doGetSurroundingRectangleCorners(); }
  inline const Coordinate2D& getPosition() const
  { return doGetPosition(); }
  inline double calcSurfaceArea() const
  { return doCalcSurfaceArea(); }
  inline double calcCircumference() const
  { return doCalcCircumference(); }
private:
  virtual void doDraw(Canvas&  canvas) const = 0;
  virtual std::pair<Coordinate2D, Coordinate2D> doGetSurroundingRectangleCorners() const = 0;
  virtual const Coordinate2D& doGetPosition() const = 0;
  virtual double doCalcSurfaceArea() const = 0;
  virtual double doCalcCircumference() const = 0;
};

class Circle : public Shape
{
public:
  Circle(
    const Coordinate2D&  position,
    const double&        radius)
  : m_position(),
    m_radius(radius)
  {
    assign(position, m_position);
  }
  inline double getRadius() const { return m_radius; }
private:
  const Coordinate2D& doGetPosition() const { return m_position; }
  std::pair<Coordinate2D, Coordinate2D> doGetSurroundingRectangleCorners() const
  {
    std::pair<Coordinate2D, Coordinate2D> result;
    result.first[0] = m_position[0] - m_radius;
    result.first[1] = m_position[1] - m_radius;
    result.second[0] = m_position[0] + m_radius;
    result.second[1] = m_position[1] + m_radius;
    return result;
  }
  void doDraw(Canvas&  canvas) const;
  double doCalcSurfaceArea() const
  {
    return ( pi * m_radius * m_radius );
  }
  double doCalcCircumference() const
  {
    return
      ( 2 * pi * m_radius );
  }
  Coordinate2D  m_position; // center
  double        m_radius;
  static const double pi; // 3.14159265358979323846;
};

class Square : public Shape
{
public:
  Square(
    const Coordinate2D&  position,
    const double&        sideLength)
  : m_position(),
    m_sideLength(sideLength)
  {
    assign(position, m_position);
  }
  inline double getSideLength() const { return m_sideLength; }
private:
  const Coordinate2D& doGetPosition() const { return m_position; }
  std::pair<Coordinate2D, Coordinate2D> doGetSurroundingRectangleCorners() const
  {
    std::pair<Coordinate2D, Coordinate2D> result;
    assign(m_position, result.first);
    result.second[0] = m_position[0] + m_sideLength;
    result.second[1] = m_position[1] + m_sideLength;
    return result;
  }
  void doDraw(Canvas&  canvas) const;
  double doCalcSurfaceArea() const
  {
    return (m_sideLength  * m_sideLength );
  }
  double doCalcCircumference() const
  {
    return ( 4 * m_sideLength );
  }
  Coordinate2D  m_position; // lower left corner
  double        m_sideLength;
};

class Canvas
{
public:
  explicit Canvas(
    std::ostream&             os);
  void draw(const Circle& cirlce);
  void draw(const Square& square);
private:
  std::ostream&             m_os;
};

const double Circle::pi = 3.14159265358979323846;
void Circle::doDraw(Canvas&  canvas) const
{ canvas.draw(*this); }

void Square::doDraw(Canvas&  canvas) const
{ canvas.draw(*this); }

template <class CharT, class Traits, typename T, std::size_t N, std::size_t  position>
struct ArrayPrinter
{
  inline void operator()(
    std::basic_ostream<CharT, Traits>&  os,
    T const                             (&arr)[N])
  {
    os << arr[position] << ", ";
    ArrayPrinter<CharT, Traits, T, N, position + 1>()(os, arr);
  }
};

template <class CharT, class Traits, typename T>
struct ArrayPrinter<CharT, Traits, T, 0, 0>
{
  inline void operator()(
    std::basic_ostream<CharT, Traits>&  os,
    T const                             (&arr)[0])
  {}
};

template <class CharT, class Traits, typename T, std::size_t  N>
struct ArrayPrinter<CharT, Traits, T, N, N - 1>
{
  inline void operator()(
    std::basic_ostream<CharT, Traits>&  os,
    T const                             (&arr)[N])
  {
    os << arr[N -1];
  }
};

// restrict to C-style arrays of double type, otherwise operator<<(..., char[N]) is ambiguous
template<class CharT, class Traits, std::size_t N>
std::basic_ostream<CharT,Traits>& operator<<(
  std::basic_ostream<CharT,Traits>&  os,
  double const                       (&arr)[N])
{
  os << '<';
  ArrayPrinter<CharT, Traits, double, N, 0>()(os, arr);
  os << '>';
  return os;
}


template <typename T, std::size_t N, std::size_t  position>
struct ArrayAssigner
{
  inline void operator()(
    T const  (&source)[N],
    T        (&destination)[N])
  {
    destination[position] = source[position];
    ArrayAssigner<T, N, position + 1>()(source, destination);
  }
};

template <typename T>
struct ArrayAssigner<T, 0, 0>
{
  inline void operator()(
    T const  (&source)[0],
    T        (&destination)[0])
  {}
};

template <typename T, std::size_t  N>
struct ArrayAssigner<T, N, N - 1>
{
  inline void operator()(
    T const  (&source)[N],
    T        (&destination)[N])
  {
    destination[N -1] = source[N -1];
  }
};

template <typename T, std::size_t  N>
T* assign(T const  (&source)[N],
          T        (&destination)[N])
{
  ArrayAssigner<T, N, 0>()(source, destination);
  return destination;
}

Canvas::Canvas(
  std::ostream&             os)
: m_os(os)
{}

void Canvas::draw(const Circle& cirlce) {
  m_os << "draw Circle(" << cirlce.getPosition() << ", " << cirlce.getRadius() << ")\n";
}
void Canvas::draw(const Square& square) {
  m_os << "draw Square(" << square.getPosition() << ", " << square.getSideLength() << ")\n";
}

typedef Shape* ShapePtr;
typedef std::vector<ShapePtr> ShapeVector;

struct AddSurfaceAreas
{
  double operator()(
    double    value,
    ShapePtr  shapePtr)
  {
    if (shapePtr) {
      return
        value + shapePtr->calcSurfaceArea();
    }
    else {
      return value;
    }
  }
};

template <std::size_t  number>
struct FibonacciCalculator
{
  static const unsigned long value =
    ( FibonacciCalculator<number - 2>::value +
      FibonacciCalculator<number - 1>::value );
};

template <>
struct FibonacciCalculator<1>
{
  static const unsigned long value = 1;
};

template <>
struct FibonacciCalculator<2>
{
  static const unsigned long value = 1;
};

unsigned long fibonacci(std::size_t  number)
{
  switch(number)
  {
    case 1:
      return FibonacciCalculator<1>::value;
    case 2:
      return FibonacciCalculator<2>::value;
    case 3:
      return FibonacciCalculator<3>::value;
    case 4:
      return FibonacciCalculator<4>::value;
    case 5:
      return FibonacciCalculator<5>::value;
    case 6:
      return FibonacciCalculator<6>::value;
    case 7:
      return FibonacciCalculator<7>::value;
    case 8:
      return FibonacciCalculator<8>::value;
    case 9:
      return FibonacciCalculator<9>::value;
    case 10:
      return FibonacciCalculator<10>::value;
    case 11:
      return FibonacciCalculator<11>::value;
    case 12:
      return FibonacciCalculator<12>::value;
    case 13:
      return FibonacciCalculator<13>::value;
    case 14:
      return FibonacciCalculator<14>::value;
    case 15:
      return FibonacciCalculator<15>::value;
    case 16:
      return FibonacciCalculator<16>::value;
    case 17:
      return FibonacciCalculator<17>::value;
    case 18:
      return FibonacciCalculator<18>::value;
    case 19:
      return FibonacciCalculator<19>::value;
    case 20:
      return FibonacciCalculator<20>::value;
    case 21:
      return FibonacciCalculator<21>::value;
    case 22:
      return FibonacciCalculator<22>::value;
    case 23:
      return FibonacciCalculator<23>::value;
    case 24:
      return FibonacciCalculator<24>::value;
    case 25:
      return FibonacciCalculator<25>::value;
    case 26:
      return FibonacciCalculator<26>::value;
    case 27:
      return FibonacciCalculator<27>::value;
    case 28:
      return FibonacciCalculator<28>::value;
    case 29:
      return FibonacciCalculator<29>::value;
    case 30:
      return FibonacciCalculator<30>::value;
    case 31:
      return FibonacciCalculator<31>::value;
    case 32:
      return FibonacciCalculator<32>::value;
    case 33:
      return FibonacciCalculator<33>::value;
    case 34:
      return FibonacciCalculator<34>::value;
    case 35:
      return FibonacciCalculator<35>::value;
    case 36:
      return FibonacciCalculator<36>::value;
    case 37:
      return FibonacciCalculator<37>::value;
    case 38:
      return FibonacciCalculator<38>::value;
    case 39:
      return FibonacciCalculator<39>::value;
    case 40:
      return FibonacciCalculator<40>::value;
    case 41:
      return FibonacciCalculator<41>::value;
    case 42:
      return FibonacciCalculator<42>::value;
    case 43:
      return FibonacciCalculator<43>::value;
    case 44:
      return FibonacciCalculator<44>::value;
    case 45:
      return FibonacciCalculator<45>::value;
    case 46:
      return FibonacciCalculator<46>::value;
    case 47:
      return FibonacciCalculator<47>::value;
    case 48:
      return FibonacciCalculator<48>::value;
    case 49:
      return FibonacciCalculator<49>::value;
    case 50:
      return FibonacciCalculator<50>::value;
    default:
      break;
  }
  return
    ( fibonacci(number - 2) +
      fibonacci(number - 1) );
}


ShapePtr indexToData(
  std::size_t  number)
{
  const std::size_t maxFib = 50;
  const std::size_t maxValue = 20000;
  unsigned long firstValue =
    fibonacci(number % maxFib + 1) +
    fibonacci((number + maxFib/2) % maxFib + 1) + number/maxFib;
  unsigned long secondValue =
    fibonacci((number + 5) % maxFib + 1) +
    fibonacci((number + maxFib/2 + 7) % maxFib + 1) + number/maxFib;
  unsigned long thirdValue =
    fibonacci((number + 11) % maxFib + 1) +
    fibonacci((number + maxFib/2 + 13) % maxFib + 1) + number/maxFib;
  std::cout << firstValue << ", " << secondValue << ", " << thirdValue << '\n';
  double firstFloatValue = static_cast<double>(static_cast<int>(firstValue % maxValue) - static_cast<int>(maxValue/2)) / 1000;
  double secondFloatValue = static_cast<double>(static_cast<int>(secondValue % maxValue) - static_cast<int>(maxValue/2)) / 1000;
  double thirdFloatValue = static_cast<double>(static_cast<int>(thirdValue % maxValue) - static_cast<int>(maxValue/2)) / 1000;
  Coordinate2D position = { firstFloatValue, secondFloatValue };
  return
    ( (number % 2) ?
      static_cast<ShapePtr>(new Square(position, thirdFloatValue)) :
      static_cast<ShapePtr>(new Circle(position, thirdFloatValue)) );
}

void fillShapeVector(ShapeVector&  shapeVector, std::size_t numShapes)
{
  for (std::size_t index = 0; index < numShapes; ++index)
  {
    shapeVector.push_back(indexToData(index));
  }
}

int main(){
  const std::size_t numShapes = 100;
  Canvas  canvas(std::cout);
  ShapeVector  shapeVector(numShapes);
  fillShapeVector(shapeVector, numShapes);
  struct timespec start_ts;
  if (clock_gettime(CLOCK_REALTIME, &start_ts) == -1) {
    std::cerr << "clock_gettime\n";
    exit(EXIT_FAILURE);
  }
  CALLGRIND_START_INSTRUMENTATION;
  CALLGRIND_TOGGLE_COLLECT;
  for (ShapeVector::const_iterator iter = shapeVector.begin();
       iter != shapeVector.end();
       ++iter)
  {
    const Shape* shapePtr = *iter;
    if (shapePtr){
      shapePtr->draw(canvas);
    }
  }
  double sumOfSurfaceAreas = 0;
  sumOfSurfaceAreas =
    std::accumulate(shapeVector.begin(), shapeVector.end(),
                    sumOfSurfaceAreas,
                    AddSurfaceAreas());
  std::cout << "sum of surface areas : " << sumOfSurfaceAreas << '\n';
  CALLGRIND_TOGGLE_COLLECT;
  CALLGRIND_STOP_INSTRUMENTATION;
  struct timespec finish_ts;
  if (clock_gettime(CLOCK_REALTIME, &finish_ts) == -1) {
    std::cerr << "clock_gettime\n";
    exit(EXIT_FAILURE);
  }
  long microseconds =
    ( start_ts.tv_nsec > finish_ts.tv_nsec ?
      ( 1000*1000*1000 + finish_ts.tv_nsec - start_ts.tv_nsec ) / 1000:
      ( finish_ts.tv_nsec - start_ts.tv_nsec ) / 1000 );

  std::cout << "execution time: " << microseconds << " us\n";
  for (ShapeVector::const_iterator iter = shapeVector.begin();
       iter != shapeVector.end();
       ++iter)
  {
    const Shape* shapePtr = *iter;
    if (shapePtr){
      delete shapePtr;
    }
  }
}
