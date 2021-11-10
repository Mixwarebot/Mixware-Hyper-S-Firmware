/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;
static lv_obj_t *fanText;

enum {
  ID_F_ADD = 1,
  ID_F_DEC,
  ID_F_HIGH,
  ID_F_MID,
  ID_F_OFF,
  ID_F_RETURN
};

static uint8_t fanSpeed;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;

  switch (obj->mks_obj_id) {
    case ID_F_ADD:
      #if ENABLED(MIXWARE_MODEL_V)
        if (fanSpeed <= 255 * 0.8) fanSpeed += (255 * 0.2);
      #else
        if (fanSpeed < 254) fanSpeed++;
      #endif
      break;
    case ID_F_DEC:
      #if ENABLED(MIXWARE_MODEL_V)
        if (fanSpeed >= (255 * 0.2)) fanSpeed -= (255 * 0.2);
      #else
        if (fanSpeed > 0) fanSpeed--;
      #endif
      break;
    case ID_F_HIGH:
      fanSpeed = 255;
      break;
    case ID_F_MID:
      fanSpeed = 127;
      break;
    case ID_F_OFF:
      #if ENABLED(MIXWARE_MODEL_V)
        fanSpeed = 0;
      #endif
      gcode.process_subcommands_now_P(PSTR("M107"));
      return;
    case ID_F_RETURN:
      lv_clear_cur_ui();
      lv_draw_return_ui();
      return;
  }
  sprintf_P(public_buf_l, PSTR("M106 S%d"), fanSpeed);
  gcode.process_subcommands_now(public_buf_l);
}

void lv_draw_fan(void) {
#if DISABLED(MIXWARE_MODEL_V)
  lv_obj_t *buttonAdd;
#endif

  #if HAS_FAN
    fanSpeed = thermalManager.fan_speed[0];
  #endif

  scr = lv_screen_create(FAN_UI);
  // Create an Image button
  #if ENABLED(MIXWARE_MODEL_V)
    lv_big_button_create(scr, "F:/img_speed_slow.bin", fan_menu.off,  button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_F_OFF);
    lv_big_button_create(scr, "F:/img_speed_high.bin", fan_menu.full, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_F_HIGH);
    lv_big_button_create(scr, "F:/img_add.bin",        fan_menu.add,  button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_F_ADD);
    lv_big_button_create(scr, "F:/img_dec.bin",        fan_menu.dec,  button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_F_DEC);
    lv_screen_menu_item_return(scr, event_handler, ID_F_RETURN);
  #else
    buttonAdd  = lv_big_button_create(scr, "F:/bmp_Add.bin", fan_menu.add, INTERVAL_V, titleHeight, event_handler, ID_F_ADD);
    lv_obj_clear_protect(buttonAdd, LV_PROTECT_FOLLOW);
    lv_big_button_create(scr, "F:/bmp_Dec.bin", fan_menu.dec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_F_DEC);
    lv_big_button_create(scr, "F:/bmp_speed255.bin", fan_menu.full, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_F_HIGH);
    lv_big_button_create(scr, "F:/bmp_speed127.bin", fan_menu.half, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_F_MID);
    lv_big_button_create(scr, "F:/bmp_speed0.bin", fan_menu.off, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_F_OFF);
    lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_F_RETURN);
  #endif

  fanText = lv_label_create_empty(scr);
  lv_obj_set_style(fanText, &tft_style_label_rel);
  disp_fan_value();
}

void disp_fan_value() {
  char buf1[10] = {0};
  public_buf_l[0] = '\0';
  strcat(public_buf_l, fan_menu.state);
  strcat_P(public_buf_l, PSTR(": "));
  #if ENABLED(MIXWARE_MODEL_V)
    sprintf_P(buf1, PSTR("%3d%%"), fanSpeed * 100 / 255);
    strcat(public_buf_l, buf1);
    lv_label_set_text(fanText, public_buf_l);
    lv_obj_align(fanText, NULL, LV_ALIGN_IN_TOP_MID, 0, 80);
  #else
    sprintf_P(buf1, PSTR("%3d"), thermalManager.fan_speed[0]);
    strcat(public_buf_l, buf1);
    lv_label_set_text(fanText, public_buf_l);
    lv_obj_align(fanText, nullptr, LV_ALIGN_CENTER, 0, -65);
  #endif
}

void lv_clear_fan() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
