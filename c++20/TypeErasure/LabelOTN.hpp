#ifndef LABEL_OTN_HPP
#define LABEL_OTN_HPP

#include "LabelTypes.hpp"

class MsgBuffer;

namespace label
{
  class LabelOTN
  {
  public:
    LabelOTN(ServiceType serviceType, const TributarySlots& tributarySlots);
    LabelOTN(ServiceType serviceType, TributarySlots&& tributarySlots);
    constexpr auto getServiceType() const -> ServiceType { return m_serviceType; }
    constexpr auto getTributarySlots() const -> const TributarySlots& { return m_tributarySlots; }
    constexpr auto accessTributarySlots() -> TributarySlots& { return m_tributarySlots; }
    auto encode(MsgBuffer&  buffer) const -> bool;
  private:
    ServiceType     m_serviceType;
    TributarySlots  m_tributarySlots;
  };
}; // namespace label

#endif /* LABEL_OTN_HPP */
