#include "esphome/core/log.h"
#include "Dcf77a.h"
#include "Transmitter.h"
#include "TransmitterConfig.h"

namespace esphome {
namespace Bilogic {

static const char *TAG = "Dcf77.switch";

void Dcf77a::setup() {        
    ESP_LOGCONFIG(TAG, "Dcf77a setup()");
    
    ledcSetup(0, 77500, 8);       // DCF77 frequency
    ledcAttachPin(ANTENNAPIN, 0); // This Pin, or another one you choose, has to be attached to the antenna
    
    pinMode(LEDBUILTIN, OUTPUT);
    digitalWrite(LEDBUILTIN, LOW); // LOW if LEDBUILTIN is inverted like in Wemos boards

    if (this->state){
        ESP_LOGCONFIG(TAG, "Dcf77a attach_ms()");    
        tickerDecisec.attach_ms(100, DcfOut); // from now on calls DcfOut every 100ms
    }
    else{
        ESP_LOGCONFIG(TAG, "Dcf77a detach_ms()");
        tickerDecisec.detach();
    }
}

void Dcf77a::write_state(bool state) {   
     if (this->time_id_.has_value()) {        
        time_id = *this->time_id_;
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

    ESP_LOGW(TAG, "W Dcf77a write_state()");
    ESP_LOGCONFIG(TAG, "Dcf77a write_state()");
    this->publish_state(state);
}

void Dcf77a::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty custom switch");
}

} //namespace Dcf77
} //namespace esphome
