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

#include "../../../module/temperature.h"
#include "../../../gcode/gcode.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;

#ifndef USE_NEW_LVGL_CONF
static lv_obj_t *scr;
#endif

static lv_obj_t *buttonType;
#if DISABLED(TFT_MIXWARE_LVGL_UI)
  static lv_obj_t *buttonIn, *buttonOut;
#endif


static lv_obj_t *labelType;

static lv_obj_t *tempText1;

  #include "../../../sd/cardreader.h"
  #if ENABLED(POWER_LOSS_RECOVERY)
    #include "../../../feature/powerloss.h"
  #endif

  extern bool flash_preview_begin, default_preview_flg;

enum {
  ID_FILAMNT_IN = 1,
  ID_FILAMNT_OUT,
  ID_FILAMNT_TYPE,
  ID_FILAMNT_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_FILAMNT_IN:
      uiCfg.filament_load_heat_flg = true;
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        if (ABS(thermalManager.degTargetHotend(uiCfg.extruderIndex) - thermalManager.wholeDegHotend(uiCfg.extruderIndex)) <= 1
          || gCfgItems.filament_limit_temp <= thermalManager.wholeDegHotend(uiCfg.extruderIndex)
        ) {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED);
        }
        else {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
          if (thermalManager.degTargetHotend(uiCfg.extruderIndex) < gCfgItems.filament_limit_temp) {
            thermalManager.setTargetHotend(gCfgItems.filament_limit_temp, uiCfg.extruderIndex);
            thermalManager.start_watching_hotend(uiCfg.extruderIndex);
          }
        }
      #else
        lv_clear_filament_change();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_SELECT);
      #endif
      break;
    case ID_FILAMNT_OUT:
      uiCfg.filament_unload_heat_flg = true;
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        if (thermalManager.degTargetHotend(uiCfg.extruderIndex)
            && (ABS(thermalManager.degTargetHotend(uiCfg.extruderIndex) - thermalManager.wholeDegHotend(uiCfg.extruderIndex)) <= 1
                || thermalManager.wholeDegHotend(uiCfg.extruderIndex) >= gCfgItems.filament_limit_temp)
        ) {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED);
        }
        else {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
          if (thermalManager.degTargetHotend(uiCfg.extruderIndex) < gCfgItems.filament_limit_temp) {
            thermalManager.setTargetHotend(gCfgItems.filament_limit_temp, uiCfg.extruderIndex);
            thermalManager.start_watching_hotend(uiCfg.extruderIndex);
          }
          filament_sprayer_temp();
        }
      #else
        lv_clear_filament_change();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_SELECT);
      #endif
      break;
    case ID_FILAMNT_TYPE:
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        #if HAS_MULTI_EXTRUDER
          uiCfg.extruderIndex = !uiCfg.extruderIndex;
        #endif
        disp_filament_type();
      #else
        if (uiCfg.print_state == PAUSED) {
          feedrate_mm_s = (float)uiCfg.moveSpeed_bak;
          planner.set_e_position_mm((destination.e = current_position.e = uiCfg.current_e_position_bak));
          thermalManager.setTargetHotend(uiCfg.hotendTargetTempBak, uiCfg.extruderIndex);
          if (gCfgItems.from_flash_pic) flash_preview_begin = true;
          else default_preview_flg = true;
          clear_cur_ui();
          lv_draw_printing();
          uiCfg.print_state = RESUMING;
        }
        #if ENABLED(POWER_LOSS_RECOVERY)
          else if (uiCfg.print_state == REPRINTING) {
            print_time.minutes = recovery.info.print_job_elapsed / 60;
            print_time.seconds = recovery.info.print_job_elapsed % 60;
            print_time.hours   = print_time.minutes / 60;
            if (gCfgItems.from_flash_pic) flash_preview_begin = true;
            else default_preview_flg = true;
            clear_cur_ui();
            lv_draw_printing();
            uiCfg.print_state = REPRINTED;
          }
        #endif
      #endif
      break;
    case ID_FILAMNT_RETURN:
      #if HAS_MULTI_EXTRUDER
        if (uiCfg.print_state != IDLE && uiCfg.print_state != REPRINTED)
          gcode.process_subcommands_now_P(uiCfg.extruderIndexBak == 1 ? PSTR("T1") : PSTR("T0"));
      #endif
      feedrate_mm_s = (float)uiCfg.moveSpeed_bak;
      if (uiCfg.print_state == PAUSED)
        planner.set_e_position_mm((destination.e = current_position.e = uiCfg.current_e_position_bak));
      thermalManager.setTargetHotend(uiCfg.hotendTargetTempBak, uiCfg.extruderIndex);

      #if DISABLED(TFT_MIXWARE_LVGL_UI)
      	clear_cur_ui();
      	draw_return_ui();
      #else
        //Reset filament flag
        uiCfg.filament_heat_completed_load   = 0;
        uiCfg.filament_heat_completed_unload = 0;
        uiCfg.filament_load_heat_flg         = 0;
        uiCfg.filament_unload_heat_flg       = 0;
        uiCfg.filament_loading_completed     = 0;
        uiCfg.filament_unloading_completed   = 0;

        clear_cur_ui();
        if (uiCfg.print_state == IDLE) {
          lv_draw_tool();
        } else {
          lv_draw_operation();
        }
      #endif
      break;
  }
}

void lv_draw_filament_change() {

  #if DISABLED(TFT_MIXWARE_LVGL_UI)
#ifdef USE_NEW_LVGL_CONF
  mks_ui.src_main = lv_screen_create(FILAMENTCHANGE_UI);
  lv_obj_t *buttonIn = lv_big_button_create(mks_ui.src_main, "F:/bmp_in.bin", filament_menu.in, INTERVAL_V, titleHeight, event_handler, ID_FILAMNT_IN);
  lv_obj_clear_protect(buttonIn, LV_PROTECT_FOLLOW);
  lv_big_button_create(mks_ui.src_main, "F:/bmp_out.bin", filament_menu.out, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_FILAMNT_OUT);
  buttonType = lv_imgbtn_create(mks_ui.src_main, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FILAMNT_TYPE);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable)
      lv_group_add_obj(g, buttonType);
  #endif
  lv_big_button_create(mks_ui.src_main, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FILAMNT_RETURN);
  // Create labels on the image buttons
  labelType = lv_label_create_empty(buttonType);
  tempText1 = lv_label_create_empty(mks_ui.src_main);
  #else
  scr = lv_screen_create(FILAMENTCHANGE_UI);
  // lv_obj_t *buttonIn = lv_big_button_create(scr, "F:/bmp_in.bin", filament_menu.in, INTERVAL_V, titleHeight, event_handler, ID_FILAMNT_IN);
  // lv_obj_clear_protect(buttonIn, LV_PROTECT_FOLLOW);
  buttonIn = lv_big_button_create(scr, "F:/bmp_in.bin", filament_menu.in, INTERVAL_V, titleHeight, event_handler, ID_FILAMNT_IN);
  buttonOut = lv_big_button_create(scr, "F:/bmp_out.bin", filament_menu.out, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_FILAMNT_OUT);

  buttonType = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FILAMNT_TYPE);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable)
      lv_group_add_obj(g, buttonType);
  #endif
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FILAMNT_RETURN);
  // Create labels on the image buttons
  labelType = lv_label_create_empty(buttonType);
  tempText1 = lv_label_create_empty(scr);
#endif
  disp_filament_type();

  lv_obj_set_style(tempText1, &tft_style_label_rel);
  disp_filament_temp();

  lv_imgbtn_set_src_both(buttonOut, "F:/bmp_out.bin");
  lv_imgbtn_set_src_both(buttonIn, "F:/bmp_in.bin");
  #else
    scr = lv_screen_create(FILAMENTCHANGE_UI);
    // Create an Image button
    lv_obj_t *buttonIn = lv_big_button_create(scr, MIMG.placeholder,  " ",  0, 340, event_handler, -1);
    lv_obj_clear_protect(buttonIn, LV_PROTECT_FOLLOW);
    lv_big_button_create(scr, MIMG.filamentIn,  MTR.filIn,  IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_FILAMNT_IN);
    lv_big_button_create(scr, MIMG.filamentOut, MTR.filOut, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), event_handler, ID_FILAMNT_OUT);

    if (uiCfg.print_state == PAUSED || uiCfg.print_state == REPRINTING) {
      lv_obj_t *button_f_resume = lv_imgbtn_create(scr, MIMG.printResume, 4, 395, event_handler, ID_FILAMNT_TYPE);
      lv_obj_t *label_f_resume = lv_label_create_empty(button_f_resume);

      lv_label_set_text(label_f_resume, printing_menu.resume);
      lv_obj_align(label_f_resume, button_f_resume, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
    }
    MUI.page_button_return(scr, event_handler, ID_FILAMNT_RETURN);
  #endif
}

void disp_filament_type() {
  if (uiCfg.extruderIndex == 1) {
    lv_imgbtn_set_src_both(buttonType, "F:/bmp_extru2.bin");
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, preheat_menu.ext2);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
  else {
    lv_imgbtn_set_src_both(buttonType, "F:/bmp_extru1.bin");
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, preheat_menu.ext1);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_filament_temp() {
  char buf[20] = {0};

  public_buf_l[0] = '\0';

  strcat(public_buf_l, uiCfg.extruderIndex < 1 ? preheat_menu.ext1 : preheat_menu.ext2);
  sprintf(buf, preheat_menu.value_state, thermalManager.wholeDegHotend(uiCfg.extruderIndex), thermalManager.degTargetHotend(uiCfg.extruderIndex));

  strcat_P(public_buf_l, PSTR(": "));
  strcat(public_buf_l, buf);
  lv_label_set_text(tempText1, public_buf_l);
  lv_obj_align(tempText1, nullptr, LV_ALIGN_CENTER, 0, -50);
}

void lv_clear_filament_change() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif

#ifdef USE_NEW_LVGL_CONF
  lv_obj_clean(mks_ui.src_main);
#else
  lv_obj_del(scr);
#endif
}

#endif // HAS_TFT_LVGL_UI
