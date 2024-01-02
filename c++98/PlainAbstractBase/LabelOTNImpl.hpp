#ifndef LABEL_OTN_IMPL_HPP
#define LABEL_OTN_IMPL_HPP

#include "LabelIF.hpp"

class LabelOTNImpl : public LabelIF
{
public:
  LabelOTNImpl(ServiceType serviceType, const TributarySlots& tributarySlots);
  LabelOTNImpl(const LabelOTNImpl& other);
  LabelIFPtr clone() const;
  ServiceType getServiceType() const;
  bool getTributarySlots(TributarySlots& tributarySlots) const;
  bool setTributarySlots(const TributarySlots& tributarySlots);
  bool getFrequencySlot(FrequencySlot& frequencySlot) const;
  bool setFrequencySlot(const FrequencySlot& frequencySlot);
  bool encode(MsgBuffer&  buffer) const;
private:
  ServiceType     m_serviceType;
  TributarySlots  m_tributarySlots;
};

#endif /* LABEL_OTN_IMPL_HPP */
