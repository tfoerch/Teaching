#include "LabelOCHImpl.hpp"

LabelOCHImpl::LabelOCHImpl(const FrequencySlot& frequencySlot)
: m_frequencySlot(frequencySlot)
{}

LabelOCHImpl::LabelOCHImpl(const LabelOCHImpl& other)
: m_frequencySlot(other.m_frequencySlot)
{}

LabelIF::LabelIFPtr LabelOCHImpl::clone() const
{
  return
    new LabelOCHImpl(*this);
}

ServiceType LabelOCHImpl::getServiceType() const
{
  return ServiceType_och;
}

bool LabelOCHImpl::getTributarySlots(TributarySlots& /* tributarySlots */) const
{
  return false;
}

bool LabelOCHImpl::setTributarySlots(const TributarySlots& /* tributarySlots */)
{
  return false;
}

bool LabelOCHImpl::getFrequencySlot(FrequencySlot& frequencySlot) const
{
  frequencySlot = m_frequencySlot;
  return true;
}

bool LabelOCHImpl::setFrequencySlot(const FrequencySlot& frequencySlot)
{
  m_frequencySlot = frequencySlot;
  return true;
}

bool LabelOCHImpl::encode(MsgBuffer&  /*buffer*/) const
{
  return true;
}
