#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace Bilogic {

class Dcf77 : public Component {
 public:
  bool can_proceed() override;
  void set_utc(uint32_t utc_seconds) { this->utc_seconds = utc_seconds; }

 protected:
  uint32_t utc_seconds;
};

}  // namespace bilogic
}  // namespace esphome