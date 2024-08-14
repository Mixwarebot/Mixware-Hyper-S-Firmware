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

#include "../../../inc/MarlinConfig.h"
#include <string.h>


#if ENABLED(TFT_MIXWARE_UI)
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif
extern uint8_t sel_id;
extern bool flash_preview_begin, default_preview_flg;
#endif

enum {
  ID_CN = 1,
  ID_T_CN,
  ID_EN,
  ID_RU,
  ID_ES,
  ID_FR,
  ID_IT,
  ID_L_RETURN
};

#define SELECTED    1
#define UNSELECTED  0

static void disp_language(uint8_t language, uint8_t state);

extern lv_group_t *g;
static lv_obj_t *scr;
static lv_obj_t *buttonCN, *buttonT_CN, *buttonEN, *buttonRU;
static lv_obj_t *buttonES, *buttonFR, *buttonIT;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_CN:
      disp_language(gCfgItems.language, UNSELECTED);
      // lv_imgbtn_set_src_both(buttonCN, "F:/bmp_simplified_cn_sel.bin");
      lv_imgbtn_set_src_both(buttonCN, TERN(TFT_MIXWARE_UI, "F:/img_language_zh_CN_sel.bin", "F:/bmp_simplified_cn_sel.bin"));
      lv_obj_refresh_ext_draw_pad(buttonCN);
      gCfgItems.language = LANG_SIMPLE_CHINESE;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_EN:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonEN, "F:/img_language_En_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonEN);
      gCfgItems.language = LANG_ENGLISH;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_L_RETURN:
      buttonCN   = nullptr;
      buttonT_CN = nullptr;
      buttonEN   = nullptr;
      buttonRU   = nullptr;
      buttonES   = nullptr;
      buttonFR   = nullptr;
      buttonFR   = nullptr;
      buttonIT   = nullptr;
      // lv_clear_language();
      clear_cur_ui();

      #if ENABLED(TFT_MIXWARE_UI)
        if (disp_state_stack._disp_index < 2) {
          #if ENABLED(POWER_LOSS_RECOVERY)
            recovery.load();
            if (recovery.valid()) {
              if (gCfgItems.from_flash_pic)
                flash_preview_begin = true;
              else
                default_preview_flg = true;

              uiCfg.print_state = REPRINTING;

              #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
                strncpy(public_buf_m, recovery.info.sd_filename, sizeof(public_buf_m));
                card.printLongPath(public_buf_m);
                strncpy(list_file.long_name[sel_id], card.longFilename, sizeof(list_file.long_name[0]));
              #else
                strncpy(list_file.long_name[sel_id], recovery.info.sd_filename, sizeof(list_file.long_name[0]));
              #endif
              strncpy(list_file.file_name[sel_id], card.longFilename, sizeof(list_file.file_name[0]));
              lv_draw_printing();
            }
            else
          #endif
          lv_draw_ready_print();
        }
        else
      #endif
      // lv_draw_set();
      draw_return_ui();
      break;
  }
}

static void disp_language(uint8_t language, uint8_t state) {
  if (gCfgItems.language == LANG_UNSELECTED)
    return;

  uint16_t id = 0;
  lv_obj_t *obj = buttonEN;

  public_buf_l[0] = '\0';

  switch (language) {
    case LANG_SIMPLE_CHINESE:
      id = ID_CN;
      // strcpy_P(public_buf_l, PSTR("F:/bmp_simplified_cn"));
      strcpy_P(public_buf_l, TERN(TFT_MIXWARE_UI, PSTR("F:/img_language_zh_CN"), PSTR("F:/bmp_simplified_cn")));
      obj = buttonCN;
      break;
    case LANG_ENGLISH:
      id = ID_EN;
      strcpy_P(public_buf_l, PSTR("F:/img_language_En"));
      obj = buttonEN;
      break;
    default:
      break;
  }

  if (state == SELECTED) strcat_P(public_buf_l, PSTR("_sel"));

  strcat_P(public_buf_l, PSTR(".bin"));

  lv_obj_set_event_cb_mks(obj, event_handler, id, "", 0);
  lv_imgbtn_set_src_both(obj, public_buf_l);

  if (state == UNSELECTED) lv_obj_refresh_ext_draw_pad(obj);
}

void lv_draw_language() {
#if ENABLED(TFT_MIXWARE_UI)
  scr = lv_screen_create(LANGUAGE_UI, language_menu.title);
  // Create image buttons
  buttonCN   = lv_big_button_create(scr, "F:/img_language_zh_CN.bin", language_menu.chinese_s, button_pixel_point[0].x, button_pixel_point[0].y, event_handler, ID_CN);
  // buttonT_CN = lv_big_button_create(scr, "F:/img_language_zh_TW.bin", language_menu.chinese_t, button_pixel_point[1].x, button_pixel_point[1].y, event_handler, ID_T_CN);
  buttonEN   = lv_big_button_create(scr, "F:/img_language_En.bin",    language_menu.english,   button_pixel_point[1].x, button_pixel_point[1].y, event_handler, ID_EN);
  // buttonRU   = lv_big_button_create(scr, "F:/img_language_Ru.bin",    language_menu.russian,   button_pixel_point[3].x, button_pixel_point[3].y, event_handler, ID_RU);
  // buttonES   = lv_big_button_create(scr, "F:/img_language_Es.bin",    language_menu.spanish,   button_pixel_point[4].x, button_pixel_point[4].y, event_handler, ID_ES);
  // buttonIT   = lv_big_button_create(scr, "F:/img_language_It.bin",    language_menu.italy,     button_pixel_point[5].x, button_pixel_point[5].y, event_handler, ID_IT);
  lv_screen_menu_item_return(scr, event_handler, ID_L_RETURN);
  disp_language(gCfgItems.language, SELECTED);
#else
  scr = lv_screen_create(LANGUAGE_UI);
  // Create image buttons
  buttonCN = lv_big_button_create(scr, "F:/bmp_simplified_cn.bin", language_menu.chinese_s, INTERVAL_V, titleHeight, event_handler, ID_CN);
  lv_obj_clear_protect(buttonCN, LV_PROTECT_FOLLOW);
  buttonT_CN = lv_big_button_create(scr, "F:/bmp_traditional_cn.bin", language_menu.chinese_t, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_T_CN);
  buttonEN = lv_big_button_create(scr, "F:/bmp_english.bin", language_menu.english, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_EN);
  buttonRU = lv_big_button_create(scr, "F:/bmp_russian.bin", language_menu.russian, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_RU);
  buttonES = lv_big_button_create(scr, "F:/bmp_spanish.bin", language_menu.spanish, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_ES);
  buttonFR = lv_big_button_create(scr, "F:/bmp_french.bin", language_menu.french, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FR);
  buttonIT = lv_big_button_create(scr, "F:/bmp_italy.bin", language_menu.italy, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_IT);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_L_RETURN);
  disp_language(gCfgItems.language, SELECTED);
#endif
}

void lv_clear_language() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
