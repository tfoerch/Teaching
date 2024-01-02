#ifndef LABEL_OTN_IMPL_HPP
#define LABEL_OTN_IMPL_HPP

#include "LabelImplBase.hpp"

namespace label {
namespace impl {
  class LabelOTNImpl : public LabelImplBase
  {
  public:
    LabelOTNImpl(ServiceType serviceType, const TributarySlots& tributarySlots);
    LabelOTNImpl(const LabelOTNImpl& other);
    LabelPtr clone() const;
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
} } // namespace label::impl

#endif /* LABEL_OTN_IMPL_HPP */
