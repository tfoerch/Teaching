#ifndef LABEL_OTN_IMPL_HPP
#define LABEL_OTN_IMPL_HPP

#include "LabelImplBase.hpp"

namespace label::impl {

class LabelOTNImpl : public LabelImplBase
{
public:
  LabelOTNImpl(ServiceType serviceType, const TributarySlots& tributarySlots);
  LabelOTNImpl(ServiceType serviceType, TributarySlots&& tributarySlots);
  auto getServiceType() const -> ServiceType override;
  auto getTributarySlots() const -> TributarySlotsConstResult override;
  auto accessTributarySlots() -> TributarySlotsNonConstResult override;
  auto getFrequencySlot() const -> FrequencySlotConstResult override;
  auto accessFrequencySlot() -> FrequencySlotNonConstResult override;
  auto encode(MsgBuffer&  buffer) const -> bool override;
private:
  ServiceType     m_serviceType;
  TributarySlots  m_tributarySlots;
};
}; // namespace label::impl

#endif /* LABEL_OTN_IMPL_HPP */
