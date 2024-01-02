#ifndef LABEL_CONCEPT_HPP
#define LABEL_CONCEPT_HPP

#include "LabelTypes.hpp"

#include <memory> // std::unique_ptr

class MsgBuffer;

namespace label::impl
{
  class LabelConcept
  {
  public:
    using TributarySlots = label::TributarySlots;
    using FrequencySlot = label::FrequencySlot;
    using TributarySlotsConstResult = label::TributarySlotsConstResult;
    using TributarySlotsNonConstResult = label::TributarySlotsNonConstResult;
    using FrequencySlotConstResult = label::FrequencySlotConstResult;
    using FrequencySlotNonConstResult = label::FrequencySlotNonConstResult;
    using LabelPtr = std::unique_ptr<LabelConcept>;
    virtual ~LabelConcept() = default;
    virtual auto clone() const -> LabelPtr = 0;
    virtual auto getServiceType() const -> ServiceType = 0;
    virtual auto getTributarySlots() const -> TributarySlotsConstResult = 0; // layer 1
    virtual auto accessTributarySlots() -> TributarySlotsNonConstResult = 0; // layer 1
    virtual auto getFrequencySlot() const -> FrequencySlotConstResult = 0; // layer 0
    virtual auto accessFrequencySlot() -> FrequencySlotNonConstResult = 0; // layer 0
    virtual auto encode(MsgBuffer&  buffer) const -> bool = 0;
  };

  using LabelPtr = LabelConcept::LabelPtr;

}; // namespace label::impl

#endif /* LABEL_CONCEPT_HPP */
