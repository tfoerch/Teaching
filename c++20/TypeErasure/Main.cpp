#include "Label.hpp"
#include "LabelOTN.hpp"
#include "LabelOCH.hpp"
#include "MsgBuffer.hpp"

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <new>
#include <vector>
#include <cassert>
#include <iostream>

auto main() -> int
{
  try {
    Buffer  buffer;
    WriteMsgBuffer  writeMsgBuffer(buffer.getBuffer());
    static constexpr std::initializer_list<std::uint8_t> c_byteSequence =
      { 12, 34, 12, 34,
        12, 34, 56, 78 }; // network order
    constexpr std::byte c_byteValue{*c_byteSequence.begin()};
    constexpr std::uint8_t c_uint8Value{*std::next(c_byteSequence.begin())};
    constexpr std::uint16_t c_uint16Value =
      (*std::next(c_byteSequence.begin(), 2) * 256) +
       *std::next(c_byteSequence.begin(), 3);
    constexpr std::uint32_t c_uint32Value =
      (*std::next(c_byteSequence.begin(), 4) * 256 * 256 * 256) +
      (*std::next(c_byteSequence.begin(), 5) * 256 * 256) +
      (*std::next(c_byteSequence.begin(), 6) * 256) +
       *std::next(c_byteSequence.begin(), 7);
    constexpr std::size_t c_msgSize =
      sizeof(std::byte) + sizeof(std::uint8_t) +
      sizeof(std::uint16_t) + sizeof(std::uint32_t);
    assert(writeMsgBuffer.write(c_byteValue));
    assert(writeMsgBuffer.write(c_uint8Value));
    assert(writeMsgBuffer.write(c_uint16Value));
    assert(writeMsgBuffer.write(c_uint32Value));
    {
      const auto& contents = writeMsgBuffer.getContents();
      std::cout << "contents.size() = " << contents.size()
                << ", c_msgSize = " << c_msgSize << '\n';
      assert(contents.size() == c_msgSize);
      assert(std::ranges::equal(contents,
                                c_byteSequence,
                              [](std::byte first, std::uint8_t second)
                                    { return
                                        (std::to_integer<std::uint8_t>(first) == second); }));
      if (contents.size() == c_msgSize)
      {
        ReadMsgBuffer  readMsgBuffer(contents);
        std::byte byteReadValue{0};
        assert(readMsgBuffer.read(byteReadValue));
        assert(byteReadValue == c_byteValue);
        std::uint8_t uint8ReadValue = 0;
        assert(readMsgBuffer.read(uint8ReadValue));
        assert(uint8ReadValue == c_uint8Value);
        std::uint16_t uint16ReadValue = 0;
        assert(readMsgBuffer.read(uint16ReadValue));
        assert(uint16ReadValue == c_uint16Value);
        std::uint32_t uint32ReadValue = 0;
        assert(readMsgBuffer.read(uint32ReadValue));
        assert(uint32ReadValue == c_uint32Value);
        std::cout << "end of code block" << '\n';
      }
    }
  } catch (std::bad_alloc&)
  {
    std::cout << "bad alloc exeption caught" << '\n';
  }

  static_assert(std::is_same_v<std::decay_t<Label&>, Label>);
#if __cpp_concepts
  static_assert(NotOfTypeLabel<label::LabelOTN>);
#else
  static_assert(notOfTypeLabel<label::LabelOTN>());
#endif // __cpp_concepts
  const Label  odu0Label(label::LabelOTN(ServiceType::odu0, label::TributarySlots({ 1 })));
  assert(odu0Label.getServiceType() == ServiceType::odu0);
  const Label  odu2eLabel(label::LabelOTN(ServiceType::odu2e, label::TributarySlots({ 4, 5, 6, 7, 12, 13, 14, 15 }))); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  const Label  ochLabel(label::LabelOCH(label::FrequencySlot{-32, 4}));
  const Label copyOfOdu0Label = odu0Label; // NOLINT(performance-unnecessary-copy-initialization)
  std::vector<Label>  labels;
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  labels.emplace_back(label::LabelOTN(ServiceType::odu0, label::TributarySlots({ 23 }))); // construct the Label in-place
  labels.emplace_back(label::LabelOCH(label::FrequencySlot{-4, 4})); // construct the Label in-place
  labels.push_back(Label(label::LabelOTN(ServiceType::odu0, label::TributarySlots({ 23 })))); // NOLINT(modernize-use-emplace) constructed then moved
  labels.push_back(Label(label::LabelOCH(label::FrequencySlot{-4, 4}))); // NOLINT(modernize-use-emplace) constructed then moved
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
  std::vector<Label>  copyofLabels; copyofLabels = labels;
}
