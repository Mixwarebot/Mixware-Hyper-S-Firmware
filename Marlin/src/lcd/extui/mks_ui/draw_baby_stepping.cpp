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

#include "../../../gcode/queue.h"
#include "../../../gcode/gcode.h"
#include "../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../module/settings.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

extern lv_group_t *g;
static lv_obj_t *scr;

static lv_obj_t *labelV, *buttonV, *zOffsetText;

#if ENABLED(TFT_MIXWARE_LVGL_UI)
  static lv_obj_t *labelAxis, *buttonAxis;
#endif
enum {
  ID_BABY_STEP_X_P = 1,
  ID_BABY_STEP_X_N,
  ID_BABY_STEP_Y_P,
  ID_BABY_STEP_Y_N,
  ID_BABY_STEP_Z_P,
  ID_BABY_STEP_Z_N,
  ID_BABY_STEP_DIST,
  ID_BABY_STEP_RETURN
  #if ENABLED(TFT_MIXWARE_LVGL_UI)
    , ID_BABYSTEP_AXIS
    , ID_BABYSTEP_ADD
    , ID_BABYSTEP_DEC
  #endif
};

static float babystep_dist  = 0.01;
static uint8_t has_adjust_z = 0;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  char baby_buf[30] = { 0 };
  char str_1[16];

  switch (obj->mks_obj_id) {
    case ID_BABY_STEP_X_P:
      sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(babystep_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_X_N:
      sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(-babystep_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Y_P:
      sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(babystep_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Y_N:
      sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(-babystep_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Z_P:
      sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(babystep_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Z_N:
      sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(-babystep_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_DIST:
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        if (ABS((int)(100 * babystep_dist)) == 1)
          babystep_dist = 0.05;
        else if (ABS((int)(100 * babystep_dist)) == 5)
          babystep_dist = 0.1;
        else
          babystep_dist = 0.01;
      #else
        if (ABS((int)(100 * babystep_dist)) == 10)
          babystep_dist = 0.05;
        else if (ABS((int)(100 * babystep_dist)) == 5)
          babystep_dist = 0.1;
      #endif
      disp_baby_step_dist();
      break;
    case ID_BABY_STEP_RETURN:
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        if (has_adjust_z == 1) {
          TERN_(EEPROM_SETTINGS, (void)settings.save());
          has_adjust_z = 0;
        }
      #else
        p_babystep.update();
      #endif
      clear_cur_ui();
      draw_return_ui();
      break;
    #if ENABLED(TFT_MIXWARE_LVGL_UI)
      case ID_BABYSTEP_AXIS:
        MUI.update_move_axis(buttonAxis, labelAxis);
        break;
      case ID_BABYSTEP_ADD:
        switch (MPRE.move_axis)
        {
        case X_AXIS: sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(babystep_dist, 1, 3, str_1)); break;
        case Y_AXIS: sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(babystep_dist, 1, 3, str_1)); break;
        case Z_AXIS: sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(babystep_dist, 1, 3, str_1)); break;

        default: break;
        }
        gcode.process_subcommands_now(PSTR(baby_buf));
        break;
      case ID_BABYSTEP_DEC:
        switch (MPRE.move_axis)
        {
        case X_AXIS: sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(-babystep_dist, 1, 3, str_1)); break;
        case Y_AXIS: sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(-babystep_dist, 1, 3, str_1)); break;
        case Z_AXIS: sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(-babystep_dist, 1, 3, str_1)); break;

        default: break;
        }
        gcode.process_subcommands_now(PSTR(baby_buf));
        break;
    #endif
  }
}

void lv_draw_baby_stepping() {
  scr = lv_screen_create(BABY_STEP_UI);
#if DISABLED(TFT_MIXWARE_LVGL_UI)
  lv_big_button_create(scr, "F:/bmp_xAdd.bin", move_menu.x_add, INTERVAL_V, titleHeight, event_handler, ID_BABYSTEP_X_P);
  lv_big_button_create(scr, "F:/bmp_xDec.bin", move_menu.x_dec, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABYSTEP_X_N);
  lv_big_button_create(scr, "F:/bmp_yAdd.bin", move_menu.y_add, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_BABYSTEP_Y_P);
  lv_big_button_create(scr, "F:/bmp_yDec.bin", move_menu.y_dec, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABYSTEP_Y_N);
  lv_big_button_create(scr, "F:/bmp_zAdd.bin", move_menu.z_add, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_BABYSTEP_Z_P);
  lv_big_button_create(scr, "F:/bmp_zDec.bin", move_menu.z_dec, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABYSTEP_Z_N);
  buttonV = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_BABYSTEP_DIST);
  labelV = lv_label_create_empty(buttonV);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable)
      lv_group_add_obj(g, buttonV);
  #endif

  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABYSTEP_RETURN);

  disp_baby_step_dist();

  zOffsetText = lv_label_create(scr, 290, TITLE_YPOS, nullptr);
  disp_z_offset_value();
#else
  babystep_dist = 0.1;
  MPRE.move_axis = Z_AXIS;
  buttonAxis = lv_imgbtn_create(scr, MIMG.axisZ, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_BABYSTEP_AXIS);
  buttonV    = lv_imgbtn_create(scr, nullptr,    IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), event_handler, ID_BABY_STEP_DIST);
  lv_big_button_create(scr, MIMG.add, MTR.offsetZAdd,  IMAGEBTN_P_X(4), IMAGEBTN_P_Y(4), event_handler, ID_BABYSTEP_ADD);
  lv_big_button_create(scr, MIMG.dec, MTR.offsetZDec,  IMAGEBTN_P_X(5), IMAGEBTN_P_Y(5), event_handler, ID_BABYSTEP_DEC);
  MUI.page_button_return(scr, event_handler, ID_BABY_STEP_RETURN);

  labelAxis = lv_label_create_empty(buttonAxis);
  lv_label_set_text(labelAxis, MTR.z);
  lv_obj_align(labelAxis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);

  labelV = lv_label_create_empty(buttonV);

  lv_obj_t *buttonPosZ = lv_img_create(scr, nullptr);
  lv_img_set_src(buttonPosZ, MIMG.stateZ);
  lv_obj_set_pos(buttonPosZ, 105, 85);

  zOffsetText = lv_label_create(scr, 150, 95, nullptr);

  disp_baby_step_dist();
  disp_z_offset_value();

  MUI.page_button_add_tips(buttonAxis);
  MUI.page_button_add_tips(buttonV);
#endif
}

void disp_baby_step_dist() {
  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    if ((int)(100 * babystep_dist) == 1)
      lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_01.bin");
    else if ((int)(100 * babystep_dist) == 5)
      lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_05.bin");
    else if ((int)(100 * babystep_dist) == 10)
      lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_1.bin");

    if (gCfgItems.multiple_language) {
      if ((int)(100 * babystep_dist) == 1) {
        lv_label_set_text(labelV, move_menu.step_001mm);
        lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
      else if ((int)(100 * babystep_dist) == 5) {
        lv_label_set_text(labelV, move_menu.step_005mm);
        lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
      else if ((int)(100 * babystep_dist) == 10) {
        lv_label_set_text(labelV, move_menu.step_01mm);
        lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
    }
  #else
    if ((int)(100 * babystep_dist) == 5)
      lv_imgbtn_set_src_both(buttonV, MIMG.move_distance_mm_1);
    else if ((int)(100 * babystep_dist) == 10)
      lv_imgbtn_set_src_both(buttonV, MIMG.move_distance_mm_10);

    if (gCfgItems.multiple_language) {
      if ((int)(100 * babystep_dist) == 5) {
        lv_label_set_text(labelV, MTR.move_distance_mm_0_0_5);
        lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      }
      else if ((int)(100 * babystep_dist) == 10) {
        lv_label_set_text(labelV, MTR.move_distance_mm_0_1);
        lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      }
    }
  #endif
}

void disp_z_offset_value() {
  char buf[20];
  #if HAS_BED_PROBE
    char str_1[16];

    #if DISABLED(TFT_MIXWARE_LVGL_UI)
      sprintf_P(buf, PSTR("Offset Z: %s mm"), dtostrf(probe.offset.z, 1, 3, str_1));
    #else
      sprintf_P(buf, PSTR("%s mm"), dtostrf(probe.offset.z, 1, 2, str_1));
    #endif
  #else
    strcpy_P(buf, PSTR("Offset Z: 0 mm"));
  #endif
  lv_label_set_text(zOffsetText, buf);
}

void lv_clear_baby_stepping() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
