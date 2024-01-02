#include "LabelOCHImpl.hpp"
#include "LabelOTNImpl.hpp"

#include <cstddef> // std::size_t
#include <cassert>
#include <algorithm>
#include <functional>
#include <vector>
#include <iterator>
#include <iostream>

struct IsNotLayer0Ftor
{
  bool operator()(LabelIF::LabelIFPtr labelPtr) const
  { return
      ( labelPtr &&
        labelPtr->getServiceType() != ServiceType_och ); }
};
struct IsNotLayer1Ftor
{
  bool operator()(LabelIF::LabelIFPtr labelPtr) const
  { return
      ( labelPtr &&
        labelPtr->getServiceType() != ServiceType_odu0 &&
        labelPtr->getServiceType() != ServiceType_odu2e ); }
};

int main()
{
  typedef std::vector<LabelIF::LabelIFPtr> LabelVector;
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  const uint32_t tribSlotsOf1stOdukLabel[] = { 1 };
  const uint32_t tribSlotsOf2ndOdukLabel[] = { 4, 5, 6, 7, 12, 13, 14, 15 }; // ordered for easier tests
  const uint32_t tribSlotsOf3rdOdukLabel[] = { 8 };
  const label::FrequencySlot freqSlotOf1stOchLabel(-32, 4);
  const label::FrequencySlot freqSlotOf2ndOchLabel(-4, 4);
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
  LabelVector  layer0Labels;
  LabelVector  layer1Labels;
  {
    const LabelOTNImpl  odu0Label(ServiceType_odu0, label::TributarySlots(tribSlotsOf1stOdukLabel, tribSlotsOf1stOdukLabel + sizeof(tribSlotsOf1stOdukLabel)/sizeof(uint32_t)));
    const LabelOCHImpl  ochLabel(freqSlotOf1stOchLabel);
    const LabelOTNImpl  odu2eLabel(ServiceType_odu2e, label::TributarySlots(tribSlotsOf2ndOdukLabel, tribSlotsOf2ndOdukLabel + sizeof(tribSlotsOf2ndOdukLabel)/sizeof(uint32_t)));
    LabelVector  labels;
    labels.push_back(new LabelOTNImpl(odu0Label));
    labels.push_back(new LabelOCHImpl(ochLabel));
    labels.push_back(new LabelOTNImpl(odu2eLabel));
    labels.push_back(new LabelOTNImpl(ServiceType_odu0, label::TributarySlots(tribSlotsOf3rdOdukLabel, tribSlotsOf3rdOdukLabel + sizeof(tribSlotsOf3rdOdukLabel)/sizeof(uint32_t))));
    labels.push_back(new LabelOCHImpl(freqSlotOf2ndOchLabel)); // constructed then moved
    assert(labels.size() == 5);
    std::remove_copy_if(labels.begin(), labels.end(),
                        std::back_inserter(layer0Labels),
                        ::IsNotLayer0Ftor());
    std::remove_copy_if(labels.begin(), labels.end(),
                        std::back_inserter(layer1Labels),
                        ::IsNotLayer1Ftor());
    assert(labels.size() == 5);
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
  for (LabelVector::const_iterator iter = layer1Labels.begin(); iter != layer1Labels.end(); ++iter)
  {
    LabelIF::LabelIFPtr labelPtr = *iter;
    const Layer1LabelParams& expLabelParam = expLayer1LabelParams[l1Index];
    // NOLINTBEGIN(clang-analyzer-core.UndefinedBinaryOperatorResult)
    assert(labelPtr);
    if (labelPtr)
    {
      assert(labelPtr->getServiceType() == expLabelParam.m_serviceType);
      label::TributarySlots tributarySlots;
      assert(labelPtr->getTributarySlots(tributarySlots));
      assert(tributarySlots.size() == expLabelParam.m_arrLen);
      assert(std::equal(tributarySlots.begin(), tributarySlots.end(),
                        expLabelParam.m_arrPtr));
    }
    // NOLINTEND(clang-analyzer-core.UndefinedBinaryOperatorResult)
    ++l1Index;
  }
  assert((layer0Labels.size() == sizeof(expLayer0LabelParams)/sizeof(label::FrequencySlot)));
  std::size_t l0Index = 0;
  for (LabelVector::const_iterator iter = layer0Labels.begin(); iter != layer0Labels.end(); ++iter)
  {
    LabelIF::LabelIFPtr labelPtr = *iter;
    const label::FrequencySlot& expLabelParam = expLayer0LabelParams[l0Index];
    assert(labelPtr);
    if (labelPtr)
    {
      assert(labelPtr->getServiceType() == ServiceType_och);
      label::FrequencySlot frequencySlot;
      assert(labelPtr->getFrequencySlot(frequencySlot));
      const bool result =
        std::equal_to<label::FrequencySlot>()(frequencySlot, expLabelParam);
      assert(result);
    }
    ++l0Index;
  }
  for (LabelVector::iterator iter = layer0Labels.begin(); iter != layer0Labels.end(); ++iter)
  {
    LabelIF::LabelIFPtr labelPtr = *iter;
    if (labelPtr)
    {
      delete labelPtr;
    }
  }
  for (LabelVector::iterator iter = layer1Labels.begin(); iter != layer1Labels.end(); ++iter)
  {
    LabelIF::LabelIFPtr labelPtr = *iter;
    if (labelPtr)
    {
      delete labelPtr;
    }
  }
}
