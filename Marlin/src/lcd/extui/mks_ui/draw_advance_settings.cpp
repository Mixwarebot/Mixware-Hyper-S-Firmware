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
#include "../../../module/temperature.h"

extern lv_group_t *g;
static lv_obj_t *scr, *buttonTP;

enum {
  ID_ADVANCE_RETURN = 1,
  ID_PAUSE_POS,
  ID_WIFI_PARA,
  ID_FILAMENT_SETTINGS,
  ID_ENCODER_SETTINGS
  , ID_PIDTEMP_SETTINGS
  , ID_ADVSETUP_STEP
  // , ID_ADVSETUP_SPEED
  , ID_ADVSETUP_ACCEL
  , ID_ADVSETUP_JERK
  , ID_ADVSETUP_PAUSEPOS
  , ID_ADVSETUP_FILAMENT
  , ID_ADVSETUP_TEMPMODE
  , ID_ADVSETUP_THERMALPROTECTION
  , ID_ADVSETUP_TMC
  , ID_ADVSETUP_IS
  , ID_ADVSETUP_MIN_TEMP
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_ADVANCE_RETURN:
      // lv_clear_advance_settings();
      // lv_draw_set();
      goto_previous_ui();
      break;
    case ID_PAUSE_POS:
      lv_clear_advance_settings();
      lv_draw_pause_position();
      break;
    case ID_FILAMENT_SETTINGS:
      lv_clear_advance_settings();
      lv_draw_filament_settings();
      break;
    case ID_PIDTEMP_SETTINGS:
      lv_clear_advance_settings();
      lv_draw_pidtemp_switch();
      break;
      case ID_ADVSETUP_STEP:
        lv_clear_advance_settings();
        lv_draw_step_settings();
        break;
      // case ID_ADVSETUP_SPEED:
      //   lv_clear_advance_settings();
      //   lv_draw_max_feedrate_settings();
      //   break;
      case ID_ADVSETUP_ACCEL:
        lv_clear_advance_settings();
        lv_draw_acceleration_settings();
        break;
      #if HAS_CLASSIC_JERK
        case ID_ADVSETUP_JERK:
          lv_clear_advance_settings();
          lv_draw_jerk_settings();
          break;
      #endif
      case ID_ADVSETUP_PAUSEPOS:
        lv_clear_advance_settings();
        lv_draw_pause_position();
        break;
      case ID_ADVSETUP_FILAMENT:
        lv_clear_advance_settings();
        lv_draw_filament_settings();
        break;
      case ID_ADVSETUP_TEMPMODE:
        lv_clear_advance_settings();
        lv_draw_pidtemp_switch();
        // MUI.page_draw_heating_mode_setup();
        break;
      case ID_ADVSETUP_THERMALPROTECTION:
        toggle_thermal_protection();
        lv_imgbtn_set_src_both(buttonTP, get_thermal_protection() ? "F:/img_enable.bin" : "F:/img_disable.bin");

        update_spi_flash();
        break;

      case ID_ADVSETUP_TMC:
        lv_clear_advance_settings();
        lv_draw_tmc_current_settings();
        break;
      case ID_ADVSETUP_IS:
        lv_clear_advance_settings();
        lv_draw_is_freq_settings();
        break;
      case ID_ADVSETUP_MIN_TEMP:
        value = extrude_min_temp;
        lv_clear_advance_settings();
        lv_draw_number_key();
        break;
  }
}

void lv_draw_advance_settings() {
  scr = lv_screen_create(ADVANCED_UI, machine_menu.AdvancedConfTitle);

  int index = 0;
  lv_screen_menu_item(scr, machine_menu.StepsConf, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(index), event_handler, ID_ADVSETUP_STEP, index);
  index++;
  lv_screen_menu_item(scr, machine_menu.AccelerationConf, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(index), event_handler, ID_ADVSETUP_ACCEL, index);
  index++;
  #if HAS_CLASSIC_JERK
  lv_screen_menu_item(scr, machine_menu.JerkConf, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(index), event_handler, ID_ADVSETUP_JERK, index);
  index++;
  #endif
  // TMC
  lv_screen_menu_item(scr, machine_menu.TMCcurrentConf, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(index), event_handler, ID_ADVSETUP_TMC, index);
  index++;

  // IS
  lv_screen_menu_item(scr, is_freq_menu.is_freq_menu_set, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(index), event_handler, ID_ADVSETUP_IS, index);
  index++;

  // EXTRUDE_MINTEMP 170
  dtostrf(thermalManager.extrude_min_temp, 1, 0, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.extrudeMinTemp, PARA_UI_POS_X, PARA_UI_MENU_ITEM_Y(index), event_handler, ID_ADVSETUP_MIN_TEMP, index, public_buf_l);

  lv_screen_menu_item_return(scr, event_handler, ID_ADVANCE_RETURN);
}

void lv_clear_advance_settings() {
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
