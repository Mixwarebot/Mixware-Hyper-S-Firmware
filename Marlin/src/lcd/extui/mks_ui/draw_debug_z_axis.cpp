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

#include "../../../inc/MarlinConfig.h"


extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_DEBUG_Z_LOW = 1,
  ID_DEBUG_Z_FAST,
  ID_DEBUG_Z_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  clear_cur_ui();
  switch (obj->mks_obj_id) {
    case ID_DEBUG_Z_LOW:
      uiCfg.leveling_first_time = 1;
      uiCfg.debug_zaxis_speed = 0;
      lv_draw_dialog(DIALOG_AXIS_Z_TEST_WAIT_START);
      break;
    case ID_DEBUG_Z_FAST:
      uiCfg.leveling_first_time = 1;
      uiCfg.debug_zaxis_speed = 1;
      lv_draw_dialog(DIALOG_AXIS_Z_TEST_WAIT_START);
      break;
    case ID_DEBUG_Z_RETURN:
      draw_return_ui();
      break;
  }
}

void lv_draw_debug_zaxis(void) {
  scr = lv_screen_create(DEBUG_ZAXIS_UI);

  // Create an Image button
  lv_big_button_create(scr, "F:/img_axis_z_test_s.bin",  debug_menu.zaxis_slow,  button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_DEBUG_Z_LOW);
  lv_big_button_create(scr, "F:/img_axis_z_test_f.bin", debug_menu.zaxis_fast, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_DEBUG_Z_FAST);

  lv_screen_menu_item_return(scr, event_handler, ID_DEBUG_Z_RETURN);
}

void lv_clear_debug_zaxis() {
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
