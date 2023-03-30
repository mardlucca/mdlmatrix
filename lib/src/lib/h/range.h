#ifndef _MDL_MATH_RANGE
#define _MDL_MATH_RANGE

#include <climits>

#include "typedefs.h"

namespace mdl {
namespace math {

  class LeftRange {
    public:
      LeftRange(size_t end);

      inline size_t Get(size_t index) const { return  index; }
      inline size_t Length() const { return end; }

      LeftRange FitToBounds(size_t max) const;
    private:
      size_t end;
  };

  class RightRange {
    public:
      RightRange(size_t start);

      inline size_t Get(size_t index) const { return start + index; }
      inline size_t GetStart() const { return start; }
      inline size_t Length() const { return length; }

      RightRange FitToBounds(size_t max) const;
    private:   
      size_t start;
      size_t length;

      RightRange(size_t start, size_t end);

      friend class Ranges;
  };

  class UnitRange {
    public:
      UnitRange(size_t index);

      inline size_t Get(size_t i) const { return index; }
      inline size_t GetIndex() const { return index; }
      inline size_t Length() const { return index >= 0; }

      UnitRange FitToBounds(size_t max) const;
    private:   
      size_t index;
  };

  class Range {
    public:
      size_t start;
      size_t end;
      size_t increment;
      size_t length;

      Range(size_t start = 0, size_t end = INT_MAX, size_t increment = 1);
      
      inline size_t Get(size_t index) const {   
          return  start + index * increment;
      }
      inline size_t GetStart() const { return start; }
      inline size_t GetEnd() const { return end; }
      inline size_t GetIncrement() const { return increment; }
      inline size_t Length() const { return length; };    

      Range FitToBounds(size_t max) const;
  };

  class Ranges {
    public:
      const static RightRange ALL;
      const static UnitRange FIRST;
      const static RightRange SKIP_FIRST;

      static LeftRange Compose(const LeftRange& current, const LeftRange& next);
      static RightRange Compose(const LeftRange& current, const RightRange& next);
      static UnitRange Compose(const LeftRange& current, const UnitRange& next);
      static Range Compose(const LeftRange& current, const Range& next);

      static RightRange Compose(const RightRange& current, const LeftRange& next);
      static RightRange Compose(const RightRange& current, const RightRange& next);
      static UnitRange Compose(const RightRange& current, const UnitRange& next);
      static Range Compose(const RightRange& current, const Range& next);

      static UnitRange Compose(const UnitRange& current, const LeftRange& next);
      static UnitRange Compose(const UnitRange& current, const RightRange& next);
      static UnitRange Compose(const UnitRange& current, const UnitRange& next);
      static UnitRange Compose(const UnitRange& current, const Range& next);

      static Range Compose(const Range& current, const LeftRange& next);
      static Range Compose(const Range& current, const RightRange& next);
      static UnitRange Compose(const Range& current, const UnitRange& next);
      static Range Compose(const Range& current, const Range& next);

      template <typename CUR, typename NXT>
      static auto FitAndCompose(const CUR& current, const NXT& next) {
        return Ranges::Compose(current, next.FitToBounds(current.Length()));
      }
  };

} // math
} // mdl

#endif //_MDL_MATH_RANGE