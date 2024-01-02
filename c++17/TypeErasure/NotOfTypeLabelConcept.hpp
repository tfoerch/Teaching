#ifndef NOT_OF_TYPE_LABEL_CONCEPT_HPP
#define NOT_OF_TYPE_LABEL_CONCEPT_HPP

#ifdef __has_include // since C++17
#  if __has_include(<concepts>)
#    include <concepts>
#  endif
#endif

#include <type_traits>

class Label;

#if __cpp_concepts
  template <typename T>
  concept NotOfTypeLabel =
    ( !std::same_as<std::decay_t<T>, Label> &&
      !std::derived_from<std::decay_t<T>, Label> );
#else
  template <typename T>
  constexpr inline bool notOfTypeLabel()
  {
    using DecayedT = typename std::decay<T>::type;
    return
      ( !std::is_same_v<DecayedT, Label> &&
        !std::is_base_of_v<Label, DecayedT> );
  }
#endif // __cpp_concepts

#endif /* NOT_OF_TYPE_LABEL_CONCEPT_HPP */
