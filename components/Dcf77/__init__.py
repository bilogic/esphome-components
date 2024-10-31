import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
 

set_ns = cg.esphome_ns.namespace('Bilogic')

dcf77 = set_ns.class_('Dcf77', cg.Component)

CONF_UTC = "utc"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(dcf77),
    cv.Required(CONF_UTC): cv.use_id(uart.UARTComponent),
})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # utc = await cg.get_variable(config[CONF_UTC])
    # cg.add(var.set_utc(utc))

    cg.add(var.set_utc(config[CONF_UTC]))