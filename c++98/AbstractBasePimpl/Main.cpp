#include "Label.hpp"

#include <cstddef> // std::size_t
#include <cassert>
#include <algorithm>
#include <functional>
#include <vector>
#include <iterator>
#include <iostream>

namespace {

struct IsNotLayer0Ftor
{
  bool operator()(const Label& label) const
  { return label.getServiceType() != ServiceType_och; }
};
struct IsNotLayer1Ftor
{
  bool operator()(const Label& label) const
  { return
      ( label.getServiceType() != ServiceType_odu0 &&
        label.getServiceType() != ServiceType_odu2e ); }
};

} // namespace anonymous

int main()
{
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  const uint32_t tribSlotsOf1stOdukLabel[] = { 1 };
  const uint32_t tribSlotsOf2ndOdukLabel[] = { 4, 5, 6, 7, 12, 13, 14, 15 }; // ordered for easier tests
  const uint32_t tribSlotsOf3rdOdukLabel[] = { 8 };
  const label::FrequencySlot freqSlotOf1stOchLabel(-32, 4);
  const label::FrequencySlot freqSlotOf2ndOchLabel(-4, 4);
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
  std::vector<Label>  layer0Labels;
  std::vector<Label>  layer1Labels;
  {
    const Label  odu0Label(ServiceType_odu0, label::TributarySlots(tribSlotsOf1stOdukLabel, tribSlotsOf1stOdukLabel + sizeof(tribSlotsOf1stOdukLabel)/sizeof(uint32_t)));
    const Label  ochLabel(freqSlotOf1stOchLabel);
    const Label  odu2eLabel(ServiceType_odu2e, label::TributarySlots(tribSlotsOf2ndOdukLabel, tribSlotsOf2ndOdukLabel + sizeof(tribSlotsOf2ndOdukLabel)/sizeof(uint32_t)));
    std::vector<Label>  labels;
    labels.push_back(odu0Label);
    labels.push_back(ochLabel);
    labels.push_back(odu2eLabel);
    labels.push_back(Label(ServiceType_odu0, label::TributarySlots(tribSlotsOf3rdOdukLabel, tribSlotsOf3rdOdukLabel + sizeof(tribSlotsOf3rdOdukLabel)/sizeof(uint32_t))));
    labels.push_back(Label(freqSlotOf2ndOchLabel));
    assert(labels.size() == 5);
    std::remove_copy_if(labels.begin(), labels.end(),
                        std::back_inserter(layer0Labels),
                        ::IsNotLayer0Ftor());
    std::remove_copy_if(labels.begin(), labels.end(),
                        std::back_inserter(layer1Labels),
                        ::IsNotLayer1Ftor());
    assert(labels.empty());
  }
  // validation
  struct Layer1LabelParams
  {
    ServiceType     m_serviceType;
    const uint32_t* m_arrPtr;
    std::size_t     m_arrLen;
  };
  const Layer1LabelParams expLayer1LabelParams[] =
    { { ServiceType_odu0, tribSlotsOf1stOdukLabel, sizeof(tribSlotsOf1stOdukLabel)/sizeof(uint32_t) },
      { ServiceType_odu2e, tribSlotsOf2ndOdukLabel, sizeof(tribSlotsOf2ndOdukLabel)/sizeof(uint32_t) },
      { ServiceType_odu0, tribSlotsOf3rdOdukLabel, sizeof(tribSlotsOf3rdOdukLabel)/sizeof(uint32_t) } };
  const label::FrequencySlot expLayer0LabelParams[] =
    { freqSlotOf1stOchLabel, freqSlotOf2ndOchLabel };
  assert((layer1Labels.size() == sizeof(expLayer1LabelParams)/sizeof(Layer1LabelParams)));
  std::size_t l1Index = 0;
  for (std::vector<Label>::const_iterator iter = layer1Labels.begin(); iter != layer1Labels.end(); ++iter)
  {
    const Label& label = *iter;
    const Layer1LabelParams& expLabelParam = expLayer1LabelParams[l1Index];
    // NOLINTBEGIN(clang-analyzer-core.UndefinedBinaryOperatorResult)
    assert(label.getServiceType() == expLabelParam.m_serviceType);
    {
      label::TributarySlots tributarySlots;
      assert(label.getTributarySlots(tributarySlots));
      assert(tributarySlots.size() == expLabelParam.m_arrLen);
      assert(std::equal(tributarySlots.begin(), tributarySlots.end(),
                        expLabelParam.m_arrPtr));
    }
    // NOLINTEND(clang-analyzer-core.UndefinedBinaryOperatorResult)
    ++l1Index;
  }
  assert((layer0Labels.size() == sizeof(expLayer0LabelParams)/sizeof(label::FrequencySlot)));
  std::size_t l0Index = 0;
  for (std::vector<Label>::const_iterator iter = layer0Labels.begin(); iter != layer0Labels.end(); ++iter)
  {
    const Label& label = *iter;
    const label::FrequencySlot& expLabelParam = expLayer0LabelParams[l0Index];
    assert(label.getServiceType() == ServiceType_och);
    label::FrequencySlot frequencySlot;
    assert(label.getFrequencySlot(frequencySlot));
    const bool result =
      std::equal_to<label::FrequencySlot>()(frequencySlot, expLabelParam);
    assert(result);
    ++l0Index;
  }
}
