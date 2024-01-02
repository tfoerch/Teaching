#ifndef LABEL_IMPL_BASE_HPP
#define LABEL_IMPL_BASE_HPP

#include "LabelTypes.hpp"

class MsgBuffer;

namespace label {
namespace impl {

  class LabelImplBase
  {
  public:
    typedef label::TributarySlots TributarySlots;
    typedef label::FrequencySlot FrequencySlot;
    typedef LabelImplBase* LabelPtr;
    virtual ~LabelImplBase() {};
    virtual LabelPtr clone() const = 0;
    virtual ServiceType getServiceType() const = 0;
    virtual bool getTributarySlots(TributarySlots& tributarySlots) const = 0;
    virtual bool setTributarySlots(const TributarySlots& tributarySlots) = 0;
    virtual bool getFrequencySlot(FrequencySlot& frequencySlot) const = 0;
    virtual bool setFrequencySlot(const FrequencySlot& frequencySlot) = 0;
    virtual bool encode(MsgBuffer&  buffer) const = 0;
  };
} } // namespace label::impl

#endif /* LABEL_IMPL_BASE_HPP */
