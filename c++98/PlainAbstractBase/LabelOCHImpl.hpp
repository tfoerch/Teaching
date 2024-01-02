#ifndef LABEL_OCH_IMPL_HPP
#define LABEL_OCH_IMPL_HPP

#include "LabelIF.hpp"

class LabelOCHImpl : public LabelIF
{
public:
  explicit LabelOCHImpl(const FrequencySlot& frequencySlot);
  LabelOCHImpl(const LabelOCHImpl& other);
  LabelIFPtr clone() const;
  ServiceType getServiceType() const;
  bool getTributarySlots(TributarySlots& tributarySlots) const;
  bool setTributarySlots(const TributarySlots& tributarySlots);
  bool getFrequencySlot(FrequencySlot& frequencySlot) const;
  bool setFrequencySlot(const FrequencySlot& frequencySlot);
  bool encode(MsgBuffer&  buffer) const;
private:
  FrequencySlot  m_frequencySlot;
};

#endif /* LABEL_OCH_IMPL_HPP */
