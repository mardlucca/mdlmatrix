#include "../h/range.h"

#include <cmath>
#include <iostream>

namespace mdl {
namespace math {

  using std::cout;
  using std::endl;

  const RightRange Ranges::ALL = RightRange(0);
  const UnitRange Ranges::FIRST = UnitRange(0);
  const RightRange Ranges::SKIP_FIRST = RightRange(1);


  LeftRange::LeftRange(size_t end) : end(end) {}

  LeftRange LeftRange::FitToBounds(size_t max) const {
    size_t newEnd = end < 0 ? 0 : end;
    return LeftRange(newEnd <= max ? newEnd : max);
  }

  RightRange::RightRange(size_t start) : RightRange(start, start) {}

  RightRange::RightRange(size_t start, size_t end) 
    : start(start), length(end - start) {}

  RightRange RightRange::FitToBounds(size_t max) const {
    size_t newStart = start < 0 ? 0 : start;
    return RightRange(newStart > max ? max : newStart, max);
  }


  UnitRange::UnitRange(size_t index) : index(index) {}

  UnitRange UnitRange::FitToBounds(size_t max) const {
    size_t newIndex = index < 0 ? -1 : index;
    return UnitRange(newIndex >= max ? -1 : newIndex);
  }


  Range::Range(size_t start, size_t end, size_t increment) : 
      start(start), end(end), increment(increment) {
    if ((increment > 0 && start > end)
        || (increment < 0 && end > start)) {
      length = 0;
    } else {
      length = (std::abs(increment) - 1 + std::abs(end - start)) / std::abs(increment);
    }
  }

  Range Range::FitToBounds(size_t max) const {
    size_t min = 0;
    if (increment < 0) {
      max--;
      min--;
    }

    size_t newStart = start < min ? min : start;
    size_t newEnd = end < min ? min : end;
    
    return Range(newStart > max ? max : newStart,
        newEnd > max ? max : newEnd, increment);
  }


  LeftRange Ranges::Compose(const LeftRange& current, const LeftRange& next) {
    return next;
  }
  RightRange Ranges::Compose(const LeftRange& current, const RightRange& next) {
    return next;
  }
  UnitRange Ranges::Compose(const LeftRange& current, const UnitRange& next) {
    return next;
  }
  Range Ranges::Compose(const LeftRange& current, const Range& next) {
    return next;
  }

  RightRange Ranges::Compose(const RightRange& current, const LeftRange& next) {
    return RightRange(current.Get(0), current.Get(next.Length()));
  }
  RightRange Ranges::Compose(const RightRange& current, const RightRange& next) {
    return RightRange(
        current.Get(next.GetStart()), 
        current.Get(next.GetStart() + next.Length()));
  }
  UnitRange Ranges::Compose(const RightRange& current, const UnitRange& next) {
    return next.Length() == 0 
        ? next
        : UnitRange(current.Get(next.GetIndex()));
  }
  Range Ranges::Compose(const RightRange& current, const Range& next) {
    return Range(current.Get(next.GetStart()),
        current.Get(next.GetEnd()),
        next.GetIncrement());
  }

  UnitRange Ranges::Compose(const UnitRange& current, const LeftRange& next) {
    return next.Length() > 0 ? current : UnitRange(-1);
  }
  UnitRange Ranges::Compose(const UnitRange& current, const RightRange& next) {
    return next.Length() > 0 ? current : UnitRange(-1);
  }
  UnitRange Ranges::Compose(const UnitRange& current, const UnitRange& next) {
    return next.Length() > 0 ? current : UnitRange(-1);
  }
  UnitRange Ranges::Compose(const UnitRange& current, const Range& next) {
    return next.Length() > 0 ? current : UnitRange(-1);
  }

  Range Ranges::Compose(const Range& current, const LeftRange& next) {
    return Range(current.Get(0),
      current.Get(next.Length()),
      current.GetIncrement());
  }
  Range Ranges::Compose(const Range& current, const RightRange& next) {
    return Range(current.Get(next.GetStart()),
      current.GetEnd(),
      current.GetIncrement());
  }
  UnitRange Ranges::Compose(const Range& current, const UnitRange& next) {
    return next.Length() == 0 
          ? next
          : UnitRange(current.Get(next.GetIndex()));
  }
  Range Ranges::Compose(const Range& current, const Range& next) {
    return Range(
        current.Get(next.GetStart()), 
        current.Get(next.GetEnd()), 
        current.GetIncrement() * next.GetIncrement());
  }

} // math
} // mdl
