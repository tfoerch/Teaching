#ifndef LABELIF_HPP
#define LABELIF_HPP

#include "LabelTypes.hpp"

class MsgBuffer;

class LabelIF
{
public:
  typedef label::TributarySlots TributarySlots;
  typedef label::FrequencySlot FrequencySlot;
  typedef LabelIF* LabelIFPtr; // cannot use std::auto_ptr<> here, refer to http://www.gotw.ca/publications/using_auto_ptr_effectively.htm
  virtual ~LabelIF() {};
  virtual LabelIFPtr clone() const = 0;
  virtual ServiceType getServiceType() const = 0;
  virtual bool getTributarySlots(TributarySlots& tributarySlots) const = 0;
  virtual bool setTributarySlots(const TributarySlots& tributarySlots) = 0;
  virtual bool getFrequencySlot(FrequencySlot& frequencySlot) const = 0;
  virtual bool setFrequencySlot(const FrequencySlot& frequencySlot) = 0;
  virtual bool encode(MsgBuffer&  buffer) const = 0;
};

#endif /* LABELIF_HPP */
