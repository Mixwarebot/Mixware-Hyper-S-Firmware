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

#ifndef USE_NEW_LVGL_CONF
static lv_obj_t *scr;
#endif

static lv_obj_t *labelV, *buttonV
                #if DISABLED(TFT_MIXWARE_LVGL_UI)
                , *labelP
                #endif
                ;
static lv_task_t *updatePosTask;
static char cur_label = 'Z';
static float cur_pos = 0;

#if ENABLED(TFT_MIXWARE_LVGL_UI)
  static lv_obj_t *labelAxis, *buttonAxis;
  static lv_obj_t *labelPosX, *labelPosY, *labelPosZ;
#endif

enum {
  ID_M_X_P = 1,
  ID_M_X_N,
  ID_M_Y_P,
  ID_M_Y_N,
  ID_M_Z_P,
  ID_M_Z_N,
  ID_M_STEP,
  ID_M_RETURN
  #if ENABLED(TFT_MIXWARE_LVGL_UI)
    , ID_M_AXIS
    , ID_M_STEP2
    , ID_M_ADD
    , ID_M_DEC
    , ID_M_PAGE_SW
    , ID_M_OFF
  #endif
};

void disp_cur_pos() {
  char str_1[16];
  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    sprintf_P(public_buf_l, PSTR("%c:%s mm"), cur_label, dtostrf(cur_pos, 1, 1, str_1));
    if (labelP) lv_label_set_text(labelP, public_buf_l);
  #else
    sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(current_position[X_AXIS], 1, 1, str_1));
    if (labelPosX)lv_label_set_text(labelPosX, public_buf_l);

    ZERO(str_1);
    sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(current_position[Y_AXIS], 1, 1, str_1));
    if (labelPosY)lv_label_set_text(labelPosY, public_buf_l);

    ZERO(str_1);
    sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(current_position[Z_AXIS], 1, 1, str_1));
    if (labelPosZ) lv_label_set_text(labelPosZ, public_buf_l);
  #endif
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  char str_1[16];
  if (event != LV_EVENT_RELEASED) return;
  if (!queue.ring_buffer.full(3)) {
    bool do_inject = true;
    float dist = uiCfg.move_dist;
    switch (obj->mks_obj_id) {
      case ID_M_X_N: dist *= -1; case ID_M_X_P: cur_label = 'X'; break;
      case ID_M_Y_N: dist *= -1; case ID_M_Y_P: cur_label = 'Y'; break;
      case ID_M_Z_N: dist *= -1; case ID_M_Z_P: cur_label = 'Z'; break;
      default: do_inject = false;
    }
    if (do_inject) {
      sprintf_P(public_buf_l, PSTR("G91\nG1 %c%s F%d\nG90"), cur_label, dtostrf(dist, 1, 3, str_1), uiCfg.moveSpeed);
      queue.inject(public_buf_l);
    }
  }

  switch (obj->mks_obj_id) {
    case ID_M_STEP:
      if (ABS(10 * (int)uiCfg.move_dist) == 100)
        uiCfg.move_dist = 0.1;
      else
        uiCfg.move_dist *= 10.0f;
      disp_move_dist();
      break;
    case ID_M_RETURN:
      // lv_clear_move_motor();
      // lv_draw_tool();
      clear_cur_ui();
      draw_return_ui();
      return;
    #if ENABLED(TFT_MIXWARE_LVGL_UI)
      case ID_M_AXIS:
        MUI.update_move_axis(buttonAxis, labelAxis);
        break;
      case ID_M_STEP2:
        if (     (int)(10 * uiCfg.move_dist) == 1)   uiCfg.move_dist = 1;
        else if ((int)(10 * uiCfg.move_dist) == 10)  uiCfg.move_dist = 5;
        else if ((int)(10 * uiCfg.move_dist) == 50)  uiCfg.move_dist = 10;
        else if ((int)(10 * uiCfg.move_dist) == 100) uiCfg.move_dist = 0.1;
        disp_move_dist();
        break;
      case ID_M_ADD:
        if (queue.ring_buffer.length <= (BUFSIZE - 3)) {
          switch (MPRE.move_axis) {
            case X_AXIS:
              if ((cur_pos = current_position[X_AXIS] + uiCfg.move_dist) > X_MAX_POS) cur_pos = X_MAX_POS;
              sprintf_P(public_buf_l, PSTR("G1 X%s F%d"), dtostrf(cur_pos, 1, 3, str_1), XY_PROBE_FEEDRATE / 2);
              break;
            case Y_AXIS:
              if ((cur_pos = current_position[Y_AXIS] + uiCfg.move_dist) > Y_MAX_POS) cur_pos = Y_MAX_POS;
              sprintf_P(public_buf_l, PSTR("G1 Y%s F%d"), dtostrf(cur_pos, 1, 3, str_1), XY_PROBE_FEEDRATE / 2);
              break;
            case Z_AXIS:
              if ((cur_pos = current_position[Z_AXIS] + uiCfg.move_dist) > Z_MAX_POS) cur_pos = Z_MAX_POS;
              sprintf_P(public_buf_l, PSTR("G1 Z%s F%d"), dtostrf(cur_pos, 1, 3, str_1), Z_PROBE_FEEDRATE_FAST);
              break;
            default: break;
          }
          queue.enqueue_one_now(public_buf_l);
        }
        break;
      case ID_M_DEC:
        if (queue.ring_buffer.length <= (BUFSIZE - 3)) {
          switch (MPRE.move_axis) {
            case X_AXIS:
              if ((cur_pos = current_position[X_AXIS] - uiCfg.move_dist) < X_MIN_POS) cur_pos = X_MIN_POS;
              sprintf_P(public_buf_l, PSTR("G1 X%s F%d"), dtostrf(cur_pos, 1, 3, str_1), XY_PROBE_FEEDRATE / 2);
              break;
            case Y_AXIS:
              if ((cur_pos = current_position[Y_AXIS] - uiCfg.move_dist) < Y_MIN_POS) cur_pos = Y_MIN_POS;
              sprintf_P(public_buf_l, PSTR("G1 Y%s F%d"), dtostrf(cur_pos, 1, 3, str_1), XY_PROBE_FEEDRATE / 2);
              break;
            case Z_AXIS:
              if ((cur_pos = current_position[Z_AXIS] - uiCfg.move_dist) < Z_MIN_POS) cur_pos = Z_MIN_POS;
              sprintf_P(public_buf_l, PSTR("G1 Z%s F%d"), dtostrf(cur_pos, 1, 3, str_1), Z_PROBE_FEEDRATE_FAST);
              break;
          }
          queue.enqueue_one_now(public_buf_l);
        }
        break;
      case ID_M_PAGE_SW:
        uiCfg.para_ui_page ^= true;
        lv_clear_move_motor();
        lv_draw_move_motor();
        break;
      case ID_M_OFF:
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
  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    lv_obj_t *buttonXI = lv_big_button_create(scr, "F:/bmp_xAdd.bin", move_menu.x_add, INTERVAL_V, titleHeight, event_handler, ID_M_X_P);
    lv_obj_clear_protect(buttonXI, LV_PROTECT_FOLLOW);
    lv_big_button_create(scr, "F:/bmp_xDec.bin", move_menu.x_dec, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_X_N);
    lv_big_button_create(scr, "F:/bmp_yAdd.bin", move_menu.y_add, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_M_Y_P);
    lv_big_button_create(scr, "F:/bmp_yDec.bin", move_menu.y_dec, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Y_N);
    lv_big_button_create(scr, "F:/bmp_zAdd.bin", move_menu.z_add, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_M_Z_P);
    lv_big_button_create(scr, "F:/bmp_zDec.bin", move_menu.z_dec, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Z_N);

    // button with image and label changed dynamically by disp_move_dist
    buttonV = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_M_STEP);
    labelV = lv_label_create_empty(buttonV);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonV);
    #endif

    lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_RETURN);

    // We need to patch the title to leave some space on the right for displaying the status
    lv_obj_t * title = lv_obj_get_child_back(scr, nullptr);
    if (title != nullptr) lv_obj_set_width(title, TFT_WIDTH - 101);
    labelP = lv_label_create(scr, TFT_WIDTH - 100, TITLE_YPOS, "Z:0.0mm");
    if (labelP != nullptr)
      updatePosTask = lv_task_create(refresh_pos, 300, LV_TASK_PRIO_LOWEST, 0);

    disp_move_dist();
    disp_cur_pos();
  #else
    if (!uiCfg.para_ui_page) {
      MPRE.move_axis = X_AXIS;
      buttonAxis = lv_imgbtn_create(scr, MIMG.axisX, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_M_AXIS);
      buttonV    = lv_imgbtn_create(scr, nullptr,    IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), event_handler, ID_M_STEP2);

      lv_big_button_create(scr, MIMG.add, MTR.add, IMAGEBTN_P_X(4), IMAGEBTN_P_Y(4), event_handler, ID_M_ADD);
      lv_big_button_create(scr, MIMG.dec, MTR.dec, IMAGEBTN_P_X(5), IMAGEBTN_P_Y(5), event_handler, ID_M_DEC);
      if (uiCfg.print_state == IDLE) {
        MUI.page_bottom_button_middle(scr, MTR.more, event_handler, ID_M_PAGE_SW);
      }

      labelAxis = lv_label_create_empty(buttonAxis);
      labelV = lv_label_create_empty(buttonV);
      lv_label_set_text(labelAxis, MTR.x);
      lv_obj_align(labelAxis, buttonAxis, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);

      disp_move_dist();
      MUI.page_button_add_tips(buttonAxis);
      MUI.page_button_add_tips(buttonV);
    }
    else{
      lv_big_button_create(scr, MIMG.motorOff, MTR.axisDisabled,  IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_M_OFF);
      MUI.page_bottom_button_middle(scr, MTR.previous, event_handler, ID_M_PAGE_SW);
    }
    MUI.page_button_return(scr, event_handler, ID_M_RETURN);

    lv_obj_t *buttonPosX = lv_img_create(scr, nullptr);
    lv_obj_t *buttonPosY = lv_img_create(scr, nullptr);
    lv_obj_t *buttonPosZ = lv_img_create(scr, nullptr);

    lv_img_set_src(buttonPosX, MIMG.stateX);
    lv_img_set_src(buttonPosY, MIMG.stateY);
    lv_img_set_src(buttonPosZ, MIMG.stateZ);

    lv_obj_set_pos(buttonPosX, 40, 60);
    lv_obj_set_pos(buttonPosY, 180, 60);
    lv_obj_set_pos(buttonPosZ, 40, 110);

    labelPosX = lv_label_create(scr, 85, 70, nullptr);
    labelPosY = lv_label_create(scr, 225, 70, nullptr);
    labelPosZ = lv_label_create(scr, 85, 120, nullptr);

    updatePosTask = lv_task_create(refresh_pos, 300, LV_TASK_PRIO_LOWEST, 0);
    disp_cur_pos();
  #endif
}

void disp_move_dist() {
#if DISABLED(TFT_MIXWARE_LVGL_UI)
  if ((int)(10 * uiCfg.move_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move0_1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 100)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move10.bin");

  if (gCfgItems.multiple_language) {
    if ((int)(10 * uiCfg.move_dist) == 1) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(10 * uiCfg.move_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_1mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(10 * uiCfg.move_dist) == 100) {
      lv_label_set_text(labelV, move_menu.step_10mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
#else
  if ((int)(10 * uiCfg.move_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, MIMG.move_distance_mm_0_1);
  else if ((int)(10 * uiCfg.move_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, MIMG.move_distance_mm_1);
  else if ((int)(10 * uiCfg.move_dist) == 50)
    lv_imgbtn_set_src_both(buttonV, MIMG.move_distance_mm_5);
  else if ((int)(10 * uiCfg.move_dist) == 100)
    lv_imgbtn_set_src_both(buttonV, MIMG.move_distance_mm_10);

  if ((int)(10 * uiCfg.move_dist) == 1)
    lv_label_set_text(labelV, MTR.move_distance_mm_0_1);
  else if ((int)(10 * uiCfg.move_dist) == 10)
    lv_label_set_text(labelV, MTR.move_distance_mm_1);
  else if ((int)(10 * uiCfg.move_dist) == 50)
    lv_label_set_text(labelV, MTR.move_distance_mm_5);
  else if ((int)(10 * uiCfg.move_dist) == 100)
    lv_label_set_text(labelV, MTR.move_distance_mm_10);

  lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
#endif
}

void lv_clear_move_motor() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_task_del(updatePosTask);
#ifdef USE_NEW_LVGL_CONF
  lv_obj_clean(mks_ui.src_main);
#else
  lv_obj_del(scr);
#endif
}

#endif // HAS_TFT_LVGL_UI
