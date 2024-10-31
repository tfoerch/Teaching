#ifndef LABEL_OTN_IMPL_HPP
#define LABEL_OTN_IMPL_HPP

#include "LabelIF.hpp"

class LabelOTNImpl : public LabelIF
{
public:
  LabelOTNImpl(ServiceType serviceType, const TributarySlots& tributarySlots);
  LabelOTNImpl(ServiceType serviceType, TributarySlots&& tributarySlots);
  [[nodiscard]] auto getServiceType() const -> ServiceType override;
  [[nodiscard]] auto getTributarySlots() const -> TributarySlotsConstResult override;
  auto accessTributarySlots() -> TributarySlotsNonConstResult override;
  [[nodiscard]] auto getFrequencySlot() const -> FrequencySlotConstResult override;
  auto accessFrequencySlot() -> FrequencySlotNonConstResult override;
  auto encode(MsgBuffer&  buffer) const -> bool override;
private:
  ServiceType     m_serviceType;
  TributarySlots  m_tributarySlots;
};

#endif /* LABEL_OTN_IMPL_HPP */
