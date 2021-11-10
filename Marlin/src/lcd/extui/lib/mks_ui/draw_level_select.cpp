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

#if ENABLED(MIXWARE_MODEL_V)

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"

extern lv_group_t * g;
static lv_obj_t * scr;

enum {
  ID_LEVEL_SELECT_AUTO = 1,
  ID_LEVEL_SELECT_MANUAL,
  ID_LEVEL_SELECT_MAIN,
  ID_LEVEL_SELECT_HEIGHT,
  ID_LEVEL_SELECT_RETURN
};

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_LEVEL_SELECT_AUTO:
      uiCfg.para_ui_page = 0;
      #if HAS_ABL_NOT_UBL
        lv_clear_level_select();
        lv_draw_dialog(DIALOG_AUTO_LEVEL_COMPLETED);
      #endif
      break;
    case ID_LEVEL_SELECT_MANUAL:
      uiCfg.leveling_first_time = 1;
      uiCfg.para_ui_page = 0;
      lv_clear_level_select();
      lv_draw_manualLevel();
      break;
    case ID_LEVEL_SELECT_HEIGHT:
      uiCfg.leveling_first_time = 1;
      uiCfg.move_dist = 0.1;
      lv_clear_level_select();
      lv_draw_dialog(DIALOG_ADJUST_Z_HEIGHT_WAIT_START);
      break;
    case ID_LEVEL_SELECT_RETURN:
        lv_clear_level_select();
      if (uiCfg.para_ui_page) {
        uiCfg.para_ui_page = 0;
        lv_draw_level_select();
      }
      else {
        disp_state_stack._disp_index = 2;
        lv_draw_tool();
      }
      break;
    case ID_LEVEL_SELECT_MAIN:
      lv_clear_level_select();
      uiCfg.para_ui_page = 1;
      lv_draw_level_select();
      break;
  }
}

void lv_draw_level_select(void) {
  scr = lv_screen_create(LEVEL_SELECT_UI);

  if (uiCfg.para_ui_page == 0) {
    if (READ(Z2_MIN_PIN) == false)
      lv_big_button_create(scr, "F:/img_level2.bin", tool_menu.leveling,  button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_LEVEL_SELECT_MAIN);
    else
      lv_big_button_create(scr, "F:/img_level_manual.bin", leveling_menu.title,    button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_LEVEL_SELECT_MANUAL);

    lv_big_button_create(scr, "F:/img_level_z_offset.bin", leveling_menu.z_offset,  button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_LEVEL_SELECT_HEIGHT);
  }
  else {
    lv_big_button_create(scr, "F:/img_level_manual.bin", leveling_menu.title,    button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_LEVEL_SELECT_MANUAL);
    lv_big_button_create(scr, "F:/img_level_auto.bin",   tool_menu.autoleveling, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_LEVEL_SELECT_AUTO);
  }

  lv_screen_menu_item_return(scr, event_handler, ID_LEVEL_SELECT_RETURN);
}

void lv_clear_level_select() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // MIXWARE_MODEL_V
#endif // HAS_TFT_LVGL_UI
