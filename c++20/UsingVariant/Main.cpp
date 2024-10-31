#include "Label.hpp"

#include <vector>
#include <cassert>

auto main() -> int
{
  const Label  odu0Label(ServiceType::odu0, label::TributarySlots({ 1 }));
  assert(odu0Label.getServiceType() == ServiceType::odu0);
  const Label  odu2eLabel(ServiceType::odu2e, label::TributarySlots({ 4, 5, 6, 7, 12, 13, 14, 15 })); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  const Label  ochLabel(label::FrequencySlot{-32, 4});
  const Label copy1OfOdu0Label = odu0Label; // NOLINT(performance-unnecessary-copy-initialization)
  std::vector<Label>  labels;
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  labels.emplace_back(ServiceType::odu0, label::TributarySlots({ 23 })); // construct the Label in-place
  labels.emplace_back(label::FrequencySlot{-4, 4}); // construct the Label in-place
  labels.push_back(Label(ServiceType::odu0, label::TributarySlots({ 23 }))); // NOLINT(modernize-use-emplace) constructed then moved
  labels.push_back(Label(label::FrequencySlot{-4, 4})); // NOLINT(modernize-use-emplace) constructed then moved
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
  std::vector<Label>  copyofLabels; copyofLabels = labels;
}
