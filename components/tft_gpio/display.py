import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core, pins
from esphome.components import display, font
from esphome.core import CORE, HexInt
from esphome.const import (
    CONF_COLOR_PALETTE,
    CONF_DC_PIN,
    CONF_ID,
    CONF_LAMBDA,
    CONF_MODEL,
    CONF_RAW_DATA_ID,
    CONF_PAGES,
    CONF_RESET_PIN,
    CONF_DIMENSIONS,
)

# DEPENDENCIES = ["spi"]


def AUTO_LOAD():
    if CORE.is_esp32:
        return ["psram", "spi"]
    return []


CODEOWNERS = ["@davbfr"]

tft_gpio_ns = cg.esphome_ns.namespace("tft_gpio")
tft_gpio = tft_gpio_ns.class_("TftGpioDisplay", cg.PollingComponent, display.DisplayBuffer)


def _validate(config):
    if not CORE.is_esp32:
        raise cv.Invalid("Provided model can't run on ESP8266. Use an ESP32 with PSRAM onboard")
    return config


CONFIG_SCHEMA = cv.All(
    font.validate_pillow_installed,
    display.FULL_DISPLAY_SCHEMA.extend({
        cv.GenerateID(): cv.declare_id(tft_gpio),
    })
    .extend(cv.polling_component_schema("1s")),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
    _validate,
)


async def to_code(config):
    if CORE.using_arduino:
        cg.add_library("Wire", None)
    cg.add_library("moononournation/GFX Library for Arduino", "1.4.0")

    var = cg.new_Pvariable(config[CONF_ID])

    await display.register_display(var, config)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
