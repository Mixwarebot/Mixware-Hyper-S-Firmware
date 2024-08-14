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
#include "../../../module/motion.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

static lv_obj_t *labelV, *buttonV;
// static lv_task_t *updatePosTask;
static char cur_label = 'Z';
static float cur_pos = 0;

  static lv_obj_t *labelAxis, *buttonAxis;
  static lv_obj_t *labelPosX, *labelPosY, *labelPosZ;

enum {
  // ID_M_X_P = 1,
  // ID_M_X_N,
  // ID_M_Y_P,
  // ID_M_Y_N,
  // ID_M_Z_P,
  // ID_M_Z_N,
  // ID_M_STEP,
  // ID_M_RETURN

  ID_M_RETURN = 1
  ,ID_M_AXIS
  ,ID_M_STEP_V
  ,ID_M_ADD
  ,ID_M_DEC
  ,ID_M_PAGE_SW
  ,ID_M_OFF_ALL
};

void disp_cur_pos() {
    char str_1[16];
    sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(current_position[X_AXIS], 1, 1, str_1));
    if (labelPosX)lv_label_set_text(labelPosX, public_buf_l);

    ZERO(str_1);
    sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(current_position[Y_AXIS], 1, 1, str_1));
    if (labelPosY)lv_label_set_text(labelPosY, public_buf_l);

    ZERO(str_1);
    sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(current_position[Z_AXIS], 1, 1, str_1));
    if (labelPosZ) lv_label_set_text(labelPosZ, public_buf_l);

  // char str_1[16];
  // sprintf_P(public_buf_l, PSTR("%c:%s mm"), cur_label, dtostrf(cur_pos, 1, 1, str_1));
  // if (labelP) lv_label_set_text(labelP, public_buf_l);
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  char str_1[16];
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_M_RETURN:
      clear_cur_ui();
      draw_return_ui();
      return;
    #if ENABLED(TFT_MIXWARE_UI)
    case ID_M_AXIS:
      switch (uiCfg.move_axis) {
        case X_AXIS: uiCfg.move_axis = Y_AXIS; break;
        case Y_AXIS: uiCfg.move_axis = Z_AXIS; break;
        case Z_AXIS: uiCfg.move_axis = X_AXIS; break;
      }
      disp_move_axis();
      break;
    case ID_M_STEP_V:
      if (     (int)(10 * uiCfg.move_dist) == 1)   uiCfg.move_dist = 1;
      else if ((int)(10 * uiCfg.move_dist) == 10)  uiCfg.move_dist = 5;
      else if ((int)(10 * uiCfg.move_dist) == 50)  uiCfg.move_dist = 10;
      else if ((int)(10 * uiCfg.move_dist) == 100) uiCfg.move_dist = 0.1;
      disp_move_dist();
      break;
    case ID_M_ADD:
      if (queue.ring_buffer.length <= (BUFSIZE - 3)) {
        switch (uiCfg.move_axis) {
          case X_AXIS:
            if ((cur_pos = current_position[X_AXIS] + uiCfg.move_dist) > X_MAX_POS) cur_pos = X_MAX_POS;
            sprintf_P(public_buf_l, PSTR("G1 X%s F%d"), dtostrf(cur_pos, 1, 3, str_1), uiCfg.moveSpeed);
            break;
          case Y_AXIS:
            if ((cur_pos = current_position[Y_AXIS] + uiCfg.move_dist) > Y_MAX_POS) cur_pos = Y_MAX_POS;
            sprintf_P(public_buf_l, PSTR("G1 Y%s F%d"), dtostrf(cur_pos, 1, 3, str_1), uiCfg.moveSpeed);
            break;
          case Z_AXIS:
            if ((cur_pos = current_position[Z_AXIS] + uiCfg.move_dist) > Z_MAX_POS) cur_pos = Z_MAX_POS;
            sprintf_P(public_buf_l, PSTR("G1 Z%s F%d"), dtostrf(cur_pos, 1, 3, str_1), uiCfg.moveSpeed/10);
            break;
          default: break;
        }
        queue.enqueue_one_now(public_buf_l);
      }
      break;
    case ID_M_DEC:
      if (queue.ring_buffer.length <= (BUFSIZE - 3)) {
        switch (uiCfg.move_axis) {
          case X_AXIS:
            if ((cur_pos = current_position[X_AXIS] - uiCfg.move_dist) < 0) cur_pos = 0;
            sprintf_P(public_buf_l, PSTR("G1 X%s F%d"), dtostrf(cur_pos, 1, 3, str_1), uiCfg.moveSpeed);
            break;
          case Y_AXIS:
            if ((cur_pos = current_position[Y_AXIS] - uiCfg.move_dist) < 0) cur_pos = 0;
            sprintf_P(public_buf_l, PSTR("G1 Y%s F%d"), dtostrf(cur_pos, 1, 3, str_1), uiCfg.moveSpeed);
            break;
          case Z_AXIS:
            if ((cur_pos = current_position[Z_AXIS] - uiCfg.move_dist) < 0) cur_pos = 0;
            sprintf_P(public_buf_l, PSTR("G1 Z%s F%d"), dtostrf(cur_pos, 1, 3, str_1), uiCfg.moveSpeed/10);
            break;
        }
        queue.enqueue_one_now(public_buf_l);
      }
      break;
    case ID_M_PAGE_SW:
      uiCfg.para_ui_page ^= 1;
      lv_clear_move_motor();
      lv_draw_move_motor();
      break;
    case ID_M_OFF_ALL:
      queue.inject_P(PSTR("M84"));
      break;
    #endif
  }
  disp_cur_pos();
}

void refresh_pos(lv_task_t *) {
  switch (cur_label) {
    case 'X': cur_pos = current_position.x; break;
    case 'Y': cur_pos = current_position.y; break;
    case 'Z': cur_pos = current_position.z; break;
    default: return;
  }
  disp_cur_pos();
}

void lv_draw_move_motor() {
  scr = lv_screen_create(MOVE_MOTOR_UI);

    if (!uiCfg.para_ui_page) {
      uiCfg.move_axis = X_AXIS;
      buttonAxis = lv_imgbtn_create(scr, nullptr, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_M_AXIS);
      buttonV = lv_imgbtn_create(scr, nullptr, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_M_STEP_V);

      lv_big_button_create(scr, "F:/img_add.bin", preheat_menu.add,  button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_M_ADD);
      lv_big_button_create(scr, "F:/img_dec.bin", preheat_menu.dec,  button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_M_DEC);
      if (uiCfg.print_state == IDLE) {
        lv_screen_menu_item_turn_page(scr, tool_menu.more, event_handler, ID_M_PAGE_SW);
      }

      labelAxis = lv_label_create_empty(buttonAxis);
      labelV = lv_label_create_empty(buttonV);
      lv_label_set_text(labelAxis, move_menu.x_axis);
      lv_obj_align(labelAxis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      disp_move_axis();
      disp_move_dist();
      lv_obj_t *l_tips_axis = lv_label_create(buttonAxis, machine_menu.ButtonTips);
      lv_obj_align(l_tips_axis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
      lv_obj_t *l_tips_v = lv_label_create(buttonV, machine_menu.ButtonTips);
      lv_obj_align(l_tips_v, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
    }
    else{
      lv_big_button_create(scr, "F:/img_motor_off_all.bin", set_menu.motoroff,  button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_M_OFF_ALL);
      lv_screen_menu_item_turn_page(scr, machine_menu.previous, event_handler, ID_M_PAGE_SW);
    }
    lv_screen_menu_item_return(scr, event_handler, ID_M_RETURN);

    lv_obj_t *buttonPosX = lv_img_create(scr, nullptr);
    lv_img_set_src(buttonPosX, "F:/img_xpos_state.bin");
    lv_obj_set_pos(buttonPosX, 40, 60);
    labelPosX = lv_label_create(scr, 85, 70, nullptr);

    lv_obj_t *buttonPosY = lv_img_create(scr, nullptr);
    lv_img_set_src(buttonPosY, "F:/img_ypos_state.bin");
    lv_obj_set_pos(buttonPosY, 180, 60);
    labelPosY = lv_label_create(scr, 225, 70, nullptr);

    lv_obj_t *buttonPosZ = lv_img_create(scr, nullptr);
    lv_img_set_src(buttonPosZ, "F:/img_zpos_state.bin");
    lv_obj_set_pos(buttonPosZ, 40, 110);
    labelPosZ = lv_label_create(scr, 85, 120, nullptr);

  disp_cur_pos();
}

void disp_move_dist() {
  if ((int)(10 * uiCfg.move_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, "F:/img_step_mm_0_1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, "F:/img_step_mm_1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 100)
    lv_imgbtn_set_src_both(buttonV, "F:/img_step_mm_10.bin");

    else if ((int)(10 * uiCfg.move_dist) == 50)
      lv_imgbtn_set_src_both(buttonV, "F:/img_step_mm_5.bin");

  if (gCfgItems.multiple_language) {
    if ((int)(10 * uiCfg.move_dist) == 1) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if ((int)(10 * uiCfg.move_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_1mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if ((int)(10 * uiCfg.move_dist) == 100) {
      lv_label_set_text(labelV, move_menu.step_10mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }

      else if ((int)(10 * uiCfg.move_dist) == 50) {
        lv_label_set_text(labelV, move_menu.step_5mm);
        lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      }
  }
}

void lv_clear_move_motor() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  // lv_task_del(updatePosTask);
  lv_obj_del(scr);
}

void disp_move_axis() {
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
#endif // HAS_TFT_LVGL_UI
