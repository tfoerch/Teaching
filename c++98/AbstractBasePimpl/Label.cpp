#include "Label.hpp"
#include "LabelOTNImpl.hpp"
#include "LabelOCHImpl.hpp"

Label::Label(ServiceType serviceType, const TributarySlots& tributarySlots)
: m_labelImplPtr(new label::impl::LabelOTNImpl(serviceType, tributarySlots))
{}

Label::Label(const FrequencySlot& frequencySlot)
: m_labelImplPtr(new label::impl::LabelOCHImpl(frequencySlot))
{}

Label::Label(const Label& other)
: m_labelImplPtr(other.m_labelImplPtr.get() ?
                 other.m_labelImplPtr->clone() :
                 0)
{}

Label& Label::operator=(const Label& other)
{
  if (m_labelImplPtr.get() != other.m_labelImplPtr.get())
  {
    m_labelImplPtr.reset(other.m_labelImplPtr.get() ?
                         other.m_labelImplPtr->clone() :
                         0);
  }
  return *this;
}

ServiceType Label::getServiceType() const
{
  return
    ( m_labelImplPtr.get() ?
      m_labelImplPtr->getServiceType() :
      ServiceType_undef );
}

bool Label::getTributarySlots(TributarySlots& tributarySlots) const
{
  return
    ( m_labelImplPtr.get() ?
      m_labelImplPtr->getTributarySlots(tributarySlots) :
      false );
}

bool Label::setTributarySlots(const TributarySlots& tributarySlots)
{
  return
    ( m_labelImplPtr.get() ?
      m_labelImplPtr->setTributarySlots(tributarySlots) :
      false );
}

bool Label::getFrequencySlot(FrequencySlot& frequencySlot) const
{
  return
    ( m_labelImplPtr.get() ?
      m_labelImplPtr->getFrequencySlot(frequencySlot) :
      false );
}

bool Label::setFrequencySlot(const FrequencySlot& frequencySlot)
{
  return
    ( m_labelImplPtr.get() ?
      m_labelImplPtr->setFrequencySlot(frequencySlot) :
      false );
}
