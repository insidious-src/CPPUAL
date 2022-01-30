#ifndef X_VALUEPARAM_HPP
#define X_VALUEPARAM_HPP

#include <map>
#include <vector>

namespace cppual {

class valueparam {
  public:
    valueparam &
    set(const uint32_t & bit, const uint32_t & value)
    {
      _M_has_changed = true;
      _M_values_map[bit] = value;
      return *this;
    }

    uint32_t
    mask(void)
    {
      return _M_mask;
    }

    uint32_t*
    values(void)
    {
      if (_M_has_changed) {
        _M_values.clear();
      }

      for (auto & item : _M_values_map) {
        _M_values.push_back(item.second);
      }

      _M_has_changed = false;

      return _M_values.data();
    }

  private:
    bool _M_has_changed = true;
    uint32_t _M_mask = 0;
    std::vector<uint32_t> _M_values;
    std::map<uint32_t, uint32_t> _M_values_map;
};

} // namespace cppual

#endif // X_VALUEPARAM_HPP
