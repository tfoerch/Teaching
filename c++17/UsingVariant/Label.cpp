#include "Label.hpp"

namespace {
template<class... Ts> struct Overload : Ts... { using Ts::operator()...; };
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;
}

Label::Label(ServiceType serviceType, const TributarySlots& tributarySlots)
: m_label(label::LabelOTN(serviceType, tributarySlots))
{}

Label::Label(ServiceType serviceType, TributarySlots&& tributarySlots)
: m_label(label::LabelOTN(serviceType, std::move(tributarySlots)))
{}

Label::Label(const FrequencySlot& frequencySlot)
: m_label(label::LabelOCH(frequencySlot))
{}

Label::Label(FrequencySlot&& frequencySlot)
: m_label(label::LabelOCH(std::move(frequencySlot)))
{}

auto Label::getServiceType() const -> ServiceType
{
  return
    std::visit(::Overload{
      [](const label::LabelOTN& otnLabel){ return otnLabel.getServiceType(); },
      [](const label::LabelOCH& ochLabel){ return ochLabel.getServiceType(); },
    }, m_label);
}

auto Label::getTributarySlots() const -> TributarySlotsConstResult
{
  return
    std::visit(::Overload{
      [](const label::LabelOTN& otnLabel) -> TributarySlotsConstResult
      { return {std::cref(otnLabel.getTributarySlots())}; },
      [](const label::LabelOCH& /*ochLabel*/) -> TributarySlotsConstResult
      { return {label::ErrorCode::not_layer_1}; },
    }, m_label);
}

auto Label::accessTributarySlots() -> TributarySlotsNonConstResult
{
  return
    std::visit(::Overload{
      [](label::LabelOTN& otnLabel) -> TributarySlotsNonConstResult
      { return {std::ref(otnLabel.accessTributarySlots())}; },
      [](label::LabelOCH& /* ochLabel */) -> TributarySlotsNonConstResult
      { return {label::ErrorCode::not_layer_1}; },
    }, m_label);
}

auto Label::getFrequencySlot() const -> FrequencySlotConstResult
{
  return
    std::visit(::Overload{
      [](const label::LabelOTN& /* otnLabel */) -> FrequencySlotConstResult
      { return {label::ErrorCode::not_layer_0}; },
      [](const label::LabelOCH& ochLabel) -> FrequencySlotConstResult
      { return {std::cref(ochLabel.getFrequencySlot())}; },
    }, m_label);
}

auto Label::accessFrequencySlot() -> FrequencySlotNonConstResult
{
  return
    std::visit(::Overload{
      [](label::LabelOTN& /* otnLabel */) -> FrequencySlotNonConstResult
      { return {label::ErrorCode::not_layer_0}; },
      [](label::LabelOCH& ochLabel) -> FrequencySlotNonConstResult
      { return {std::ref(ochLabel.accessFrequencySlot())}; },
    }, m_label);
}



