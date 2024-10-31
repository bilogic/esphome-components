#include "esphome/core/log.h"
#include "Dcf77.h"

namespace esphome {
namespace Bilogic {

static const char *TAG = "Dcf77.switch";

void Dcf77::setup() {    
    ESP_LOGW(TAG, "W Dcf77 setup()");
    ESP_LOGCONFIG(TAG, "Dcf77 setup()");
}

void Dcf77::write_state(bool state) {   
     if (this->time_id_.has_value()) {
        time::ESPTime now = this->time_id->now();
        uint8_t year = now.year - 2000;
        uint8_t month = now.month;
        uint8_t day_of_month = now.day_of_month;
        uint8_t hour = now.hour;
        uint8_t minute = now.minute;
        uint8_t second = now.second;
        ESP_LOGW(TAG, "date %d-%d-%d", year, month, day_of_month);
        ESP_LOGW(TAG, "date %d:%d:%d", hour, minute, second);
      } else {
        ESP_LOGW(TAG, "LOCAL_TIME_QUERY is not handled because time is not configured");
      }

    ESP_LOGW(TAG, "W Dcf77 state()");
    ESP_LOGCONFIG(TAG, "Dcf77 setup()");
    this->publish_state(state);
}

void Dcf77::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty custom switch");
}

} //namespace Dcf77
} //namespace esphome
