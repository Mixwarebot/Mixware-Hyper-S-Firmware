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

#include "../../../module/temperature.h"
#include "../../../inc/MarlinConfig.h"

#ifndef USE_NEW_LVGL_CONF
  static lv_obj_t *scr;
#endif

extern lv_group_t*  g;
static lv_obj_t *buttonType, *buttonStep, *buttonAdd, *buttonDec;
static lv_obj_t *labelType;
static lv_obj_t *labelStep;
#if DISABLED(TFT_MIXWARE_LVGL_UI)
  static lv_obj_t *tempText1;
  static lv_obj_t *btn_pla;
  static lv_obj_t *btn_abs;
  static lv_obj_t *label_abs;
  static lv_obj_t *label_pla;

  static lv_style_t btn_style_pre;
  static lv_style_t btn_style_rel;
#else
  #include "../../../gcode/queue.h"
  #include "../../../module/motion.h"

  static lv_obj_t *labelExt;
  #if HAS_HEATED_BED
    static lv_obj_t *labelBed;
  #endif
#endif

enum {
  ID_P_ADD = 1,
  ID_P_DEC,
  ID_P_TYPE,
  ID_P_STEP,
  ID_P_OFF,
  ID_P_RETURN,
  ID_P_ABS,
  ID_P_PLA
  #if ENABLED(TFT_MIXWARE_LVGL_UI)
    , ID_P_PETG
    , ID_P_PACF
    , ID_P_BED
    , ID_P_PAGE_SW
  #endif
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_P_ADD: {
      if (uiCfg.curTempType == 0) {

        // #if ANY(WATCH_TEMP_INCREASE, WATCH_BED_TEMP_INCREASE)
        int16_t max_target = HEATER_0_MAXTEMP;
        // #endif

        thermalManager.temp_hotend[uiCfg.extruderIndex].target += uiCfg.stepHeat;
#ifdef WATCH_TEMP_INCREASE
        if (uiCfg.extruderIndex == 0){
            max_target = TERN(TFT_MIXWARE_LVGL_UI, MUI.get_heating_mode_temperature(), HEATER_0_MAXTEMP) - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);

        }
        else {
          #if HAS_MULTI_HOTEND
            max_target = HEATER_1_MAXTEMP - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
          #endif
        }
#endif
        if (thermalManager.degTargetHotend(uiCfg.extruderIndex) > max_target)
          thermalManager.setTargetHotend(max_target, uiCfg.extruderIndex);
        thermalManager.start_watching_hotend(uiCfg.extruderIndex);

      }
      else {
        #if HAS_HEATED_BED
          thermalManager.temp_bed.target += uiCfg.stepHeat;
          #ifdef WATCH_BED_TEMP_INCREASE
          constexpr int16_t max_target = BED_MAXTEMP - (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1);
          if (thermalManager.degTargetBed() > max_target)
            thermalManager.setTargetBed(max_target);
          thermalManager.start_watching_bed();
          #endif
        #endif
      }
      disp_desire_temp();
    } break;

    case ID_P_DEC:
      if (uiCfg.curTempType == 0) {
        if (thermalManager.degTargetHotend(uiCfg.extruderIndex) > uiCfg.stepHeat)
          thermalManager.temp_hotend[uiCfg.extruderIndex].target -= uiCfg.stepHeat;
        else
          thermalManager.setTargetHotend(0, uiCfg.extruderIndex);
        thermalManager.start_watching_hotend(uiCfg.extruderIndex);
      }
      else {
        #if HAS_HEATED_BED
          if (thermalManager.degTargetBed() > uiCfg.stepHeat)
            thermalManager.temp_bed.target -= uiCfg.stepHeat;
          else
            thermalManager.setTargetBed(0);

          thermalManager.start_watching_bed();
        #endif
      }
      disp_desire_temp();
      break;
    case ID_P_TYPE:
      if (uiCfg.curTempType == 0) {
        if (ENABLED(HAS_MULTI_EXTRUDER) && (DISABLED(SINGLENOZZLE))) {
          if (uiCfg.extruderIndex == 0) {
            uiCfg.extruderIndex = 1;
          }
          else if (uiCfg.extruderIndex == 1) {
            if (ENABLED(HAS_HEATED_BED)) {
              uiCfg.curTempType = 1;
            }
            else {
              uiCfg.curTempType = 0;
              uiCfg.extruderIndex = 0;
            }
          }
        }
        else if (uiCfg.extruderIndex == 0) {
          uiCfg.curTempType = TERN(HAS_HEATED_BED, 1, 0);
        }
      }
      else if (uiCfg.curTempType == 1) {
        uiCfg.extruderIndex = 0;
        uiCfg.curTempType = 0;
      }
      disp_temp_type();
      break;
    case ID_P_STEP:
      switch (uiCfg.stepHeat) {
        case  1: uiCfg.stepHeat =  5; break;
        case  5: uiCfg.stepHeat = 10; break;
        case 10: uiCfg.stepHeat =  1; break;
        default: break;
      }
      disp_step_heat();
      break;
    case ID_P_OFF:
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        if (uiCfg.curTempType == 0) {
          thermalManager.setTargetHotend(0, uiCfg.extruderIndex);
          thermalManager.start_watching_hotend(uiCfg.extruderIndex);
        }
        else {
          #if HAS_HEATED_BED
            thermalManager.temp_bed.target = 0;
            thermalManager.start_watching_bed();
          #endif
        }
      #else
        thermalManager.setTargetHotend(0, uiCfg.extruderIndex);
        thermalManager.start_watching_hotend(uiCfg.extruderIndex);
        thermalManager.temp_bed.target = 0;
        thermalManager.start_watching_bed();
      #endif
      disp_desire_temp();
      break;
    case ID_P_RETURN:
      clear_cur_ui();
      draw_return_ui();
      break;
    #if DISABLED(TFT_MIXWARE_LVGL_UI)
      case ID_P_ABS:
        if (uiCfg.curTempType == 0) {
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(PREHEAT_2_TEMP_HOTEND, 0));
        }
        else if (uiCfg.curTempType == 1) {
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(PREHEAT_2_TEMP_BED));
        }
        break;
      case ID_P_PLA:
        if (uiCfg.curTempType == 0) {
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND, 0));
        }
        else if (uiCfg.curTempType == 1) {
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(PREHEAT_1_TEMP_BED));
        }
        break;
    #else
      case ID_P_ABS:
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(PREHEAT_2_TEMP_HOTEND, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(PREHEAT_2_TEMP_BED));
        break;
      case ID_P_PLA:
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(PREHEAT_1_TEMP_BED));
        break;
      case ID_P_PETG:
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(PREHEAT_3_TEMP_HOTEND, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(PREHEAT_3_TEMP_BED));
        break;
      case ID_P_PACF:
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(PREHEAT_4_TEMP_HOTEND, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(PREHEAT_4_TEMP_BED));
        break;
      case ID_P_BED:
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(PREHEAT_1_TEMP_BED));
        break;
      case ID_P_PAGE_SW:
        uiCfg.para_ui_page ^= true;
        lv_clear_preHeat();
        lv_draw_preHeat();
        break;
    #endif
  }
}

void disp_add_dec() {
  // Create image buttons
#ifndef USE_NEW_LVGL_CONF
  buttonAdd = lv_big_button_create(scr, "F:/bmp_Add.bin", preheat_menu.add, INTERVAL_V, titleHeight, event_handler, ID_P_ADD);
  buttonDec = lv_big_button_create(scr, "F:/bmp_Dec.bin", preheat_menu.dec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_P_DEC);
#else
  buttonAdd = lv_big_button_create(mks_ui.src_main, "F:/bmp_Add.bin", preheat_menu.add, INTERVAL_V, titleHeight, event_handler, ID_P_ADD);
  buttonDec = lv_big_button_create(mks_ui.src_main, "F:/bmp_Dec.bin", preheat_menu.dec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_P_DEC);
#endif
}

void lv_draw_preHeat() {
  scr = lv_screen_create(PRE_HEAT_UI);

  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    // Create image buttons
    disp_add_dec();

    buttonType = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_TYPE);
    buttonStep = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_STEP);

    if (uiCfg.curTempType == 0) disp_ext_heart();
    if (uiCfg.curTempType == 1) disp_ext_heart();

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonType);
        lv_group_add_obj(g, buttonStep);
      }
    #endif

    lv_big_button_create(scr, "F:/bmp_speed0.bin", preheat_menu.off, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_OFF);
    lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_RETURN);

    // Create labels on the image buttons
    labelType = lv_label_create_empty(buttonType);
    labelStep = lv_label_create_empty(buttonStep);

    disp_temp_type();
    disp_step_heat();

    tempText1 = lv_label_create_empty(scr);
    lv_obj_set_style(tempText1, &tft_style_label_rel);
    disp_desire_temp();
  #else
    if (!uiCfg.para_ui_page) {
      buttonType = lv_imgbtn_create(scr, nullptr, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_P_TYPE);
      buttonStep = lv_imgbtn_create(scr, nullptr, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), event_handler, ID_P_STEP);
      lv_big_button_create(scr, MIMG.add, MTR.add, IMAGEBTN_P_X(4), IMAGEBTN_P_Y(4), event_handler, ID_P_ADD);
      lv_big_button_create(scr, MIMG.dec, MTR.dec, IMAGEBTN_P_X(5), IMAGEBTN_P_Y(5), event_handler, ID_P_DEC);
      if (uiCfg.print_state == IDLE) {
        MUI.page_bottom_button_middle(scr, MTR.preheat, event_handler, ID_P_PAGE_SW);
      }

      // Create labels on the image buttons
      labelType = lv_label_create_empty(buttonType);
      labelStep = lv_label_create_empty(buttonStep);

      disp_temp_type();
      disp_step_heat();

      MUI.page_button_add_tips(buttonType);
      MUI.page_button_add_tips(buttonStep);
    }
    else {
      if (MUI.get_heating_mode()) {
        lv_big_button_create(scr, MIMG.preheatPLA,  MTR.preheatPLA, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_P_PLA);
        lv_big_button_create(scr, MIMG.preheatPETG, MTR.preheatPETG, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), event_handler, ID_P_PETG);
      }
      else {
        lv_big_button_create(scr, MIMG.preheatPETG, MTR.preheatPETG, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_P_PETG);
        lv_big_button_create(scr, MIMG.preheatPACF, MTR.preheatPACF, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), event_handler, ID_P_PACF);
      }

      lv_big_button_create(scr, MIMG.preheatBed,  MTR.bed,  IMAGEBTN_P_X(4), IMAGEBTN_P_Y(4), event_handler, ID_P_BED);
      lv_big_button_create(scr, MIMG.preheatCool, MTR.Cool,     IMAGEBTN_P_X(5), IMAGEBTN_P_Y(5), event_handler, ID_P_OFF);
      MUI.page_bottom_button_middle(scr, MTR.temp, event_handler, ID_P_PAGE_SW);
    }

    MUI.page_button_return(scr, event_handler, ID_P_RETURN);

    #if HAS_HEATED_BED
      lv_obj_t *buttonExt = lv_img_create(scr, nullptr);
      lv_img_set_src(buttonExt, MIMG_HM(state_extruct));
      lv_obj_set_pos(buttonExt, 30, 82);
      labelExt = lv_label_create(scr, 75, 92, nullptr);

      lv_obj_t *buttonBedstate = lv_img_create(scr, nullptr);
      lv_img_set_src(buttonBedstate, MIMG.stateBed);
      lv_obj_set_pos(buttonBedstate, 175, 82);
      labelBed = lv_label_create(scr, 220, 92, nullptr);
    #else
      lv_obj_t *buttonExt = lv_img_create(scr, nullptr);
      lv_img_set_src(buttonExt, MIMG_HM(state_extruct));
      lv_obj_set_pos(buttonExt, 103, 82);
      labelExt = lv_label_create(scr, 148, 92, nullptr);
    #endif

    disp_desire_temp();
  #endif
}

void disp_ext_heart() {
  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    btn_abs = lv_btn_create(scr, 160, 40, 80, 40, event_handler, ID_P_ABS);
    btn_pla = lv_btn_create(scr, 260, 40, 80, 40, event_handler, ID_P_PLA);

    lv_btn_set_style(btn_abs, LV_BTN_STYLE_PR, &btn_style_pre);
    lv_btn_set_style(btn_abs, LV_BTN_STYLE_REL, &btn_style_rel);
    lv_btn_set_style(btn_pla, LV_BTN_STYLE_PR, &btn_style_pre);
    lv_btn_set_style(btn_pla, LV_BTN_STYLE_REL, &btn_style_rel);

    label_abs = lv_label_create(btn_abs, PREHEAT_2_LABEL);
    label_pla = lv_label_create(btn_pla, PREHEAT_1_LABEL);
  #endif
}

void disp_temp_type() {
  if (uiCfg.curTempType == 0) {

    if (uiCfg.extruderIndex == 1 && (DISABLED(SINGLENOZZLE))) {
      lv_imgbtn_set_src_both(buttonType, "F:/bmp_extru2.bin");
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelType, preheat_menu.ext2);
        lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
    }
    else if (ENABLED(HAS_HOTEND)) {
      lv_imgbtn_set_src_both(buttonType, TERN(TFT_MIXWARE_LVGL_UI, MIMG_HM(extruct), "F:/bmp_extru1.bin"));
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelType, TERN(TFT_MIXWARE_LVGL_UI, MTR.nozzle, preheat_menu.ext1));
        lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, TERN(TFT_MIXWARE_LVGL_UI, BUTTON_TEXT_Y_OFFSET-10, BUTTON_TEXT_Y_OFFSET));
      }
    }
  }
  else if (ENABLED(HAS_HEATED_BED)) {
    lv_imgbtn_set_src_both(buttonType, TERN(TFT_MIXWARE_LVGL_UI, MIMG.bed, "F:/bmp_bed.bin"));
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, preheat_menu.hotbed);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, TERN(TFT_MIXWARE_LVGL_UI, BUTTON_TEXT_Y_OFFSET-10, BUTTON_TEXT_Y_OFFSET));
    }
  }
}

void disp_desire_temp() {
  #if DISABLED(TFT_MIXWARE_LVGL_UI)
    char buf[20] = { 0 };
    public_buf_l[0] = '\0';

    if (uiCfg.curTempType == 0) {
      #if HAS_HOTEND
        strcat(public_buf_l, uiCfg.extruderIndex < 1 ? preheat_menu.ext1 : preheat_menu.ext2);
        sprintf(buf, preheat_menu.value_state, thermalManager.wholeDegHotend(uiCfg.extruderIndex), thermalManager.degTargetHotend(uiCfg.extruderIndex));
      #endif
    }
    else {
      #if HAS_HEATED_BED
        strcat(public_buf_l, preheat_menu.hotbed);
        sprintf(buf, preheat_menu.value_state, thermalManager.wholeDegBed(), thermalManager.degTargetBed());
      #endif
    }
    strcat_P(public_buf_l, PSTR(": "));
    strcat(public_buf_l, buf);
    lv_label_set_text(tempText1, public_buf_l);
    lv_obj_align(tempText1, nullptr, LV_ALIGN_CENTER, 0, -50);
  #else
    sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_hotend[0].celsius,  (int)thermalManager.temp_hotend[0].target);
    lv_label_set_text(labelExt, public_buf_l);
    #if HAS_HEATED_BED
      sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_bed.celsius,  (int)thermalManager.temp_bed.target);
      lv_label_set_text(labelBed, public_buf_l);
    #endif
  #endif
}

void disp_step_heat() {
  if (uiCfg.stepHeat == 1) {
    lv_imgbtn_set_src_both(buttonStep, TERN(TFT_MIXWARE_LVGL_UI, MIMG.temperatureRange1, "F:/bmp_step1_degree.bin"));
  }
  else if (uiCfg.stepHeat == 5) {
    lv_imgbtn_set_src_both(buttonStep, TERN(TFT_MIXWARE_LVGL_UI, MIMG.temperatureRange5, "F:/bmp_step5_degree.bin"));
  }
  else if (uiCfg.stepHeat == 10) {
    lv_imgbtn_set_src_both(buttonStep, TERN(TFT_MIXWARE_LVGL_UI, MIMG.temperatureRange10, "F:/bmp_step10_degree.bin"));
  }

  if (gCfgItems.multiple_language) {
    if (uiCfg.stepHeat == 1) {
      lv_label_set_text(labelStep, preheat_menu.step_1c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, TERN(TFT_MIXWARE_LVGL_UI, BUTTON_TEXT_Y_OFFSET-10, BUTTON_TEXT_Y_OFFSET));
    }
    else if (uiCfg.stepHeat == 5) {
      lv_label_set_text(labelStep, preheat_menu.step_5c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, TERN(TFT_MIXWARE_LVGL_UI, BUTTON_TEXT_Y_OFFSET-10, BUTTON_TEXT_Y_OFFSET));
    }
    else if (uiCfg.stepHeat == 10) {
      lv_label_set_text(labelStep, preheat_menu.step_10c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, TERN(TFT_MIXWARE_LVGL_UI, BUTTON_TEXT_Y_OFFSET-10, BUTTON_TEXT_Y_OFFSET));
    }
  }
}

void lv_clear_preHeat() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
#ifndef USE_NEW_LVGL_CONF
  lv_obj_del(scr);
#else
  lv_obj_clean(mks_ui.src_main);
#endif
}

#endif // HAS_TFT_LVGL_UI
