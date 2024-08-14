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

#if ENABLED(TFT_MIXWARE_UI)

#include "../../../gcode/queue.h"
#include "../../../module/motion.h"
#include "../../../module/probe.h"

extern lv_group_t * g;
static lv_obj_t * scr;

static lv_obj_t *buttonStep, *labelStep;
static lv_obj_t *labelZ_offset;
static float    z_offset_bak;

enum {
  ID_ADJUST_Z_ADD = 1,
  ID_ADJUST_Z_DEC,
  ID_ADJUST_Z_SAVE,
  ID_ADJUST_Z_STEP,
  ID_ADJUST_Z_RETURN,
};

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  char str_1[16];
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_ADJUST_Z_ADD:
      if (queue.ring_buffer.length <= (BUFSIZE - 3)) {
        if (uiCfg.leveling_first_time) {
          if (!all_axes_trusted())
            queue.enqueue_now_P(PSTR("G28"));
          queue.enqueue_now_P(PSTR("G1 X150 Y150\nG1 Z0"));
          uiCfg.leveling_first_time = 0;
        }
        else {
          ZERO(public_buf_l);
          queue.enqueue_one(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Z%s F300"), dtostrf(uiCfg.move_dist, 1, 2, str_1));
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_one(PSTR("G90"));
          probe.offset[Z_AXIS] += uiCfg.move_dist;
          disp_z_offset_pos();
        }
      }
      break;
    case ID_ADJUST_Z_DEC:
      if (queue.ring_buffer.length <= (BUFSIZE - 3)) {
        if (uiCfg.leveling_first_time) {
          if (!all_axes_trusted())
            queue.enqueue_now_P(PSTR("G28"));
          queue.enqueue_now_P(PSTR("G1 X150 Y150\nG1 Z0"));
          uiCfg.leveling_first_time = 0;
        }
        else {
          ZERO(public_buf_l);
          queue.enqueue_one(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Z-%s F300"), dtostrf(uiCfg.move_dist, 1, 2, str_1));
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_one(PSTR("G90"));
          probe.offset[Z_AXIS] -= uiCfg.move_dist;
          disp_z_offset_pos();
        }
      }
      break;
    case ID_ADJUST_Z_SAVE:
      lv_clear_adjust_z_offset();
      lv_draw_dialog(DIALOG_STORE_EEPROM_TIPS);
      break;
    case ID_ADJUST_Z_STEP:
      if ((int)(100 * uiCfg.move_dist) == 5)
        uiCfg.move_dist = 0.1;
      else
        uiCfg.move_dist = 0.05;

      disp_adjust_dist();
      break;
    case ID_ADJUST_Z_RETURN:
      if (!offset_save_flag)
        probe.offset[Z_AXIS] = z_offset_bak;

      queue.inject_P(PSTR("G91\nG1 Z10 F300\nG90\nM84"));
      clear_cur_ui();
      lv_draw_level_select();
      break;
  }
}

void lv_draw_adjust_z_offset(void) {
  scr = lv_screen_create(ADJUST_Z_OFFSET_UI);
  z_offset_bak = probe.offset[Z_AXIS];
  offset_save_flag = false;

  lv_big_button_create(scr, "F:/img_save.bin", adjust_z_menu.save, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_ADJUST_Z_SAVE);

  buttonStep = lv_imgbtn_create(scr, nullptr, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_ADJUST_Z_STEP);

  lv_big_button_create(scr, "F:/img_add.bin", adjust_z_menu.add,  button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_ADJUST_Z_ADD);
  lv_big_button_create(scr, "F:/img_dec.bin", adjust_z_menu.dec,  button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_ADJUST_Z_DEC);

  lv_screen_menu_item_return(scr, event_handler, ID_ADJUST_Z_RETURN);

  labelStep = lv_label_create_empty(buttonStep);

  /*Create an Image button*/
  lv_obj_t  *buttonZ_offset = lv_img_create(scr, NULL);
  lv_img_set_src(buttonZ_offset, "F:/img_zpos_state.bin");
  lv_obj_set_pos(buttonZ_offset, 125, 85);

  labelZ_offset = lv_label_create(scr, 170, 95, nullptr);

  disp_adjust_dist();
  disp_z_offset_pos();

  lv_obj_t *l_tips_step = lv_label_create(buttonStep, machine_menu.ButtonTips);
  lv_obj_align(l_tips_step, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
}

void disp_z_offset_pos() {
  char str_1[16];
  sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(probe.offset[Z_AXIS], 1, 2, str_1));
  lv_label_set_text(labelZ_offset, public_buf_l);
}

void disp_adjust_dist() {
  if ((int)(100 * uiCfg.move_dist) == 5) {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, "F:/img_step_mm_1.bin");
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, "F:/img_step_mm_1.bin");
  }
  else if ((int)(100 * uiCfg.move_dist) == 10) {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, "F:/img_step_mm_10.bin");
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, "F:/img_step_mm_10.bin");
  }
  if (gCfgItems.multiple_language != 0) {
    if ((int)(100 * uiCfg.move_dist) == 5) {
      lv_label_set_text(labelStep, move_menu.step_005mm);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if ((int)(100 * uiCfg.move_dist) == 10) {
      lv_label_set_text(labelStep, move_menu.step_01mm);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
  }
}

void lv_clear_adjust_z_offset() {
  lv_obj_del(scr);
}

#endif // MIXWARE_MODEL_V
#endif // HAS_TFT_LVGL_UI
