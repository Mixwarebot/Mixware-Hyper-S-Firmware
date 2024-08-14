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
#include "../../../sd/cardreader.h"
#include "../../../module/temperature.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_FILAMENT_TEMP_170 = 170,
  ID_FILAMENT_TEMP_180 = 180,
  ID_FILAMENT_TEMP_190 = 190,
  ID_FILAMENT_TEMP_200 = 200,
  ID_FILAMENT_TEMP_210 = 210,
  ID_FILAMENT_TEMP_220 = 220,
  ID_FILAMENT_TEMP_230 = 230,
  ID_FILAMENT_TEMP_240 = 240,
  ID_FILAMENT_TEMP_250 = 250,
  ID_FILAMENT_TEMP_260 = 260,
  ID_FILAMENT_TEMP_270 = 270,
  ID_FILAMENT_TEMP_280 = 280,
  ID_FILAMENT_TEMP_290 = 290,
  ID_FILAMENT_TEMP_300 = 300,
  ID_FILAMENT_TEMP_310 = 310,
  ID_FILAMENT_TEMP_320 = 320,
  ID_FILAMENT_TEMP_330 = 330,
  ID_FILAMENT_TEMP_340 = 340,
  ID_FILAMENT_TEMP_350 = 350,
  ID_FILAMENT_TEMP_PAGE,
  ID_FILAMENT_TEMP_RETURN
};
static void filament_temperature_set(int16_t t)
{
  thermalManager.temp_hotend[uiCfg.extruderIndex].target = t;
  thermalManager.start_watching_hotend(uiCfg.extruderIndex);
  gCfgItems.filament_limit_temp = t;
}

static void filament_temperature_draw_dialog() {
  lv_clear_filament_temperature_select();

  if (uiCfg.print_state == IDLE) {
    uiCfg.leveling_first_time = 1;
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
  }
  else if (uiCfg.print_state == WORKING) {
    #if ENABLED(SDSUPPORT)
      card.pauseSDPrint();
      stop_print_time();
      uiCfg.print_state = PAUSING;
    #endif

    lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
  }
  else {
      if (uiCfg.filament_load_heat_flg) {
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
      }
      else if (uiCfg.filament_unload_heat_flg) {
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
      }
  }
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_FILAMENT_TEMP_170:
      filament_temperature_set(ID_FILAMENT_TEMP_170);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_180:
      filament_temperature_set(ID_FILAMENT_TEMP_180);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_190:
      filament_temperature_set(ID_FILAMENT_TEMP_190);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_200:
      filament_temperature_set(ID_FILAMENT_TEMP_200);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_210:
      filament_temperature_set(ID_FILAMENT_TEMP_210);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_220:
      filament_temperature_set(ID_FILAMENT_TEMP_220);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_230:
      filament_temperature_set(ID_FILAMENT_TEMP_230);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_240:
      filament_temperature_set(ID_FILAMENT_TEMP_240);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_250:
      filament_temperature_set(ID_FILAMENT_TEMP_250);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_260:
      filament_temperature_set(ID_FILAMENT_TEMP_260);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_270:
      filament_temperature_set(ID_FILAMENT_TEMP_270);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_280:
      filament_temperature_set(ID_FILAMENT_TEMP_280);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_290:
      filament_temperature_set(ID_FILAMENT_TEMP_290);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_300:
      filament_temperature_set(ID_FILAMENT_TEMP_300);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_310:
      filament_temperature_set(ID_FILAMENT_TEMP_310);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_320:
      filament_temperature_set(ID_FILAMENT_TEMP_320);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_330:
      filament_temperature_set(ID_FILAMENT_TEMP_330);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_340:
      filament_temperature_set(ID_FILAMENT_TEMP_340);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_350:
      filament_temperature_set(ID_FILAMENT_TEMP_350);
      filament_temperature_draw_dialog();
      break;
    case ID_FILAMENT_TEMP_PAGE:
      uiCfg.para_ui_page == 0 ? uiCfg.para_ui_page = 1 : uiCfg.para_ui_page = 0;
      lv_clear_filament_temperature_select();
      lv_draw_filament_temperature_select();
      break;
    case ID_FILAMENT_TEMP_RETURN:
      uiCfg.filament_load_heat_flg = 0;
      uiCfg.filament_unload_heat_flg = 0;
      lv_clear_filament_temperature_select();
      lv_draw_filament_change();
      break;
  }
}

void lv_draw_filament_temperature_select() {
  scr = lv_obj_create(nullptr, nullptr);
  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  disp_state = FILAMENT_TEMPERATURE_SELECT_UI;

  // title
  lv_obj_t *titleLabel = lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, filament_temp_select.title);
  lv_obj_set_style(titleLabel, &tft_style_label_rel);

  if (gCfgItems.filament_max_temper > 300) {
    lv_obj_t *mode = lv_label_create(scr, "");
    lv_label_set_text(mode, "HIGH");
    lv_obj_align(mode, nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
  }

  lv_refr_now(lv_refr_get_disp_refreshing());
  if (uiCfg.para_ui_page == 0) {
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_170) lv_screen_menu_item(scr, filament_temp_select.temp_170,  15, PARA_UI_SIZE_Y+76*0, event_handler, ID_FILAMENT_TEMP_170);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_180) lv_screen_menu_item(scr, filament_temp_select.temp_180, 167, PARA_UI_SIZE_Y+76*0, event_handler, ID_FILAMENT_TEMP_180);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_190) lv_screen_menu_item(scr, filament_temp_select.temp_190,  15, PARA_UI_SIZE_Y+76*1, event_handler, ID_FILAMENT_TEMP_190);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_200) lv_screen_menu_item(scr, filament_temp_select.temp_200, 167, PARA_UI_SIZE_Y+76*1, event_handler, ID_FILAMENT_TEMP_200);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_210) lv_screen_menu_item(scr, filament_temp_select.temp_210,  15, PARA_UI_SIZE_Y+76*2, event_handler, ID_FILAMENT_TEMP_210);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_220) lv_screen_menu_item(scr, filament_temp_select.temp_220, 167, PARA_UI_SIZE_Y+76*2, event_handler, ID_FILAMENT_TEMP_220);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_230) lv_screen_menu_item(scr, filament_temp_select.temp_230,  15, PARA_UI_SIZE_Y+76*3, event_handler, ID_FILAMENT_TEMP_230);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_240) lv_screen_menu_item(scr, filament_temp_select.temp_240, 167, PARA_UI_SIZE_Y+76*3, event_handler, ID_FILAMENT_TEMP_240);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_250) lv_screen_menu_item(scr, filament_temp_select.temp_250,  15, PARA_UI_SIZE_Y+76*4, event_handler, ID_FILAMENT_TEMP_250);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_260) lv_screen_menu_item(scr, filament_temp_select.temp_260, 167, PARA_UI_SIZE_Y+76*4, event_handler, ID_FILAMENT_TEMP_260);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_280) lv_screen_menu_item_turn_page(scr, machine_menu.next, event_handler, ID_FILAMENT_TEMP_PAGE);
  }
  else if (uiCfg.para_ui_page == 1) {
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_270) lv_screen_menu_item(scr, filament_temp_select.temp_270,  15, PARA_UI_SIZE_Y+76*0, event_handler, ID_FILAMENT_TEMP_270);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_280) lv_screen_menu_item(scr, filament_temp_select.temp_280, 167, PARA_UI_SIZE_Y+76*0, event_handler, ID_FILAMENT_TEMP_280);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_290) lv_screen_menu_item(scr, filament_temp_select.temp_290,  15, PARA_UI_SIZE_Y+76*1, event_handler, ID_FILAMENT_TEMP_290);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_300) lv_screen_menu_item(scr, filament_temp_select.temp_300, 167, PARA_UI_SIZE_Y+76*1, event_handler, ID_FILAMENT_TEMP_300);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_310) lv_screen_menu_item(scr, filament_temp_select.temp_310,  15, PARA_UI_SIZE_Y+76*2, event_handler, ID_FILAMENT_TEMP_310);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_320) lv_screen_menu_item(scr, filament_temp_select.temp_320, 167, PARA_UI_SIZE_Y+76*2, event_handler, ID_FILAMENT_TEMP_320);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_330) lv_screen_menu_item(scr, filament_temp_select.temp_330,  15, PARA_UI_SIZE_Y+76*3, event_handler, ID_FILAMENT_TEMP_330);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_340) lv_screen_menu_item(scr, filament_temp_select.temp_340, 167, PARA_UI_SIZE_Y+76*3, event_handler, ID_FILAMENT_TEMP_340);
    if (gCfgItems.filament_max_temper >= ID_FILAMENT_TEMP_350) lv_screen_menu_item(scr, filament_temp_select.temp_350,  15, PARA_UI_SIZE_Y+76*4, event_handler, ID_FILAMENT_TEMP_350);
    lv_screen_menu_item_turn_page(scr, machine_menu.previous, event_handler, ID_FILAMENT_TEMP_PAGE);
  }

  lv_screen_menu_item_return(scr, event_handler, ID_FILAMENT_TEMP_RETURN);
}

void lv_clear_filament_temperature_select() {
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
