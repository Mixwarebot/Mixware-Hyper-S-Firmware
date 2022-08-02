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
#pragma once

#define MTR_PRINT_TCN                        "列印"
#define MTR_TOOL_TCN                         "工具"
#define MTR_SETTINGS_TCN                     "設置"
#define MTR_OPERATION_TCN                    "操作"
#define MTR_SPEED_TCN                        "速度"
#define MTR_BABYSTEP_TCN                     "微移動"

#define MTR_ADD_TCN                          "增加"
#define MTR_REDUCE_TCN                       "減少"
#define MTR_REBACK_TCN                       "返回"
#define MTR_SAVE_TCN                         "保存"
#define MTR_CLICK_TIPS_TCN                   "(點擊切換)"
#define MTR_WAIT_TIPS_TCN                    "正在啟動, 請稍等"

#define MTR_EXTRUCT_TCN                      "噴嘴"
#define MTR_Bed_TCN                          "熱床"
#define MTR_TEMPERATURE_TCN                  "溫度"
#define MTR_PREHEAT_TCN                      "預熱"
#define MTR_PREHEAT_COOL_TCN                 "冷卻"
#define MTR_PREHEAT_MORE_TCN                 "更多"

#define MTR_FAN_TCN                          "風扇"
#define MTR_FAN_ON_TCN                       "啟動 " MTR_FAN_TCN
#define MTR_FAN_OFF_TCN                      "關閉 " MTR_FAN_TCN

#define MTR_FILAMENT_TCN                     "換料"
#define MTR_FILAMENT_IN_TCN                  "進料"
#define MTR_FILAMENT_TIPS_IN_TCN             "準備進料,請選擇需要裝載的耗材!"
#define MTR_FILAMENT_TIPS_IN_HEAT_TCN        "正在加熱,請稍等!"
#define MTR_FILAMENT_TIPS_IN_CONFIRM_TCN     "請裝載耗材,按<確定>開始進料!"
#define MTR_FILAMENT_TIPS_IN_WORK_TCN        "正在進料,請等待耗材載入完成!"
#define MTR_FILAMENT_TIPS_IN_EXIT_TCN        "進料完成,請按<確定>返回"
#define MTR_FILAMENT_OUT_TCN                 "退料"
#define MTR_FILAMENT_TIPS_OUT_TCN            "準備退料,請選擇需要卸載的耗材!"
#define MTR_FILAMENT_TIPS_OUT_HEAT_TCN       MTR_FILAMENT_TIPS_IN_HEAT_TCN
#define MTR_FILAMENT_TIPS_OUT_WORK_TCN       "正在退料,請等待耗材卸載完成!"
#define MTR_FILAMENT_TIPS_OUT_EXIT_TCN       "退料完成,請按<確定>返回"
#define MTR_FILAMENT_TEMP_ADJUST_TCN         "調整溫度"

#define MTR_AXIS_MOVE_TCN                    "移動"
#define MTR_AXIS_HOME_TCN                    "回零"
#define MTR_AXIS_HOME_ALL_TCN                MTR_AXIS_HOME_TCN
#define MTR_AXIS_DISABLED_TCN                "關閉電機"

#define MTR_LEVELING_TCN                     "調平"
#define MTR_LEVELING_AUTO_TCN                "自動調平"
#define MTR_LEVELING_AUTO_TIPS_START_TCN     "按<確定>開始自動調平"
#define MTR_LEVELING_AUTO_TIPS_WORKING_TCN   "正在進行調平,請等待調平完成"
#define MTR_LEVELING_AUTO_TIPS_FINISHED_TCN  "  自動調平完成,按<確定>返回,\n按<調整>開始調整Z軸補償高度"
#define MTR_LEVELING_AUTO_TIPS_FAILED_TCN    "自動調平失敗,按<確定>返回"
#define MTR_LEVELING_MANUAL_TCN              "手動調平"
#define MTR_LEVELING_MANUAL_FL_TCN           "左前方"
#define MTR_LEVELING_MANUAL_FR_TCN           "右前方"
#define MTR_LEVELING_MANUAL_C_TCN            "中心點"
#define MTR_LEVELING_MANUAL_BL_TCN           "左後方"
#define MTR_LEVELING_MANUAL_BR_TCN           "右後方"

#define MTR_OFFSET_X_TCN                     "X軸偏移補償"
#define MTR_OFFSET_Y_TCN                     "Y軸偏移補償"
#define MTR_OFFSET_Z_TCN                     "Z軸偏移補償"
#define MTR_OFFSET_Z_ADD_TCN                 "抬升噴嘴"
#define MTR_OFFSET_Z_DEC_TCN                 "下降噴嘴"

#define MTR_FLASH_TITLE_TCN                  "出廠設置"
#define MTR_FLASH_RESET_TCN                  "恢復出廠設置"
#define MTR_FLASH_RESET_TIPS_TCN             "是否恢復出廠設置?"
#define MTR_FLASH_STORE_TCN                  "保存參數至印表機"
#define MTR_FLASH_STORE_TIPS_TCN             "是否保存參數到印表機?"

#define MTR_ADVSET_TCN                       "高級設置"
#define MTR_ADVSET_ACCELERATION_TCN          "加速度設置"
#define MTR_ADVSET_ACCEL_PRINT_TCN           "列印加速度"
#define MTR_ADVSET_ACCEL_RETRACT_TCN         "回抽加速度"
#define MTR_ADVSET_ACCEL_TRAVEL_TCN          "空載加速度"
#define MTR_ADVSET_ACCEL_X_AXIS_TCN          MTR_X "軸加速度"
#define MTR_ADVSET_ACCEL_Y_AXIS_TCN          MTR_Y "軸加速度"
#define MTR_ADVSET_ACCEL_Z_AXIS_TCN          MTR_Z "軸加速度"
#define MTR_ADVSET_ACCEL_E_AXIS_TCN          MTR_E "軸加速度"
#define MTR_ADVSET_MAX_SPEED_TCN             "最大速度設置"
#define MTR_ADVSET_MAX_SPEED_X_AXIS_TCN      MTR_X "軸最大速度"
#define MTR_ADVSET_MAX_SPEED_Y_AXIS_TCN      MTR_Y "軸最大速度"
#define MTR_ADVSET_MAX_SPEED_Z_AXIS_TCN      MTR_Z "軸最大速度"
#define MTR_ADVSET_MAX_SPEED_E_AXIS_TCN      MTR_E "軸最大速度"
#define MTR_ADVSET_JERK_TCN                  "突變速度設置"
#define MTR_ADVSET_JERK_X_AXIS_TCN           MTR_X "軸突變速度"
#define MTR_ADVSET_JERK_Y_AXIS_TCN           MTR_Y "軸突變速度"
#define MTR_ADVSET_JERK_Z_AXIS_TCN           MTR_Z "軸突變速度"
#define MTR_ADVSET_JERK_E_AXIS_TCN           MTR_E "軸突變速度"
#define MTR_ADVSET_STEP_TCN                  "脈衝當量設置"
#define MTR_ADVSET_STEP_X_AXIS_TCN           MTR_X "軸脈衝當量"
#define MTR_ADVSET_STEP_Y_AXIS_TCN           MTR_Y "軸脈衝當量"
#define MTR_ADVSET_STEP_Z_AXIS_TCN           MTR_Z "軸脈衝當量"
#define MTR_ADVSET_STEP_E_AXIS_TCN           MTR_E "軸脈衝當量"
#define MTR_ADVSET_PAUSE_POSITION_TCN        "列印暫停位置設置"
#define MTR_ADVSET_PAUSE_POSITION_X_TCN      MTR_X "軸暫停位置(相對位置)"
#define MTR_ADVSET_PAUSE_POSITION_Y_TCN      MTR_Y "軸暫停位置(相對位置)"
#define MTR_ADVSET_PAUSE_POSITION_Z_TCN      MTR_Z "軸暫停位置(相對位置)"
#define MTR_ADVSET_PAUSE_POSITION_TIPS_TCN   "(提示: -1無效.)"
#define MTR_ADVSET_FILAMENT_TCN              "換料設置"
#define MTR_ADVSET_FILAMENT_IN_LENGTH_TCN    "進料長度"
#define MTR_ADVSET_FILAMENT_IN_SPEED_TCN     "進料速度"
#define MTR_ADVSET_FILAMENT_OUT_LENGTH_TCN   "退料長度"
#define MTR_ADVSET_FILAMENT_OUT_SPEED_TCN    "退料速度"
#define MTR_ADVSET_EHEATING_MODE_TCN         "加熱模式設置"
#define MTR_ADVSET_THERMALPROTECTION_TCN     "熱保護設置"

#define MTR_EHEATMODE_NORMAL_TCN             "普通模式"
#define MTR_EHEATMODE_HIGH_TCN               "高溫模式"
#define MTR_EHEATMODE_CUR_NORMAL_TCN         "當前加熱模式: 普通模式"
#define MTR_EHEATMODE_CUR_HIGH_TCN           "當前加熱模式: 高溫模式"
#define MTR_EHEATMODE_TIPS_NORMAL_TCN        "   點擊<確定>更改為普通模式.\n(此模式需配置普通噴頭元件使用)"
#define MTR_EHEATMODE_TIPS_HIGH_TCN          "   點擊<確定>更改為高溫模式.\n(此模式需配置高溫噴頭元件使用)"
#define MTR_EHEATMODE_TIPS_TCN               "高溫模式"
#define MTR_EHEATMODE_TIPS_ABB_TCN           "高溫模式"

#define MTR_ABOUT_TCN                        "關於"

#define MTR_LANGUAGE_TCN                     "語言"

#define MTR_FILDET_TCN                       "材料檢測"
#define MTR_FILDET_ON_TCN                    "材料檢測: 開"
#define MTR_FILDET_OFF_TCN                   "材料檢測: 關"
#define MTR_FILDET_TIPS_PAUSE_TCN            "耗材狀態異常, 暫停列印中..."
#define MTR_FILDET_CLOG_TCN                  "材料檢測: 耗材狀態異常"
#define MTR_FILDET_TIPS_CLOG_TCN             "檢測到耗材堵塞, 暫停列印\n請清理噴嘴, 按<確定>返回"
#define MTR_FILDET_TIPS_HOW_TCN              "擠出更多耗材或繼續列印?"
#define MTR_FILDET_PURGE_TCN                 "擠出"
#define MTR_FILDET_PRINT_TCN                 "列印"

#define MTR_DEBUG_DEVICE_TITLE_TCN           "自檢模式"
#define MTR_DEBUG_DEVICE_CONFIRM_TCN         "檢 測"
#define MTR_DEBUG_DEVICE_WORKING_TCN         "正在檢測, 請稍等."
#define MTR_DEBUG_DEVICE_TIPS_E_TEMP_TCN     "噴嘴溫度"
#define MTR_DEBUG_DEVICE_TIPS_B_TEMP_TCN     "熱床溫度"
#define MTR_DEBUG_DEVICE_TIPS_E_HEAT_TCN     "噴嘴加熱"
#define MTR_DEBUG_DEVICE_TIPS_B_HEAT_TCN     "熱床加熱"
#define MTR_DEBUG_DEVICE_TIPS_FANS_TCN       "風扇"
#define MTR_DEBUG_DEVICE_TIPS_X_AXIS_TCN     MTR_X "軸電機"
#define MTR_DEBUG_DEVICE_TIPS_Y_AXIS_TCN     MTR_Y "軸電機"
#define MTR_DEBUG_DEVICE_TIPS_Z_AXIS_TCN     MTR_Z "軸電機"
#define MTR_DEBUG_DEVICE_TIPS_E_AXIS_TCN     MTR_E "軸電機"
#define MTR_DEBUG_DEVICE_TIPS_SERVO_TCN      "3D Touch"
#define MTR_DEBUG_ZAXIS_TITLE_TCN            "Z軸調試"
#define MTR_DEBUG_ZAXIS_SLOWMODE_TCN         "低速測試"
#define MTR_DEBUG_ZAXIS_FASTMODE_TCN         "高速測試"
#define MTR_DEBUG_ZAXIS_WORKING_TCN          "正在進行Z軸調試"

#define MTR_PRINT_AGAIN_TCN                  "再次列印"
#define MTR_PRINT_TIPS_SAVEOFFSET_TCN        "            已使用微移動,\n可使用<保存並退出>保存參數."
#define MTR_PRINT_TIPS_SAVE_TCN              "保存並退出"
#define MTR_PRINT_PAUSE_TCN                  "暫停"
#define MTR_PRINT_RESUME_TCN                 "恢復"
#define MTR_PRINT_STOP_TCN                   "停止"
#define MTR_PRINT_NOFILE_TIPS_TCN            "錯誤:找不到檔,請插入u盤!"

#define MTR_PAGE_PREV_TCN                    "上一頁"
#define MTR_PAGE_NEXT_TCN                    "下一頁"

#define MTR_NO_KEY_BACK_TCN                  "退格"
#define MTR_NO_KEY_RESET_TCN                 "重置"
#define MTR_NO_KEY_CONFIRM_TCN               "確定"


