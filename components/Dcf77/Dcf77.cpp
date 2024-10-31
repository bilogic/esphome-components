#include "esphome/core/log.h"
#include "Dcf77.h"
#include "Tranmitter.h"

namespace esphome {
namespace Bilogic {

static const char *TAG = "Dcf77.switch";

void Dcf77::setup() {    
    ESP_LOGW(TAG, "W Dcf77 setup()");
    ESP_LOGCONFIG(TAG, "Dcf77 setup()");
    
    ledcSetup(0, 77500, 8);       // DCF77 frequency
    ledcAttachPin(ANTENNAPIN, 0); // This Pin, or another one you choose, has to be attached to the antenna
    
    pinMode(LEDBUILTIN, OUTPUT);
    digitalWrite(LEDBUILTIN, LOW); // LOW if LEDBUILTIN is inverted like in Wemos boards

    tickerDecisec.attach_ms(100, DcfOut); // from now on calls DcfOut every 100ms
}

void Dcf77::write_state(bool state) {   
     if (this->time_id_.has_value()) {        
        auto *time_id = *this->time_id_;
        auto now = time_id->now();
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
