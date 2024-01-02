#include "LabelImpl.hpp"
#include "LabelOTN.hpp"
#include "LabelOCH.hpp"

namespace label::impl
{
  auto getServiceType(const LabelOTN& label) -> ServiceType
  { return label.getServiceType(); }
  auto getServiceType(const LabelOCH& label) -> ServiceType
  { return label.getServiceType(); }

  auto getTributarySlots(const LabelOTN& label) -> label::TributarySlotsConstResult
  { return {std::cref(label.getTributarySlots())}; }
  auto getTributarySlots(const LabelOCH& /*label*/) -> label::TributarySlotsConstResult
  { return {label::ErrorCode::not_layer_1}; }
  auto accessTributarySlots(LabelOTN& label) -> label::TributarySlotsNonConstResult
  { return {std::ref(label.accessTributarySlots())}; }
  auto accessTributarySlots(LabelOCH& /*label*/) -> label::TributarySlotsNonConstResult
  { return {label::ErrorCode::not_layer_1}; }

  auto getFrequencySlot(const LabelOTN& /*label*/) -> label::FrequencySlotConstResult
  { return {label::ErrorCode::not_layer_0}; }
  auto getFrequencySlot(const LabelOCH& label) -> label::FrequencySlotConstResult
  { return {std::cref(label.getFrequencySlot())}; }
  auto accessFrequencySlot(LabelOTN& /*label*/) -> label::FrequencySlotNonConstResult
  { return {label::ErrorCode::not_layer_0}; }
  auto accessFrequencySlot(LabelOCH& label) -> label::FrequencySlotNonConstResult
  { return {std::ref(label.accessFrequencySlot())}; }

  auto encode(const LabelOTN& label, MsgBuffer&  buffer) -> bool
  { return label.encode(buffer); }
  auto encode(const LabelOCH& label, MsgBuffer&  buffer) -> bool
  { return label.encode(buffer); }

}; // namespace label::impl
