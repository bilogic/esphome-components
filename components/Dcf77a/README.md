# Emit a DCF77 signal based on the MCU's time
# Works only on an ESP32's ledc

Example:
```yaml
esp32:
  board: esp32dev
  framework:
    type: arduino # make sure to compile using arduino, esp-idf won't work

# reference the repository
external_components:
  - source: github://bilogic/esphome-external-components
    # refresh: 0s # used only during development

# ask NTP for an accurate time
time:
  - platform: sntp
    id: sntp_time
    
switch:
  - platform: Dcf77a
    name: Dcf77 Tx
    time_id: sntp_time
```
