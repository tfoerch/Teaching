#include "Label.hpp"
#include "LabelOTNImpl.hpp"
#include "LabelOCHImpl.hpp"

const Label::DeleteFtor  Label::c_deleter;

void Label::DeleteFtor::operator()(label::impl::LabelImplBase* ptr)
{
  delete ptr; // NOLINT(cppcoreguidelines-owning-memory)
}

Label::~Label() = default; // std::unique_ptr requires declaration of LabelImplBase to call delete

Label::Label(ServiceType serviceType, const TributarySlots& tributarySlots)
: m_labelImplPtr(new label::impl::LabelOTNImpl(serviceType, tributarySlots), c_deleter)
{}

Label::Label(ServiceType serviceType, TributarySlots&& tributarySlots)
: m_labelImplPtr(new label::impl::LabelOTNImpl(serviceType, std::move(tributarySlots)), c_deleter)
{}

Label::Label(const FrequencySlot& frequencySlot)
: m_labelImplPtr(new label::impl::LabelOCHImpl(frequencySlot), c_deleter)
{}

Label::Label(FrequencySlot&& frequencySlot)
: m_labelImplPtr(new label::impl::LabelOCHImpl(std::move(frequencySlot)), c_deleter)
{}

auto Label::getServiceType() const -> ServiceType
{
  return
    ( m_labelImplPtr ?
      m_labelImplPtr->getServiceType() :
      ServiceType::undef );
}

auto Label::getTributarySlots() const -> TributarySlotsConstResult
{
  return
    ( m_labelImplPtr ?
      m_labelImplPtr->getTributarySlots() :
      TributarySlotsConstResult{label::ErrorCode::not_layer_1} );
}

auto Label::accessTributarySlots() -> TributarySlotsNonConstResult
{
  return
    ( m_labelImplPtr ?
      m_labelImplPtr->accessTributarySlots() :
      TributarySlotsNonConstResult{label::ErrorCode::not_layer_1} );
}


auto Label::getFrequencySlot() const -> FrequencySlotConstResult
{
  return
    ( m_labelImplPtr ?
      m_labelImplPtr->getFrequencySlot() :
      FrequencySlotConstResult{label::ErrorCode::not_layer_0} );
}

auto Label::accessFrequencySlot() -> FrequencySlotNonConstResult
{
  return
    ( m_labelImplPtr ?
      m_labelImplPtr->accessFrequencySlot() :
      FrequencySlotNonConstResult{label::ErrorCode::not_layer_0} );
}



