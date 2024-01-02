#include "LabelOTN.hpp"

namespace label
{

LabelOTN::LabelOTN(ServiceType serviceType, const TributarySlots& tributarySlots)
: m_serviceType(serviceType),
  m_tributarySlots(tributarySlots)
{}

LabelOTN::LabelOTN(ServiceType serviceType, TributarySlots&& tributarySlots)
: m_serviceType(serviceType),
  m_tributarySlots(std::move(tributarySlots))
{}

auto LabelOTN::encode(MsgBuffer&  /*buffer*/) const -> bool // NOLINT(readability-convert-member-functions-to-static)
{
  // TODO
  return true;
}


}; // namespace label
