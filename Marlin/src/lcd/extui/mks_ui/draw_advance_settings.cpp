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

#if ENABLED(TFT_MIXWARE_LVGL_UI)
  static lv_obj_t *buttonTP;
#endif

enum {
  ID_ADVANCE_RETURN = 1,
  ID_PAUSE_POS,
  ID_WIFI_PARA,
  ID_FILAMENT_SETTINGS,
  ID_ENCODER_SETTINGS
  #if ENABLED(TFT_MIXWARE_LVGL_UI)
    ,ID_ADVSETUP_STEP
    ,ID_ADVSETUP_SPEED
    ,ID_ADVSETUP_ACCEL
    ,ID_ADVSETUP_JERK
    ,ID_ADVSETUP_PAUSEPOS
    ,ID_ADVSETUP_FILAMENT
    ,ID_ADVSETUP_TEMPMODE
    ,ID_ADVSETUP_THERMALPROTECTION
  #endif
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_ADVANCE_RETURN:
      lv_clear_advance_settings();
      TERN(TFT_MIXWARE_LVGL_UI, lv_draw_set(), draw_return_ui());
      break;
    case ID_PAUSE_POS:
      lv_clear_advance_settings();
      lv_draw_pause_position();
      break;
    case ID_FILAMENT_SETTINGS:
      lv_clear_advance_settings();
      lv_draw_filament_settings();
      break;
    #if ENABLED(MKS_WIFI_MODULE)
      case ID_WIFI_PARA:
        lv_clear_advance_settings();
        lv_draw_wifi_settings();
        break;
    #endif
    #if HAS_ROTARY_ENCODER
      case ID_ENCODER_SETTINGS:
        lv_clear_advance_settings();
        lv_draw_encoder_settings();
        break;
    #endif
    #if ENABLED(TFT_MIXWARE_LVGL_UI)
      case ID_ADVSETUP_STEP:
        lv_clear_advance_settings();
        lv_draw_step_settings();
        break;
      case ID_ADVSETUP_SPEED:
        lv_clear_advance_settings();
        lv_draw_max_feedrate_settings();
        break;
      case ID_ADVSETUP_ACCEL:
        lv_clear_advance_settings();
        lv_draw_acceleration_settings();
        break;
      case ID_ADVSETUP_JERK:
        lv_clear_advance_settings();
        lv_draw_jerk_settings();
        break;
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
        MUI.page_draw_heating_mode_setup();
        break;
      case ID_ADVSETUP_THERMALPROTECTION:
        MUI.toggle_thermal_protection();
        MUI.update_button_enabled(buttonTP, MUI.get_thermal_protection());
        update_spi_flash();
        break;
    #endif
  }
}

void lv_draw_advance_settings() {
  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    scr = lv_screen_create(ADVANCED_UI, machine_menu.AdvancedConfTitle);

    int index = 0;
    lv_screen_menu_item(scr, machine_menu.PausePosition, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_PAUSE_POS, index++);
    lv_screen_menu_item(scr, machine_menu.FilamentConf, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_FILAMENT_SETTINGS, index++);
    #if ENABLED(MKS_WIFI_MODULE)
      lv_screen_menu_item(scr, machine_menu.WifiSettings, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_WIFI_PARA, index++);
    #endif
    #if HAS_ROTARY_ENCODER
      lv_screen_menu_item(scr, machine_menu.EncoderSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (index + 1), event_handler, ID_ENCODER_SETTINGS, index);
      index++;
    #endif
    lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACK_POS_X + 10, PARA_UI_BACK_POS_Y, event_handler, ID_ADVANCE_RETURN, true);
  #else
    int index = 0;

    scr = lv_screen_create(ADVANCED_UI, MTR.ADVSet);

    M_SCREEN_ITEM(MTR.ADVSetStep,         ID_ADVSETUP_STEP, index);
    index++;
    M_SCREEN_ITEM(MTR.ADVSetMaxSpeed,     ID_ADVSETUP_SPEED, index);
    index++;
    M_SCREEN_ITEM(MTR.ADVSetAccel,        ID_ADVSETUP_ACCEL, index);
    index++;
    M_SCREEN_ITEM(MTR.ADVSetJerk,         ID_ADVSETUP_JERK, index);
    index++;
    M_SCREEN_ITEM(MTR.ADVSetPausePos,     ID_ADVSETUP_PAUSEPOS, index);
    index++;
    M_SCREEN_ITEM(MTR.ADVSetFil,          ID_ADVSETUP_FILAMENT, index);
    index++;
    M_SCREEN_ITEM(MTR.ADVSeteHeatingMode, ID_ADVSETUP_TEMPMODE, index);
    index++;
    buttonTP = MUI.page_button_enabled(scr, MTR.thermal_protection, PARA_UI_POS_X, M_ITEM_POS_Y(index), event_handler, ID_ADVSETUP_THERMALPROTECTION, index, MUI.get_thermal_protection());

    MUI.page_button_return(scr, event_handler, ID_ADVANCE_RETURN);
  #endif
}

void lv_clear_advance_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
