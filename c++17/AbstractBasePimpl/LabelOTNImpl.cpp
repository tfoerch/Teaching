#include "LabelOTNImpl.hpp"

namespace label::impl
{

LabelOTNImpl::LabelOTNImpl(ServiceType serviceType, const TributarySlots& tributarySlots)
: m_serviceType(serviceType),
  m_tributarySlots(tributarySlots)
{}

LabelOTNImpl::LabelOTNImpl(ServiceType serviceType, TributarySlots&& tributarySlots)
: m_serviceType(serviceType),
  m_tributarySlots(std::move(tributarySlots))
{}

auto LabelOTNImpl::getServiceType() const -> ServiceType
{
  return m_serviceType;
}

auto LabelOTNImpl::getTributarySlots() const -> TributarySlotsConstResult
{
  return std::cref(m_tributarySlots);
}

auto LabelOTNImpl::accessTributarySlots() -> TributarySlotsNonConstResult
{
  return std::ref(m_tributarySlots);
}

auto LabelOTNImpl::getFrequencySlot() const -> FrequencySlotConstResult
{
  return
    {label::ErrorCode::not_layer_0};
}

auto LabelOTNImpl::accessFrequencySlot() -> FrequencySlotNonConstResult
{
  return
    {label::ErrorCode::not_layer_0};
}

auto LabelOTNImpl::encode(MsgBuffer&  /*buffer*/) const -> bool
{
  return true;
}

}; // namespace label::impl
