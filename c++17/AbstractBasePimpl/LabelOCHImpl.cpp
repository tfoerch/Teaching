#include "LabelOCHImpl.hpp"

namespace label::impl
{

LabelOCHImpl::LabelOCHImpl(const FrequencySlot& frequencySlot)
: m_frequencySlot(frequencySlot)
{}

LabelOCHImpl::LabelOCHImpl(FrequencySlot&& frequencySlot)
: m_frequencySlot(std::move(frequencySlot))
{}

auto LabelOCHImpl::getServiceType() const -> ServiceType
{
  return ServiceType::och;
}

auto LabelOCHImpl::getTributarySlots() const -> TributarySlotsConstResult
{
  return
    {label::ErrorCode::not_layer_1};
}

auto LabelOCHImpl::accessTributarySlots() -> TributarySlotsNonConstResult
{
  return
    {label::ErrorCode::not_layer_1};
}

auto LabelOCHImpl::getFrequencySlot() const -> FrequencySlotConstResult
{
  return std::cref(m_frequencySlot);
}

auto LabelOCHImpl::accessFrequencySlot() -> FrequencySlotNonConstResult
{
  return std::ref(m_frequencySlot);
}

auto LabelOCHImpl::encode(MsgBuffer&  /*buffer*/) const -> bool
{
  return true;
}

}; // namespace label::impl
