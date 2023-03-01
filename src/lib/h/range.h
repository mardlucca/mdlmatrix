#ifndef _MDL_MATH_RANGE
#define _MDL_MATH_RANGE

#include <climits>

namespace mdl {
namespace math {

  class LeftRange {
    public:
      LeftRange(int end);

      inline int Get(int index) const { return  index; }
      inline int Length() const { return end; }

      LeftRange FitToBounds(int max) const;
    private:
      int end;
  };

  class RightRange {
    public:
      RightRange(int start);

      inline int Get(int index) const { return start + index; }
      inline int GetStart() const { return start; }
      inline int Length() const { return length; }

      RightRange FitToBounds(int max) const;
    private:   
      int start;
      int length;

      RightRange(int start, int end);

      friend class Ranges;
  };

  class UnitRange {
    public:
      UnitRange(int index);

      inline int Get(int i) const { return index; }
      inline int GetIndex() const { return index; }
      inline int Length() const { return index >= 0; }

      UnitRange FitToBounds(int max) const;
    private:   
      int index;
  };

  class Range {
    public:
      int start;
      int end;
      int increment;
      int length;

      Range(int start = 0, int end = INT_MAX, int increment = 1);
      
      inline int Get(int index) const {   
          return  start + index * increment;
      }
      inline int GetStart() const { return start; }
      inline int GetEnd() const { return end; }
      inline int GetIncrement() const { return increment; }
      inline int Length() const { return length; };    

      Range FitToBounds(int max) const;
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