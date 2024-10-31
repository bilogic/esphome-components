#include "Dcf77.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace Bilogic {

bool Dcf77::can_proceed() {
  return millis() >= this->utc_seconds;
}

}  // namespace boot_delay
}  // namespace esphome