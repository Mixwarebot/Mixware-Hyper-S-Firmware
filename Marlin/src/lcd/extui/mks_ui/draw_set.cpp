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

#ifndef USE_NEW_LVGL_CONF
  static lv_obj_t *scr;
#endif

#if ENABLED(TFT_MIXWARE_LVGL_UI)
#include "../../../module/motion.h"
#include "../../../module/probe.h"

static lv_obj_t *buttonFilamentDet, *labelFilamentDet;
#endif
extern lv_group_t*  g;

enum {
  ID_S_WIFI = 1,
  ID_S_FAN,
  ID_S_ABOUT,
  ID_S_CONTINUE,
  ID_S_MOTOR_OFF,
  ID_S_LANGUAGE,
  ID_S_MACHINE_PARA,
  ID_S_EEPROM_SET,
  ID_S_RETURN
#if ENABLED(TFT_MIXWARE_LVGL_UI)
  ,ID_S_FILAMENT_DET
  ,ID_S_DEBUG
#endif
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (obj->mks_obj_id == ID_S_CONTINUE) return;
  if (obj->mks_obj_id == ID_S_MOTOR_OFF) {
    TERN(HAS_SUICIDE, suicide(), queue.enqueue_now_P(PSTR("M84")));
    return;
  }
  #if ENABLED(TFT_MIXWARE_LVGL_UI)
    if (obj->mks_obj_id == ID_S_FILAMENT_DET) {
      MUI.toggle_filament_detector();
      lv_imgbtn_set_src_both(buttonFilamentDet, MUI.get_filament_detector_image_path());
      lv_label_set_text(labelFilamentDet, MUI.get_filament_detector_tr());
      lv_obj_refresh_ext_draw_pad(labelFilamentDet);
      update_spi_flash();
      return;
    }
  #endif
  lv_clear_set();
  switch (obj->mks_obj_id) {
    case ID_S_FAN:
      lv_draw_fan();
      break;
    case ID_S_ABOUT:
      lv_draw_about();
      break;
    case ID_S_LANGUAGE:
      lv_draw_language();
      break;
    case ID_S_MACHINE_PARA:
      #if DISABLED(TFT_MIXWARE_LVGL_UI)
        lv_draw_machine_para();
      #else
        lv_draw_advance_settings();
      #endif
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
    #if ENABLED(TFT_MIXWARE_LVGL_UI)
      case ID_S_DEBUG:
        MUI.page_draw_device_debug();
        break;
    #endif
  }
}

void lv_draw_set() {
#ifdef USE_NEW_LVGL_CONF
  mks_ui.src_main = lv_set_scr_id_title(mks_ui.src_main, SET_UI, "");
#else
  scr = lv_screen_create(SET_UI);
#endif

  #if DISABLED(TFT_MIXWARE_LVGL_UI)

	#else
  lv_big_button_create(scr, MIMG.EEPROM,        MTR.flash,          IMAGEBTN_P_X(0), IMAGEBTN_P_Y(0), event_handler, ID_S_EEPROM_SET);
  lv_big_button_create(scr, MIMG.configuration, MTR.ADVSet,         IMAGEBTN_P_X(1), IMAGEBTN_P_Y(1), event_handler, ID_S_MACHINE_PARA);
  lv_big_button_create(scr, MIMG.about,         MTR.about,          IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), event_handler, ID_S_ABOUT);
  lv_big_button_create(scr, MIMG.language,      MTR.language,       IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), event_handler, ID_S_LANGUAGE);
  buttonFilamentDet = lv_imgbtn_create(scr, MUI.get_filament_detector_image_path(), IMAGEBTN_P_X(4), IMAGEBTN_P_Y(4), event_handler, ID_S_FILAMENT_DET);
  lv_big_button_create(scr, MIMG.deviceDebug,   MTR.debugDevTitle,  IMAGEBTN_P_X(5), IMAGEBTN_P_Y(5), event_handler, ID_S_DEBUG);
  MUI.page_button_return(scr, event_handler, ID_S_RETURN);

  labelFilamentDet = lv_label_create_empty(buttonFilamentDet);
  lv_label_set_text(labelFilamentDet, MUI.get_filament_detector_tr());
  lv_obj_align(labelFilamentDet, buttonFilamentDet, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
#endif
}

void lv_clear_set() {
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
