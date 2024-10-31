#ifndef LABEL_TYPES_HPP
#define LABEL_TYPES_HPP

#include <cstdint> // std::uint32_t and std::int32_t
#include <utility> // std::pair
#include <set> // std::set
#include <variant> // std::variant
#include <functional> // std::reference_wrapper

enum class ServiceType : std::uint8_t { och, odu0, odu2e, undef };

namespace label {

using TributarySlots = std::set<std::uint32_t>; // layer 1
using FrequencySlot =
  std::pair<std::int32_t /* channel number <=> center frequency in 6.25 GHz */,
            std::uint32_t /* slot width in 12.5 GHz */>; // layer 0
enum class ErrorCode : std::uint8_t { not_layer_1, not_layer_0 };
using TributarySlotsConstRef = std::reference_wrapper<const TributarySlots>;
using TributarySlotsRef = std::reference_wrapper<TributarySlots>;
using FrequencySlotConstRef = std::reference_wrapper<const FrequencySlot>;
using FrequencySlotRef = std::reference_wrapper<FrequencySlot>;
using TributarySlotsConstResult = std::variant<TributarySlotsConstRef, ErrorCode>;
using TributarySlotsNonConstResult = std::variant<TributarySlotsRef, ErrorCode>;
using FrequencySlotConstResult = std::variant<FrequencySlotConstRef, ErrorCode>;
using FrequencySlotNonConstResult = std::variant<FrequencySlotRef, ErrorCode>;
} // namespace label

#endif /* LABEL_TYPES_HPP */
