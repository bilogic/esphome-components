import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch, time
from esphome.const import CONF_ID, CONF_TIME_ID

set_ns = cg.esphome_ns.namespace('Bilogic')
Dcf77 = set_ns.class_('Dcf77', switch.Switch, time.RealTimeClock, cg.Component)
CONF_UTC = "utc_seconds"

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Dcf77),    
    cv.Optional(CONF_TIME_ID): cv.use_id(time.RealTimeClock)
    # cv.Optional(CONF_UTC): 28800,
    # cv.Required(CONF_UTC): cv.use_id(uart.UARTComponent)
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)

    # cg.add_library("ESP32Ticker", None)
    # cg.add_library("framework-arduinoespressif32", "3.20011.230801")
    # cg.add_library("espressif32", "6.0.0")

    if CONF_TIME_ID in config:
        time_ = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time_id(time_))
    # cg.add(var.set_utc(config[CONF_UTC]))