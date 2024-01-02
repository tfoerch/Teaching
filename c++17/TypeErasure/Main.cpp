#include "Label.hpp"
#include "LabelOTN.hpp"
#include "LabelOCH.hpp"

#include <vector>
#include <cassert>

auto main() -> int
{
  static_assert(std::is_same_v<std::decay_t<Label&>, Label>);
#if __cpp_concepts
  static_assert(NotOfTypeLabel<label::LabelOTN>);
#else
  static_assert(notOfTypeLabel<label::LabelOTN>());
#endif // __cpp_concepts
  const Label  odu0Label(label::LabelOTN(ServiceType::odu0, label::TributarySlots({ 1 })));
  assert(odu0Label.getServiceType() == ServiceType::odu0);
  const Label  odu2eLabel(label::LabelOTN(ServiceType::odu2e, label::TributarySlots({ 4, 5, 6, 7, 12, 13, 14, 15 }))); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  const Label  ochLabel(label::LabelOCH(label::FrequencySlot{-32, 4}));
  const Label copy1OfOdu0Label = odu0Label; // NOLINT(performance-unnecessary-copy-initialization)
  std::vector<Label>  labels;
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  labels.emplace_back(label::LabelOTN(ServiceType::odu0, label::TributarySlots({ 23 }))); // construct the Label in-place
  labels.emplace_back(label::LabelOCH(label::FrequencySlot{-4, 4})); // construct the Label in-place
  labels.push_back(Label(label::LabelOTN(ServiceType::odu0, label::TributarySlots({ 23 })))); // NOLINT(modernize-use-emplace) constructed then moved
  labels.push_back(Label(label::LabelOCH(label::FrequencySlot{-4, 4}))); // NOLINT(modernize-use-emplace) constructed then moved
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
  std::vector<Label>  copyofLabels; copyofLabels = labels;
}
