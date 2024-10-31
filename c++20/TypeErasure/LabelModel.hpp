#ifndef LABEL_MODEL_HPP
#define LABEL_MODEL_HPP

#include "LabelConcept.hpp"
#include "NotOfTypeLabelConcept.hpp"
#include "LabelImpl.hpp"

namespace label::impl
{
#if __cpp_concepts
  template <NotOfTypeLabel T>
#else
  template <typename T>
#endif // __cpp_concepts
  class LabelModel final : public LabelConcept
  {
  public:
    LabelModel(T&&  label) // NOLINT(cppcoreguidelines-rvalue-reference-param-not-moved)
    : m_label(std::forward<T>(label))
    {}
    [[nodiscard]] auto clone() const -> LabelPtr override
    {
      return
        std::make_unique<LabelModel>(*this);
    }
    [[nodiscard]] auto getServiceType() const -> ServiceType override
    {
      return
        label::impl::getServiceType(m_label);
    }
    [[nodiscard]] auto getTributarySlots() const -> TributarySlotsConstResult override // layer 1
    {
      return
        label::impl::getTributarySlots(m_label);
    }
    auto accessTributarySlots() -> TributarySlotsNonConstResult override // layer 1
    {
      return
        label::impl::accessTributarySlots(m_label);
    }
    [[nodiscard]] auto getFrequencySlot() const -> FrequencySlotConstResult override // layer 0
    {
      return
        label::impl::getFrequencySlot(m_label);
    }
    auto accessFrequencySlot() -> FrequencySlotNonConstResult override // layer 0
    {
      return
        label::impl::accessFrequencySlot(m_label);
    }
    auto encode(MsgBuffer&  buffer) const -> bool override
    {
      return
        label::impl::encode(m_label, buffer);
    }
  private:
     T  m_label{};
  };
}; // namespace label::impl

#endif /* LABEL_MODEL_HPP */
