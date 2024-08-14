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

#include "draw_ready_print.h"
#include "draw_tool.h"
#include <lv_conf.h>
#include "tft_lvgl_configuration.h"
#include "draw_ui.h"

#include <lvgl.h>

#include "../../../module/temperature.h"
#include "../../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  #include "../../tft_io/touch_calibration.h"
  #include "draw_touch_calibration.h"
#endif

#include "mks_hardware.h"
#include <stdio.h>
#include "../../../sd/cardreader.h"

// #define ICON_POS_Y          260
// #define TARGET_LABEL_MOD_Y -36
// #define LABEL_MOD_Y         30

extern lv_group_t*  g;
static lv_obj_t *scr;
// static lv_obj_t *buttonExt1, *labelExt1, *buttonFanstate, *labelFan;
  static lv_obj_t *label_R_Ext, *buttonExt;
  #if HAS_HEATED_BED
    static lv_obj_t *label_R_Bed, *buttonBedstate;
  #endif

  static lv_style_t style_para;

// #if HAS_MULTI_HOTEND
//   static lv_obj_t *labelExt2;
//   static lv_obj_t *buttonExt2;
// #endif

// #if HAS_HEATED_BED
//   static lv_obj_t* labelBed;
//   static lv_obj_t* buttonBedstate;
// #endif

#if ENABLED(MKS_TEST)
  uint8_t current_disp_ui = 0;
#endif

enum { ID_TOOL = 1, ID_SET, ID_PRINT, ID_INFO_EXT, ID_INFO_BED, ID_INFO_FAN };

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_ready_print();
  if (uiCfg.para_ui_page) uiCfg.para_ui_page = false;
  switch (obj->mks_obj_id) {
    case ID_TOOL:   lv_draw_tool(); break;
    case ID_SET:    lv_draw_set(); break;
    case ID_INFO_EXT:  uiCfg.curTempType = 0; lv_draw_preHeat(); break;
    case ID_INFO_BED:  uiCfg.curTempType = 1; lv_draw_preHeat(); break;
    case ID_INFO_FAN:  lv_draw_fan(); break;
    case ID_PRINT:
        card.mount();

        if (card.isMounted())
          lv_draw_print_file();
        else
          lv_draw_dialog(DIALOG_TYPE_REPRINT_NO_FILE);
      break;
  }
}

// lv_obj_t *limit_info, *det_info;
// lv_style_t limit_style, det_style;
void disp_Limit_ok() {
  // limit_style.text.color.full = 0xFFFF;
  // lv_obj_set_style(limit_info, &limit_style);
  // lv_label_set_text(limit_info, "Limit:ok");
}
void disp_Limit_error() {
  // limit_style.text.color.full = 0xF800;
  // lv_obj_set_style(limit_info, &limit_style);
  // lv_label_set_text(limit_info, "Limit:error");
}

void disp_det_ok() {
  // det_style.text.color.full = 0xFFFF;
  // lv_obj_set_style(det_info, &det_style);
  // lv_label_set_text(det_info, "det:ok");
}

void disp_det_error() {
  // det_style.text.color.full = 0xF800;
  // lv_obj_set_style(det_info, &det_style);
  // lv_label_set_text(det_info, "det:error");
}

// lv_obj_t *e1, *e2, *e3, *bed;
void mks_disp_test() {
  // char buf[30] = {0};
  // #if HAS_HOTEND
  //   sprintf_P(buf, PSTR("e1:%d"), thermalManager.wholeDegHotend(0));
  //   lv_label_set_text(e1, buf);
  // #endif
  // #if HAS_MULTI_HOTEND
  //   sprintf_P(buf, PSTR("e2:%d"), thermalManager.wholeDegHotend(1));
  //   lv_label_set_text(e2, buf);
  // #endif
  // #if HAS_HEATED_BED
  //   sprintf_P(buf, PSTR("bed:%d"), thermalManager.wholeDegBed());
  //   lv_label_set_text(bed, buf);
  // #endif
}

void lv_draw_ready_print() {
  // char buf[30] = {0};
  // lv_obj_t *buttonTool;

  disp_state_stack._disp_index = 0;
  ZERO(disp_state_stack._disp_state);
  scr = lv_screen_create(PRINT_READY_UI, "");
  #if ENABLED(TFT_MIXWARE_UI)

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

    page_logo_main(scr);

    #if HAS_HEATED_BED
      buttonExt = lv_big_button_create(scr, get_heating_mode() ? "F:/bmp_ext_state.bin" : "F:/HI_ext_state.bin", " ", 30, 92, event_handler, ID_INFO_EXT);
      buttonBedstate = lv_big_button_create(scr, "F:/bmp_bed_state.bin", " ", 165, 92, event_handler, ID_INFO_BED);
      label_R_Ext = lv_label_create_empty(scr);
      label_R_Bed = lv_label_create_empty(scr);
    #else
      lv_obj_t *buttonExt = lv_img_create(scr, nullptr);
      lv_img_set_src(buttonExt, MIXWARE_UI_SELECT("F:/bmp_ext_state.bin", "F:/HI_ext_state.bin"));
      lv_obj_set_pos(buttonExt, 103, 93);
      label_R_Ext = lv_label_create(scr, 148, 103, nullptr);
    #endif

    lv_big_button_create(scr, "F:/img_tool.bin",     main_menu.tool,  28,  340, event_handler, ID_TOOL);
    lv_big_button_create(scr, "F:/img_set.bin",      main_menu.set,   173, 340, event_handler, ID_SET);
    lv_big_button_create(scr, get_heating_mode() ? "F:/img_printing.bin" : "F:/HI_printing.bin", main_menu.print, 101, 198, event_handler, ID_PRINT);

    lv_temp_refr();
  #else
  if (mks_test_flag == 0x1E) {
    // Create image buttons
    buttonTool = lv_imgbtn_create(scr, "F:/bmp_tool.bin", event_handler, ID_TOOL);

    lv_obj_set_pos(buttonTool, 360, 180);

    lv_obj_t *label_tool = lv_label_create_empty(buttonTool);
    if (gCfgItems.multiple_language) {
      lv_label_set_text(label_tool, main_menu.tool);
      lv_obj_align(label_tool, buttonTool, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }

    #if HAS_HOTEND
      e1 = lv_label_create_empty(scr);
      lv_obj_set_pos(e1, 20, 20);
      sprintf_P(buf, PSTR("e1:  %d"), thermalManager.wholeDegHotend(0));
      lv_label_set_text(e1, buf);
    #endif
    #if HAS_MULTI_HOTEND
      e2 = lv_label_create_empty(scr);
      lv_obj_set_pos(e2, 20, 45);
      sprintf_P(buf, PSTR("e2:  %d"), thermalManager.wholeDegHotend(1));
      lv_label_set_text(e2, buf);
    #endif
    #if HAS_HEATED_BED
      bed = lv_label_create_empty(scr);
      lv_obj_set_pos(bed, 20, 95);
      sprintf_P(buf, PSTR("bed:  %d"), thermalManager.wholeDegBed());
      lv_label_set_text(bed, buf);
    #endif

    limit_info = lv_label_create_empty(scr);

    lv_style_copy(&limit_style, &lv_style_scr);
    limit_style.body.main_color.full = 0x0000;
    limit_style.body.grad_color.full = 0x0000;
    limit_style.text.color.full      = 0xFFFF;
    lv_obj_set_style(limit_info, &limit_style);

    lv_obj_set_pos(limit_info, 20, 120);
    lv_label_set_text(limit_info, " ");

    det_info = lv_label_create_empty(scr);

    lv_style_copy(&det_style, &lv_style_scr);
    det_style.body.main_color.full = 0x0000;
    det_style.body.grad_color.full = 0x0000;
    det_style.text.color.full      = 0xFFFF;
    lv_obj_set_style(det_info, &det_style);

    lv_obj_set_pos(det_info, 20, 145);
    lv_label_set_text(det_info, " ");
  }
  else {
    lv_big_button_create(scr, "F:/bmp_tool.bin", main_menu.tool, 20, 90, event_handler, ID_TOOL);
    lv_big_button_create(scr, "F:/bmp_set.bin", main_menu.set, 180, 90, event_handler, ID_SET);
    lv_big_button_create(scr, "F:/bmp_printing.bin", main_menu.print, 340, 90, event_handler, ID_PRINT);

    // Monitoring
    #if HAS_HOTEND
      buttonExt1 = lv_big_button_create(scr, "F:/bmp_ext1_state.bin", " ", 20, ICON_POS_Y, event_handler, ID_INFO_EXT);
    #endif
    #if HAS_MULTI_HOTEND
      buttonExt2 = lv_big_button_create(scr, "F:/bmp_ext2_state.bin", " ", 180, ICON_POS_Y, event_handler, ID_INFO_EXT);
    #endif
    #if HAS_HEATED_BED
      buttonBedstate = lv_big_button_create(scr, "F:/bmp_bed_state.bin", " ", TERN(HAS_MULTI_HOTEND, 340, 210), ICON_POS_Y, event_handler, ID_INFO_BED);
    #endif

    TERN_(HAS_HOTEND, labelExt1 = lv_label_create_empty(scr));
    TERN_(HAS_MULTI_HOTEND, labelExt2 = lv_label_create_empty(scr));
    TERN_(HAS_HEATED_BED, labelBed = lv_label_create_empty(scr));
    TERN_(HAS_FAN, labelFan = lv_label_create_empty(scr));

    lv_temp_refr();
  }
  #endif

  #if ENABLED(TOUCH_SCREEN_CALIBRATION)
    // If calibration is required, let's trigger it now, handles the case when there is default value in configuration files
    if (!touch_calibration.calibration_loaded()) {
      lv_clear_ready_print();
      lv_draw_touch_calibration_screen();
    }
  #endif
}

void lv_temp_refr() {
#if ENABLED(TFT_MIXWARE_UI)
  sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_hotend[0].celsius,  (int)thermalManager.temp_hotend[0].target);
  lv_label_set_text(label_R_Ext, public_buf_l);
  lv_obj_align(label_R_Ext, buttonExt, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

  #if HAS_HEATED_BED
    sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_bed.celsius,  (int)thermalManager.temp_bed.target);
    lv_label_set_text(label_R_Bed, public_buf_l);
    lv_obj_align(label_R_Bed, buttonBedstate, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  #endif
  #else
  #if HAS_HOTEND
    sprintf(public_buf_l, printing_menu.temp1, thermalManager.wholeDegHotend(0), thermalManager.degTargetHotend(0));
    lv_label_set_text(labelExt1, public_buf_l);
    lv_obj_align(labelExt1, buttonExt1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  #endif
  #if HAS_MULTI_HOTEND
    sprintf(public_buf_l, printing_menu.temp1, thermalManager.wholeDegHotend(1), thermalManager.degTargetHotend(1));
    lv_label_set_text(labelExt2, public_buf_l);
    lv_obj_align(labelExt2, buttonExt2, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  #endif
  #if HAS_HEATED_BED
    sprintf(public_buf_l, printing_menu.bed_temp, thermalManager.wholeDegBed(), thermalManager.degTargetBed());
    lv_label_set_text(labelBed, public_buf_l);
    lv_obj_align(labelBed, buttonBedstate, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  #endif
  #if HAS_FAN
    sprintf_P(public_buf_l, PSTR("%d%%"), (int)thermalManager.fanSpeedPercent(0));
    lv_label_set_text(labelFan, public_buf_l);
    lv_obj_align(labelFan, buttonFanstate, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  #endif
#endif
}

void lv_clear_ready_print() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
