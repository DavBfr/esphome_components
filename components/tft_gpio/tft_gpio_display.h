#pragma once
#include "esphome/components/display/display_buffer.h"

class Arduino_ESP32RGBPanel;
class Arduino_RGB_Display;

namespace esphome {
namespace tft_gpio {

class TftGpioDisplay : public PollingComponent, public display::Display {
 public:
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void update() override;
  void dump_config() override;
  void setup() override;
  display::DisplayType get_display_type() override {
    return display::DisplayType::DISPLAY_TYPE_COLOR;
  }
  int get_width() override;
  int get_height() override;
  void draw_pixel_at(int x, int y, Color color) override;

 protected:
  Arduino_ESP32RGBPanel* bus;
  Arduino_RGB_Display* gfx;
  uint16_t* fb_;
};

}  // namespace tft_gpio
}  // namespace esphome
