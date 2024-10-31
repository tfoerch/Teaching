#ifndef LABEL_IMPL_BASE_HPP
#define LABEL_IMPL_BASE_HPP

#include "LabelTypes.hpp"

class MsgBuffer;

namespace label::impl {

class LabelImplBase
{
public:
  using TributarySlots = label::TributarySlots;
  using FrequencySlot = label::FrequencySlot;
  using TributarySlotsConstResult = label::TributarySlotsConstResult;
  using TributarySlotsNonConstResult = label::TributarySlotsNonConstResult;
  using FrequencySlotConstResult = label::FrequencySlotConstResult;
  using FrequencySlotNonConstResult = label::FrequencySlotNonConstResult;
  virtual ~LabelImplBase() = default;
  [[nodiscard]] virtual auto getServiceType() const -> ServiceType = 0;
  [[nodiscard]] virtual auto getTributarySlots() const -> TributarySlotsConstResult = 0; // layer 1
  virtual auto accessTributarySlots() -> TributarySlotsNonConstResult = 0; // layer 1
  [[nodiscard]] virtual auto getFrequencySlot() const -> FrequencySlotConstResult = 0; // layer 0
  virtual auto accessFrequencySlot() -> FrequencySlotNonConstResult = 0; // layer 0
  virtual auto encode(MsgBuffer&  buffer) const -> bool = 0;
};
} // namespace label::impl

#endif /* LABEL_IMPL_BASE_HPP */
