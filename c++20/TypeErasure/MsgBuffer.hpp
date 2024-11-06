#ifndef MSG_BUFFER_HPP
#define MSG_BUFFER_HPP

#include <algorithm>
#include <bit>
#include <cstddef>
#include <array>
#include <memory>
#include <span>
#include <type_traits>

template <std::size_t  capacity>
class RawHeapBuffer
{
public:
  RawHeapBuffer() // might throw std::bad_alloc
  : m_bufferPtr(std::make_unique<BufferType>())
  {}
  [[nodiscard]] auto getBuffer() -> decltype(auto)
  { return std::span{*m_bufferPtr.get()}; }
private:
  using BufferType =
    std::array<std::byte, capacity>;
  std::unique_ptr<BufferType>  m_bufferPtr;
};

template <std::size_t  capacity>
class RawInPlaceBuffer
{
public:
  using BufferType =
    std::array<std::byte, capacity>;
  [[nodiscard]] auto getBuffer() -> decltype(auto)
  { return std::span{m_buffer}; }
private:
  BufferType  m_buffer{};
};

constexpr std::size_t  c_bufferCapacity = 200;
// using Buffer = RawInPlaceBuffer<c_bufferCapacity>;
using Buffer = RawHeapBuffer<c_bufferCapacity>;
using BufferView =
  std::invoke_result_t<decltype(&Buffer::getBuffer), Buffer>;

class WriteMsgBuffer
{
public:
  explicit WriteMsgBuffer(
    const BufferView& bufferView)
  : m_bufferView(bufferView),
    m_posIter(m_bufferView.begin())
  {}
  [[nodiscard]] auto getContents() const -> decltype(auto)
  { return m_bufferView.subspan(0, static_cast<std::size_t>(std::distance(m_bufferView.begin(), m_posIter))); }
  auto write(std::byte byte) -> bool
  { if (m_posIter != m_bufferView.end())
    {
      *m_posIter = byte;
      ++m_posIter;
      return true;
    }
    return false; }
  template <std::integral T>
  auto write(T  value) -> bool
  { if (sizeof(T) <= std::distance(m_posIter, m_bufferView.end()))
    {
      auto valueRepresentation =
        std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
      if constexpr (std::endian::native == std::endian::little)
      {
        std::ranges::reverse(valueRepresentation);
      }
      std::copy(valueRepresentation.begin(), valueRepresentation.end(), m_posIter);
      m_posIter += sizeof(T);
      return true;
    }
    return false; }
private:
  using BufferViewIter = BufferView::iterator;
  BufferView      m_bufferView;
  BufferViewIter  m_posIter;
};

using ContentsView =
  std::invoke_result_t<decltype(&WriteMsgBuffer::getContents), WriteMsgBuffer>;

class ReadMsgBuffer
{
public:
  explicit ReadMsgBuffer(
    const ContentsView& contentsView)
  : m_contentsView(contentsView),
    m_posIter(m_contentsView.begin())
  {}
  auto read(std::byte& byte) -> bool
  { if (m_posIter != m_contentsView.end())
    {
      byte = *m_posIter;
      ++m_posIter;
      return true;
    }
    return false; }
  template <std::integral T>
  auto read(T&  value) -> bool
  { if (sizeof(T) <= std::distance(m_posIter, m_contentsView.end()))
    {
      std::array<std::byte, sizeof(T)> valueRepresentation;
      std::copy(m_posIter, std::next(m_posIter, sizeof(T)), valueRepresentation.begin());
      if constexpr (std::endian::native == std::endian::little)
      {
        std::ranges::reverse(valueRepresentation);
      }
      value = std::bit_cast<T>(valueRepresentation);
      m_posIter += sizeof(T);
      return true;
    }
    return false; }
private:
  using ContentsViewIter = ContentsView::iterator;
  ContentsView      m_contentsView;
  ContentsViewIter  m_posIter;
};

#endif /* MSG_BUFFER_HPP */
