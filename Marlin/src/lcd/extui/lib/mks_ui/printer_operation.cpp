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

#include "../../../../gcode/gcode.h"
#include "../../../../module/temperature.h"
#include "../../../../module/planner.h"
#include "../../../../module/motion.h"
#include "../../../../sd/cardreader.h"
#include "../../../../inc/MarlinConfig.h"
#include "../../../../MarlinCore.h"
#include "../../../../gcode/queue.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

extern uint32_t To_pre_view;
extern bool flash_preview_begin, default_preview_flg, gcode_preview_over;

void printer_state_polling() {
  char str_1[16];
  if (uiCfg.print_state == PAUSING) {
    #if ENABLED(SDSUPPORT)
      if (!planner.has_blocks_queued() && card.getIndex() > MIN_FILE_PRINTED)
        uiCfg.waitEndMoves++;

      if (uiCfg.waitEndMoves > 20) {
        uiCfg.waitEndMoves = 0;
        planner.synchronize();
        gcode.process_subcommands_now_P(PSTR("M25"));
        //save the positon
        uiCfg.current_x_position_bak = current_position.x;
        uiCfg.current_y_position_bak = current_position.y;
        uiCfg.current_z_position_bak = current_position.z;

        if (gCfgItems.pausePosZ != (float)-1) {
          gcode.process_subcommands_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Z%s"), dtostrf(gCfgItems.pausePosZ, 1, 1, str_1));
          gcode.process_subcommands_now(public_buf_l);
          gcode.process_subcommands_now_P(PSTR("G90"));
        }
        if (gCfgItems.pausePosX != (float)-1) {
          sprintf_P(public_buf_l, PSTR("G1 X%s"), dtostrf(gCfgItems.pausePosX, 1, 1, str_1));
          gcode.process_subcommands_now(public_buf_l);
        }
        if (gCfgItems.pausePosY != (float)-1) {
          sprintf_P(public_buf_l, PSTR("G1 Y%s"), dtostrf(gCfgItems.pausePosY, 1, 1, str_1));
          gcode.process_subcommands_now(public_buf_l);
        }
        uiCfg.print_state = PAUSED;
        uiCfg.current_e_position_bak = current_position.e;
        uiCfg.moveSpeed_bak = feedrate_mm_s;

        #if ENABLED(POWER_LOSS_RECOVERY)
          // Power off when printing is paused.
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
    TERN_(TFT_MIXWARE_LVGL_UI, MUI.update_pause_print_ui());
  }

  if (uiCfg.print_state == RESUMING) {
    if (IS_SD_PAUSED()) {
      if (gCfgItems.pausePosX != (float)-1) {
        sprintf_P(public_buf_m, PSTR("G1 X%s"), dtostrf(uiCfg.current_x_position_bak, 1, 1, str_1));
        gcode.process_subcommands_now(public_buf_m);
      }
      if (gCfgItems.pausePosY != (float)-1) {
        sprintf_P(public_buf_m, PSTR("G1 Y%s"), dtostrf(uiCfg.current_y_position_bak, 1, 1, str_1));
        gcode.process_subcommands_now(public_buf_m);
      }
      if (gCfgItems.pausePosZ != (float)-1) {
        ZERO(public_buf_m);
        sprintf_P(public_buf_m, PSTR("G1 Z%s"), dtostrf(uiCfg.current_z_position_bak, 1, 1, str_1));
        gcode.process_subcommands_now(public_buf_m);
      }

      ZERO(public_buf_m);
      sprintf_P(public_buf_m, PSTR("G92 E%s"),  dtostrf(uiCfg.current_e_position_bak, 1, 1, str_1));
      gcode.process_subcommands_now(public_buf_m);

      gcode.process_subcommands_now_P(M24_STR);
      uiCfg.print_state = WORKING;
      start_print_time();

      #if ENABLED(TFT_MIXWARE_LVGL_UI)
        detector.reset();

        TERN_(POWER_LOSS_RECOVERY, if (recovery.enabled) recovery.save(true));
      #endif

      gCfgItems.pause_reprint = false;
      update_spi_flash();
    }
  }
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (uiCfg.print_state == REPRINTED) {
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        #if HAS_HOTEND
          HOTEND_LOOP() {
            const int16_t et = recovery.info.target_temperature[e];
            if (et) {
              #if HAS_MULTI_HOTEND
                sprintf_P(public_buf_m, PSTR("T%i"), e);
                gcode.process_subcommands_now(public_buf_m);
              #endif
              sprintf_P(public_buf_m, PSTR("M109 S%i"), et);
              gcode.process_subcommands_now(public_buf_m);
            }
          }
        #endif
      #endif

      recovery.resume();
      uiCfg.print_state = WORKING;
      start_print_time();
      TERN_(TFT_MIXWARE_LVGL_UI, detector.reset());

      gCfgItems.pause_reprint = false;
      update_spi_flash();
    }
  #endif
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

extern uint8_t sel_id;
void filament_check() {
  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    #if (PIN_EXISTS(MT_DET_1) || PIN_EXISTS(MT_DET_2) || PIN_EXISTS(MT_DET_3))
      const int FIL_DELAY = 20;
    #endif
    #if PIN_EXISTS(MT_DET_1)
      static int fil_det_count_1 = 0;
      if (!READ(MT_DET_1_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (READ(MT_DET_1_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (fil_det_count_1 > 0)
        fil_det_count_1--;

      if (!READ(MT_DET_1_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (READ(MT_DET_1_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (fil_det_count_1 > 0)
        fil_det_count_1--;
    #endif

    #if PIN_EXISTS(MT_DET_2)
      static int fil_det_count_2 = 0;
      if (!READ(MT_DET_2_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (READ(MT_DET_2_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (fil_det_count_2 > 0)
        fil_det_count_2--;

      if (!READ(MT_DET_2_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (READ(MT_DET_2_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (fil_det_count_2 > 0)
        fil_det_count_2--;
    #endif

    #if PIN_EXISTS(MT_DET_3)
      static int fil_det_count_3 = 0;
      if (!READ(MT_DET_3_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_3++;
      else if (READ(MT_DET_3_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_3++;
      else if (fil_det_count_3 > 0)
        fil_det_count_3--;

      if (!READ(MT_DET_3_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_3++;
      else if (READ(MT_DET_3_PIN) && MT_DET_PIN_INVERTING)
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
      lv_clear_cur_ui();
      TERN_(SDSUPPORT, card.pauseSDPrint());
      stop_print_time();
      uiCfg.print_state = PAUSING;

      if (gCfgItems.from_flash_pic)
        flash_preview_begin = true;
      else
        default_preview_flg = true;

      lv_draw_printing();
    }
  #else
    detector.check();

    //file check test
    static bool file_check = true;
    static millis_t file_check_time = 0;

    if (print_time.minutes * 60 + print_time.minutes < 300
      && thermalManager.degTargetHotend(0) == 170 && thermalManager.degTargetHotend(0) <= thermalManager.degHotend(0)
      && thermalManager.degTargetBed() == 50 && thermalManager.degTargetBed() <= thermalManager.degBed())
      file_check = true;
    else
      file_check = false;

    if (file_check){
      if (queue.length == 0) {
        if (file_check_time == 0)
          file_check_time = print_time.minutes * 60 + print_time.minutes;
        else {
          if (print_time.minutes * 60 + print_time.minutes - file_check_time > 10) {
            SERIAL_ECHOLNPGM("\r\nRe Print. ");

            card.endFilePrint();
            card.openFileRead(list_file.file_name[sel_id]);
            if (card.isFileOpen()) {
              SERIAL_ECHOLNPGM(" file open.");
              gCfgItems.curFilesize = card.getFileSize();
              update_spi_flash();
              feedrate_percentage = 100;
              planner.flow_percentage[0] = 100;
              planner.e_factor[0] = planner.flow_percentage[0] * 0.01f;
              card.startFileprint();
              TERN_(POWER_LOSS_RECOVERY,  if (recovery.enabled) recovery.prepare());
              detector.reset();
              p_babystep.reset();
            }

            file_check_time = 0;
          }
        }
      }
    }
  #endif
}

#endif // HAS_TFT_LVGL_UI
