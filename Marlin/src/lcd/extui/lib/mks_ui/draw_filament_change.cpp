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

#if ENABLED(MIXWARE_MODEL_V)
  #include "../../../../sd/cardreader.h"
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

  extern bool flash_preview_begin, default_preview_flg;
#endif

extern lv_group_t *g;
static lv_obj_t *scr;
#if DISABLED(MIXWARE_MODEL_V)
static lv_obj_t *buttonType;
static lv_obj_t *labelType;
static lv_obj_t *tempText1;
#endif

enum {
  ID_FILAMNT_IN = 1,
  ID_FILAMNT_OUT,
  ID_FILAMNT_TYPE,
  ID_FILAMNT_RETURN
  #if ENABLED(MIXWARE_MODEL_V)
  ,ID_FILAMNT_RESUME
  #endif
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_FILAMNT_IN:
      uiCfg.filament_load_heat_flg = 1;
      #if ENABLED(MIXWARE_MODEL_V)
        // if (uiCfg.print_state == IDLE) {
        //   uiCfg.leveling_first_time = 1;
        //   lv_clear_filament_change();
        //   lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
        // }
        // else if (uiCfg.print_state == WORKING) {
        //   #if ENABLED(SDSUPPORT)
        //     card.pauseSDPrint();
        //     stop_print_time();
        //     uiCfg.print_state = PAUSING;
        //   #endif

        //   lv_clear_filament_change();
        //   lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
        // }
        // else
        {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_SELECT);
        }
      #else
        #if ENABLED(SINGLENOZZLE)
          if ((abs(thermalManager.temp_hotend[0].target - thermalManager.temp_hotend[0].celsius) <= 1)
              || (gCfgItems.filament_limit_temper <= thermalManager.temp_hotend[0].celsius)) {
            lv_clear_filament_change();
            lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED);
          }
          else {
            lv_clear_filament_change();
            lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
            if (thermalManager.temp_hotend[0].target < gCfgItems.filament_limit_temper) {
              thermalManager.temp_hotend[0].target = gCfgItems.filament_limit_temper;
              thermalManager.start_watching_hotend(0);
            }
          }
        #else
          if ((abs(thermalManager.temp_hotend[uiCfg.curSprayerChoose].target - thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius) <= 1)
              || (gCfgItems.filament_limit_temper <= thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius)) {
            lv_clear_filament_change();
            lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED);
          }
          else {
            lv_clear_filament_change();
            lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
            if (thermalManager.temp_hotend[uiCfg.curSprayerChoose].target < gCfgItems.filament_limit_temper) {
              thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = gCfgItems.filament_limit_temper;
              thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
            }
          }
        #endif
      #endif
      break;
    case ID_FILAMNT_OUT:
      uiCfg.filament_unload_heat_flg=1;
      #if ENABLED(MIXWARE_MODEL_V)
        // if (uiCfg.print_state == IDLE) {
        //   uiCfg.leveling_first_time = 1;
        //   lv_clear_filament_change();
        //   lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
        // }
        // else if (uiCfg.print_state == WORKING) {
        //   #if ENABLED(SDSUPPORT)
        //     card.pauseSDPrint();
        //     stop_print_time();
        //     uiCfg.print_state = PAUSING;
        //   #endif

        //   lv_clear_filament_change();
        //   lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
        // }
        // else
        {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_SELECT);
        }
      #else
      #if ENABLED(SINGLENOZZLE)
        if ((thermalManager.temp_hotend[0].target > 0)
          && ((abs((int)((int)thermalManager.temp_hotend[0].target - thermalManager.temp_hotend[0].celsius)) <= 1)
          || ((int)thermalManager.temp_hotend[0].celsius >= gCfgItems.filament_limit_temper))
        ) {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED);
        }
        else {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
          if (thermalManager.temp_hotend[0].target < gCfgItems.filament_limit_temper) {
            thermalManager.temp_hotend[0].target = gCfgItems.filament_limit_temper;
            thermalManager.start_watching_hotend(0);
          }
          filament_sprayer_temp();
        }
      #else
        if ((thermalManager.temp_hotend[uiCfg.curSprayerChoose].target > 0)
          && ((abs((int)((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target - thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius)) <= 1)
          || ((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius >= gCfgItems.filament_limit_temper))
        ) {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED);
        }
        else {
          lv_clear_filament_change();
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
          if (thermalManager.temp_hotend[uiCfg.curSprayerChoose].target < gCfgItems.filament_limit_temper) {
            thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = gCfgItems.filament_limit_temper;
            thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
          }
          filament_sprayer_temp();
        }
      #endif
      #endif
      break;
    case ID_FILAMNT_TYPE:
      #if HAS_MULTI_EXTRUDER
        uiCfg.curSprayerChoose = !uiCfg.curSprayerChoose;
      #endif
      disp_filament_type();
      break;
    case ID_FILAMNT_RETURN:
      #if HAS_MULTI_EXTRUDER
        if (uiCfg.print_state != IDLE && uiCfg.print_state != REPRINTED)
          gcode.process_subcommands_now_P(uiCfg.curSprayerChoose_bak == 1 ? PSTR("T1") : PSTR("T0"));
      #endif
      feedrate_mm_s = (float)uiCfg.moveSpeed_bak;
      if (uiCfg.print_state == PAUSED)
        planner.set_e_position_mm((destination.e = current_position.e = uiCfg.current_e_position_bak));
      thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = uiCfg.desireSprayerTempBak;
      thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);

      //Reset filament flag
      uiCfg.filament_heat_completed_load   = 0;
      uiCfg.filament_heat_completed_unload = 0;
      uiCfg.filament_load_heat_flg         = 0;
      uiCfg.filament_unload_heat_flg       = 0;
      uiCfg.filament_loading_completed     = 0;
      uiCfg.filament_unloading_completed   = 0;

      lv_clear_cur_ui();
      if (uiCfg.print_state == IDLE) {
        lv_draw_tool();
      } else {
        lv_draw_operation();
      }
      break;
    #if ENABLED(MIXWARE_MODEL_V)
      case ID_FILAMNT_RESUME:
        if (uiCfg.print_state == PAUSED) {
          feedrate_mm_s = (float)uiCfg.moveSpeed_bak;
          planner.set_e_position_mm((destination.e = current_position.e = uiCfg.current_e_position_bak));
          thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = uiCfg.desireSprayerTempBak;
          if (gCfgItems.from_flash_pic) flash_preview_begin = true;
          else default_preview_flg = true;
          lv_clear_cur_ui();
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
            lv_clear_cur_ui();
            lv_draw_printing();
            uiCfg.print_state = REPRINTED;
          }
        #endif
        break;
    #endif
  }
}

void lv_draw_filament_change(void) {
  scr = lv_screen_create(FILAMENTCHANGE_UI);
  // Create an Image button
#if ENABLED(MIXWARE_MODEL_V)
  lv_big_button_create(scr, "F:/img_filamentchange_in.bin",  filament_menu.in,  button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_FILAMNT_IN);

  lv_big_button_create(scr, "F:/img_filamentchange_out.bin", filament_menu.out, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_FILAMNT_OUT);

  if (uiCfg.print_state == PAUSED || uiCfg.print_state == REPRINTING) {
    lv_obj_t *button_f_resume = lv_imgbtn_create(scr, "F:/bmp_resume.bin", 5, 400, event_handler, ID_FILAMNT_RESUME);
    lv_obj_t *label_f_resume = lv_label_create_empty(button_f_resume);

    lv_label_set_text(label_f_resume, printing_menu.resume);
    lv_obj_align(label_f_resume, button_f_resume, LV_ALIGN_CENTER, 20, 0);
  }

  lv_screen_menu_item_return(scr, event_handler, ID_FILAMNT_RETURN);
#else
  lv_obj_t *buttonIn = lv_big_button_create(scr, "F:/bmp_in.bin", filament_menu.in, INTERVAL_V, titleHeight, event_handler, ID_FILAMNT_IN);
  lv_obj_clear_protect(buttonIn, LV_PROTECT_FOLLOW);
  lv_big_button_create(scr, "F:/bmp_out.bin", filament_menu.out, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_FILAMNT_OUT);

  buttonType = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FILAMNT_TYPE);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonType);
    }
  #endif

  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FILAMNT_RETURN);

  // Create labels on the image buttons
  labelType = lv_label_create_empty(buttonType);

  disp_filament_type();

  tempText1 = lv_label_create_empty(scr);
  lv_obj_set_style(tempText1, &tft_style_label_rel);
  disp_filament_temp();
#endif
}

void disp_filament_type() {
#if DISABLED(MIXWARE_MODEL_V)
  if (uiCfg.curSprayerChoose == 1) {
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
#endif
}

void disp_filament_temp() {
#if DISABLED(MIXWARE_MODEL_V)
  char buf[20] = {0};

  public_buf_l[0] = '\0';

  strcat(public_buf_l, uiCfg.curSprayerChoose < 1 ? preheat_menu.ext1 : preheat_menu.ext2);
  sprintf(buf, preheat_menu.value_state, (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius,  (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target);

  strcat_P(public_buf_l, PSTR(": "));
  strcat(public_buf_l, buf);
  lv_label_set_text(tempText1, public_buf_l);
  lv_obj_align(tempText1, nullptr, LV_ALIGN_CENTER, 0, -50);
#endif
}

void lv_clear_filament_change() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
