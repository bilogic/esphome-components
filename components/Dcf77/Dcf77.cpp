#include "esphome/core/log.h"
#include "Dcf77.h"

namespace esphome {
namespace Bilogic {

static const char *TAG = "Dcf77.switch";

void Dcf77::setup() {    
    ESP_LOGW(TAG, "W Dcf77 setup()");
    ESP_LOGCONFIG(TAG, "Dcf77 setup()");
}

// void Dcf77::loop() {
//     ESP_LOGCONFIG(TAG, "Dcf77 loop()");
// }

void Dcf77::write_state(bool state) {
    __super::write_state(state);
}

void Dcf77::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty custom switch");
}

} //namespace Dcf77
} //namespace esphome