#include "LabelOTNImpl.hpp"

LabelOTNImpl::LabelOTNImpl(ServiceType serviceType, const TributarySlots& tributarySlots)
: m_serviceType(serviceType),
  m_tributarySlots(tributarySlots)
{}

LabelOTNImpl::LabelOTNImpl(const LabelOTNImpl& other)
: m_serviceType(other.m_serviceType),
  m_tributarySlots(other.m_tributarySlots)
{}

LabelIF::LabelIFPtr LabelOTNImpl::clone() const
{
  return
    new LabelOTNImpl(*this);
}

ServiceType LabelOTNImpl::getServiceType() const
{
  return m_serviceType;
}

bool LabelOTNImpl::getTributarySlots(TributarySlots& tributarySlots) const
{
  tributarySlots = m_tributarySlots;
  return true;
}

bool LabelOTNImpl::setTributarySlots(const TributarySlots& tributarySlots)
{
  m_tributarySlots = tributarySlots;
  return true;
}

bool LabelOTNImpl::getFrequencySlot(FrequencySlot& /* frequencySlot */) const
{
  return false;
}

bool LabelOTNImpl::setFrequencySlot(const FrequencySlot& /* frequencySlot */)
{
  return false;
}

bool LabelOTNImpl::encode(MsgBuffer&  /*buffer*/) const
{
  return true;
}
