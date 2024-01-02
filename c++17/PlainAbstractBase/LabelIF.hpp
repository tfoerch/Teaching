#ifndef LABELIF_HPP
#define LABELIF_HPP

#include "LabelTypes.hpp"

#include <memory> // std::unique_ptr

class MsgBuffer;

class LabelIF
{
public:
  using TributarySlots = label::TributarySlots;
  using FrequencySlot = label::FrequencySlot;
  using TributarySlotsConstResult = label::TributarySlotsConstResult;
  using TributarySlotsNonConstResult = label::TributarySlotsNonConstResult;
  using FrequencySlotConstResult = label::FrequencySlotConstResult;
  using FrequencySlotNonConstResult = label::FrequencySlotNonConstResult;
  virtual ~LabelIF() = default;
  virtual auto getServiceType() const -> ServiceType = 0;
  virtual auto getTributarySlots() const -> TributarySlotsConstResult = 0; // layer 1
  virtual auto accessTributarySlots() -> TributarySlotsNonConstResult = 0; // layer 1
  virtual auto getFrequencySlot() const -> FrequencySlotConstResult = 0; // layer 0
  virtual auto accessFrequencySlot() -> FrequencySlotNonConstResult = 0; // layer 0
  virtual auto encode(MsgBuffer&  buffer) const -> bool = 0;
};

namespace label
{
  using LabelIFPtr = std::unique_ptr<LabelIF>;
};

#endif /* LABELIF_HPP */
