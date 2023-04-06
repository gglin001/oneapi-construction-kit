// Copyright (C) Codeplay Software Limited. All Rights Reserved.

#ifndef MULTI_LLVM_OPTIONAL_HELPER_H_INCLUDED
#define MULTI_LLVM_OPTIONAL_HELPER_H_INCLUDED

#include <llvm/ADT/None.h>
#include <llvm/ADT/Optional.h>

#include <optional>

namespace multi_llvm {
#if (LLVM_VERSION_MAJOR >= 16)
template <typename T>
using Optional = std::optional<T>;
auto None = std::nullopt;
#else
using llvm::None;
using llvm::NoneType;
template <typename T>
class Optional : public llvm::Optional<T> {
 public:
  constexpr Optional() = default;
  constexpr Optional(llvm::NoneType) {}

  constexpr Optional(const T &value) : llvm::Optional<T>(value) {}
  constexpr Optional(T &&value) : llvm::Optional<T>(std::move(value)) {}

  Optional &operator=(const T &y) {
    llvm::Optional<T>::operator=(y);
    return *this;
  }
  Optional &operator=(T &&y) {
    llvm::Optional<T>::operator=(std::forward<T>(y));
    return *this;
  }

  constexpr Optional(llvm::Optional<T> &&value)
      : llvm::Optional<T>(std::move(value)) {}

  inline constexpr bool has_value() const {
    return llvm::Optional<T>::hasValue();
  }

#if (LLVM_VERSION_MAJOR <= 14)
  inline constexpr const T &value() const {
    return llvm::Optional<T>::getValue();
  }
  inline constexpr T &value() { return llvm::Optional<T>::getValue(); }

  template <typename U>
  constexpr T value_or(U &&alt) const & {
    return llvm::Optional<T>::getValueOr(alt);
  }
#endif
};

#endif

}  // namespace multi_llvm

#endif  // MULTI_LLVM_OPTIONAL_HELPER_H_INCLUDED
