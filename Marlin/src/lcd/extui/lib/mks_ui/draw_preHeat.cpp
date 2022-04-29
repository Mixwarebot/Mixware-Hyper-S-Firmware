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

#include "../../../../module/temperature.h"
#include "../../../../inc/MarlinConfig.h"
#include "../../../../module/motion.h"

static lv_obj_t *scr;
extern lv_group_t*  g;
static lv_obj_t *buttonType, *buttonStep;
static lv_obj_t *labelType;
static lv_obj_t *labelStep;
#if DISABLED(MIXWARE_MODEL_V)
static lv_obj_t *tempText1;
#endif
#if ENABLED(MIXWARE_MODEL_V)
  #include "../../../../gcode/queue.h"

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
  ID_P_RETURN
  #if ENABLED(MIXWARE_MODEL_V)
    ,ID_P_PLA
    ,ID_P_ABS
    ,ID_P_PETG
    ,ID_P_PACF
    ,ID_P_BED
    ,ID_P_COOL
    ,ID_P_SKIP
  #endif
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_P_ADD:
      if (uiCfg.curTempType == 0) {
        thermalManager.temp_hotend[uiCfg.curSprayerChoose].target += uiCfg.stepHeat;
        if (uiCfg.curSprayerChoose == 0) {
          if ((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target > gCfgItems.filament_max_temper) {
            thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = gCfgItems.filament_max_temper;
          }
        }
        #if DISABLED(SINGLENOZZLE) && HAS_MULTI_EXTRUDER
          else if ((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target > (HEATER_1_MAXTEMP - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1))) {
            thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = (float)HEATER_1_MAXTEMP - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
          }
        #endif
        thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
      }
      #if HAS_HEATED_BED
        else {
          thermalManager.temp_bed.target += uiCfg.stepHeat;
          if ((int)thermalManager.temp_bed.target > 105) {
            thermalManager.temp_bed.target = 105.0f;
          }
          thermalManager.start_watching_bed();
        }
      #endif
      disp_desire_temp();
      break;
    case ID_P_DEC:
      if (uiCfg.curTempType == 0) {
        if ((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target > uiCfg.stepHeat)
          thermalManager.temp_hotend[uiCfg.curSprayerChoose].target -= uiCfg.stepHeat;
        else
          thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = 0;

        thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
      }
      #if HAS_HEATED_BED
        else {
          if ((int)thermalManager.temp_bed.target > uiCfg.stepHeat)
            thermalManager.temp_bed.target -= uiCfg.stepHeat;
          else
            thermalManager.temp_bed.target = 0;

          thermalManager.start_watching_bed();
        }
      #endif
      disp_desire_temp();
      break;
    case ID_P_TYPE:
      if (uiCfg.curTempType == 0) {
        if (ENABLED(HAS_MULTI_EXTRUDER)) {
          #if DISABLED(SINGLENOZZLE)
            if (uiCfg.curSprayerChoose == 0) {
              uiCfg.curSprayerChoose = 1;
            }
            else if (uiCfg.curSprayerChoose == 1) {
              if (TEMP_SENSOR_BED != 0) {
                uiCfg.curTempType = 1;
              }
              else {
                uiCfg.curTempType      = 0;
                uiCfg.curSprayerChoose = 0;
              }
            }
          #else
            if (TEMP_SENSOR_BED != 0) {
              uiCfg.curTempType = 1;
            }
            else {
              uiCfg.curTempType = 0;
            }
          #endif
        }
        else if (uiCfg.curSprayerChoose == 0) {
          if (TEMP_SENSOR_BED != 0)
            uiCfg.curTempType = 1;
          else
            uiCfg.curTempType = 0;
        }
      }
      else if (uiCfg.curTempType == 1) {
        uiCfg.curSprayerChoose = 0;
        uiCfg.curTempType      = 0;
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
      if (uiCfg.curTempType == 0) {
        thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = 0;
        thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
      }
      #if HAS_HEATED_BED
        else {
          thermalManager.temp_bed.target = 0;
          thermalManager.start_watching_bed();
        }
      #endif
      disp_desire_temp();
      break;
    case ID_P_RETURN:
      lv_clear_cur_ui();
      lv_draw_return_ui();
      break;
    #if ENABLED(MIXWARE_MODEL_V)
      case ID_P_PLA:
        queue.inject_P(PSTR("M140 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND)));
        break;
      case ID_P_ABS:
        queue.inject_P(PSTR("M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND)));
        break;
      case ID_P_PETG:
        queue.inject_P(PSTR("M140 S" STRINGIFY(PREHEAT_3_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_3_TEMP_HOTEND)));
        break;
      case ID_P_PACF:
        queue.inject_P(PSTR("M140 S" STRINGIFY(PREHEAT_4_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_4_TEMP_HOTEND)));
        break;
      case ID_P_BED:
        queue.inject_P(PSTR("M140 S" STRINGIFY(PREHEAT_1_TEMP_BED)));
        break;
      case ID_P_COOL:
        queue.inject_P(PSTR("M140 S0\nM104 S0"));
        break;
      case ID_P_SKIP:
        if (uiCfg.para_ui_page) uiCfg.para_ui_page = 0;
        else uiCfg.para_ui_page = 1;
        lv_clear_preHeat();
        lv_draw_preHeat();
        break;
    #endif
  }
}

void lv_draw_preHeat(void) {
  scr = lv_screen_create(PRE_HEAT_UI);

  // Create image buttons
  #if ENABLED(MIXWARE_MODEL_V)
    if (uiCfg.para_ui_page != 1) {
      buttonType = lv_imgbtn_create(scr, nullptr, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_P_TYPE);
      buttonStep = lv_imgbtn_create(scr, nullptr, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_P_STEP);
      lv_big_button_create(scr, "F:/img_add.bin", preheat_menu.add, button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_P_ADD);
      lv_big_button_create(scr, "F:/img_dec.bin", preheat_menu.dec, button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_P_DEC);

      // Create labels on the image buttons
      labelType = lv_label_create_empty(buttonType);
      labelStep = lv_label_create_empty(buttonStep);
      lv_obj_t *l_tips_type = lv_label_create(buttonType, machine_menu.ButtonTips);
      lv_obj_t *l_tips_step = lv_label_create(buttonStep, machine_menu.ButtonTips);

      if (uiCfg.print_state == IDLE) {
        lv_screen_menu_item_turn_page(scr, preheat_menu.title, event_handler, ID_P_SKIP);
      }

      disp_temp_type();
      disp_step_heat();
      lv_obj_align(l_tips_type, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
      lv_obj_align(l_tips_step, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
    }
    else {
      if (gCfgItems.filament_max_temper < 300) {
        lv_big_button_create(scr, "F:/img_preheat_PLA.bin",  preheat_menu.heatPLA, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_P_PLA);
        lv_big_button_create(scr, "F:/img_preheat_PETG.bin",  preheat_menu.heatPETG, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_P_PETG);
      }
      else {
        lv_big_button_create(scr, "F:/img_preheat_PETG.bin",  preheat_menu.heatPETG, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_P_PETG);
        lv_big_button_create(scr, "F:/img_preheat_PACF.bin",  preheat_menu.heatPACF, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_P_PACF);
      }

      lv_big_button_create(scr, "F:/img_preheat_Bed.bin",  preheat_menu.hotbed,  button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_P_BED);
      lv_big_button_create(scr, "F:/img_preheat_Cool.bin", preheat_menu.off,     button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_P_COOL);
      lv_screen_menu_item_turn_page(scr, preheat_menu.adjust_title, event_handler, ID_P_SKIP);
    }

    lv_screen_menu_item_return(scr, event_handler, ID_P_RETURN);

    lv_obj_t *buttonExt = lv_img_create(scr, nullptr);
    lv_img_set_src(buttonExt, MIXWARE_UI_SELECT("F:/bmp_ext_state.bin", "F:/HI_ext_state.bin"));
    lv_obj_set_pos(buttonExt, 30, 82);
    labelExt = lv_label_create(scr, 75, 92, nullptr);

    lv_obj_t *buttonBedstate = lv_img_create(scr, nullptr);
    lv_img_set_src(buttonBedstate, "F:/bmp_bed_state.bin");
    lv_obj_set_pos(buttonBedstate, 175, 82);
    #if HAS_HEATED_BED
      labelBed = lv_label_create(scr, 220, 92, nullptr);
    #endif

    disp_desire_temp();
  #else
    lv_big_button_create(scr, "F:/bmp_Add.bin", preheat_menu.add, INTERVAL_V, titleHeight, event_handler, ID_P_ADD);
    lv_big_button_create(scr, "F:/bmp_Dec.bin", preheat_menu.dec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_P_DEC);

    buttonType = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_TYPE);
    buttonStep = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_STEP);
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

    #if ENABLED(SINGLENOZZLE)
      uiCfg.curSprayerChoose = 0;
    #else
      uiCfg.curSprayerChoose = active_extruder;
    #endif

    disp_temp_type();
    disp_step_heat();

    tempText1 = lv_label_create_empty(scr);
    lv_obj_set_style(tempText1, &tft_style_label_rel);
    disp_desire_temp();
  #endif
}

void disp_temp_type() {
  if (uiCfg.curTempType == 0) {
    #if DISABLED(SINGLENOZZLE)
      if (uiCfg.curSprayerChoose == 1) {
        lv_imgbtn_set_src_both(buttonType, TERN(MIXWARE_MODEL_V, MIXWARE_UI_SELECT("F:/img_extruct.bin", "F:/HI_extruct.bin"), "F:/bmp_extru2.bin"));
        if (gCfgItems.multiple_language) {
          lv_label_set_text(labelType, preheat_menu.ext2);
          lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
        }
      }
      else {
        lv_imgbtn_set_src_both(buttonType, TERN(MIXWARE_MODEL_V, MIXWARE_UI_SELECT("F:/img_extruct.bin", "F:/HI_extruct.bin"), "F:/bmp_extru1.bin"));
        if (gCfgItems.multiple_language) {
          lv_label_set_text(labelType, preheat_menu.ext1);
          lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
        }
      }
    #else
      lv_imgbtn_set_src_both(buttonType, TERN(MIXWARE_MODEL_V, (gCfgItems.filament_max_temper < 300 ? "F:/img_extruct.bin" : "F:/HI_extruct.bin"), "F:/bmp_extru1.bin"));
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelType, preheat_menu.ext1);
        lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
    #endif
  }
  else {
    lv_imgbtn_set_src_both(buttonType, TERN(MIXWARE_MODEL_V, "F:/img_bed.bin", "F:/bmp_bed.bin"));
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, preheat_menu.hotbed);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
  }
}

void disp_desire_temp() {
  #if ENABLED(MIXWARE_MODEL_V)
    sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_hotend[0].celsius,  (int)thermalManager.temp_hotend[0].target);
    lv_label_set_text(labelExt, public_buf_l);
    #if HAS_HEATED_BED
      sprintf(public_buf_l, preheat_menu.value_state, (int)thermalManager.temp_bed.celsius,  (int)thermalManager.temp_bed.target);
      lv_label_set_text(labelBed, public_buf_l);
    #endif
  #else
    char buf[20] = {0};

    public_buf_l[0] = '\0';
    if (uiCfg.curTempType == 0) {
      #if DISABLED(SINGLENOZZLE)
        strcat(public_buf_l, uiCfg.curSprayerChoose < 1 ? preheat_menu.ext1 : preheat_menu.ext2);
        sprintf(buf, preheat_menu.value_state, (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius,  (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target);
      #else
        strcat(public_buf_l, preheat_menu.ext1);
        sprintf(buf, preheat_menu.value_state, (int)thermalManager.temp_hotend[0].celsius,  (int)thermalManager.temp_hotend[0].target);
      #endif
    }
    #if HAS_HEATED_BED
      else {
        strcat(public_buf_l, preheat_menu.hotbed);
        sprintf(buf, preheat_menu.value_state, (int)thermalManager.temp_bed.celsius,  (int)thermalManager.temp_bed.target);
      }
    #endif
    strcat_P(public_buf_l, PSTR(": "));
    strcat(public_buf_l, buf);
    lv_label_set_text(tempText1, public_buf_l);
    lv_obj_align(tempText1, nullptr, LV_ALIGN_CENTER, 0, -50);
  #endif
}

void disp_step_heat() {
  if (uiCfg.stepHeat == 1) {
    lv_imgbtn_set_src_both(buttonStep, TERN(MIXWARE_MODEL_V, "F:/img_step_degree_1.bin", "F:/bmp_step1_degree.bin"));
  }
  else if (uiCfg.stepHeat == 5) {
    lv_imgbtn_set_src_both(buttonStep, TERN(MIXWARE_MODEL_V, "F:/img_step_degree_5.bin", "F:/bmp_step5_degree.bin"));
  }
  else if (uiCfg.stepHeat == 10) {
    lv_imgbtn_set_src_both(buttonStep, TERN(MIXWARE_MODEL_V, "F:/img_step_degree_10.bin", "F:/bmp_step10_degree.bin"));
  }

  if (gCfgItems.multiple_language) {
    if (uiCfg.stepHeat == 1) {
      lv_label_set_text(labelStep, preheat_menu.step_1c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if (uiCfg.stepHeat == 5) {
      lv_label_set_text(labelStep, preheat_menu.step_5c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
    else if (uiCfg.stepHeat == 10) {
      lv_label_set_text(labelStep, preheat_menu.step_10c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
    }
  }
}

void lv_clear_preHeat() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
