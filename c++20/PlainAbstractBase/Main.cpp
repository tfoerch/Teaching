#include "LabelOCHImpl.hpp"
#include "LabelOTNImpl.hpp"

#include <vector>
#include <cassert>
#include <algorithm>

namespace {

template<class... Ts>
struct Overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

auto isEqual(
  const std::initializer_list<uint32_t>& tribSlots,
  const label::TributarySlotsConstResult& tribSlotResult) -> bool
{
  return
    std::visit(Overloaded{
      [&tribSlots](const label::TributarySlotsConstRef& tribSlotsRef)
      { return
          std::equal(tribSlots.begin(), tribSlots.end(),
                     tribSlotsRef.get().begin(), tribSlotsRef.get().end()); },
      [](const label::ErrorCode&  /* errorCode */)
      { return false; }
    }, tribSlotResult);
}

auto isEqual(
  const label::FrequencySlot& freqSlot,
  const label::FrequencySlotConstResult& freqSlotResult) -> bool
{
  return
    std::visit(Overloaded{
      [&freqSlot](const label::FrequencySlotConstRef& freqSlotRef)
      { return
          ( freqSlot == freqSlotRef.get() ); },
      [](const label::ErrorCode&  /* errorCode */)
      { return false; }
    }, freqSlotResult);
}

} // namespace anonymous

auto main() -> int // NOLINT(bugprone-exception-escape)
{
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  const std::initializer_list<uint32_t> tribSlotsOf1stOdukLabel = { 1 };
  const std::initializer_list<uint32_t> tribSlotsOf2ndOdukLabel = { 4, 5, 6, 7, 12, 13, 14, 15 }; // ordered for easier tests
  const std::initializer_list<uint32_t> tribSlotsOf3rdOdukLabel = { 8 };
  const label::FrequencySlot freqSlotOf1stOchLabel{-32, 4};
  const label::FrequencySlot freqSlotOf2ndOchLabel{-4, 4};
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
  std::vector<label::LabelIFPtr>  layer0Labels;
  std::vector<label::LabelIFPtr>  layer1Labels;
  {
    const LabelOTNImpl odu0Label(ServiceType::odu0, label::TributarySlots(tribSlotsOf1stOdukLabel.begin(), tribSlotsOf1stOdukLabel.end()));
    const LabelOCHImpl ochLabel(freqSlotOf1stOchLabel);
    const LabelOTNImpl odu2eLabel(ServiceType::odu2e, label::TributarySlots(tribSlotsOf2ndOdukLabel.begin(), tribSlotsOf2ndOdukLabel.end()));
    std::vector<label::LabelIFPtr>  labels;
    labels.push_back(std::make_unique<LabelOTNImpl>(odu0Label));
    labels.push_back(std::make_unique<LabelOCHImpl>(ochLabel));
    labels.push_back(std::make_unique<LabelOTNImpl>(odu2eLabel));
    labels.push_back(std::make_unique<LabelOTNImpl>(ServiceType::odu0, label::TributarySlots(tribSlotsOf3rdOdukLabel.begin(), tribSlotsOf3rdOdukLabel.end())));
    labels.push_back(std::make_unique<LabelOCHImpl>(freqSlotOf2ndOchLabel));
    assert(labels.size() == 5);
    std::copy_if(std::make_move_iterator(labels.begin()), std::make_move_iterator(labels.end()),
                 std::back_inserter(layer0Labels),
                 [](const auto& entry)
                 { return ( entry &&
                            entry->getServiceType() == ServiceType::och );});
    assert(layer0Labels.size() == 2);
    assert(std::count_if(labels.begin(), labels.end(),
                         [](const auto& entry){ return entry.get(); /* has valid object */}) == 3);
    std::copy_if(std::make_move_iterator(labels.begin()), std::make_move_iterator(labels.end()),
                 std::back_inserter(layer1Labels),
                 [](const auto& entry)
                 { return ( entry &&
                            ( entry->getServiceType() == ServiceType::odu0 ||
                              entry->getServiceType() == ServiceType::odu2e ) );});
  }
  using TribSlotInitListConstRef = std::reference_wrapper<const std::initializer_list<uint32_t>>;
  using Layer1LabelParams = std::pair<ServiceType, TribSlotInitListConstRef>;
  const std::initializer_list<Layer1LabelParams> expLayer1LabelParams =
    { { ServiceType::odu0, std::cref(tribSlotsOf1stOdukLabel) },
      { ServiceType::odu2e, std::cref(tribSlotsOf2ndOdukLabel) },
      { ServiceType::odu0, std::cref(tribSlotsOf3rdOdukLabel) } };
  const std::initializer_list<label::FrequencySlot> expLayer0LabelParams =
    { freqSlotOf1stOchLabel, freqSlotOf2ndOchLabel };
  assert(std::ranges::equal(expLayer1LabelParams,
                    layer1Labels,
                    [](const auto& expParam, const auto& layer1Label)
                    { return
                        ( layer1Label &&
                          expParam.first == layer1Label->getServiceType() &&
                          ::isEqual(expParam.second.get(), layer1Label->getTributarySlots()) );
                    }));
  assert(std::ranges::equal(expLayer0LabelParams,
                    layer0Labels,
                    [](const auto& expParam, const auto& layer0Label)
                    { return
                        ( layer0Label &&
                          ServiceType::och == layer0Label->getServiceType() &&
                          ::isEqual(expParam, layer0Label->getFrequencySlot()) );
                    }));
}
