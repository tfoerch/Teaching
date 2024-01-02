#ifndef LABEL_IMPL_HPP
#define LABEL_IMPL_HPP

#include "LabelTypes.hpp"

class MsgBuffer;

namespace label
{
  class LabelOTN;
  class LabelOCH;

  namespace impl
  {
    auto getServiceType(const LabelOTN& label) -> ServiceType;
    auto getServiceType(const LabelOCH& label) -> ServiceType;

    auto getTributarySlots(const LabelOTN& label) -> label::TributarySlotsConstResult;
    auto getTributarySlots(const LabelOCH& label) -> label::TributarySlotsConstResult;
    auto accessTributarySlots(LabelOTN& label) -> label::TributarySlotsNonConstResult;
    auto accessTributarySlots(LabelOCH& label) -> label::TributarySlotsNonConstResult;

    auto getFrequencySlot(const LabelOTN& label) -> label::FrequencySlotConstResult;
    auto getFrequencySlot(const LabelOCH& label) -> label::FrequencySlotConstResult;
    auto accessFrequencySlot(LabelOTN& label) -> label::FrequencySlotNonConstResult;
    auto accessFrequencySlot(LabelOCH& label) -> label::FrequencySlotNonConstResult;

    auto encode(const LabelOTN& label, MsgBuffer&  buffer) -> bool;
    auto encode(const LabelOCH& label, MsgBuffer&  buffer) -> bool;
  }; // namespace impl
}; // namespace label

#endif /* LABEL_IMPL_HPP */
