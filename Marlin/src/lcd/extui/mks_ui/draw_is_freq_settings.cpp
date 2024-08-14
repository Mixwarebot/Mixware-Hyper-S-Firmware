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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../module/planner.h"
#include "../../../module/stepper.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_IS_FREQ_RETURN = 1,
  ID_IS_FREQ_X,
  ID_IS_FREQ_Y
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;

  lv_clear_is_freq_settings();
  switch (obj->mks_obj_id) {
    case ID_IS_FREQ_RETURN:
      draw_return_ui();
      return;
    case ID_IS_FREQ_X:
      value = is_freq_x;
      break;
    case ID_IS_FREQ_Y:
      value = is_freq_y;
      break;
  }
  lv_draw_number_key();
}

void lv_draw_is_freq_settings() {
  scr = lv_screen_create(IS_FREQ_UI, is_freq_menu.is_freq_menu);

#if EITHER(INPUT_SHAPING_X, INPUT_SHAPING_Y)
  #if ENABLED(INPUT_SHAPING_X)
  dtostrf(stepper.get_shaping_frequency(X_AXIS), 1, 2, public_buf_l);
  lv_screen_menu_item_1_edit(scr, is_freq_menu.is_freq_menu_x, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(0), event_handler, ID_IS_FREQ_X, 0, public_buf_l);
  #endif
  #if ENABLED(INPUT_SHAPING_Y)
  dtostrf(stepper.get_shaping_frequency(Y_AXIS), 1, 2, public_buf_l);
  lv_screen_menu_item_1_edit(scr, is_freq_menu.is_freq_menu_y, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(1), event_handler, ID_IS_FREQ_Y, 1, public_buf_l);
  #endif

  lv_obj_t *labelTips = lv_label_create(scr, is_freq_menu.is_freq_menu_tips);
  lv_obj_align(labelTips, nullptr, LV_ALIGN_CENTER, 0, -20);
#endif

  lv_screen_menu_item_return(scr, event_handler, ID_IS_FREQ_RETURN);
}

void lv_clear_is_freq_settings() {
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
