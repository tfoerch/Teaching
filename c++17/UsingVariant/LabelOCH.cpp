#include "LabelOCH.hpp"

namespace label
{

LabelOCH::LabelOCH(const FrequencySlot& frequencySlot)
: m_frequencySlot(frequencySlot)
{}

LabelOCH::LabelOCH(FrequencySlot&& frequencySlot)
: m_frequencySlot(std::move(frequencySlot))
{}

auto LabelOCH::encode(MsgBuffer&  /*buffer*/) const -> bool // NOLINT(readability-convert-member-functions-to-static)
{
  // TODO
  return true;
}

}; // namespace label
