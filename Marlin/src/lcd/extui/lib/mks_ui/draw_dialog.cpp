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

/**
 * draw_dialog.cpp
 */

#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../sd/cardreader.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/temperature.h"
#include "../../../../module/planner.h"
#include "../../../../gcode/gcode.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../../module/settings.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../../../feature/pause.h"
#endif

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  #include "../../../tft_io/touch_calibration.h"
  #include "draw_touch_calibration.h"
#endif

#if ENABLED(MIXWARE_MODEL_V)
  #include "printer_filament_det.h"
  #include "../../../../module/stepper.h"
  extern bool flash_preview_begin, default_preview_flg, level_update_flag;
#endif

extern lv_group_t *g;
static lv_obj_t *scr, *tempText1, *filament_bar;

extern uint8_t sel_id;
extern bool once_flag, gcode_preview_over;
extern int upload_result;
extern uint32_t upload_time;
extern uint32_t upload_size;
extern bool temps_update_flag;

static void btn_ok_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (DIALOG_IS(TYPE_PRINT_FILE, TYPE_FINISH_PRINT)) {
    #if HAS_GCODE_PREVIEW
      preview_gcode_prehandle(list_file.file_name[sel_id]);
    #endif

    #if DISABLED(MIXWARE_MODEL_V)
      reset_print_time();
      start_print_time();

      uiCfg.print_state = WORKING;
      lv_clear_dialog();
      lv_draw_printing();
    #endif

    #if ENABLED(SDSUPPORT)
      if (!gcode_preview_over) {
        card.endFilePrint();
        card.openFileRead(list_file.file_name[sel_id]);
        if (card.isFileOpen()) {
          gCfgItems.curFilesize = card.getFileSize();
          update_spi_flash();
          feedrate_percentage = 100;
          planner.flow_percentage[0] = 100;
          planner.e_factor[0]        = planner.flow_percentage[0] * 0.01f;
          #if HAS_MULTI_EXTRUDER
            planner.flow_percentage[1] = 100;
            planner.e_factor[1]        = planner.flow_percentage[1] * 0.01f;
          #endif
          card.startFileprint();
          #if ENABLED(POWER_LOSS_RECOVERY)
            recovery.prepare();
          #endif
          once_flag = false;
        }
        #if ENABLED(MIXWARE_MODEL_V)
          else {
            flash_preview_begin = false;
            default_preview_flg = false;
            lv_clear_dialog();
            lv_draw_dialog(DIALOG_TYPE_REPRINT_NO_FILE);
            return;
          }

          gcode.process_subcommands_now(PSTR("M140 S50"));//TEST
          gcode.process_subcommands_now(PSTR("M104 S170"));//TEST

          reset_print_time();
          start_print_time();
          planner.set_e_position_mm(destination.e = current_position.e = 0);
          detector.reset();
          p_babystep.reset();
          uiCfg.print_state = WORKING;
          #if ENABLED(POWER_LOSS_RECOVERY)
            recovery.info.print_paused = 0;
          #endif

          lv_clear_dialog();
          lv_draw_printing();
        #endif
      }
    #endif
  }
  else if (DIALOG_IS(TYPE_STOP)) {
    detector.reset();
    wait_for_heatup = false;
    stop_print_time();

    #if HAS_AUTOLEVEL
      if (false == planner.leveling_active)
        planner.leveling_active = true;
    #endif

    #if ENABLED(SDSUPPORT)
      uiCfg.print_state           = IDLE;
      card.flag.abort_sd_printing = true;
    #endif

    lv_clear_dialog();
    lv_draw_ready_print();
  }
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    else if (DIALOG_IS(PAUSE_MESSAGE_WAITING, PAUSE_MESSAGE_INSERT, PAUSE_MESSAGE_HEAT))
      wait_for_user = false;
    else if (DIALOG_IS(PAUSE_MESSAGE_OPTION))
      pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
    else if (DIALOG_IS(PAUSE_MESSAGE_RESUME)) {
      lv_clear_cur_ui();
      lv_draw_return_ui();
    }
  #endif
  else if (DIALOG_IS(STORE_EEPROM_TIPS)) {
    TERN_(EEPROM_SETTINGS, (void)settings.save());
    TERN_(MIXWARE_MODEL_V, offset_save_flag = true);
    lv_clear_cur_ui();
    lv_draw_return_ui();
  }
  else if (DIALOG_IS(READ_EEPROM_TIPS)) {
    TERN_(EEPROM_SETTINGS, (void)settings.load());
    lv_clear_cur_ui();
    lv_draw_return_ui();
  }
  else if (DIALOG_IS(REVERT_EEPROM_TIPS)) {
    TERN_(EEPROM_SETTINGS, (void)settings.reset());
    lv_clear_cur_ui();
    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      const bool do_draw_cal = touch_calibration.need_calibration();
      if (do_draw_cal) {
        disp_state_stack._disp_index--; // We are asynchronous from the dialog, so let's remove the dialog from the stack
        lv_draw_touch_calibration_screen();
      }
    #else
      constexpr bool do_draw_cal = false;
    #endif
    if (!do_draw_cal) lv_draw_return_ui();
  }
  else if (DIALOG_IS(WIFI_CONFIG_TIPS)) {
    uiCfg.configWifi = 1;
    lv_clear_cur_ui();
    lv_draw_return_ui();
  }
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_LOAD_COMPLETED)) {
    uiCfg.filament_heat_completed_load = 1;
  }
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED)) {
    uiCfg.filament_heat_completed_unload = 1;
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_COMPLETED, TYPE_FILAMENT_UNLOAD_COMPLETED)) {
    thermalManager.temp_hotend[uiCfg.curSprayerChoose].target= uiCfg.desireSprayerTempBak;
    thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
    lv_clear_cur_ui();
    lv_draw_filament_change();
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UNBIND)) {
      cloud_unbind();
      lv_clear_cur_ui();
      lv_draw_return_ui();
    }
  #endif
  #if ENABLED(MIXWARE_MODEL_V)
    else if (DIALOG_IS(TYPE_REPRINT_NO_FILE)) {
      lv_clear_cur_ui();
      lv_draw_ready_print();
    }
    else if (DIALOG_IS(RUNOUT_FINISH, RUNOUT_LOADTIPS)) {
      uiCfg.filament_heat_completed_load = 1;
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_RUNOUT_LOAD);
    }
    else if (DIALOG_IS(TYPE_FILAMENT_LOAD_SELECT)) {
      thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = PREHEAT_1_TEMP_HOTEND;
      thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
      gCfgItems.filament_limit_temper = PREHEAT_1_TEMP_HOTEND;

      if (uiCfg.print_state == IDLE) {
        uiCfg.leveling_first_time = 1;
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
      }
      else if (uiCfg.print_state == WORKING) {
        #if ENABLED(SDSUPPORT)
          card.pauseSDPrint();
          stop_print_time();
          uiCfg.print_state = PAUSING;
        #endif

        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
      }
      else {
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
      }
    }
    else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_SELECT)) {
      thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = PREHEAT_1_TEMP_HOTEND;
      thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
      gCfgItems.filament_limit_temper = PREHEAT_1_TEMP_HOTEND;

      if (uiCfg.print_state == IDLE) {
        uiCfg.leveling_first_time = 1;
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
      }
      else if (uiCfg.print_state == WORKING) {
        #if ENABLED(SDSUPPORT)
          card.pauseSDPrint();
          stop_print_time();
          uiCfg.print_state = PAUSING;
        #endif

        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
      }
      else {
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
      }
    }
    #if HAS_ABL_NOT_UBL
      else if (DIALOG_IS(AUTO_LEVEL_COMPLETED)) {
        level_state = LEVEL_STATE_NULL;
        lv_clear_cur_ui();
        lv_draw_dialog(DIALOG_AUTO_LEVELING);
      }
      else if (DIALOG_IS(AUTO_LEVEL_FINISHED, AUTO_LEVEL_LEVELERR)) {
        level_state = LEVEL_STATE_NULL;
        lv_clear_cur_ui();
        lv_draw_return_ui();
      }
    #endif
    #if (FLASH_INF_VALID_FLAG >= 0x20210629)
      else if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT)) {
        if (uiCfg.filament_load_heat_flg == 1 || uiCfg.filament_unload_heat_flg == 1 ) {
          value = filament_temp_set;
          lv_clear_dialog();
          lv_draw_number_key();
        }
      }
      else if (DIALOG_IS(PIDTEMP_SW_NORMAL)) {
        // temp_range[0].maxtemp = 285;
        // while (analog_to_celsius_hotend(temp_range[0].raw_max, 0) > tmax)
        //   temp_range[0].raw_max -= TEMPDIR(0) * (OVERSAMPLENR);

        gcode.process_subcommands_now_P(PSTR("M301 P13.14 I0.85 D55.20\nM500"));
        gCfgItems.filament_max_temper = 260;
        update_spi_flash();
        lv_clear_cur_ui();
        lv_draw_return_ui();
      }
      else if (DIALOG_IS(PIDTEMP_SW_HIGH)) {
        // temp_range[0].maxtemp = 375;
        // while (analog_to_celsius_hotend(temp_range[0].raw_max, 0) > tmax)
        //   temp_range[0].raw_max -= TEMPDIR(0) * (OVERSAMPLENR);

        gcode.process_subcommands_now_P(PSTR("M301 P20.30 I1.23 D57.77\nM500"));
        gCfgItems.filament_max_temper = 350;
        update_spi_flash();
        lv_clear_cur_ui();
        lv_draw_return_ui();
      }
    #endif
  #endif
  else {
    lv_clear_cur_ui();
    lv_draw_return_ui();
  }
}

static void btn_cancel_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (DIALOG_IS(PAUSE_MESSAGE_OPTION)) {
    TERN_(ADVANCED_PAUSE_FEATURE, pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT, TYPE_FILAMENT_HEAT_LOAD_COMPLETED, TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED)) {
    thermalManager.temp_hotend[uiCfg.curSprayerChoose].target= uiCfg.desireSprayerTempBak;
    thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
    lv_clear_cur_ui();
    lv_draw_filament_change();
    // lv_draw_return_ui();
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOADING, TYPE_FILAMENT_UNLOADING)) {
    queue.enqueue_one_P(PSTR("M410"));
    uiCfg.filament_rate                = 0;
    uiCfg.filament_loading_completed   = 0;
    uiCfg.filament_unloading_completed = 0;
    uiCfg.filament_loading_time_flg    = 0;
    uiCfg.filament_loading_time_cnt    = 0;
    uiCfg.filament_unloading_time_flg  = 0;
    uiCfg.filament_unloading_time_cnt  = 0;
    thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = uiCfg.desireSprayerTempBak;
    thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
    lv_clear_cur_ui();
    lv_draw_filament_change();
    // lv_draw_return_ui();
  }
  #if ENABLED(MIXWARE_MODEL_V)
    else if (DIALOG_IS(RUNOUT_FINISH)) {
      feedrate_mm_s = (float)uiCfg.moveSpeed_bak;
      planner.set_e_position_mm((destination.e = current_position.e = uiCfg.current_e_position_bak));
      if (gCfgItems.from_flash_pic) flash_preview_begin = true;
      else default_preview_flg = true;
      lv_clear_cur_ui();
      lv_draw_printing();
      uiCfg.print_state = RESUMING;
    }
    else if (DIALOG_IS(TYPE_FILAMENT_LOAD_SELECT)) {
      thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = PREHEAT_2_TEMP_HOTEND;
      thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
      gCfgItems.filament_limit_temper = PREHEAT_2_TEMP_HOTEND;

      if (uiCfg.print_state == IDLE) {
        uiCfg.leveling_first_time = 1;
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
      }
      else if (uiCfg.print_state == WORKING) {
        #if ENABLED(SDSUPPORT)
          card.pauseSDPrint();
          stop_print_time();
          uiCfg.print_state = PAUSING;
        #endif

        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
      }
      else {
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
      }
    }
    else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_SELECT)) {
      thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = PREHEAT_2_TEMP_HOTEND;
      thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
      gCfgItems.filament_limit_temper = PREHEAT_2_TEMP_HOTEND;

      if (uiCfg.print_state == IDLE) {
        uiCfg.leveling_first_time = 1;
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
      }
      else if (uiCfg.print_state == WORKING) {
        #if ENABLED(SDSUPPORT)
          card.pauseSDPrint();
          stop_print_time();
          uiCfg.print_state = PAUSING;
        #endif

        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
      }
      else {
        lv_clear_dialog();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
      }
    }
    else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_COMPLETED)) {
      if (uiCfg.print_state != IDLE) {
        uiCfg.filament_load_heat_flg = 1;
        lv_clear_filament_change();
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_SELECT);
      }
    }
    #if HAS_ABL_NOT_UBL
      else if (DIALOG_IS(AUTO_LEVELING)) {

      }
      else if (DIALOG_IS(AUTO_LEVEL_FINISHED)) {
        level_state = LEVEL_STATE_NULL;
        uiCfg.leveling_first_time = 1;
        uiCfg.move_dist = 0.1;
        lv_clear_cur_ui();
        lv_draw_dialog(DIALOG_ADJUST_Z_HEIGHT_WAIT_START);
      }
    #endif
    else if (DIALOG_IS(AXIS_Z_TEST)) {
      if (!all_axes_trusted()) {
        queue.enqueue_one_P(PSTR("M410"));
      }
      else {
        gcode.process_subcommands_now(PSTR("M410"));
      }

      lv_clear_cur_ui();
      lv_draw_return_ui();
    }
    #if (FLASH_INF_VALID_FLAG >= 0x20210629)
      else if (DIALOG_IS(PIDTEMP_SW_NORMAL, PIDTEMP_SW_HIGH)) {
        lv_clear_cur_ui();
        lv_draw_return_ui();
      }
      else if (DIALOG_IS(TYPE_FINISH_PRINT)) {
        lv_clear_cur_ui();
        lv_draw_ready_print();
      }
    #endif
  #endif
  else {
    lv_clear_cur_ui();
    lv_draw_return_ui();
  }
}

#if ENABLED(MIXWARE_MODEL_V)
static void btn_more_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_SELECT, TYPE_FILAMENT_LOAD_SELECT)) {
    lv_clear_dialog();
    uiCfg.para_ui_page = 0;
    lv_draw_filament_temperature_select();
  }
  if ((uiCfg.filament_load_heat_flg == 1 || uiCfg.filament_unload_heat_flg == 1) && DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT)) {
    value = filament_temp_set;
    lv_clear_dialog();
    lv_draw_number_key();
  }
}

static void btn_return_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_SELECT, TYPE_FILAMENT_LOAD_SELECT)) {
    uiCfg.filament_load_heat_flg = 0;
    uiCfg.filament_unload_heat_flg = 0;
    lv_clear_dialog();
    lv_draw_filament_change();
  }
  else if (DIALOG_IS(TYPE_FINISH_PRINT)) {
    if (p_babystep.isChanged()) {
      TERN_(EEPROM_SETTINGS, (void)settings.save());
      p_babystep.init();
    }
    lv_clear_cur_ui();
    lv_draw_ready_print();
  }
}
#endif

void lv_draw_dialog(uint8_t type) {
  lv_obj_t *btnOk = nullptr, *btnCancel = nullptr;
  uiCfg.dialogType = type;
  scr = lv_screen_create(DIALOG_UI);

  lv_obj_t *labelDialog = lv_label_create(scr, "");

  #if ENABLED(MIXWARE_MODEL_V)
    lv_obj_t *buttonNull = lv_img_create(scr, NULL);// Empty picture.
    lv_img_set_src(buttonNull, "F:/img_null.bin");
    lv_obj_set_pos(buttonNull,  0, TFT_HEIGHT - 50);
  #endif

  if (DIALOG_IS(TYPE_FINISH_PRINT)) {
    btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_LEFT_X, BTN_OK_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
    lv_obj_t *labelOk = lv_label_create_empty(btnOk);             // Add a label to the button
    lv_label_set_text(labelOk, print_file_dialog_menu.print_again);    // Set the labels text

    btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_RIGHT_X, BTN_CANCEL_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_CANCEL_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.confirm);

    if (p_babystep.isChanged()) {
        lv_obj_t *btnReturn = lv_button_btn_create(scr, BTN_RIGHT_X, BTN_POS_Y+100, BTN_SIZE_WIDTH, BTN_SIZE_HEIGHT, btn_return_event_cb);
        lv_obj_t *labelReturn = lv_label_create_empty(btnReturn);
        lv_label_set_text(labelReturn, print_file_dialog_menu.saveExit);
    }
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_RESUME)) {
    btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
    lv_obj_t *labelOk = lv_label_create_empty(btnOk);             // Add a label to the button
    lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_WAITING, PAUSE_MESSAGE_INSERT, PAUSE_MESSAGE_HEAT)) {
    btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
    lv_obj_t *labelOk = lv_label_create_empty(btnOk);             // Add a label to the button
    lv_label_set_text(labelOk, print_file_dialog_menu.confirm);    // Set the labels text
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_PAUSING, PAUSE_MESSAGE_CHANGING, PAUSE_MESSAGE_UNLOAD, PAUSE_MESSAGE_LOAD, PAUSE_MESSAGE_PURGE, PAUSE_MESSAGE_RESUME, PAUSE_MESSAGE_HEATING)) {
    // nothing to do
  }
  else if (DIALOG_IS(WIFI_ENABLE_TIPS)) {
    btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
  }
  else if (DIALOG_IS(TRANSFER_NO_DEVICE)) {
    btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UPLOAD_FILE)) {
      if (upload_result == 2) {
        btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
        lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
        lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
      }
      else if (upload_result == 3) {
        btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
        lv_obj_t *labelOk = lv_label_create_empty(btnOk);
        lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
      }
    }
    else if (DIALOG_IS(TYPE_UPDATE_ESP_FIRMARE)) {
      // nothing to do
    }
  #endif
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT)) {
    btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_LEFT_X, BTN_OK_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
    lv_obj_t *labelAdjustTemp = lv_label_create_empty(btnOk);             // Add a label to the button
    lv_label_set_text(labelAdjustTemp, filament_temp_select.temp_adjust);

    btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_RIGHT_X, BTN_CANCEL_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_CANCEL_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);

      // lv_obj_t *buttont = lv_btn_create(scr, nullptr);
      // lv_obj_set_pos(buttont, TFT_WIDTH/4, TFT_HEIGHT/2-70);
      // lv_obj_set_size(buttont, TFT_WIDTH/2, PARA_UI_SIZE_Y);
      // lv_obj_set_event_cb_mks(buttont, btn_more_event_cb, 0, "", 0);
      // lv_btn_use_label_style(buttont);
      // lv_btn_set_layout(buttont, LV_LAYOUT_OFF);

    tempText1 = lv_label_create(scr, "");
    filament_sprayer_temp();
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_COMPLETED, TYPE_FILAMENT_UNLOAD_COMPLETED)) {
    if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_COMPLETED) && uiCfg.print_state != IDLE) {
      btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_LEFT_X, BTN_OK_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
      lv_obj_t *labelAdjustTemp = lv_label_create_empty(btnOk);             // Add a label to the button
      lv_label_set_text(labelAdjustTemp, print_file_dialog_menu.confirm);

      btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_RIGHT_X, BTN_CANCEL_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_CANCEL_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
      lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
      lv_label_set_text(labelCancel, filament_menu.in);
    }
    else {
      btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
      lv_obj_t *labelOk = lv_label_create_empty(btnOk);
      lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
    }
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOADING, TYPE_FILAMENT_UNLOADING)) {
    btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);

    filament_bar = lv_bar_create(scr, nullptr);
    lv_obj_set_pos(filament_bar, (TFT_WIDTH-300)/2, ((TFT_HEIGHT - titleHeight)-40)/2);
    lv_obj_set_size(filament_bar, 300, 25);
    lv_bar_set_style(filament_bar, LV_BAR_STYLE_INDIC, &lv_bar_style_indic);
    lv_bar_set_anim_time(filament_bar, 1000);
    lv_bar_set_value(filament_bar, 0, LV_ANIM_ON);
  }
  else if(DIALOG_IS(TYPE_MACHINE_PAUSING_TIPS)) {
    //nothing to do
  }
  #if ENABLED(MIXWARE_MODEL_V)
    else if (DIALOG_IS(TYPE_REPRINT_NO_FILE, TYPE_FILAMENT_CLOG, RUNOUT_LOADTIPS)) {
      btnOk = lv_button_btn_create(scr, BTN_CENTER_X, BTN_POS_Y, BTN_SIZE_WIDTH, BTN_SIZE_HEIGHT, btn_ok_event_cb);
      lv_obj_t *labelOk = lv_label_create_empty(btnOk);
      lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
    }
    else if (DIALOG_IS(RUNOUT_PAUSING, TYPE_FILAMENT_PAUSING, TYPE_FILAMENT_WAIT_START, ADJUST_Z_HEIGHT_WAIT_START, AXIS_Z_TEST_WAIT_START)) {
      //nothing to do
    }
    else if (DIALOG_IS(RUNOUT_UNLOAD, RUNOUT_LOAD)) {
      filament_bar = lv_bar_create(scr, NULL);
      lv_obj_set_pos(filament_bar, (TFT_WIDTH-300)/2, ((TFT_HEIGHT-titleHeight))/2+30);
      lv_obj_set_size(filament_bar, 300, 25);
      lv_bar_set_style(filament_bar, LV_BAR_STYLE_INDIC, &lv_bar_style_indic);
      lv_bar_set_anim_time(filament_bar, 1000);
      lv_bar_set_value(filament_bar, 0, LV_ANIM_ON);
    }
    else if (DIALOG_IS(AXIS_Z_TEST)) {
      //nothing to do
      btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_CENTER_X, (BTN_OK_X + 90)), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
      lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
      lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
    }
    #if HAS_ABL_NOT_UBL
      else if (DIALOG_IS(AUTO_LEVELING)) {
        //nothing to do
      }
      else if (DIALOG_IS(AUTO_LEVEL_LEVELERR)) {
        btnOk = lv_button_btn_create(scr, BTN_CENTER_X, BTN_POS_Y, BTN_SIZE_WIDTH, BTN_SIZE_HEIGHT, btn_ok_event_cb);
        lv_obj_t *labelOk = lv_label_create_empty(btnOk);
        lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
      }
    #endif
  #endif
  else {
    btnOk = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_LEFT_X, BTN_OK_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_OK_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_ok_event_cb);
    lv_obj_t *labelOk = lv_label_create_empty(btnOk);             // Add a label to the button

    btnCancel = lv_button_btn_create(scr, TERN(MIXWARE_MODEL_V, BTN_RIGHT_X, BTN_CANCEL_X), TERN(MIXWARE_MODEL_V, BTN_POS_Y, BTN_CANCEL_Y), TERN(MIXWARE_MODEL_V, BTN_SIZE_WIDTH, 100), TERN(MIXWARE_MODEL_V, BTN_SIZE_HEIGHT, 50), btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);     // Add a label to the button

    if (DIALOG_IS(PAUSE_MESSAGE_OPTION)) {
      lv_label_set_text(labelOk, pause_msg_menu.purgeMore);        // Set the labels text
      lv_label_set_text(labelCancel, pause_msg_menu.continuePrint);
    }
    #if ENABLED(MIXWARE_MODEL_V)
      else if (DIALOG_IS(RUNOUT_FINISH)) {
        lv_label_set_text(labelOk, pause_msg_menu.purgeMore);        // Set the labels text
        lv_label_set_text(labelCancel, pause_msg_menu.continuePrint);
      }
      else if (DIALOG_IS(TYPE_FILAMENT_LOAD_SELECT, TYPE_FILAMENT_UNLOAD_SELECT)) {
        lv_label_set_text(labelOk,     (gCfgItems.filament_max_temper < 300 ? preheat_menu.heatPLA  : preheat_menu.heatPETG));        // Set the labels text
        lv_label_set_text(labelCancel, (gCfgItems.filament_max_temper < 300 ? preheat_menu.heatPETG : preheat_menu.heatPACF));

        lv_obj_t *btnMore = lv_button_btn_create(scr, BTN_LEFT_X, BTN_POS_Y+100, BTN_SIZE_WIDTH, BTN_SIZE_HEIGHT, btn_more_event_cb);
        lv_obj_t *labelMore = lv_label_create_empty(btnMore);
        lv_label_set_text(labelMore, main_menu.more);

        lv_obj_t *btnReturn = lv_button_btn_create(scr, BTN_RIGHT_X, BTN_POS_Y+100, BTN_SIZE_WIDTH, BTN_SIZE_HEIGHT, btn_return_event_cb);
        lv_obj_t *labelReturn = lv_label_create_empty(btnReturn);
        lv_label_set_text(labelReturn, common_menu.text_back);
      }
      #if HAS_ABL_NOT_UBL
        else if (DIALOG_IS(AUTO_LEVEL_FINISHED)) {
          lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
          lv_label_set_text(labelCancel, preheat_menu.adjust_title);
        }
      #endif
    #endif
    else {
      lv_label_set_text(labelOk, print_file_dialog_menu.confirm);  // Set the labels text
      lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
    }
  }

  if (DIALOG_IS(TYPE_PRINT_FILE)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.print_file);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t *labelFile = lv_label_create(scr, list_file.long_name[sel_id]);
    lv_obj_align(labelFile, nullptr, LV_ALIGN_CENTER, 0, -70);
  }
  else if (DIALOG_IS(TYPE_STOP)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.cancel_print);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(TYPE_FINISH_PRINT)) {
    if (p_babystep.isChanged()) {
      lv_label_set_text(labelDialog, print_file_dialog_menu.print_finish);
      lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -110);

      lv_obj_t * labelTime = lv_label_create(scr, "");
      lv_obj_set_style(labelTime, &tft_style_label_rel);

      sprintf_P(public_buf_l, PSTR("%s: %d%d:%d%d:%d%d"), print_file_dialog_menu.print_time, print_time.hours / 10, print_time.hours % 10, print_time.minutes / 10, print_time.minutes % 10, print_time.seconds / 10, print_time.seconds % 10);

      lv_label_set_text(labelTime, public_buf_l);
      lv_obj_align(labelTime, nullptr, LV_ALIGN_CENTER, 0, -70);

      lv_obj_t * labelTips = lv_label_create(scr, "");
      lv_obj_set_style(labelTips, &tft_style_label_rel);
      lv_label_set_text(labelTips, print_file_dialog_menu.saveExitTips);
      lv_obj_align(labelTips, nullptr, LV_ALIGN_CENTER, 0, -10);
    }
    else {
      lv_label_set_text(labelDialog, print_file_dialog_menu.print_finish);
      lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -70);
      #if ENABLED(MIXWARE_MODEL_V)
        lv_obj_t * labelTime = lv_label_create(scr, "");
        lv_obj_set_style(labelTime, &tft_style_label_rel);

        sprintf_P(public_buf_l, PSTR("%s: %d%d:%d%d:%d%d"), print_file_dialog_menu.print_time, print_time.hours / 10, print_time.hours % 10, print_time.minutes / 10, print_time.minutes % 10, print_time.seconds / 10, print_time.seconds % 10);

        lv_label_set_text(labelTime, public_buf_l);
        lv_obj_align(labelTime, nullptr, LV_ALIGN_CENTER, 0, -30);
      #endif
    }
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_PAUSING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.pausing);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_CHANGING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.changing);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_UNLOAD)) {
    lv_label_set_text(labelDialog, pause_msg_menu.unload);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_WAITING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.waiting);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_INSERT)) {
    lv_label_set_text(labelDialog, pause_msg_menu.insert);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_LOAD)) {
    lv_label_set_text(labelDialog, pause_msg_menu.load);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_PURGE)) {
    lv_label_set_text(labelDialog, pause_msg_menu.purge);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_RESUME)) {
    lv_label_set_text(labelDialog, pause_msg_menu.resume);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_HEAT)) {
    lv_label_set_text(labelDialog, pause_msg_menu.heat);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_HEATING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.heating);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_OPTION)) {
    lv_label_set_text(labelDialog, pause_msg_menu.option);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(STORE_EEPROM_TIPS)) {
    lv_label_set_text(labelDialog, eeprom_menu.storeTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(READ_EEPROM_TIPS)) {
    lv_label_set_text(labelDialog, eeprom_menu.readTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(REVERT_EEPROM_TIPS)) {
    lv_label_set_text(labelDialog, eeprom_menu.revertTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(WIFI_CONFIG_TIPS)) {
    lv_label_set_text(labelDialog, machine_menu.wifiConfigTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(WIFI_ENABLE_TIPS)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.wifi_enable_tips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(TRANSFER_NO_DEVICE)) {
    lv_label_set_text(labelDialog, DIALOG_UPDATE_NO_DEVICE_EN);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UPLOAD_FILE)) {
      if (upload_result == 1) {
        lv_label_set_text(labelDialog, DIALOG_UPLOAD_ING_EN);
        lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
      }
      else if (upload_result == 2) {
        lv_label_set_text(labelDialog, DIALOG_UPLOAD_ERROR_EN);
        lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
      }
      else if (upload_result == 3) {
        char buf[200];
        int _index = 0;

        strcpy(buf, DIALOG_UPLOAD_FINISH_EN);
        _index = strlen(buf);
        buf[_index] = '\n';
        _index++;
        strcat(buf, DIALOG_UPLOAD_SIZE_EN);

        _index = strlen(buf);
        buf[_index] = ':';
        _index++;
        sprintf(&buf[_index], " %d KBytes\n", (int)(upload_size / 1024));

        strcat(buf, DIALOG_UPLOAD_TIME_EN);
        _index = strlen(buf);
        buf[_index] = ':';
        _index++;
        sprintf(&buf[_index], " %d s\n", (int)upload_time);

        strcat(buf, DIALOG_UPLOAD_SPEED_EN);
        _index = strlen(buf);
        buf[_index] = ':';
        _index++;
        sprintf(&buf[_index], " %d KBytes/s\n", (int)(upload_size / upload_time / 1024));

        lv_label_set_text(labelDialog, buf);
        lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
      }
    }
    else if (DIALOG_IS(TYPE_UPDATE_ESP_FIRMARE)) {
      lv_label_set_text(labelDialog, DIALOG_UPDATE_WIFI_FIRMWARE_EN);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -20);
    }
  #endif // MKS_WIFI_MODULE
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_heat);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -30);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_LOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_heat_confirm);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_HEAT)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unload_heat);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -30);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unload_heat_confirm);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_completed);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unload_completed);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -50);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOADING)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_loading);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -70);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_UNLOADING)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unloading);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -70);
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UNBIND)) {
      lv_label_set_text(labelDialog, common_menu.unbind_printer_tips);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
    }
  #endif
  else if(DIALOG_IS(TYPE_MACHINE_PAUSING_TIPS)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.machinePausingTips);
    lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, 0);
  }
  #if ENABLED(MIXWARE_MODEL_V)
    else if (DIALOG_IS(TYPE_FILAMENT_PAUSING)) {
      lv_label_set_text(labelDialog, pause_msg_menu.pausing);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
    }
    else if (DIALOG_IS(RUNOUT_PAUSING)) {
      lv_label_set_text(labelDialog, machine_menu.FilamentDetPausing);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
    }
    else if (DIALOG_IS(TYPE_REPRINT_NO_FILE)) {
      lv_label_set_text(labelDialog, print_file_dialog_menu.no_file_print_tips);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
    }
    else if (DIALOG_IS(ADJUST_Z_HEIGHT_WAIT_START, AXIS_Z_TEST_WAIT_START)) {
      lv_label_set_text(labelDialog, adjust_z_menu.wait_start);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
    }
    else if (DIALOG_IS(TYPE_FILAMENT_WAIT_START)) {
      lv_label_set_text(labelDialog, filament_menu.filament_dialog_wait_start);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
    }
    else if (DIALOG_IS(TYPE_FILAMENT_LOAD_SELECT)) {
      lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_select);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
    }
    else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_SELECT)) {
      lv_label_set_text(labelDialog, filament_menu.filament_dialog_unload_select);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
    }
    else if (DIALOG_IS(RUNOUT_LOADTIPS)) {
      lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_heat_confirm);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -30);

      lv_obj_t * label_Det_title = lv_label_create(scr, filament_menu.filament_clogging_title);
      lv_obj_set_style(label_Det_title, &tft_style_label_rel);
      lv_obj_align(label_Det_title, nullptr, LV_ALIGN_CENTER, 0, -70);
    }
    else if (DIALOG_IS(RUNOUT_LOAD)) {
      lv_label_set_text(labelDialog, filament_menu.filament_dialog_loading);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -30);

      lv_obj_t * label_Det_title = lv_label_create(scr, filament_menu.filament_clogging_title);
      lv_obj_set_style(label_Det_title, &tft_style_label_rel);
      lv_obj_align(label_Det_title, nullptr, LV_ALIGN_CENTER, 0, -70);
    }
    else if (DIALOG_IS(RUNOUT_UNLOAD)) {
      lv_label_set_text(labelDialog, filament_menu.filament_dialog_unloading);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -30);

      lv_obj_t * label_Det_title = lv_label_create(scr, filament_menu.filament_clogging_title);
      lv_obj_set_style(label_Det_title, &tft_style_label_rel);
      lv_obj_align(label_Det_title, nullptr, LV_ALIGN_CENTER, 0, -70);
    }
    else if (DIALOG_IS(RUNOUT_FINISH)) {
      lv_label_set_text(labelDialog, pause_msg_menu.option);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -30);

      lv_obj_t * label_Det_title = lv_label_create(scr, filament_menu.filament_clogging_title);
      lv_obj_set_style(label_Det_title, &tft_style_label_rel);
      lv_obj_align(label_Det_title, nullptr, LV_ALIGN_CENTER, 0, -70);
    }
    else if (DIALOG_IS(TYPE_FILAMENT_CLOG)) {
      lv_label_set_text(labelDialog, filament_menu.filament_clogging);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
    }
    #if HAS_ABL_NOT_UBL
      else if (DIALOG_IS(AUTO_LEVEL_COMPLETED)) {
        lv_label_set_text(labelDialog, autolevel_menu.completed);
        lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
      }
      else if (DIALOG_IS(AUTO_LEVELING)) {
        lv_label_set_text(labelDialog, autolevel_menu.leveling);
        lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
      }
      else if (DIALOG_IS(AUTO_LEVEL_FINISHED)) {
        lv_label_set_text(labelDialog, autolevel_menu.finished);
        lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
      }
      else if (DIALOG_IS(AUTO_LEVEL_LEVELERR)) {
        lv_label_set_text(labelDialog, autolevel_menu.failed);
        lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
      }
    #endif
    else if (DIALOG_IS(AXIS_Z_TEST)) {
      lv_label_set_text(labelDialog, set_menu.axisztesting);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
    }
    #if (FLASH_INF_VALID_FLAG >= 0x20210629)
      else if (DIALOG_IS(PIDTEMP_SW_NORMAL)) {
        lv_label_set_text(labelDialog, pidtemp_sw_menu.normal_confirm);
        lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
      }
      else if (DIALOG_IS(PIDTEMP_SW_HIGH)) {
        lv_label_set_text(labelDialog, pidtemp_sw_menu.high_confirm);
        lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -50);
      }
    #endif
  #endif

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      if (btnOk) lv_group_add_obj(g, btnOk);
      if (btnCancel) lv_group_add_obj(g, btnCancel);
    }
  #endif
}

void filament_sprayer_temp() {
  char buf[20] = {0};
  sprintf(buf, preheat_menu.value_state, (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius, (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target);

  ZERO(public_buf_l);
  strcpy(public_buf_l, uiCfg.curSprayerChoose < 1 ? extrude_menu.ext1 : extrude_menu.ext2);
  strcat_P(public_buf_l, PSTR(": "));
  strcat(public_buf_l, buf);
  lv_label_set_text(tempText1, public_buf_l);
  lv_obj_align(tempText1, nullptr, LV_ALIGN_CENTER, 0, -70);
}

void filament_dialog_handle() {
  #if ENABLED(MIXWARE_MODEL_V)
    if (uiCfg.print_state != IDLE && uiCfg.print_state != PAUSED && uiCfg.print_state != REPRINTING)
      return;
    if(temps_update_flag) {
      temps_update_flag = false;
      if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT)) {
        filament_sprayer_temp(); // display ext temperature.
      }
      else if (DIALOG_IS(TYPE_FILAMENT_WAIT_START)) {
        if (uiCfg.leveling_first_time) {
          uiCfg.leveling_first_time = 0;
          if (!all_axes_trusted())
            queue.enqueue_now_P(PSTR("G28"));
          if (uiCfg.print_state == IDLE)
            queue.enqueue_now_P(PSTR("G1 X5 Y0 Z50 F600"));
        }
        else {
          if (!planner.has_blocks_queued()) {
            lv_clear_dialog();
            if (uiCfg.filament_load_heat_flg) {
              lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
            }
            else if (uiCfg.filament_unload_heat_flg) {
              lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
            }
          }
        }
      }
    }
  #else
    if (temps_update_flag && (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT))) {
      filament_sprayer_temp();
      temps_update_flag = false;
    }
  #endif
  if (uiCfg.filament_heat_completed_load == 1) {
    uiCfg.filament_heat_completed_load = 0;
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOADING);
    planner.synchronize();
    uiCfg.filament_loading_time_flg = 1;
    uiCfg.filament_loading_time_cnt = 0;
    sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E%d F%d\nG90"), uiCfg.curSprayerChoose, gCfgItems.filamentchange_load_length, gCfgItems.filamentchange_load_speed);
    queue.inject(public_buf_m);
  }
  if (uiCfg.filament_heat_completed_unload == 1) {
    uiCfg.filament_heat_completed_unload = 0;
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOADING);
    planner.synchronize();
    uiCfg.filament_unloading_time_flg = 1;
    uiCfg.filament_unloading_time_cnt = 0;
    sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E15 F100\nG1 E-%d F%d\nG90"), uiCfg.curSprayerChoose, gCfgItems.filamentchange_unload_length, gCfgItems.filamentchange_unload_speed);
    queue.inject(public_buf_m);
  }

  if (((abs((int)((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius - gCfgItems.filament_limit_temper)) <= 1)
    || ((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius > gCfgItems.filament_limit_temper))
    && (uiCfg.filament_load_heat_flg == 1)
    #if ENABLED(MIXWARE_MODEL_V)
      && DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT)
    #endif
  ) {
    uiCfg.filament_load_heat_flg = 0;
    #if ENABLED(MIXWARE_MODEL_V)
      // if (uiCfg.print_state == IDLE)
      //   queue.inject_P(PSTR("G1 X5 Y0 Z50 F600"));
    #endif
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED);
  }

  if (uiCfg.filament_loading_completed == 1) {
    uiCfg.filament_rate = 0;
    uiCfg.filament_loading_completed = 0;
    thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = 0;
    thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_COMPLETED);
  }
  if (((abs((int)((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius - gCfgItems.filament_limit_temper)) <= 1)
    || ((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius > gCfgItems.filament_limit_temper))
    && (uiCfg.filament_unload_heat_flg == 1)
    #if ENABLED(MIXWARE_MODEL_V)
      && DIALOG_IS(TYPE_FILAMENT_UNLOAD_HEAT)
    #endif
  ) {
    uiCfg.filament_unload_heat_flg = 0;
    #if ENABLED(MIXWARE_MODEL_V)
      // if (uiCfg.print_state == IDLE)
      //   queue.inject_P(PSTR("G1 X5 Y0 Z50 F600"));

      uiCfg.filament_heat_completed_unload = 1;
    #else
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED);
    #endif
  }

  if (uiCfg.filament_unloading_completed == 1) {
    uiCfg.filament_rate = 0;
    uiCfg.filament_unloading_completed = 0;
    thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = 0;
    thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED);
  }

  if (DIALOG_IS(TYPE_FILAMENT_LOADING, TYPE_FILAMENT_UNLOADING))
    lv_filament_setbar();
}

void lv_filament_setbar() {
  lv_bar_set_value(filament_bar, uiCfg.filament_rate, LV_ANIM_ON);
}

void lv_clear_dialog() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#if ENABLED(MIXWARE_MODEL_V)
void axis_z_test_dialog_handle() {
	if(axis_z_test_start_flag) {
		axis_z_test_start_flag = false;
    if (DIALOG_IS(AXIS_Z_TEST_WAIT_START)) {
      if (uiCfg.leveling_first_time) {
        uiCfg.leveling_first_time = 0;
        if (!all_axes_trusted())
          gcode.process_subcommands_now_P(PSTR("G28"));
      }
      else {
        if (!planner.has_blocks_queued()) {
          lv_clear_dialog();
          uiCfg.leveling_first_time = 1;
          lv_draw_dialog(DIALOG_AXIS_Z_TEST);
        }
      }
    }
    else if (DIALOG_IS(AXIS_Z_TEST)) {
      if (uiCfg.leveling_first_time) {
        uiCfg.leveling_first_time = 0;

        if (uiCfg.debug_zaxis_speed) {
          gcode.process_subcommands_now_P(PSTR("G1 X150 Y100 F2000\nG1 Z399 F1500\nG1 Z1 F1500"));
        } else {
          gcode.process_subcommands_now_P(PSTR("G1 X150 Y100 F2000\nG1 Z399 F200\nG1 Z1 F200"));
        }
      }
      else {
        if (!planner.has_blocks_queued()) {
          uiCfg.leveling_first_time = 1;
        }
      }
    }
  }
}

void auto_leveling_dialog_handle() {
  if (DIALOG_IS(AUTO_LEVELING) && level_state == LEVEL_STATE_NULL) {
    uiCfg.dialogType = DIALOG_AUTO_LEVEL_HOMING;
    queue.inject_P(PSTR("G28\nG29"));
  }
  else if (DIALOG_IS(AUTO_LEVEL_HOMING) && level_state == LEVEL_STATE_LEVELING) {
    uiCfg.dialogType = DIALOG_AUTO_LEVEL_LEVELING;
  }
  else if (DIALOG_IS(AUTO_LEVEL_LEVELING)) {
    if (level_state == LEVEL_STATE_LEVELERR) {
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_AUTO_LEVEL_LEVELERR);
    }
    else if (level_state == LEVEL_STATE_FINISHED) {
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_AUTO_LEVEL_FINISHED);
    }
  }

	if(level_update_flag) {
		level_update_flag = false;
    if (DIALOG_IS(ADJUST_Z_HEIGHT_WAIT_START)) {
      if (uiCfg.leveling_first_time) {
        if (planner.has_blocks_queued())
          return;
        uiCfg.leveling_first_time = 0;
        if (!all_axes_trusted()) queue.enqueue_now_P(PSTR("G28"));
        queue.enqueue_now_P(PSTR("G1 X150 Y150 F3000"));
        queue.enqueue_now_P(PSTR("G1 Z0 F500"));
      }
      else {
        if (current_position.z == 0) {
          lv_clear_dialog();
          lv_draw_adjust_z_offset();
        }
      }
    }
  }
}

void runout_dialog_handle() {
  if (DIALOG_IS(RUNOUT_UNLOAD) && (abs(thermalManager.temp_hotend[0].target - thermalManager.temp_hotend[0].celsius) <= 2)) {
    if (uiCfg.filament_heat_completed_unload) {
      uiCfg.filament_heat_completed_unload = 0;
      planner.synchronize();
      uiCfg.filament_unloading_time_flg = 1;
      uiCfg.filament_unloading_time_cnt = 0;
      ZERO(public_buf_m);
      sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E10 F100\nG1 E-%d F%d\nG90"), uiCfg.curSprayerChoose, gCfgItems.filamentchange_unload_length, gCfgItems.filamentchange_unload_speed);
      queue.inject_P(PSTR(public_buf_m));
    }
    if (uiCfg.filament_unloading_completed) {
      uiCfg.filament_rate = 0;
      uiCfg.filament_unloading_completed = 0;
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_RUNOUT_LOADTIPS);
    }
  }

  if (DIALOG_IS(RUNOUT_LOAD) && (abs(thermalManager.temp_hotend[0].target - thermalManager.temp_hotend[0].celsius) <= 2)) {
    if (uiCfg.filament_heat_completed_load) {
      uiCfg.filament_heat_completed_load = 0;
      planner.synchronize();
      uiCfg.filament_loading_time_flg = 1;
      uiCfg.filament_loading_time_cnt = 0;
      ZERO(public_buf_m);
      sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E%d F%d\nG90"), uiCfg.curSprayerChoose, gCfgItems.filamentchange_load_length, gCfgItems.filamentchange_load_speed);
      queue.inject_P(PSTR(public_buf_m));
    }

    if (uiCfg.filament_loading_completed == 1) {
      uiCfg.filament_rate = 0;
      uiCfg.filament_loading_completed = 0;
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_RUNOUT_FINISH);
    }
  }

  if (DIALOG_IS(RUNOUT_LOAD, RUNOUT_UNLOAD)) lv_filament_setbar();
}
#endif

#endif // HAS_TFT_LVGL_UI
