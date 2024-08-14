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
#include "draw_set.h"
#include "draw_ui.h"
#include <lv_conf.h>

#include "pic_manager.h"

#include "../../../gcode/queue.h"
#include "../../../inc/MarlinConfig.h"

#if HAS_SUICIDE
  #include "../../../MarlinCore.h"
#endif

#include "../../../module/motion.h"
#include "../../../module/probe.h"
#if ENABLED(EEPROM_SETTINGS)
  #include "../../../module/settings.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

static lv_obj_t *buttonFilamentDet, *labelFilamentDet;
static lv_obj_t *buttonThermal, *labelThermal;
static lv_obj_t *buttonPowerLoss, *labelPowerLoss;
static lv_obj_t *buttonHeatMode, *labelHeatMode;

static lv_obj_t *scr;
extern lv_group_t*  g;

enum {
  // ID_S_WIFI = 1,
  // ID_S_FAN,
  // ID_S_ABOUT,
  // ID_S_CONTINUE,
  // ID_S_MOTOR_OFF,
  #if 1
    ID_SET_ABOUT = 1,
    ID_SET_EEPROM,
    ID_SET_CLOG_DET,
    ID_SET_HEAT_MODE,
    ID_SET_LANGUAGE,
    ID_SET_DEBUG_MACHINE,
    ID_SET_DEBUG_Z,
    ID_SET_MACHINE_PARA,
    ID_SET_POWER_LOSS,
    ID_SET_THERMAL_PRO,
    ID_SET_PAGE_CHANGE,
    ID_SET_RETURN
  #else
  ID_S_ABOUT = 1,
  ID_S_LANGUAGE,
  ID_S_MACHINE_PARA,
  ID_S_EEPROM_SET,
  ID_S_RETURN
  ,ID_S_FILAMENT_DET
  ,ID_S_DEBUG
  #endif
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  #if 1
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_SET_ABOUT:
      lv_clear_set();
      lv_draw_about();
      break;
    case ID_SET_EEPROM:
      lv_clear_set();
      lv_draw_eeprom_settings();
      break;
    case ID_SET_CLOG_DET:
      gCfgItems.filament_det_enable ^= true;
      lv_imgbtn_set_src_both(buttonFilamentDet, gCfgItems.filament_det_enable ? "F:img_run_out_on.bin" : "F:img_run_out_off.bin");
      lv_label_set_text(labelFilamentDet, gCfgItems.filament_det_enable ? operation_menu.filament_sensor_on : operation_menu.filament_sensor_off);
      lv_obj_align(labelFilamentDet, buttonFilamentDet, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      lv_obj_refresh_ext_draw_pad(labelFilamentDet);
      update_spi_flash();
      break;
    case ID_SET_HEAT_MODE:
      lv_clear_set();
      if (get_heating_mode()) {
        lv_draw_dialog(DIALOG_PIDTEMP_SW_HIGH);
      }
      else {
        lv_draw_dialog(DIALOG_PIDTEMP_SW_NORMAL);
      }
      break;
    case ID_SET_LANGUAGE:
      lv_clear_set();
      lv_draw_language();
      break;
    case ID_SET_DEBUG_MACHINE:
      lv_clear_set();
      lv_draw_debug_selfc();
      break;
    case ID_SET_DEBUG_Z:
      lv_clear_set();
      lv_draw_debug_zaxis();
      break;
    case ID_SET_MACHINE_PARA:
      lv_clear_set();
      lv_draw_advance_settings();
      break;
    case ID_SET_POWER_LOSS:
      TERN_(POWER_LOSS_RECOVERY, recovery.enable(!recovery.enabled));

      lv_label_set_text(labelPowerLoss, recovery.enabled ? set_menu.powerloss_on : set_menu.powerloss_off);
      lv_obj_align(labelPowerLoss, buttonPowerLoss, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      lv_obj_refresh_ext_draw_pad(labelPowerLoss);

      TERN_(EEPROM_SETTINGS, (void)settings.save());
      break;
    case ID_SET_THERMAL_PRO:
      toggle_thermal_protection();
      lv_label_set_text(labelThermal, get_thermal_protection() ? set_menu.thermal_on : set_menu.thermal_off);
      lv_obj_align(labelThermal, buttonThermal, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      lv_obj_refresh_ext_draw_pad(labelThermal);
      update_spi_flash();
      break;
    case ID_SET_PAGE_CHANGE:
      uiCfg.para_ui_page ^= true;
      lv_clear_set();
      lv_draw_set();
      break;
    case ID_SET_RETURN:
      lv_clear_set();
      lv_draw_ready_print();
      break;
  }
  #else
  if (event != LV_EVENT_RELEASED) return;
  // if (obj->mks_obj_id == ID_S_CONTINUE) return;
  // if (obj->mks_obj_id == ID_S_MOTOR_OFF) {
  //   TERN(HAS_SUICIDE, suicide(), queue.enqueue_now(F("M84")));
  //   return;
  // }
  if (obj->mks_obj_id != ID_S_FILAMENT_DET)
    lv_clear_set();
  switch (obj->mks_obj_id) {
    // case ID_S_FAN:
    //   lv_draw_fan();
    //   break;
    case ID_S_ABOUT:
      lv_draw_about();
      break;
    case ID_S_LANGUAGE:
      lv_draw_language();
      break;
    case ID_S_MACHINE_PARA:
      // lv_draw_machine_para();
      lv_draw_advance_settings();
      break;
    case ID_S_EEPROM_SET:
      lv_draw_eeprom_settings();
      break;
    case ID_S_RETURN:
      lv_draw_ready_print();
      break;

    #if ENABLED(MKS_WIFI_MODULE)
      case ID_S_WIFI:
        if (gCfgItems.wifi_mode_sel == STA_MODEL) {
          if (wifi_link_state == WIFI_CONNECTED) {
            last_disp_state = SET_UI;
            lv_draw_wifi();
          }
          else {
            if (uiCfg.command_send) {
              uint8_t cmd_wifi_list[] = { 0xA5, 0x07, 0x00, 0x00, 0xFC };
              raw_send_to_wifi(cmd_wifi_list, COUNT(cmd_wifi_list));
              last_disp_state = SET_UI;
              lv_draw_wifi_list();
            }
            else {
              last_disp_state = SET_UI;
              lv_draw_dialog(DIALOG_WIFI_ENABLE_TIPS);
            }
          }
        }
        else {
          last_disp_state = SET_UI;
          lv_draw_wifi();
        }
        break;
    #endif
    #if ENABLED(TFT_MIXWARE_UI)
      case ID_S_FILAMENT_DET:
        gCfgItems.filament_det_enable ^= true;
        lv_imgbtn_set_src_both(buttonFilamentDet, gCfgItems.filament_det_enable ? "F:img_run_out_on.bin" : "F:img_run_out_off.bin");
        lv_label_set_text(labelFilamentDet, gCfgItems.filament_det_enable ? operation_menu.filament_sensor_on : operation_menu.filament_sensor_off);
        lv_obj_align(labelFilamentDet, buttonFilamentDet, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
        lv_obj_refresh_ext_draw_pad(labelFilamentDet);
        update_spi_flash();
        break;
      case ID_S_DEBUG:
        lv_draw_debug_selfc();
        break;
    #endif
  }
    #endif
}

void lv_draw_set() {
  scr = lv_screen_create(SET_UI);

  #if ENABLED(TFT_MIXWARE_UI)
  #if 1
    if (!uiCfg.para_ui_page) {
      lv_big_button_create(scr, "F:/img_machine_para.bin", set_menu.machine_para, button_pixel_point[0].x, button_pixel_point[0].y, event_handler, ID_SET_MACHINE_PARA);

      buttonHeatMode = lv_imgbtn_create(scr, get_heating_mode() ? "F:/img_normal_mode.bin" : "F:/HI_high_mode.bin",
                                            button_pixel_point[1].x, button_pixel_point[1].y, event_handler, ID_SET_HEAT_MODE);
      labelHeatMode = lv_label_create_empty(buttonHeatMode);
      lv_label_set_text(labelHeatMode, get_heating_mode() ? pidtemp_sw_menu.normal : pidtemp_sw_menu.high);
      lv_obj_align(labelHeatMode, buttonHeatMode, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
      lv_obj_t *l_tips = lv_label_create(buttonHeatMode, machine_menu.ButtonTips);
      lv_obj_align(l_tips, buttonHeatMode, LV_ALIGN_IN_BOTTOM_MID, 0, 2);

      buttonFilamentDet = lv_imgbtn_create(scr, gCfgItems.filament_det_enable ? "F:img_run_out_on.bin" : "F:img_run_out_off.bin",
                                            button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_SET_CLOG_DET);
      labelFilamentDet = lv_label_create_empty(buttonFilamentDet);
      lv_label_set_text(labelFilamentDet, gCfgItems.filament_det_enable ? operation_menu.filament_sensor_on : operation_menu.filament_sensor_off);
      lv_obj_align(labelFilamentDet, buttonFilamentDet, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      // lv_big_button_create(scr, "F:/img_thermalpro.bin", "thermalpro", button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_SET_THERMAL_PRO);
      buttonThermal = lv_imgbtn_create(scr, "F:/img_thermalpro.bin",
                                            button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_SET_THERMAL_PRO);
      labelThermal = lv_label_create_empty(buttonThermal);
      lv_label_set_text(labelThermal, get_thermal_protection() ? set_menu.thermal_on : set_menu.thermal_off);
      lv_obj_align(labelThermal, buttonThermal, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_big_button_create(scr, "F:/img_eeprom.bin", set_menu.eepromSet, button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_SET_EEPROM);

      lv_big_button_create(scr, "F:/img_about.bin", set_menu.about, button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_SET_ABOUT);

      lv_screen_menu_item_turn_page(scr, machine_menu.next, event_handler, ID_SET_PAGE_CHANGE);
    }
    else {
      lv_big_button_create(scr, "F:/img_self_check.bin", debug_menu.selfc_title, button_pixel_point[0].x, button_pixel_point[0].y, event_handler, ID_SET_DEBUG_MACHINE);
      lv_big_button_create(scr, "F:/img_debug_z.bin", debug_menu.zaxis_title, button_pixel_point[1].x, button_pixel_point[1].y, event_handler, ID_SET_DEBUG_Z);
      lv_big_button_create(scr, "F:/img_language.bin", set_menu.language, button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_SET_LANGUAGE);

      buttonPowerLoss = lv_imgbtn_create(scr, "F:/img_powerloss.bin",
                                            button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_SET_POWER_LOSS);
      labelPowerLoss = lv_label_create_empty(buttonPowerLoss);
      lv_label_set_text(labelPowerLoss, recovery.enabled ? set_menu.powerloss_on : set_menu.powerloss_off);
      lv_obj_align(labelPowerLoss, buttonPowerLoss, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_screen_menu_item_turn_page(scr, machine_menu.previous, event_handler, ID_SET_PAGE_CHANGE);
    }
    lv_screen_menu_item_return(scr, event_handler, ID_SET_RETURN);
  #else
  lv_big_button_create(scr, "F:/img_eeprom.bin",       set_menu.eepromSet,    button_pixel_point[0].x, button_pixel_point[0].y, event_handler, ID_S_EEPROM_SET);
  lv_big_button_create(scr, "F:/img_machine_para.bin", set_menu.machine_para, button_pixel_point[1].x, button_pixel_point[1].y, event_handler, ID_S_MACHINE_PARA);
  lv_big_button_create(scr, "F:/img_about.bin",        set_menu.about,        button_pixel_point[2].x, button_pixel_point[2].y, event_handler, ID_S_ABOUT);
  #if HAS_LANG_SELECT_SCREEN
    lv_big_button_create(scr, "F:/img_language.bin", set_menu.language, button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_S_LANGUAGE);
  #endif

  buttonFilamentDet = lv_imgbtn_create(scr, gCfgItems.filament_det_enable ? "F:img_run_out_on.bin" : "F:img_run_out_off.bin", button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_S_FILAMENT_DET);
  labelFilamentDet = lv_label_create_empty(buttonFilamentDet);
  lv_label_set_text(labelFilamentDet, gCfgItems.filament_det_enable ? operation_menu.filament_sensor_on : operation_menu.filament_sensor_off);
  lv_obj_align(labelFilamentDet, buttonFilamentDet, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

  lv_big_button_create(scr, "F:/img_self_check.bin", debug_menu.selfc_title, button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_S_DEBUG);
  lv_screen_menu_item_return(scr, event_handler, ID_S_RETURN);
  #endif
  #else
  lv_big_button_create(scr, "F:/bmp_eeprom_settings.bin", set_menu.eepromSet, INTERVAL_V, titleHeight, event_handler, ID_S_EEPROM_SET);
  lv_big_button_create(scr, "F:/bmp_fan.bin", set_menu.fan, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_S_FAN);
  lv_big_button_create(scr, "F:/bmp_about.bin", set_menu.about, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_S_ABOUT);
  lv_big_button_create(scr, ENABLED(HAS_SUICIDE) ? "F:/bmp_manual_off.bin" : "F:/bmp_function1.bin", set_menu.TERN(HAS_SUICIDE, shutdown, motoroff), BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_S_MOTOR_OFF);
  lv_big_button_create(scr, "F:/bmp_machine_para.bin", set_menu.machine_para, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_MACHINE_PARA);
  #if HAS_LANG_SELECT_SCREEN
    lv_big_button_create(scr, "F:/bmp_language.bin", set_menu.language, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_LANGUAGE);
  #endif
  #if ENABLED(MKS_WIFI_MODULE)
    lv_big_button_create(scr, "F:/bmp_wifi.bin", set_menu.wifi, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_WIFI);
  #endif
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_RETURN);
  #endif
}

void lv_clear_set() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
