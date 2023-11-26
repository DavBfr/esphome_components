#include "tft_gpio_display.h"

#include <Arduino_GFX_Library.h>

#include "esphome/core/application.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

#define GFX_BL DF_GFX_BL
#define TFT_BL 2

namespace esphome {
namespace tft_gpio {

static const char* const TAG = "tft_gpio";

void TftGpioDisplay::setup() {
  set_auto_clear(false);

  bus = new Arduino_ESP32RGBPanel(
      40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */, 45 /* R0 */,
      48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */, 5 /* G0 */,
      6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */, 8 /* B0 */,
      3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */, 0 /* hsync_polarity */,
      8 /* hsync_front_porch */, 4 /* hsync_pulse_width */,
      8 /* hsync_back_porch */, 0 /* vsync_polarity */,
      8 /* vsync_front_porch */, 4 /* vsync_pulse_width */,
      8 /* vsync_back_porch */, 1 /* pclk_active_neg */,
      16000000 /* prefer_speed */);
  gfx = new Arduino_RGB_Display(800, 480, bus);
  gfx->begin();
  gfx->fillScreen(WHITE);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);


  ExternalRAMAllocator<uint8_t> allocator(ExternalRAMAllocator<uint8_t>::NONE);
  fb_ = (uint16_t*)allocator.allocate(gfx->width() * gfx->height() * 2);
}

int TftGpioDisplay::get_width() {
  return gfx->width();
}

int TftGpioDisplay::get_height() {
  return gfx->height();
}

void HOT TftGpioDisplay::draw_pixel_at(int x, int y, Color color) {
  auto color565 = display::ColorUtil::color_to_565( color, display::ColorOrder::COLOR_ORDER_RGB);
  fb_[x + y * gfx->width()] = color565;
}

void TftGpioDisplay::update() {
  this->do_update_();
  gfx->draw16bitRGBBitmap(0, 0, fb_, gfx->width(), gfx->height());
}

void TftGpioDisplay::dump_config() {
  LOG_DISPLAY("", "tft_gpio", this);
  ESP_LOGCONFIG(TAG, "  Color mode: 16bit");
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace tft_gpio
}  // namespace esphome
