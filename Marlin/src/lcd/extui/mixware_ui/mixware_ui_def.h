/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// image name define
#define MIMG_N_HOME_ALL "img_home_all.bin"
#define MIMG_N_HOME_X "img_home_x.bin"
#define MIMG_N_HOME_Y "img_home_y.bin"
#define MIMG_N_HOME_Z "img_home_z.bin"
#define MIMG_N_MOTOR_OFF "img_motor_off_all.bin"

#define MIMG_N_PREHEAT "img_preheat.bin"
#define MIMG_N_FAN "img_fan.bin"
#define MIMG_N_MOVE "img_move.bin"
#define MIMG_N_LEVEL "img_level.bin"
#define MIMG_N_FILAMENT "img_filamentchange.bin"
#define MIMG_N_HOME "img_home.bin"

#define MIMG_N_ADD "img_add.bin"
#define MIMG_N_DEC "img_dec.bin"
#define MIMG_N_SPEED_RATE_1 "img_step_percent_1.bin"
#define MIMG_N_SPEED_RATE_5 "img_step_percent_5.bin"
#define MIMG_N_SPEED_RATE_10 "img_step_percent_10.bin"

#define MIMG_N_BED "img_bed.bin"
#define MIMG_N_TEMP_RANGE_1 "img_step_degree_1.bin"
#define MIMG_N_TEMP_RANGE_5 "img_step_degree_5.bin"
#define MIMG_N_TEMP_RANGE_10 "img_step_degree_10.bin"

#define MIMG_N_FILAMENT_IN "img_filamentchange_in.bin"
#define MIMG_N_FILAMENT_OUT "img_filamentchange_out.bin"
#define MIMG_N_EXT "img_extruct.bin"
#define MIMG_N_SPEED_HIGH "img_speed_high.bin"
#define MIMG_N_SPEED_SLOW "img_speed_slow.bin"
#define MIMG_N_SPEED_NORMAL "img_speed_normal.bin"
#define MIMG_N_STEP_MM_1 "img_step_mm_1.bin"
#define MIMG_N_STEP_MM_5 "img_step_mm_5.bin"
#define MIMG_N_STEP_MM_10 "img_step_mm_10.bin"

#define MIMG_N_DIR "img_dir.bin"
#define MIMG_N_FILE "img_file.bin"

#define MIMG_N_AXIS_X "img_axis_x.bin"
#define MIMG_N_AXIS_Y "img_axis_y.bin"
#define MIMG_N_AXIS_Z "img_axis_z.bin"
#define MIMG_N_STEP_MM_0_1 "img_step_mm_0_1.bin"

#define MIMG_N_SPEED "img_speed.bin"

#define MIMG_N_PRINT_PAUSE "bmp_pause.bin"
#define MIMG_N_PRINT_RESUME "bmp_resume.bin"
#define MIMG_N_PRINT_STOP "bmp_stop.bin"
#define MIMG_N_STATE_X "img_xpos_state.bin"
#define MIMG_N_STATE_Y "img_ypos_state.bin"
#define MIMG_N_STATE_Z "img_zpos_state.bin"
#define MIMG_N_STATE_EXT "bmp_ext_state.bin"
#define MIMG_N_STATE_BED "bmp_bed_state.bin"
#define MIMG_N_STATE_FAN "bmp_fan_state.bin"
#define MIMG_N_STATE_TIME "bmp_time_state.bin"
#define MIMG_N_OPERATE "bmp_operate.bin"

#define MIMG_N_TOOL "img_tool.bin"
#define MIMG_N_SET "img_set.bin"
#define MIMG_N_PRINT "img_printing.bin"

#define MIMG_N_PREHEAT_PLA "img_preheat_PLA.bin"
#define MIMG_N_PREHEAT_PETG "img_preheat_PETG.bin"
#define MIMG_N_PREHEAT_PACF "img_preheat_PACF.bin"
#define MIMG_N_PREHEAT_BED "img_preheat_Bed.bin"
#define MIMG_N_PREHEAT_COOL "img_preheat_Cool.bin"

#define MIMG_N_LEVEL2 "img_level2.bin"
#define MIMG_N_LEVEL_P1 "img_level_1.bin"
#define MIMG_N_LEVEL_P2 "img_level_2.bin"
#define MIMG_N_LEVEL_P3 "img_level_3.bin"
#define MIMG_N_LEVEL_P4 "img_level_4.bin"
#define MIMG_N_LEVEL_P5 "img_level_5.bin"
#define MIMG_N_LEVEL_AUTO "img_level_auto.bin"
#define MIMG_N_LEVEL_MANUAL "img_level_manual.bin"
#define MIMG_N_SAVE "img_save.bin"
#define MIMG_N_BABYSTEP "img_babystep.bin"
#define MIMG_N_Z_OFFSET "img_level_z_offset.bin"

#define MIMG_N_EEPROM "img_eeprom.bin"
#define MIMG_N_ABOUT "img_about.bin"
#define MIMG_N_CONFIG "img_machine_para.bin"

#define MIMG_N_DEBUG_Z_SLOW "img_axis_z_test_s.bin"
#define MIMG_N_DEBUG_Z_FAST "img_axis_z_test_f.bin"
#define MIMG_N_DEBUG_DEV "img_self_check.bin"

#define MIMG_N_LANG "img_language.bin"
#define MIMG_N_LANG_EN "img_language_En.bin"
#define MIMG_N_LANG_EN_SEL "img_language_En_sel.bin"
#define MIMG_N_LANG_SCN "img_language_zh_CN.bin"
#define MIMG_N_LANG_SCN_SEL "img_language_zh_CN_sel.bin"
#define MIMG_N_LANG_TCN "img_language_zh_TW.bin"
#define MIMG_N_LANG_TCN_SEL "img_language_zh_TW_sel.bin"

#define MIMG_N_FILAMENT_DET "img_run_out.bin"
#define MIMG_N_FILAMENT_DET_ON "img_run_out_on.bin"
#define MIMG_N_FILAMENT_DET_OFF "img_run_out_off.bin"

#define MIMG_N_ARROW "bmp_arrow.bin"
#define MIMG_N_NULL "img_null.bin"

#define MIMG_N_E_HEAT_M_NORMAL "img_normal_mode.bin"
#define MIMG_N_E_HEAT_M_HIGH "HI_high_mode.bin"
#define MIMG_N_PRINT_HM "HI_printing.bin"
#define MIMG_N_PREHEAT_HM "HI_preheat.bin"
#define MIMG_N_EXT_HM "HI_extruct.bin"
#define MIMG_N_STATE_EXT_HM "HI_ext_state.bin"
#define MIMG_N_E_HEAT_M_TIP_HM "HI_mode_tip.bin"

#define MIMG_N_PREVIEW "bmp_preview.bin"
#define MIMG_N_LOGO "bmp_logo.bin"
#define MIMG_N_MIXWARE "img_mixware.bin"

#define MIMG_N_ENABLE "img_enable.bin"
#define MIMG_N_DISABLE "img_disable.bin"

#define MIMAGEDIR "F:/"
#define MIMAGEPATH(NAME) MIMAGEDIR NAME

// page value define
#undef TFT_LV_PARA_BACK_BODY_COLOR
#define TFT_LV_PARA_BACK_BODY_COLOR LV_COLOR_MAKE(0xFF, 0x5A, 0x00)

#undef BTN_Y_PIXEL
#define BTN_Y_PIXEL 130

#undef BUTTON_TEXT_Y_OFFSET
#define BUTTON_TEXT_Y_OFFSET -5

#undef FILE_BTN_CNT
#define FILE_BTN_CNT 5

#undef FILE_PRE_PIC_Y_OFFSET
#define FILE_PRE_PIC_Y_OFFSET 10

#undef PARA_UI_POS_Y
#define PARA_UI_POS_Y 48

#undef PARA_UI_SIZE_X
#define PARA_UI_SIZE_X (TFT_WIDTH - 30)

#undef PARA_UI_BACK_POS_X
#define PARA_UI_BACK_POS_X (TFT_WIDTH - PARA_UI_BACK_BTN_X_SIZE - 5)

#undef PARA_UI_BACK_POS_Y
#define PARA_UI_BACK_POS_Y (TFT_HEIGHT - PARA_UI_BACK_BTN_Y_SIZE - 2)

#undef PARA_UI_TURN_PAGE_POS_X
#define PARA_UI_TURN_PAGE_POS_X (PARA_UI_BACK_POS_X - PARA_UI_BACK_BTN_X_SIZE - 5)

#undef PARA_UI_TURN_PAGE_POS_Y
#define PARA_UI_TURN_PAGE_POS_Y (PARA_UI_BACK_POS_Y)

#undef PARA_UI_VALUE_POS_X
#define PARA_UI_VALUE_POS_X (TFT_WIDTH - 90)

#undef PARA_UI_STATE_POS_X
#define PARA_UI_STATE_POS_X (TFT_WIDTH - 100)

#undef PARA_UI_VALUE_POS_X_2
#define PARA_UI_VALUE_POS_X_2 (TFT_WIDTH - 160)

#undef PARA_UI_BACK_BTN_X_SIZE
#define PARA_UI_BACK_BTN_X_SIZE 100

#undef PARA_UI_BACK_BTN_Y_SIZE
#define PARA_UI_BACK_BTN_Y_SIZE 50

#define PREVIEW_POS_X   0
#define PREVIEW_POS_Y   80

#define BUTTON_POS_X 28
#define BUTTON_POS_Y 36
#define PARA_UI_ITEM_TEXT_V 10
#define PARA_UI_ITEM_TEXT_H 10
#define UNLOAD_PRELOAD_TIME (MMS_TO_MMM(15) / 100) // MMS_TO_MMM(lenght) / speed

#define M_ITEM_POS_Y(n) (PARA_UI_POS_Y * (n + 1))
#define M_SCREEN_ITEM(T, ID, n) \
    lv_screen_menu_item(scr, T, PARA_UI_POS_X, M_ITEM_POS_Y(n), event_handler, ID, n)
#define M_SCREEN_EDITITEM(T, ID, n) \
    lv_screen_menu_item_1_edit(scr, T, PARA_UI_POS_X, M_ITEM_POS_Y(n), event_handler, ID, n, public_buf_l)

#define TOOLBTN_WIDTH 145
#define TOOLBTN_HEIGHT 67
#define TOOLBTN_P_LX 12
#define TOOLBTN_P_RX (TFT_WIDTH - TOOLBTN_P_LX - TOOLBTN_WIDTH)
#define TOOLBTN_SPACE_Y 11
#define TOOLBTN_P_X(n) ((n % 2 == 0) ? TOOLBTN_P_LX : TOOLBTN_P_RX)
#define TOOLBTN_P_Y(n) (PARA_UI_SIZE_Y + (TOOLBTN_HEIGHT + TOOLBTN_SPACE_Y) * (int16_t)(n / 2))

#define IMAGEBTN_WIDTH 117
#define IMAGEBTN_HEIGHT 130
#define IMAGEBTN_P_LX 28
#define IMAGEBTN_P_RX (TFT_WIDTH - IMAGEBTN_P_LX - IMAGEBTN_WIDTH)
#define IMAGEBTN_P_X(n) ((n % 2 == 0) ? (IMAGEBTN_P_LX) : (IMAGEBTN_P_RX)) // N: 0-6
#define IMAGEBTN_P_Y(n) (BUTTON_POS_Y + IMAGEBTN_HEIGHT * (int16_t)(n / 2))

#define BOTTOMBTN_P_LX (TFT_WIDTH - (PARA_UI_BACK_BTN_X_SIZE + 5) * 3)
#define BOTTOMBTN_P_MX (TFT_WIDTH - (PARA_UI_BACK_BTN_X_SIZE + 5) * 2)
#define BOTTOMBTN_P_RX (TFT_WIDTH - (PARA_UI_BACK_BTN_X_SIZE + 5) * 1)
#define BOTTOMBTN_P_Y (TFT_HEIGHT - PARA_UI_BACK_BTN_Y_SIZE - 2)

//
#define MUI mixware_ui
#define MTR MUI.info.tr
#define MIMG MUI.info.image
#define MPRE MUI.info.preference
#define MIMG_HM(p) MUI.get_heating_mode() ? MIMG.p : MIMG.heating_mode_## p

#define M_EHEATING_MODE_NORMAL_TEMP 260
#define M_EHEATING_MODE_HIGH_TEMP   350

#define M_CMD "M301 P20.30 I1.23 D57.77\nM500"
