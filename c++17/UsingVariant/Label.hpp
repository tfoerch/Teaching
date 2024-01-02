#ifndef LABEL_HPP
#define LABEL_HPP

#include "LabelTypes.hpp"
#include "LabelOTN.hpp"
#include "LabelOCH.hpp"

class MsgBuffer;


class Label
{
public:
  using TributarySlots = label::TributarySlots;
  using FrequencySlot = label::FrequencySlot;
  using TributarySlotsConstResult = label::TributarySlotsConstResult;
  using TributarySlotsNonConstResult = label::TributarySlotsNonConstResult;
  using FrequencySlotConstResult = label::FrequencySlotConstResult;
  using FrequencySlotNonConstResult = label::FrequencySlotNonConstResult;
  Label(ServiceType serviceType, const TributarySlots& tributarySlots);
  Label(ServiceType serviceType, TributarySlots&& tributarySlots);
  explicit Label(const FrequencySlot& frequencySlot);
  explicit Label(FrequencySlot&& frequencySlot);
  auto getServiceType() const -> ServiceType;
  auto getTributarySlots() const -> TributarySlotsConstResult; // layer 1
  auto accessTributarySlots() -> TributarySlotsNonConstResult; // layer 1
  auto getFrequencySlot() const -> FrequencySlotConstResult; // layer 0
  auto accessFrequencySlot() -> FrequencySlotNonConstResult; // layer 0
  auto encode(MsgBuffer&  buffer) const -> bool;
  static Label decode(const MsgBuffer&  buffer);
private:
  using LabelType =
    std::variant<label::LabelOTN,
                 label::LabelOCH>;
  LabelType  m_label;
};

#endif /* LABEL_HPP */
