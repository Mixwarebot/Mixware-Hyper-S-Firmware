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

#include "../../../MarlinCore.h" // for marlin_state
#include "../../../module/temperature.h"
#include "../../../module/motion.h"
#include "../../../sd/cardreader.h"
#include "../../../gcode/queue.h"
#include "../../../gcode/gcode.h"
#include "../../../inc/MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#if ENABLED(SET_REMAINING_TIME)
  #include "../../marlinui.h"
#endif

extern lv_group_t *g;
static lv_obj_t *scr;
static lv_obj_t *labelExt1, *labelFan, *labelZpos, *labelTime;
static lv_obj_t *labelPause, *labelStop, *labelOperat;
static lv_obj_t *bar1, *bar1ValueText;
// static lv_obj_t *buttonPause, *buttonOperat, *buttonStop, *buttonExt1, *buttonFanstate, *buttonZpos;
static lv_obj_t *buttonPause, *buttonOperat, *buttonStop;
static lv_obj_t *labelBabystep, *buttonBabystep, *labelDet, *buttonDet;

#if HAS_MULTI_EXTRUDER
  static lv_obj_t *labelExt2;
  static lv_obj_t *buttonExt2;
#endif

#if HAS_HEATED_BED
  static lv_obj_t* labelBed;
  // static lv_obj_t* buttonBedstate;
#endif

enum {
  ID_PAUSE = 1,
  ID_STOP,
  ID_OPTION,
  // ID_TEMP_EXT,
  // ID_TEMP_BED,
  ID_BABYSTEP,
  // ID_FAN
  ID_FILAMENT_DET
};

bool once_flag; // = false
extern bool flash_preview_begin, default_preview_flg, gcode_preview_over;
extern uint32_t To_pre_view;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (gcode_preview_over) return;
  switch (obj->mks_obj_id) {
    case ID_PAUSE:
      if (uiCfg.print_state == WORKING) {
        #if HAS_MEDIA
          card.pauseSDPrint();
          stop_print_time();
          uiCfg.print_state = PAUSING;
        #endif
        lv_imgbtn_set_src_both(buttonPause, "F:/bmp_resume.bin");
        lv_imgbtn_set_src_both(buttonBabystep, "F:/img_babystep_dis.bin");
        lv_label_set_text(labelPause, printing_menu.resume);
        lv_obj_align(labelPause, buttonPause, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
      }
      else if (uiCfg.print_state == PAUSED) {
        uiCfg.print_state = RESUMING;
        lv_imgbtn_set_src_both(obj, "F:/bmp_pause.bin");
        lv_imgbtn_set_src_both(buttonBabystep, "F:/img_babystep.bin");
        lv_label_set_text(labelPause, printing_menu.pause);
        lv_obj_align(labelPause, buttonPause, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
      }
      #if ENABLED(POWER_LOSS_RECOVERY)
        else if (uiCfg.print_state == REPRINTING) {
          uiCfg.print_state = REPRINTED;
          lv_imgbtn_set_src_both(obj, "F:/bmp_pause.bin");
          lv_imgbtn_set_src_both(buttonBabystep, "F:/img_babystep.bin");
          lv_imgbtn_set_src_both(buttonOperat, "F:/bmp_operate.bin");
          lv_label_set_text(labelPause, printing_menu.pause);
          lv_obj_align(labelPause, buttonPause, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
          print_time.minutes = recovery.info.print_job_elapsed / 60;
          print_time.seconds = recovery.info.print_job_elapsed % 60;
          print_time.hours   = print_time.minutes / 60;
        }
      #endif
      break;
    case ID_STOP:
      lv_clear_printing();
      lv_draw_dialog(DIALOG_TYPE_STOP);
      break;
    case ID_OPTION:
      if (uiCfg.print_state == REPRINTING || uiCfg.print_state == REPRINTED)
        break;
      lv_clear_printing();
      lv_draw_operation();
      break;
    // case ID_TEMP_EXT:
    //   uiCfg.curTempType = 0;
    //   lv_clear_printing();
    //   lv_draw_preHeat();
    //   break;
    // case ID_TEMP_BED:
    //   uiCfg.curTempType = 1;
    //   lv_clear_printing();
    //   lv_draw_preHeat();
    //   break;
    // case ID_BABYSTEP:
    //   lv_clear_printing();
    //   lv_draw_baby_stepping();
    //   break;
    // case ID_FAN:
    //   lv_clear_printing();
    //   lv_draw_fan();
    //   break;
    #if ENABLED(TFT_MIXWARE_UI)
      case ID_FILAMENT_DET:
        gCfgItems.filament_det_enable ^= true;
        lv_label_set_text(labelDet, gCfgItems.filament_det_enable ? operation_menu.filament_sensor_on : operation_menu.filament_sensor_off);
        update_spi_flash();

        detector.reset();
        break;
      case ID_BABYSTEP:
        if (uiCfg.print_state == WORKING) {
          TERN_(TFT_MIXWARE_UI, uiCfg.move_axis = Z_AXIS);
          lv_clear_printing();
          lv_draw_baby_stepping();
        }
        break;
    #endif
  }
}

void lv_draw_printing() {
  disp_state_stack._disp_index = 0;
  ZERO(disp_state_stack._disp_state);
  scr = lv_screen_create(PRINTING_UI);

  #if ENABLED(TFT_MIXWARE_UI)
  draw_default_preview(0, 80, 0);
  // Create image buttons
  lv_obj_t *buttonExt1      = lv_img_create(scr, nullptr);
  lv_obj_t *buttonBedstate  = lv_img_create(scr, nullptr);
  lv_obj_t *buttonFanstate  = lv_img_create(scr, nullptr);
  lv_obj_t *buttonTime      = lv_img_create(scr, nullptr);
  lv_obj_t *buttonZpos      = lv_img_create(scr, nullptr);

  lv_img_set_src(buttonExt1,      get_heating_mode() ? "F:/bmp_ext_state.bin" : "F:/HI_ext_state.bin");
  lv_img_set_src(buttonBedstate,  "F:/bmp_bed_state.bin");
  lv_img_set_src(buttonFanstate,  "F:/bmp_fan_state.bin");
  lv_img_set_src(buttonTime,      "F:/bmp_time_state.bin");
  lv_img_set_src(buttonZpos,      "F:/img_zpos_state.bin");

  lv_obj_set_pos(buttonExt1,      165, 170);
  lv_obj_set_pos(buttonBedstate,  165, 215);
  lv_obj_set_pos(buttonFanstate,  165, 260);
  lv_obj_set_pos(buttonTime,      165,  80);
  lv_obj_set_pos(buttonZpos,      165, 125);

  buttonDet       = lv_imgbtn_create(scr, "F:/img_run_out.bin", 5, 325, event_handler, ID_FILAMENT_DET);
  buttonBabystep  = lv_imgbtn_create(scr, uiCfg.print_state == WORKING ? "F:/img_babystep.bin" : "F:/img_babystep_dis.bin", 161, 325, event_handler, ID_BABYSTEP);
  buttonPause     = lv_imgbtn_create(scr, uiCfg.print_state == WORKING ? "F:/bmp_pause.bin" : "F:/bmp_resume.bin", 4, 395, event_handler, ID_PAUSE);
  buttonStop      = lv_imgbtn_create(scr, "F:/bmp_stop.bin", 108, 395, event_handler, ID_STOP);
  buttonOperat    = lv_imgbtn_create(scr, uiCfg.print_state == REPRINTING ? "F:/img_opreate_dis.bin" : "F:/bmp_operate.bin", 212, 395, event_handler, ID_OPTION);

  labelExt1 = lv_label_create(scr, 234, 180, nullptr);
  labelBed  = lv_label_create(scr, 234, 225, nullptr);
  labelFan  = lv_label_create(scr, 234, 270, nullptr);
  labelTime = lv_label_create(scr, 234,  90, nullptr);
  labelZpos = lv_label_create(scr, 234, 135, nullptr);

  labelDet = lv_label_create_empty(buttonDet);
  labelBabystep = lv_label_create_empty(buttonBabystep);
  labelPause  = lv_label_create_empty(buttonPause);
  labelStop   = lv_label_create_empty(buttonStop);
  labelOperat = lv_label_create_empty(buttonOperat);

  if (gCfgItems.filament_max_temper > 300) {
    lv_obj_t *b_mode = lv_img_create(scr, nullptr);
    lv_img_set_src(b_mode, "F:/HI_mode_tip.bin");
    lv_obj_set_pos(b_mode, 20, 285);
    lv_obj_t *l_mode = lv_label_create(b_mode, filament_temp_select.temp_mode, true);
    lv_obj_align(l_mode, b_mode, LV_ALIGN_CENTER, 0, 0);
  }

  if (gCfgItems.multiple_language) {
    lv_label_set_text(labelPause, uiCfg.print_state == WORKING ? printing_menu.pause : printing_menu.resume);
    lv_obj_align(labelPause, buttonPause, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    lv_label_set_text(labelStop, printing_menu.stop);
    lv_obj_align(labelStop, buttonStop, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    lv_label_set_text(labelOperat, printing_menu.option);
    lv_obj_align(labelOperat, buttonOperat, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    lv_label_set_text(labelDet, gCfgItems.filament_det_enable ? operation_menu.filament_sensor_on : operation_menu.filament_sensor_off);
    lv_obj_align(labelDet, buttonDet, LV_ALIGN_CENTER, 15, 0);

    lv_label_set_text(labelBabystep, operation_menu.babystep);
    lv_obj_align(labelBabystep, buttonBabystep, LV_ALIGN_CENTER, 15, 0);
  }

    bar1 = lv_bar_create(scr, nullptr);
    lv_obj_set_pos(bar1, 10, 36);
    lv_obj_set_size(bar1, 300, 36);
    lv_bar_set_style(bar1, LV_BAR_STYLE_INDIC, &lv_bar_style_indic);
    lv_bar_set_anim_time(bar1, 1000);
    lv_bar_set_value(bar1, 0, LV_ANIM_ON);
    bar1ValueText  = lv_label_create_empty(bar1);
    lv_label_set_text(bar1ValueText,"0%");
    lv_obj_align(bar1ValueText, bar1, LV_ALIGN_CENTER, 0, 0);

    disp_ext_temp();
    disp_bed_temp();
    disp_fan_speed();
    disp_print_time();
    disp_fan_Zpos();
  #else
  // Create image buttons
  buttonExt1 = lv_imgbtn_create(scr, "F:/bmp_ext1_state.bin", 206, 136, event_handler, ID_TEMP_EXT);

  #if HAS_MULTI_EXTRUDER
    buttonExt2 = lv_imgbtn_create(scr, "F:/bmp_ext2_state.bin", 350, 136, event_handler, ID_TEMP_EXT);
  #endif

  #if HAS_HEATED_BED
    buttonBedstate = lv_imgbtn_create(scr, "F:/bmp_bed_state.bin", 206, 186, event_handler, ID_TEMP_BED);
  #endif

  buttonFanstate = lv_imgbtn_create(scr, "F:/bmp_fan_state.bin", 350, 186, event_handler, ID_FAN);

  lv_obj_t *buttonTime = lv_img_create(scr, nullptr);
  lv_img_set_src(buttonTime, "F:/bmp_time_state.bin");
  lv_obj_set_pos(buttonTime, 206, 86);

  buttonZpos = lv_imgbtn_create(scr, "F:/bmp_zpos_state.bin", 350, 86, event_handler, ID_BABYSTEP);

  buttonPause  = lv_imgbtn_create(scr, uiCfg.print_state == WORKING ? "F:/bmp_pause.bin" : "F:/bmp_resume.bin", 5, 240, event_handler, ID_PAUSE);
  buttonStop   = lv_imgbtn_create(scr, "F:/bmp_stop.bin", 165, 240, event_handler, ID_STOP);
  buttonOperat = lv_imgbtn_create(scr, "F:/bmp_operate.bin", 325, 240, event_handler, ID_OPTION);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonPause);
      lv_group_add_obj(g, buttonStop);
      lv_group_add_obj(g, buttonOperat);
      lv_group_add_obj(g, buttonPause);
      lv_group_add_obj(g, buttonPause);
      lv_group_add_obj(g, buttonPause);
    }
  #endif

  labelExt1 = lv_label_create(scr, 250, 146, nullptr);

  #if HAS_MULTI_EXTRUDER
    labelExt2 = lv_label_create(scr, 395, 146, nullptr);
  #endif

  #if HAS_HEATED_BED
    labelBed = lv_label_create(scr, 250, 196, nullptr);
  #endif

  labelFan  = lv_label_create(scr, 395, 196, nullptr);
  labelTime = lv_label_create(scr, 250, 96, nullptr);
  labelZpos = lv_label_create(scr, 395, 96, nullptr);

  labelPause  = lv_label_create_empty(buttonPause);
  labelStop   = lv_label_create_empty(buttonStop);
  labelOperat = lv_label_create_empty(buttonOperat);

  if (gCfgItems.multiple_language) {
    lv_label_set_text(labelPause, uiCfg.print_state == WORKING ? printing_menu.pause : printing_menu.resume);
    lv_obj_align(labelPause, buttonPause, LV_ALIGN_CENTER, 20, 0);

    lv_label_set_text(labelStop, printing_menu.stop);
    lv_obj_align(labelStop, buttonStop, LV_ALIGN_CENTER, 20, 0);

    lv_label_set_text(labelOperat, printing_menu.option);
    lv_obj_align(labelOperat, buttonOperat, LV_ALIGN_CENTER, 20, 0);
  }

  bar1 = lv_bar_create(scr, nullptr);
  lv_obj_set_pos(bar1, 205, 36);
  lv_obj_set_size(bar1, 270, 40);
  lv_bar_set_style(bar1, LV_BAR_STYLE_INDIC, &lv_bar_style_indic);
  lv_bar_set_anim_time(bar1, 1000);
  lv_bar_set_value(bar1, 0, LV_ANIM_ON);
  bar1ValueText = lv_label_create_empty(bar1);
  lv_label_set_text(bar1ValueText, "0%");
  lv_obj_align(bar1ValueText, bar1, LV_ALIGN_CENTER, 0, 0);

  disp_ext_temp();
  disp_bed_temp();
  disp_fan_speed();
  disp_print_time();
  disp_fan_Zpos();
  #endif
}

void disp_ext_temp() {
  sprintf(public_buf_l, printing_menu.temp1, thermalManager.wholeDegHotend(0), thermalManager.degTargetHotend(0));
  lv_label_set_text(labelExt1, public_buf_l);

  #if HAS_MULTI_EXTRUDER
    sprintf(public_buf_l, printing_menu.temp1, thermalManager.wholeDegHotend(1), thermalManager.degTargetHotend(1));
    lv_label_set_text(labelExt2, public_buf_l);
  #endif
}

void disp_bed_temp() {
  #if HAS_HEATED_BED
    sprintf(public_buf_l, printing_menu.bed_temp, thermalManager.wholeDegBed(), thermalManager.degTargetBed());
    lv_label_set_text(labelBed, public_buf_l);
  #endif
}

void disp_fan_speed() {
  sprintf_P(public_buf_l, PSTR("%d%%"), (int)thermalManager.fanSpeedPercent(0));
  lv_label_set_text(labelFan, public_buf_l);
}

void disp_print_time() {
  #if ENABLED(SET_REMAINING_TIME)
    const uint32_t r = ui.get_remaining_time();
    sprintf_P(public_buf_l, PSTR("%02d:%02d R"), r / 3600, (r % 3600) / 60);
  #else
    sprintf_P(public_buf_l, PSTR("%d%d:%d%d:%d%d"), print_time.hours / 10, print_time.hours % 10, print_time.minutes / 10, print_time.minutes % 10, print_time.seconds / 10, print_time.seconds % 10);
  #endif
  lv_label_set_text(labelTime, public_buf_l);
}

void disp_fan_Zpos() {
  dtostrf(current_position.z, 1, 3, public_buf_l);
  lv_label_set_text(labelZpos, public_buf_l);
}

void reset_print_time() {
  print_time.hours   = 0;
  print_time.minutes = 0;
  print_time.seconds = 0;
  print_time.ms_10   = 0;
}

void start_print_time() { print_time.start = 1; }

void stop_print_time() { print_time.start = 0; }

void setProBarRate() {
  int rate;
  volatile long long rate_tmp_r;

  if (!gCfgItems.from_flash_pic) {
    #if HAS_MEDIA
      rate_tmp_r = (long long)card.getIndex() * 100;
    #endif
    rate = rate_tmp_r / gCfgItems.curFilesize;
  }
  else {
    #if HAS_MEDIA
      rate_tmp_r = (long long)card.getIndex();
    #endif
    rate = (rate_tmp_r - (PREVIEW_SIZE + To_pre_view)) * 100 / (gCfgItems.curFilesize - (PREVIEW_SIZE + To_pre_view));
  }

  static uint8_t last_print_state = IDLE;
  if (last_print_state != uiCfg.print_state) {
    last_print_state = uiCfg.print_state;
    lv_imgbtn_set_src_both(buttonPause, (uiCfg.print_state == WORKING || uiCfg.print_state == REPRINTED) ? "F:/bmp_pause.bin" : "F:/bmp_resume.bin");
    lv_label_set_text(labelPause, (uiCfg.print_state == WORKING || uiCfg.print_state == REPRINTED) ? printing_menu.pause : printing_menu.resume);
    lv_obj_align(labelPause, buttonPause, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
  }

  if (rate <= 0) return;

  if (disp_state == PRINTING_UI) {
    lv_bar_set_value(bar1, rate, LV_ANIM_ON);
    sprintf_P(public_buf_l, "%d%%", rate);
    lv_label_set_text(bar1ValueText, public_buf_l);
    lv_obj_align(bar1ValueText, bar1, LV_ALIGN_CENTER, 0, 0);

    // if (marlin_state == MF_SD_COMPLETE) {
    //   if (once_flag == 0) {
    //     stop_print_time();

    //     flash_preview_begin = false;
    //     default_preview_flg = false;
    //     lv_clear_printing();
    //     lv_draw_dialog(DIALOG_TYPE_FINISH_PRINT);

    //     once_flag = true;

    //     #if HAS_SUICIDE
    //       if (gCfgItems.finish_power_off) {
    //         gcode.process_subcommands_now(F("M1001"));
    //         queue.inject(F("M81"));
    //         marlin_state = MF_RUNNING;
    //       }
    //     #endif
    //   }
    // }
  }
}

void lv_clear_printing() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
