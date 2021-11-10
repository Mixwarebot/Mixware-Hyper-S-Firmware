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
#include "../../../../gcode/gcode.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"
#include "../../../../inc/MarlinConfig.h"

#if (FLASH_INF_VALID_FLAG >= 0x20210629)

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_PIDTEMP_NORMAL = 1,
  ID_PIDTEMP_HIGH,
  ID_PIDTEMP_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_PIDTEMP_NORMAL:
      if (gCfgItems.filament_max_temper < 300)
        break;
      lv_clear_pidtemp_switch();
      lv_draw_dialog(DIALOG_PIDTEMP_SW_NORMAL);
      break;
    case ID_PIDTEMP_HIGH:
      if (gCfgItems.filament_max_temper > 300)
        break;
      lv_clear_pidtemp_switch();
      lv_draw_dialog(DIALOG_PIDTEMP_SW_HIGH);
      break;
    case ID_PIDTEMP_RETURN:
      lv_clear_cur_ui();
      lv_draw_return_ui();
      break;
  }
}

void lv_draw_pidtemp_switch(void) {
  scr = lv_screen_create(PIDTEMP_SW_UI, pidtemp_sw_menu.title);
  // Create an Image button
  lv_big_button_create(scr, "F:/img_normal_mode.bin",  pidtemp_sw_menu.normal,  button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_PIDTEMP_NORMAL);
  lv_big_button_create(scr, "F:/HI_high_mode.bin", pidtemp_sw_menu.high, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_PIDTEMP_HIGH);

  lv_obj_t *labelTips = lv_label_create(scr, gCfgItems.filament_max_temper < 300 ? pidtemp_sw_menu.n_model : pidtemp_sw_menu.h_model);
  lv_obj_align(labelTips, nullptr, LV_ALIGN_CENTER, 0, -130);

  lv_screen_menu_item_return(scr, event_handler, ID_PIDTEMP_RETURN);
}

void lv_clear_pidtemp_switch() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif

#endif // HAS_TFT_LVGL_UI
