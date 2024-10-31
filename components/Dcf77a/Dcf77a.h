#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace Bilogic {

class Dcf77a : public switch_::Switch, public Component {
    public:
        void setup() override;          
        void write_state(bool state) override;
        void dump_config() override;
        void set_utc(uint32_t utc_seconds) { this->utc_seconds = utc_seconds; };
        void set_time_id(time::RealTimeClock *time_id) { this->time_id_ = time_id; };

    protected:
        optional<time::RealTimeClock *> time_id_{};         
        uint32_t utc_seconds;
};

} //namespace Bilogic
} //namespace esphome