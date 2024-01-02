#ifndef LABEL_OCH_IMPL_HPP
#define LABEL_OCH_IMPL_HPP

#include "LabelImplBase.hpp"

namespace label::impl
{
  class LabelOCHImpl : public LabelImplBase
  {
  public:
    explicit LabelOCHImpl(const FrequencySlot& frequencySlot);
    explicit LabelOCHImpl(FrequencySlot&& frequencySlot);
    auto getServiceType() const -> ServiceType override;
    auto getTributarySlots() const -> TributarySlotsConstResult override;
    auto accessTributarySlots() -> TributarySlotsNonConstResult override;
    auto getFrequencySlot() const -> FrequencySlotConstResult override;
    auto accessFrequencySlot() -> FrequencySlotNonConstResult override;
    auto encode(MsgBuffer&  buffer) const -> bool override;
  private:
    FrequencySlot  m_frequencySlot;
  };
}; // namespace label::impl

#endif /* LABEL_OCH_IMPL_HPP */
