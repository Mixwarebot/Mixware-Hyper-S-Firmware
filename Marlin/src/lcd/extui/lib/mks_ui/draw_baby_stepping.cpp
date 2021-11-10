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

#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../../module/settings.h"
#endif

#if HAS_BED_PROBE
  #include "../../../../module/probe.h"
#endif

extern lv_group_t *g;
static lv_obj_t *scr;

static lv_obj_t *labelV, *buttonV, *zOffsetText;

#if ENABLED(MIXWARE_MODEL_V)
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
  #if ENABLED(MIXWARE_MODEL_V)
  ,ID_BABY_STEP_AXIS
  ,ID_BABY_STEP_ADD
  ,ID_BABY_STEP_DEC
  #endif
};

static float babystep_dist=0.01;
static uint8_t has_adjust_z = 0;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  char baby_buf[30] = { 0 };
  char str_1[16];
  switch (obj->mks_obj_id) {
    case ID_BABY_STEP_X_P:
      sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(babystep_dist, 1, 2, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_X_N:
      sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(-babystep_dist, 1, 2, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Y_P:
      sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(babystep_dist, 1, 2, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Y_N:
      sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(-babystep_dist, 1, 2, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Z_P:
      sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(babystep_dist, 1, 2, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Z_N:
      sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(-babystep_dist, 1, 2, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_DIST:
      if (abs((int)(100 * babystep_dist)) == 10)
        babystep_dist = 0.05;
      else if (abs((int)(100 * babystep_dist)) == 5)
        babystep_dist = 0.1;

      disp_baby_step_dist();
      break;
    case ID_BABY_STEP_RETURN:
      // if (has_adjust_z == 1) {
      //   TERN_(EEPROM_SETTINGS, (void)settings.save());
      //   has_adjust_z = 0;
      // }
      lv_clear_cur_ui();
      lv_draw_return_ui();
      break;
    #if ENABLED(MIXWARE_MODEL_V)
    case ID_BABY_STEP_AXIS:
      switch (uiCfg.move_axis) {
        case X_AXIS: uiCfg.move_axis = Y_AXIS; break;
        case Y_AXIS: uiCfg.move_axis = Z_AXIS; break;
        case Z_AXIS: uiCfg.move_axis = X_AXIS; break;
      }
      disp_baby_step_axis();
      break;
    case ID_BABY_STEP_ADD:
        switch (uiCfg.move_axis) {
          case X_AXIS: sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(babystep_dist, 1, 2, str_1)); break;
          case Y_AXIS: sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(babystep_dist, 1, 2, str_1)); break;
          case Z_AXIS: sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(babystep_dist, 1, 2, str_1)); break;
          default: break;
        }
        gcode.process_subcommands_now_P(PSTR(baby_buf));
      break;
    case ID_BABY_STEP_DEC:
        switch (uiCfg.move_axis) {
          case X_AXIS: sprintf_P(baby_buf, PSTR("M290 X%s"), dtostrf(-babystep_dist, 1, 2, str_1)); break;
          case Y_AXIS: sprintf_P(baby_buf, PSTR("M290 Y%s"), dtostrf(-babystep_dist, 1, 2, str_1)); break;
          case Z_AXIS: sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(-babystep_dist, 1, 2, str_1)); break;
          default: break;
        }
        gcode.process_subcommands_now_P(PSTR(baby_buf));
      break;
    #endif
  }
}

void lv_draw_baby_stepping(void) {
  scr = lv_screen_create(BABY_STEP_UI);

  #if ENABLED(MIXWARE_MODEL_V)
    babystep_dist = 0.1;
    buttonAxis = lv_imgbtn_create(scr, nullptr, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_BABY_STEP_AXIS);
    buttonV    = lv_imgbtn_create(scr, nullptr, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_BABY_STEP_DIST);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonAxis);
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonV);
    #endif

    lv_big_button_create(scr, "F:/img_add.bin", adjust_z_menu.add,  button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_BABY_STEP_ADD);
    lv_big_button_create(scr, "F:/img_dec.bin", adjust_z_menu.dec,  button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_BABY_STEP_DEC);

    labelAxis = lv_label_create_empty(buttonAxis);
    labelV = lv_label_create_empty(buttonV);

    lv_screen_menu_item_return(scr, event_handler, ID_BABY_STEP_RETURN);

    lv_obj_t *buttonPosZ = lv_img_create(scr, nullptr);
    lv_img_set_src(buttonPosZ, "F:/img_zpos_state.bin");
    lv_obj_set_pos(buttonPosZ, 105, 85);

    zOffsetText = lv_label_create(scr, 150, 95, nullptr);

    disp_baby_step_axis();
    disp_baby_step_dist();
    disp_z_offset_value();

      lv_obj_t *l_tips_axis = lv_label_create(buttonAxis, machine_menu.ButtonTips);
      lv_obj_align(l_tips_axis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
      lv_obj_t *l_tips_v = lv_label_create(buttonV, machine_menu.ButtonTips);
      lv_obj_align(l_tips_v, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
  #else
    lv_big_button_create(scr, "F:/bmp_xAdd.bin", move_menu.x_add, INTERVAL_V, titleHeight, event_handler, ID_BABY_STEP_X_P);
    lv_big_button_create(scr, "F:/bmp_xDec.bin", move_menu.x_dec, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_X_N);
    lv_big_button_create(scr, "F:/bmp_yAdd.bin", move_menu.y_add, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_BABY_STEP_Y_P);
    lv_big_button_create(scr, "F:/bmp_yDec.bin", move_menu.y_dec, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_Y_N);
    lv_big_button_create(scr, "F:/bmp_zAdd.bin", move_menu.z_add, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_BABY_STEP_Z_P);
    lv_big_button_create(scr, "F:/bmp_zDec.bin", move_menu.z_dec, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_Z_N);
    buttonV = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_BABY_STEP_DIST);
    labelV = lv_label_create_empty(buttonV);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonV);
      }
    #endif

    lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_RETURN);

    disp_baby_step_dist();

    zOffsetText = lv_label_create(scr, 290, TITLE_YPOS, nullptr);
    disp_z_offset_value();
  #endif
}

void disp_baby_step_dist() {
  if ((int)(100 * babystep_dist) == 5)
    lv_imgbtn_set_src_both(buttonV, TERN(MIXWARE_MODEL_V, "F:/img_step_mm_1.bin", "F:/bmp_baby_move0_05.bin"));
  else if ((int)(100 * babystep_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, TERN(MIXWARE_MODEL_V, "F:/img_step_mm_10.bin", "F:/bmp_baby_move0_1.bin"));

  if (gCfgItems.multiple_language) {
    if ((int)(100 * babystep_dist) == 5) {
      lv_label_set_text(labelV, move_menu.step_005mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if ((int)(100 * babystep_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
  }
}

void disp_z_offset_value() {
  #if ENABLED(MIXWARE_MODEL_V)
    char str_1[16];
    sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(probe.offset[Z_AXIS], 1, 2, str_1));
    if (zOffsetText)lv_label_set_text(zOffsetText, public_buf_l);
  #else
    char buf[20];
    #if HAS_BED_PROBE
    char str_1[16];
    #endif
    sprintf_P(buf, PSTR("offset Z: %s mm"), TERN(HAS_BED_PROBE, dtostrf(probe.offset.z, 1, 3, str_1), "0"));
    lv_label_set_text(zOffsetText, buf);
  #endif
}

void lv_clear_baby_stepping() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#if ENABLED(MIXWARE_MODEL_V)
void disp_baby_step_axis() {
  switch (uiCfg.move_axis) {
    case X_AXIS:
      lv_imgbtn_set_src_both(buttonAxis, "F:/img_axis_x.bin");
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelAxis, move_menu.x_axis);
        lv_obj_align(labelAxis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      }
    break;
    case Y_AXIS:
      lv_imgbtn_set_src_both(buttonAxis, "F:/img_axis_y.bin");
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelAxis, move_menu.y_axis);
        lv_obj_align(labelAxis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      }
    break;
    case Z_AXIS:
      lv_imgbtn_set_src_both(buttonAxis, "F:/img_axis_z.bin");
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelAxis, move_menu.z_axis);
        lv_obj_align(labelAxis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      }
    break;
  }
}
#endif

#endif // HAS_TFT_LVGL_UI
