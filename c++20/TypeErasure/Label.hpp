#ifndef LABEL_HPP
#define LABEL_HPP

#include "LabelTypes.hpp"
#include "NotOfTypeLabelConcept.hpp"
#include "LabelModel.hpp" // and its base class LabelConcept

class MsgBuffer;


namespace label::impl {
class LabelConcept;
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
  Label(const Label&  label)
  : m_labelPtr(label.m_labelPtr ?
               label.m_labelPtr->clone() :
               LabelPtr())
  {}
  Label(Label&&  label) = default;
#if __cpp_concepts
  template <NotOfTypeLabel T>
#else
  template <typename T, std::enable_if_t<notOfTypeLabel<T>(), bool> = true>
#endif // __cpp_concepts
  explicit Label(T&& label)
  : m_labelPtr(std::make_unique<label::impl::LabelModel<T>>(std::forward<T>(label)))
  {}
  auto operator=(const Label&  label) -> Label&
  { Label tmp(label); m_labelPtr = std::move(tmp.m_labelPtr);
    return *this; }
  auto operator=(Label&&  label) -> Label& = default;
  ~Label() = default;
  [[nodiscard]] auto getServiceType() const -> ServiceType
  { return
      ( m_labelPtr ?
        m_labelPtr->getServiceType() :
        ServiceType::undef ); }
  [[nodiscard]] auto getTributarySlots() const -> TributarySlotsConstResult // layer 1
  { return
      ( m_labelPtr ?
        m_labelPtr->getTributarySlots() :
        TributarySlotsConstResult{label::ErrorCode::not_layer_1} ); }
  auto accessTributarySlots() -> TributarySlotsNonConstResult // layer 1
  { return
      ( m_labelPtr ?
        m_labelPtr->accessTributarySlots() :
        TributarySlotsNonConstResult{label::ErrorCode::not_layer_1} ); }
  [[nodiscard]] auto getFrequencySlot() const -> FrequencySlotConstResult // layer 0
  { return
      ( m_labelPtr ?
        m_labelPtr->getFrequencySlot() :
        FrequencySlotConstResult{label::ErrorCode::not_layer_0} ); }
  auto accessFrequencySlot() -> FrequencySlotNonConstResult
  { return
      ( m_labelPtr ?
        m_labelPtr->accessFrequencySlot() :
        FrequencySlotNonConstResult{label::ErrorCode::not_layer_0} ); }
  auto encode(MsgBuffer&  buffer) const -> bool
  {
    if (m_labelPtr)
    {
      return m_labelPtr->encode(buffer);
    }
    return false;
  }
//  static Label decode(const MsgBuffer&  buffer);
private:
  using LabelPtr = label::impl::LabelPtr;
  LabelPtr  m_labelPtr;
};

#endif /* LABEL_HPP */
