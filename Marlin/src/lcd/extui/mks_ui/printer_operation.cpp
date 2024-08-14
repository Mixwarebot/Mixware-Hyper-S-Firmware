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

#include "../../../gcode/gcode.h"
#include "../../../module/planner.h"
#include "../../../module/motion.h"
#include "../../../sd/cardreader.h"
#include "../../../inc/MarlinConfig.h"
#include "../../../MarlinCore.h"
#include "../../../gcode/queue.h"

#include "../../../module/temperature.h"
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

extern uint8_t sel_id;
extern uint32_t To_pre_view;
extern bool flash_preview_begin, default_preview_flg, gcode_preview_over;

void printer_state_polling() {
  char str_1[16];
  if (uiCfg.print_state == PAUSING) {
    #if HAS_MEDIA
      if (!planner.has_blocks_queued() && card.getIndex() > MIN_FILE_PRINTED)
        uiCfg.waitEndMoves++;

      if (uiCfg.waitEndMoves > 20) {
        uiCfg.waitEndMoves = 0;
        planner.synchronize();

        gcode.process_subcommands_now(F("M25"));

        // save the position
        uiCfg.current_x_position_bak = current_position.x;
        uiCfg.current_y_position_bak = current_position.y;
        uiCfg.current_z_position_bak = current_position.z;

        const bool e_axis_is_relative = gcode.axis_is_relative(E_AXIS);
        if (gCfgItems.pausePosZ != (float)-1) {
          sprintf_P(public_buf_l, PSTR("G91\nG1 F1000 Z%s\nG90"), dtostrf(gCfgItems.pausePosZ, 1, 1, str_1));
          gcode.process_subcommands_now(public_buf_l);
        }
        if (gCfgItems.pausePosX != (float)-1 && gCfgItems.pausePosY != (float)-1) {
          sprintf_P(public_buf_l, PSTR("G1 F3000 X%s Y%s"), dtostrf(gCfgItems.pausePosX, 1, 1, str_1), dtostrf(gCfgItems.pausePosY, 1, 1, str_1));
          gcode.process_subcommands_now(public_buf_l);
        }
        uiCfg.print_state = PAUSED;
        if (!e_axis_is_relative)
          uiCfg.current_e_position_bak = current_position.e;
        else
          gcode.process_subcommands_now("M83");
        uiCfg.moveSpeed_bak = feedrate_mm_s;

        #if ENABLED(POWER_LOSS_RECOVERY)
          if (recovery.enabled) {
            float print_paused_raised = gCfgItems.pausePosZ;
            if (uiCfg.current_z_position_bak + gCfgItems.pausePosZ > Z_MAX_POS)
              print_paused_raised = Z_MAX_POS - uiCfg.current_z_position_bak;
            recovery.save(true, print_paused_raised, true);
          }
        #endif

        gCfgItems.pause_reprint = true;
        update_spi_flash();
      }
    #endif
  }
  else
    uiCfg.waitEndMoves = 0;

  if (uiCfg.print_state == PAUSED) {
    #if ENABLED(TFT_MIXWARE_UI)
      // planner.synchronize();  //can not add.
      if (uiCfg.dialogType == DIALOG_RUNOUT_PAUSING) {
        uiCfg.moveSpeed_bak = feedrate_mm_s;
        uiCfg.hotendTargetTempBak = thermalManager.temp_hotend[0].target;
        clear_cur_ui();
        lv_draw_dialog(DIALOG_RUNOUT_UNLOAD);
      }
      else if (uiCfg.dialogType == DIALOG_TYPE_FILAMENT_PAUSING) {
        lv_clear_dialog();
        if (uiCfg.filament_load_heat_flg) {
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
        }
        else if (uiCfg.filament_unload_heat_flg) {
          lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
        }
      }
    #endif
  }

  if (uiCfg.print_state == RESUMING) {
    if (IS_SD_PAUSED()) {
      if (gCfgItems.pausePosX != (float)-1 && gCfgItems.pausePosY != (float)-1) {
        sprintf_P(public_buf_m, PSTR("G1 F2000 X%s Y%s"), dtostrf(uiCfg.current_x_position_bak, 1, 1, str_1), dtostrf(uiCfg.current_y_position_bak, 1, 1, str_1));
        gcode.process_subcommands_now(public_buf_m);
      }
      if (gCfgItems.pausePosZ != (float)-1) {
        ZERO(public_buf_m);
        sprintf_P(public_buf_m, PSTR("G1 F1000 Z%s"), dtostrf(uiCfg.current_z_position_bak, 1, 1, str_1));
        gcode.process_subcommands_now(public_buf_m);
      }
      gcode.process_subcommands_now(FPSTR(M24_STR));
      TERN_(TFT_MIXWARE_UI, detector.reset());
      uiCfg.print_state = WORKING;
      start_print_time();

      #if ENABLED(POWER_LOSS_RECOVERY)
        recovery.save();
      #endif
      gCfgItems.pause_reprint = false;
      update_spi_flash();
    }
  }
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (uiCfg.print_state == REPRINTED) {
      // #if HAS_HOTEND
      //   HOTEND_LOOP() {
      //     const int16_t et = recovery.info.target_temperature[e];
      //     if (et) {
      //       #if HAS_MULTI_HOTEND
      //         sprintf_P(public_buf_m, PSTR("T%i"), e);
      //         gcode.process_subcommands_now(public_buf_m);
      //       #endif
      //       sprintf_P(public_buf_m, PSTR("M109 S%i"), et);
      //       gcode.process_subcommands_now(public_buf_m);
      //     }
      //   }
      // #endif

      recovery.resume();
      TERN_(TFT_MIXWARE_UI, detector.reset());
      // #if 0
      //   // Move back to the saved XY
      //   char str_1[16], str_2[16];
      //   sprintf_P(public_buf_m, PSTR("G1 X%s Y%s F2000"),
      //     dtostrf(recovery.info.current_position.x, 1, 3, str_1),
      //     dtostrf(recovery.info.current_position.y, 1, 3, str_2)
      //   );
      //   gcode.process_subcommands_now(public_buf_m);

      //   if (gCfgItems.pause_reprint && gCfgItems.pausePosZ != -1.0f) {
      //     sprintf_P(public_buf_l, PSTR("G91\nG1 Z-%s\nG90"), dtostrf(gCfgItems.pausePosZ, 1, 1, str_2));
      //     gcode.process_subcommands_now(public_buf_l);
      //   }
      // #endif
      uiCfg.print_state = WORKING;
      start_print_time();

      gCfgItems.pause_reprint = false;
      update_spi_flash();
    }
  #endif

  // if (uiCfg.print_state == WORKING)
    // filament_check();

  // TERN_(MKS_WIFI_MODULE, wifi_looping());
}

void filament_pin_setup() {
  #if PIN_EXISTS(MT_DET_1)
    SET_INPUT_PULLUP(MT_DET_1_PIN);
  #endif
  #if PIN_EXISTS(MT_DET_2)
    SET_INPUT_PULLUP(MT_DET_2_PIN);
  #endif
  #if PIN_EXISTS(MT_DET_3)
    SET_INPUT_PULLUP(MT_DET_3_PIN);
  #endif
}

void filament_check() {
  #if ENABLED(TFT_MIXWARE_UI)
    // mixware detector
    detector.update();

    if (detector.has_break()) {
      uiCfg.moveSpeed_bak = feedrate_mm_s;
      uiCfg.filament_broken = true;

      card.pauseSDPrint();
      stop_print_time();
      uiCfg.print_state = PAUSING;

      WRITE(BEEPER_PIN, LOW);
      clear_cur_ui();
      if (detector.has_block()) {
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_CLOG);
      }
      else {
        uiCfg.filament_heat_completed_unload = 1;
        lv_draw_dialog(DIALOG_RUNOUT_PAUSING);
      }
    }

    //file check test
    static bool file_check = true;
    static millis_t file_check_time = 0;

    if (print_time.minutes * 60 + print_time.minutes > 30 && print_time.minutes * 60 + print_time.minutes < 300
      && thermalManager.degTargetHotend(0) == 170 && thermalManager.degTargetBed() == 50
      )
      file_check = true;
    else
      file_check = false;

    if (file_check){
      if (queue.ring_buffer.length == 0) {
        if (file_check_time == 0)
          file_check_time = print_time.minutes * 60 + print_time.minutes;
        else {
          if (print_time.minutes * 60 + print_time.minutes - file_check_time > 10) {
            card.abortFilePrintNow();
            card.openFileRead(list_file.file_name[sel_id]);
            if (card.isFileOpen()) {
              gCfgItems.curFilesize = card.getFileSize();
              update_spi_flash();
              feedrate_percentage = 100;
              planner.flow_percentage[0] = 100;
              planner.e_factor[0] = planner.flow_percentage[0] * 0.01f;
              card.startOrResumeFilePrinting();
              TERN_(POWER_LOSS_RECOVERY,  if (recovery.enabled) recovery.prepare());
              detector.reset();
              p_babystep.reset();
            }

            file_check_time = 0;
          }
        }
      }
    }
  #else // MKS DET
  #if ANY_PIN(MT_DET_1, MT_DET_2, MT_DET_3)
    const int FIL_DELAY = 20;
  #endif
  #if PIN_EXISTS(MT_DET_1)
    static int fil_det_count_1 = 0;
    if (READ(MT_DET_1_PIN) == MT_DET_PIN_STATE)
      fil_det_count_1++;
    else if (fil_det_count_1 > 0)
      fil_det_count_1--;
  #endif

  #if PIN_EXISTS(MT_DET_2)
    static int fil_det_count_2 = 0;
    if (READ(MT_DET_2_PIN) == MT_DET_PIN_STATE)
      fil_det_count_2++;
    else if (fil_det_count_2 > 0)
      fil_det_count_2--;
  #endif

  #if PIN_EXISTS(MT_DET_3)
    static int fil_det_count_3 = 0;
    if (READ(MT_DET_3_PIN) == MT_DET_PIN_STATE)
      fil_det_count_3++;
    else if (fil_det_count_3 > 0)
      fil_det_count_3--;
  #endif

  if (false
    #if PIN_EXISTS(MT_DET_1)
      || fil_det_count_1 >= FIL_DELAY
    #endif
    #if PIN_EXISTS(MT_DET_2)
      || fil_det_count_2 >= FIL_DELAY
    #endif
    #if PIN_EXISTS(MT_DET_3)
      || fil_det_count_3 >= FIL_DELAY
    #endif
  ) {
    clear_cur_ui();
    card.pauseSDPrint();
    stop_print_time();
    uiCfg.print_state = PAUSING;

    if (gCfgItems.from_flash_pic)
      flash_preview_begin = true;
    else
      default_preview_flg = true;

    lv_draw_printing();
  }
  #endif
}

#endif // HAS_TFT_LVGL_UI
