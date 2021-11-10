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

#include "../../../../module/planner.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;
static lv_obj_t *labelStep, *buttonStep;
#if DISABLED(MIXWARE_MODEL_V)
static lv_obj_t *labelMov, *labelExt, *buttonMov, *buttonExt;
#endif
static lv_obj_t *printSpeedText;

#if ENABLED(MIXWARE_MODEL_V)
  static lv_obj_t *labelType, *buttonType;
#endif

enum {
  ID_C_ADD = 1,
  ID_C_DEC,
  ID_C_MOVE,
  ID_C_EXT,
  ID_C_STEP,
  ID_C_RETURN
  #if ENABLED(MIXWARE_MODEL_V)
    ,ID_C_TYPE
  #endif
};

static bool editingFlowrate;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_C_ADD:
      if (!editingFlowrate) {
        if (feedrate_percentage < MAX_EXT_SPEED_PERCENT - uiCfg.stepPrintSpeed)
          feedrate_percentage += uiCfg.stepPrintSpeed;
        else
          feedrate_percentage = MAX_EXT_SPEED_PERCENT;
      }
      else {
        if (planner.flow_percentage[0] < MAX_EXT_SPEED_PERCENT - uiCfg.stepPrintSpeed)
          planner.flow_percentage[0] += uiCfg.stepPrintSpeed;
        else
          planner.flow_percentage[0] = MAX_EXT_SPEED_PERCENT;
        planner.refresh_e_factor(0);
        #if HAS_MULTI_EXTRUDER
          planner.flow_percentage[1] = planner.flow_percentage[0];
          planner.refresh_e_factor(1);
        #endif
      }
      disp_print_speed();
      break;
    case ID_C_DEC:
      if (!editingFlowrate) {
        if (feedrate_percentage > MIN_EXT_SPEED_PERCENT + uiCfg.stepPrintSpeed)
          feedrate_percentage -= uiCfg.stepPrintSpeed;
        else
          feedrate_percentage = MIN_EXT_SPEED_PERCENT;
      }
      else {
        if (planner.flow_percentage[0] > MIN_EXT_SPEED_PERCENT + uiCfg.stepPrintSpeed)
          planner.flow_percentage[0] -= uiCfg.stepPrintSpeed;
        else
          planner.flow_percentage[0] = MIN_EXT_SPEED_PERCENT;
        planner.refresh_e_factor(0);
        #if HAS_MULTI_EXTRUDER
          planner.flow_percentage[1] = planner.flow_percentage[0];
          planner.refresh_e_factor(1);
        #endif
      }
      disp_print_speed();
      break;
    case ID_C_MOVE:
      editingFlowrate = false;
      disp_speed_type();
      disp_print_speed();
      break;
    case ID_C_EXT:
      editingFlowrate = true;
      disp_speed_type();
      disp_print_speed();
      break;
    case ID_C_STEP:
      if (uiCfg.stepPrintSpeed == 1)
        uiCfg.stepPrintSpeed = 5;
      else if (uiCfg.stepPrintSpeed == 5)
        uiCfg.stepPrintSpeed = 10;
      else
        uiCfg.stepPrintSpeed = 1;
      disp_speed_step();
      break;
    case ID_C_RETURN:
      lv_clear_cur_ui();
      lv_draw_return_ui();
      break;
    #if ENABLED(MIXWARE_MODEL_V)
    case ID_C_TYPE:
      editingFlowrate ^= true;
      disp_speed_type();
      disp_print_speed();
      break;
    #endif
  }
}

void lv_draw_change_speed(void) {
  scr = lv_screen_create(CHANGE_SPEED_UI);
  // Create an Image button
  #if ENABLED(MIXWARE_MODEL_V)
    buttonType = lv_imgbtn_create(scr, nullptr, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_C_TYPE);
    buttonStep = lv_imgbtn_create(scr, nullptr, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_C_STEP);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonType);
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonStep);
    #endif

    lv_big_button_create(scr, "F:/img_add.bin", speed_menu.add,  button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_C_ADD);
    lv_big_button_create(scr, "F:/img_dec.bin", speed_menu.dec,  button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_C_DEC);
    lv_screen_menu_item_return(scr, event_handler, ID_C_RETURN);

    labelType = lv_label_create_empty(buttonType);
    labelStep = lv_label_create_empty(buttonStep);
  #else
  lv_big_button_create(scr, "F:/bmp_Add.bin", speed_menu.add, INTERVAL_V, titleHeight, event_handler, ID_C_ADD);
  lv_big_button_create(scr, "F:/bmp_Dec.bin", speed_menu.dec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_C_DEC);
  buttonMov  = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_C_MOVE);
  buttonExt  = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_C_EXT);
  buttonStep = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_C_STEP);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonMov);
      lv_group_add_obj(g, buttonExt);
      lv_group_add_obj(g, buttonStep);
    }
  #endif
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_C_RETURN);

  // Create labels on the image buttons
  labelMov  = lv_label_create_empty(buttonMov);
  labelExt  = lv_label_create_empty(buttonExt);
  labelStep = lv_label_create_empty(buttonStep);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonMov);
      lv_group_add_obj(g, buttonExt);
      lv_group_add_obj(g, buttonStep);
    }
  #endif
  #endif

  disp_speed_type();
  disp_speed_step();

  printSpeedText = lv_label_create_empty(scr);
  lv_obj_set_style(printSpeedText, &tft_style_label_rel);
  disp_print_speed();

  lv_obj_t *l_tips_type = lv_label_create(buttonType, machine_menu.ButtonTips);
  lv_obj_align(l_tips_type, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
  lv_obj_t *l_tips_step = lv_label_create(buttonStep, machine_menu.ButtonTips);
  lv_obj_align(l_tips_step, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
}

void disp_speed_step() {
  if (uiCfg.stepPrintSpeed == 1)
    lv_imgbtn_set_src_both(buttonStep, TERN(MIXWARE_MODEL_V, "F:/img_step_percent_1.bin",  "F:/bmp_step1_percent.bin"));
  else if (uiCfg.stepPrintSpeed == 5)
    lv_imgbtn_set_src_both(buttonStep, TERN(MIXWARE_MODEL_V, "F:/img_step_percent_5.bin",  "F:/bmp_step5_percent.bin"));
  else if (uiCfg.stepPrintSpeed == 10)
    lv_imgbtn_set_src_both(buttonStep, TERN(MIXWARE_MODEL_V, "F:/img_step_percent_10.bin", "F:/bmp_step10_percent.bin"));

  if (gCfgItems.multiple_language) {
    if (uiCfg.stepPrintSpeed == 1) {
      lv_label_set_text(labelStep, speed_menu.step_1percent);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if (uiCfg.stepPrintSpeed == 5) {
      lv_label_set_text(labelStep, speed_menu.step_5percent);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if (uiCfg.stepPrintSpeed == 10) {
      lv_label_set_text(labelStep, speed_menu.step_10percent);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
  }
}

void disp_print_speed() {
  char buf[30] = { 0 };

  public_buf_l[0] = '\0';

  #if ENABLED(MIXWARE_MODEL_V)
    strcat(public_buf_l, "  ");
    strcat(public_buf_l, speed_menu.move_speed);
    strcat_P(public_buf_l, PSTR(": "));
    sprintf_P(buf, PSTR("%d%%\n"), feedrate_percentage);
    strcat(public_buf_l, buf);

    strcat(public_buf_l, speed_menu.extrude_speed);
    strcat_P(public_buf_l, PSTR(": "));
    sprintf_P(buf, PSTR("%d%%"), planner.flow_percentage[0]);
    strcat(public_buf_l, buf);

    lv_label_set_text(printSpeedText, public_buf_l);

    lv_obj_align(printSpeedText, NULL, LV_ALIGN_IN_TOP_MID, 0, 85);
  #else
    int16_t val;
    const char *lbl;
    if (editingFlowrate) {
      lbl = speed_menu.extrude_speed;
      val = planner.flow_percentage[0];
    }
    else {
      lbl = speed_menu.move_speed;
      val = feedrate_percentage;
    }
    strcpy(public_buf_l, lbl);
    strcat_P(public_buf_l, PSTR(": "));
    sprintf_P(buf, PSTR("%d%%"), val);
    strcat(public_buf_l, buf);
    lv_label_set_text(printSpeedText, public_buf_l);
    lv_obj_align(printSpeedText, nullptr, LV_ALIGN_CENTER, 0, -65);
  #endif
}

void disp_speed_type() {
  #if ENABLED(MIXWARE_MODEL_V)
    lv_imgbtn_set_src_both(buttonType, editingFlowrate ? (gCfgItems.filament_max_temper < 300 ? "F:/img_extruct.bin" : "F:/HI_extruct.bin") : "F:/img_move.bin");
    lv_obj_refresh_ext_draw_pad(buttonType);
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, editingFlowrate ? speed_menu.extrude : speed_menu.move);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
  #else
    lv_imgbtn_set_src_both(buttonMov, editingFlowrate ? "F:/bmp_mov_changeSpeed.bin" : "F:/bmp_mov_sel.bin");
    lv_imgbtn_set_src_both(buttonExt, editingFlowrate ? "F:/bmp_extruct_sel.bin" : "F:/bmp_speed_extruct.bin");
    lv_obj_refresh_ext_draw_pad(buttonExt);
    lv_obj_refresh_ext_draw_pad(buttonMov);

    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelMov, speed_menu.move);
      lv_obj_align(labelMov, buttonMov, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(labelExt, speed_menu.extrude);
      lv_obj_align(labelExt, buttonExt, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  #endif
}

void lv_clear_change_speed() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
