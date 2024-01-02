#ifndef LABEL_HPP
#define LABEL_HPP

#include "LabelTypes.hpp"

#include <memory> // std::auto_ptr

class MsgBuffer;

namespace label
{
  namespace impl
  {
    class LabelImplBase;
  }
}

class Label
{
public:
  typedef label::TributarySlots TributarySlots;
  typedef label::FrequencySlot FrequencySlot;
  ~Label(){};
  Label(ServiceType serviceType, const TributarySlots& tributarySlots);
  explicit Label(const FrequencySlot& frequencySlot);
  Label(const Label& other);
  Label& operator=(const Label& other);
  ServiceType getServiceType() const;
  bool getTributarySlots(TributarySlots& tributarySlots) const; // layer 1
  bool setTributarySlots(const TributarySlots& tributarySlots); // layer 1
  bool getFrequencySlot(FrequencySlot& frequencySlot) const; // layer 0
  bool setFrequencySlot(const FrequencySlot& frequencySlot); // layer 0
  bool encode(MsgBuffer&  buffer) const;
  static Label decode(const MsgBuffer&  buffer);
private:
  typedef std::auto_ptr<label::impl::LabelImplBase> LabelPtr;
  LabelPtr           m_labelImplPtr;
};

#endif /* LABEL_HPP */
