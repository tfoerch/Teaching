#ifndef LABEL_OCH_HPP
#define LABEL_OCH_HPP

#include "LabelTypes.hpp"

class MsgBuffer;

namespace label
{
  class LabelOCH
  {
  public:
    explicit LabelOCH(const FrequencySlot& frequencySlot);
    explicit LabelOCH(FrequencySlot&& frequencySlot);
    constexpr auto getServiceType() const -> ServiceType { return ServiceType::och; }
    constexpr auto getFrequencySlot() const -> const FrequencySlot& { return m_frequencySlot; }
    constexpr auto accessFrequencySlot() -> FrequencySlot& { return m_frequencySlot; }
    auto encode(MsgBuffer&  buffer) const -> bool;
  private:
    FrequencySlot  m_frequencySlot;
  };
}; // namespace label

#endif /* LABEL_OCH_HPP */
