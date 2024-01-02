#ifndef LABEL_TYPES_HPP
#define LABEL_TYPES_HPP

#include <stdint.h> // uint32_t and int32_t
#include <utility> // std::pair
#include <set> // std::set

enum ServiceType { ServiceType_och, ServiceType_odu0, ServiceType_odu2e, ServiceType_undef };

namespace label {
typedef std::set<uint32_t> TributarySlots; // layer 1
typedef std::pair<int32_t /* channel number <=> center frequency in 6.25 GHz */,
                  uint32_t /* slot width in 12.5 GHz */> FrequencySlot; // layer 0
} // namespace label

#endif /* LABEL_TYPES_HPP */
