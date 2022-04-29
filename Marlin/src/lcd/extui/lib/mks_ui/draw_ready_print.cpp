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

#include "draw_ready_print.h"
#include "draw_tool.h"
#include <lv_conf.h>
#include "tft_lvgl_configuration.h"
#include "mks_hardware_test.h"
#include "draw_ui.h"

#include <lvgl.h>

#include "../../../../module/temperature.h"
#include "../../../../sd/cardreader.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  #include "../../../tft_io/touch_calibration.h"
  #include "draw_touch_calibration.h"
#endif

#include <stdio.h>

extern lv_group_t*  g;
static lv_obj_t *scr;
#if ENABLED(MKS_TEST)
  uint8_t curent_disp_ui = 0;
#endif
#if ENABLED(MIXWARE_MODEL_V)
  static lv_obj_t *label_R_Ext;
  #if HAS_HEATED_BED
    static lv_obj_t *label_R_Bed;
  #endif

  static lv_style_t style_para;
#endif

enum {
  ID_TOOL = 1,
  ID_SET,
  ID_PRINT
};
static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_ready_print();

  switch (obj->mks_obj_id) {
    case ID_TOOL:
      lv_draw_tool();
      break;
    case ID_SET:
      lv_draw_set();
      break;
    case ID_PRINT:
      #if (FLASH_INF_VALID_FLAG >= 0x20210629)
        card.mount();

        if (card.isMounted())
          lv_draw_print_file();
        else
          lv_draw_dialog(DIALOG_TYPE_REPRINT_NO_FILE);
      #else
        lv_draw_print_file();
      #endif
      break;
  }
}

lv_obj_t *limit_info, *det_info;
lv_obj_t *tmc_state_info;
lv_style_t limit_style, det_style, tmc_state_style;
void disp_Limit_ok() {
  limit_style.text.color.full = 0xFFFF;
  lv_obj_set_style(limit_info, &limit_style);
  lv_label_set_text(limit_info, "Limit:ok");
}
void disp_Limit_error() {
  limit_style.text.color.full = 0xF800;
  lv_obj_set_style(limit_info, &limit_style);
  lv_label_set_text(limit_info, "Limit:error");
}

void disp_det_ok() {
  det_style.text.color.full = 0xFFFF;
  lv_obj_set_style(det_info, &det_style);
  lv_label_set_text(det_info, "det:ok");
}
void disp_det_error() {
  det_style.text.color.full = 0xF800;
  lv_obj_set_style(det_info, &det_style);
  lv_label_set_text(det_info, "det:error");
}

void disp_tmc_ok() {
  tmc_state_style.text.color.full = 0xFFFF;
  lv_obj_set_style(tmc_state_info, &tmc_state_style);
  lv_label_set_text(tmc_state_info, "TMC CONNECTION OK");
}
void disp_tmc_error() {
  tmc_state_style.text.color.full = 0xF800;
  lv_obj_set_style(tmc_state_info, &tmc_state_style);
  lv_label_set_text(tmc_state_info, "TMC CONNECTION ERROR");
}

lv_obj_t *e1, *e2, *e3, *bed;
void mks_disp_test() {
  char buf[30] = {0};
  sprintf_P(buf, PSTR("e1:%d"), (int)thermalManager.temp_hotend[0].celsius);
  lv_label_set_text(e1, buf);
  #if HAS_MULTI_HOTEND
    sprintf_P(buf, PSTR("e2:%d"), (int)thermalManager.temp_hotend[1].celsius);
    lv_label_set_text(e2, buf);
  #endif
  #if HAS_HEATED_BED
    sprintf_P(buf, PSTR("bed:%d"), (int)thermalManager.temp_bed.celsius);
    lv_label_set_text(bed, buf);
  #endif
}

void lv_draw_ready_print(void) {
  char buf[30] = {0};
  lv_obj_t *buttonTool;

  disp_state_stack._disp_index = 0;
  ZERO(disp_state_stack._disp_state);
  scr = lv_screen_create(PRINT_READY_UI, "");

  if (mks_test_flag == 0x1E) {
    // Create image buttons
    buttonTool = lv_imgbtn_create(scr, "F:/bmp_tool.bin", event_handler, ID_TOOL);

    lv_obj_set_pos(buttonTool, 360, 180);

    lv_obj_t *label_tool = lv_label_create_empty(buttonTool);
    if (gCfgItems.multiple_language) {
      lv_label_set_text(label_tool, main_menu.tool);
      lv_obj_align(label_tool, buttonTool, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }

    #if 1
      e1 = lv_label_create_empty(scr);
      lv_obj_set_pos(e1, 20, 20);
      sprintf_P(buf, PSTR("e1:  %d"), (int)thermalManager.temp_hotend[0].celsius);
      lv_label_set_text(e1, buf);
      #if HAS_MULTI_HOTEND
        e2 = lv_label_create_empty(scr);
        lv_obj_set_pos(e2, 20, 45);
        sprintf_P(buf, PSTR("e1:  %d"), (int)thermalManager.temp_hotend[1].celsius);
        lv_label_set_text(e2, buf);
      #endif

      #if HAS_HEATED_BED
        bed = lv_label_create_empty(scr);
        lv_obj_set_pos(bed, 20, 95);
        sprintf_P(buf, PSTR("bed:  %d"), (int)thermalManager.temp_bed.celsius);
        lv_label_set_text(bed, buf);
      #endif

      limit_info = lv_label_create_empty(scr);

      lv_style_copy(&limit_style, &lv_style_scr);
      limit_style.body.main_color.full = 0X0000;
      limit_style.body.grad_color.full = 0X0000;
      limit_style.text.color.full      = 0Xffff;
      lv_obj_set_style(limit_info, &limit_style);

      lv_obj_set_pos(limit_info, 20, 120);
      lv_label_set_text(limit_info, " ");

      det_info = lv_label_create_empty(scr);

      lv_style_copy(&det_style, &lv_style_scr);
      det_style.body.main_color.full = 0X0000;
      det_style.body.grad_color.full = 0X0000;
      det_style.text.color.full      = 0Xffff;
      lv_obj_set_style(det_info, &det_style);

      lv_obj_set_pos(det_info, 20, 145);
      lv_label_set_text(det_info, " ");

      tmc_state_info = lv_label_create_empty(scr);

      lv_style_copy(&tmc_state_style, &lv_style_scr);
      tmc_state_style.body.main_color.full = 0X0000;
      tmc_state_style.body.grad_color.full = 0X0000;
      tmc_state_style.text.color.full      = 0Xffff;
      lv_obj_set_style(tmc_state_info, &tmc_state_style);

      lv_obj_set_pos(tmc_state_info, 20, 170);
      lv_label_set_text(tmc_state_info, " ");
    #endif // if 1

  }
  else {
    #if ENABLED(MIXWARE_MODEL_V)
      if (gCfgItems.filament_max_temper > 300) {
        // lv_obj_t *mode = lv_label_create(scr, filament_temp_select.temp_mode_tips);
        // lv_obj_align(mode, nullptr, LV_ALIGN_CENTER, 0, -86);
        lv_style_copy(&style_para, &lv_style_plain);
        style_para.body.border.color = LV_COLOR_BACKGROUND;
        style_para.body.border.width = 1;
        style_para.body.main_color   = LV_COLOR_BACKGROUND;
        style_para.body.grad_color   = LV_COLOR_BACKGROUND;
        style_para.body.shadow.width = 0;
        style_para.body.radius       = 3;
        style_para.body.border.color = TFT_LV_PARA_BACK_BODY_COLOR;
        style_para.body.border.width = 2;
        style_para.text.color        = LV_COLOR_WHITE;
        style_para.text.font         = &TERN(HAS_SPI_FLASH_FONT, gb2312_puhui32, lv_font_roboto_22);

        lv_obj_t* btn = lv_btn_create(scr, nullptr);
        lv_obj_t* label = lv_label_create(btn, filament_temp_select.temp_mode_tips, true);
        lv_btn_set_style_both(btn, &style_para);
        lv_obj_set_size(btn, 240, 36);
        lv_obj_align(label, btn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_align(btn, nullptr, LV_ALIGN_CENTER, 0, -78);
      }

      lv_big_button_create(scr, "F:/img_tool.bin",     main_menu.tool,  28,  340, event_handler, ID_TOOL);
      lv_big_button_create(scr, "F:/img_set.bin",      main_menu.set,   173, 340, event_handler, ID_SET);
      lv_big_button_create(scr, MIXWARE_UI_SELECT("F:/img_printing.bin", "F:/HI_printing.bin"), main_menu.print, 101, 198, event_handler, ID_PRINT);

      lv_obj_t *buttonExt = lv_img_create(scr, nullptr);
      lv_img_set_src(buttonExt, MIXWARE_UI_SELECT("F:/bmp_ext_state.bin", "F:/HI_ext_state.bin"));
      lv_obj_set_pos(buttonExt, 30, 82);
      label_R_Ext = lv_label_create(scr, 75, 92, nullptr);

      lv_obj_t *buttonBedstate = lv_img_create(scr, nullptr);
      lv_img_set_src(buttonBedstate, "F:/bmp_bed_state.bin");
      lv_obj_set_pos(buttonBedstate, 175, 82);
      #if HAS_HEATED_BED
        label_R_Bed = lv_label_create(scr, 220, 92, nullptr);
      #endif

      disp_ready_print_temp();
    #else
      lv_big_button_create(scr, "F:/bmp_tool.bin", main_menu.tool, 20, 90, event_handler, ID_TOOL);
      lv_big_button_create(scr, "F:/bmp_set.bin", main_menu.set, 180, 90, event_handler, ID_SET);
      lv_big_button_create(scr, "F:/bmp_printing.bin", main_menu.print, 340, 90, event_handler, ID_PRINT);
    #endif
  }

  #if ENABLED(TOUCH_SCREEN_CALIBRATION)
    // If calibration is required, let's trigger it now, handles the case when there is default value in configuration files
    if (!touch_calibration.calibration_loaded()) {
      lv_clear_ready_print();
      lv_draw_touch_calibration_screen();
    }
  #endif
}

void lv_clear_ready_print() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#if ENABLED(MIXWARE_MODEL_V)
void disp_ready_print_temp() {
  sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_hotend[0].celsius,  (int)thermalManager.temp_hotend[0].target);
  lv_label_set_text(label_R_Ext, public_buf_l);

  #if HAS_HEATED_BED
    sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_bed.celsius,  (int)thermalManager.temp_bed.target);
    lv_label_set_text(label_R_Bed, public_buf_l);
  #endif
}
#endif

#endif // HAS_TFT_LVGL_UI
