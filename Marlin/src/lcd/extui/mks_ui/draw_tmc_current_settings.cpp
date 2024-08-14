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

#if BOTH(HAS_TFT_LVGL_UI, HAS_TRINAMIC_CONFIG)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../module/stepper/indirection.h"
#include "../../../feature/tmc_util.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

// static lv_obj_t *btn_con_x, *btn_con_y, *btn_con_z, *btn_con_e;

enum {
  ID_TMC_CURRENT_RETURN = 1,
  ID_TMC_CURRENT_X,
  ID_TMC_CURRENT_Y,
  ID_TMC_CURRENT_Z,
  ID_TMC_CURRENT_E0,
  // ID_TMC_CURRENT_E1,
  // ID_TMC_CURRENT_UPDATE
  // ID_TMC_CURRENT_DOWN,
  // ID_TMC_CURRENT_UP
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  // if (obj->mks_obj_id == -1) return;
  lv_clear_tmc_current_settings();
  switch (obj->mks_obj_id) {
    case ID_TMC_CURRENT_RETURN:
      // uiCfg.para_ui_page = false;
      draw_return_ui();
      return;
    #if AXIS_IS_TMC(X)
      case ID_TMC_CURRENT_X:  value = Xcurrent;  break;
    #endif
    #if AXIS_IS_TMC(Y)
      case ID_TMC_CURRENT_Y:  value = Ycurrent;  break;
    #endif
    #if AXIS_IS_TMC(Z)
      case ID_TMC_CURRENT_Z:  value = Zcurrent;  break;
    #endif
    #if AXIS_IS_TMC(E0)
      case ID_TMC_CURRENT_E0: value = E0current; break;
    #endif
    #if AXIS_IS_TMC(E1)
      case ID_TMC_CURRENT_E1: value = E1current; break;
    #endif
    // case ID_TMC_CURRENT_UPDATE:
    //   bool tmc_x_res = (test_tmc_connection(true, false, false, false) == 0);
    //   bool tmc_y_res = (test_tmc_connection(false, true, false, false) == 0);
    //   bool tmc_z_res = (test_tmc_connection(false, false, true, false) == 0);
    //   bool tmc_e_res = (test_tmc_connection(false, false, false, true) == 0);
    //   lv_screen_menu_item_onoff_update(btn_con_x, tmc_x_res);
    //   lv_screen_menu_item_onoff_update(btn_con_y, tmc_y_res);
    //   lv_screen_menu_item_onoff_update(btn_con_z, tmc_z_res);
    //   lv_screen_menu_item_onoff_update(btn_con_e, tmc_e_res);
    //   break;

    // case ID_TMC_CURRENT_UP:
    //   uiCfg.para_ui_page = false;
    //   lv_draw_tmc_current_settings();
    //   return;
    // case ID_TMC_CURRENT_DOWN:
    //   uiCfg.para_ui_page = true;
    //   lv_draw_tmc_current_settings();
    //   return;
  }
  lv_draw_number_key();

}

void lv_draw_tmc_current_settings() {
  scr = lv_screen_create(TMC_CURRENT_UI, machine_menu.TmcCurrentConfTitle);

  float milliamps;

  // bool tmc_x_res = (test_tmc_connection(true, false, false, false) == 0);
  // bool tmc_y_res = (test_tmc_connection(false, true, false, false) == 0);
  // bool tmc_z_res = (test_tmc_connection(false, false, true, false) == 0);
  // bool tmc_e_res = (test_tmc_connection(false, false, false, true) == 0);

  // if (!uiCfg.para_ui_page) {
    #if AXIS_IS_TMC(X)
      milliamps = stepperX.getMilliamps();
    #else
      milliamps = -1;
    #endif
    dtostrf(milliamps, 1, 1, public_buf_l);
    // lv_imgbtn_create(scr, tmc_x_res ? "F:/img_enable.bin" : "F:/img_disable.bin", PARA_UI_POS_X, PARA_UI_ENABLED_Y(0), event_handler, -1);
    lv_screen_menu_item_1_edit(scr, machine_menu.X_Current, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(0), event_handler, ID_TMC_CURRENT_X, 0, public_buf_l);

    #if AXIS_IS_TMC(Y)
      milliamps = stepperY.getMilliamps();
    #else
      milliamps = -1;
    #endif
    dtostrf(milliamps, 1, 1, public_buf_l);
    // lv_imgbtn_create(scr, tmc_y_res ? "F:/img_enable.bin" : "F:/img_disable.bin", PARA_UI_POS_X, PARA_UI_ENABLED_Y(1), event_handler, -1);
    lv_screen_menu_item_1_edit(scr, machine_menu.Y_Current, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(1), event_handler, ID_TMC_CURRENT_Y, 1, public_buf_l);

    #if AXIS_IS_TMC(Z)
      milliamps = stepperZ.getMilliamps();
    #else
      milliamps = -1;
    #endif
    dtostrf(milliamps, 1, 1, public_buf_l);
    // lv_imgbtn_create(scr, tmc_z_res ? "F:/img_enable.bin" : "F:/img_disable.bin", PARA_UI_POS_X, PARA_UI_ENABLED_Y(2), event_handler, -1);
    lv_screen_menu_item_1_edit(scr, machine_menu.Z_Current, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(2), event_handler, ID_TMC_CURRENT_Z, 2, public_buf_l);

    #if AXIS_IS_TMC(E0)
      milliamps = stepperE0.getMilliamps();
    #else
      milliamps = -1;
    #endif
    dtostrf(milliamps, 1, 1, public_buf_l);
    // lv_imgbtn_create(scr, tmc_e_res ? "F:/img_enable.bin" : "F:/img_disable.bin", PARA_UI_POS_X, PARA_UI_ENABLED_Y(3), event_handler, -1);
    lv_screen_menu_item_1_edit(scr, machine_menu.E0_Current, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(3), event_handler, ID_TMC_CURRENT_E0, 3, public_buf_l);

  // lv_screen_menu_item_turn_page(scr, machine_menu.update, event_handler, ID_TMC_CURRENT_UPDATE);
  lv_screen_menu_item_return(scr, event_handler, ID_TMC_CURRENT_RETURN);
}

void lv_clear_tmc_current_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && HAS_TRINAMIC_CONFIG
