#ifndef LABEL_OCH_IMPL_HPP
#define LABEL_OCH_IMPL_HPP

#include "LabelImplBase.hpp"

namespace label {
namespace impl {
  class LabelOCHImpl : public LabelImplBase
  {
  public:
    explicit LabelOCHImpl(const FrequencySlot& frequencySlot);
    LabelOCHImpl(const LabelOCHImpl& other);
    LabelPtr clone() const;
    ServiceType getServiceType() const;
    bool getTributarySlots(TributarySlots& tributarySlots) const;
    bool setTributarySlots(const TributarySlots& tributarySlots);
    bool getFrequencySlot(FrequencySlot& frequencySlot) const;
    bool setFrequencySlot(const FrequencySlot& frequencySlot);
    bool encode(MsgBuffer&  buffer) const;
  private:
    FrequencySlot  m_frequencySlot;
  };
} } // namespace label::impl

#endif /* LABEL_OCH_IMPL_HPP */
