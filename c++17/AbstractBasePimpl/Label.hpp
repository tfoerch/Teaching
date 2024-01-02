#ifndef LABEL_HPP
#define LABEL_HPP

#include "LabelTypes.hpp"

#include <memory> // std::unique_ptr

class MsgBuffer;

namespace label::impl {
class LabelImplBase;
}

class Label
{
public:
  using TributarySlots = label::TributarySlots;
  using FrequencySlot = label::FrequencySlot;
  using TributarySlotsConstResult = label::TributarySlotsConstResult;
  using TributarySlotsNonConstResult = label::TributarySlotsNonConstResult;
  using FrequencySlotConstResult = label::FrequencySlotConstResult;
  using FrequencySlotNonConstResult = label::FrequencySlotNonConstResult;
  Label(ServiceType serviceType, const TributarySlots& tributarySlots);
  Label(ServiceType serviceType, TributarySlots&& tributarySlots);
  explicit Label(const FrequencySlot& frequencySlot);
  explicit Label(FrequencySlot&& frequencySlot);
  Label() = default;
  Label(const Label&) = delete; // implicitly deleted due to deleted copy ctor of std::unique_ptr
  Label(Label&&) = default;
  Label& operator=(const Label&) = delete; // implicitly deleted
  Label& operator=(Label&&) = default;
  ~Label();
  auto getServiceType() const -> ServiceType;
  auto getTributarySlots() const -> TributarySlotsConstResult; // layer 1
  auto accessTributarySlots() -> TributarySlotsNonConstResult; // layer 1
  auto getFrequencySlot() const -> FrequencySlotConstResult; // layer 0
  auto accessFrequencySlot() -> FrequencySlotNonConstResult; // layer 0
  auto encode(MsgBuffer&  buffer) const -> bool;
  static Label decode(const MsgBuffer&  buffer);
private:
  struct DeleteFtor
  {
    void operator()(label::impl::LabelImplBase* ptr);
  };
  using LabelPtr = std::unique_ptr<label::impl::LabelImplBase, DeleteFtor>;
  LabelPtr           m_labelImplPtr;
  static const DeleteFtor  c_deleter;
};

#endif /* LABEL_HPP */
