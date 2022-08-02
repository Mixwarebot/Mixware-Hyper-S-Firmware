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

#define MTR_PRINT_SCN                        "打印"
#define MTR_TOOL_SCN                         "工具"
#define MTR_SETTINGS_SCN                     "设置"
#define MTR_OPERATION_SCN                    "操作"
#define MTR_SPEED_SCN                        "速度"
#define MTR_BABYSTEP_SCN                     "微移动"

#define MTR_ADD_SCN                          "增加"
#define MTR_REDUCE_SCN                       "减少"
#define MTR_REBACK_SCN                       "返回"
#define MTR_SAVE_SCN                         "保存"
#define MTR_CLICK_TIPS_SCN                   "(点击切换)"
#define MTR_WAIT_TIPS_SCN                    "正在启动, 请稍等"

#define MTR_EXTRUCT_SCN                      "喷嘴"
#define MTR_Bed_SCN                          "热床"
#define MTR_TEMPERATURE_SCN                  "温度"
#define MTR_PREHEAT_SCN                      "预热"
#define MTR_PREHEAT_COOL_SCN                 "冷却"
#define MTR_PREHEAT_MORE_SCN                 "更多"

#define MTR_FAN_SCN                          "风扇"
#define MTR_FAN_ON_SCN                       "启动 " MTR_FAN_SCN
#define MTR_FAN_OFF_SCN                      "关闭 " MTR_FAN_SCN

#define MTR_FILAMENT_SCN                     "换料"
#define MTR_FILAMENT_IN_SCN                  "进料"
#define MTR_FILAMENT_TIPS_IN_SCN             "准备进料,请选择需要装载的耗材!"
#define MTR_FILAMENT_TIPS_IN_HEAT_SCN        "正在加热,请稍等!"
#define MTR_FILAMENT_TIPS_IN_CONFIRM_SCN     "请装载耗材,按<确定>开始进料!"
#define MTR_FILAMENT_TIPS_IN_WORK_SCN        "正在进料,请等待耗材加载完成!"
#define MTR_FILAMENT_TIPS_IN_EXIT_SCN        "进料完成,请按<确定>返回"
#define MTR_FILAMENT_OUT_SCN                 "退料"
#define MTR_FILAMENT_TIPS_OUT_SCN            "准备退料,请选择需要卸载的耗材!"
#define MTR_FILAMENT_TIPS_OUT_HEAT_SCN       MTR_FILAMENT_TIPS_IN_HEAT_SCN
#define MTR_FILAMENT_TIPS_OUT_WORK_SCN       "正在退料,请等待耗材卸载完成!"
#define MTR_FILAMENT_TIPS_OUT_EXIT_SCN       "退料完成,请按<确定>返回"
#define MTR_FILAMENT_TEMP_ADJUST_SCN         "调整温度"

#define MTR_AXIS_MOVE_SCN                    "移动"
#define MTR_AXIS_HOME_SCN                    "回零"
#define MTR_AXIS_HOME_ALL_SCN                MTR_AXIS_HOME_SCN
#define MTR_AXIS_DISABLED_SCN                "关闭电机"

#define MTR_LEVELING_SCN                     "调平"
#define MTR_LEVELING_AUTO_SCN                "自动调平"
#define MTR_LEVELING_AUTO_TIPS_START_SCN     "按<确定>开始自动调平"
#define MTR_LEVELING_AUTO_TIPS_WORKING_SCN   "正在进行调平,请等待调平完成"
#define MTR_LEVELING_AUTO_TIPS_FINISHED_SCN  "  自动调平完成,按<确定>返回,\n按<调整>开始调整Z轴补偿高度"
#define MTR_LEVELING_AUTO_TIPS_FAILED_SCN    "自动调平失败,按<确定>返回"
#define MTR_LEVELING_MANUAL_SCN              "手动调平"
#define MTR_LEVELING_MANUAL_FL_SCN           "左前方"
#define MTR_LEVELING_MANUAL_FR_SCN           "右前方"
#define MTR_LEVELING_MANUAL_C_SCN            "中心点"
#define MTR_LEVELING_MANUAL_BL_SCN           "左后方"
#define MTR_LEVELING_MANUAL_BR_SCN           "右后方"

#define MTR_OFFSET_X_SCN                     "X轴偏移补偿"
#define MTR_OFFSET_Y_SCN                     "Y轴偏移补偿"
#define MTR_OFFSET_Z_SCN                     "Z轴偏移补偿"
#define MTR_OFFSET_Z_ADD_SCN                 "抬升喷嘴"
#define MTR_OFFSET_Z_DEC_SCN                 "下降喷嘴"

#define MTR_FLASH_TITLE_SCN                  "出厂设置"
#define MTR_FLASH_RESET_SCN                  "恢复出厂设置"
#define MTR_FLASH_RESET_TIPS_SCN             "是否恢复出厂设置?"
#define MTR_FLASH_STORE_SCN                  "保存参数至打印机"
#define MTR_FLASH_STORE_TIPS_SCN             "是否保存参数到打印机?"

#define MTR_ADVSET_SCN                       "高级设置"
#define MTR_ADVSET_ACCELERATION_SCN          "加速度设置"
#define MTR_ADVSET_ACCEL_PRINT_SCN           "打印加速度"
#define MTR_ADVSET_ACCEL_RETRACT_SCN         "回抽加速度"
#define MTR_ADVSET_ACCEL_TRAVEL_SCN          "空载加速度"
#define MTR_ADVSET_ACCEL_X_AXIS_SCN          MTR_X "轴加速度"
#define MTR_ADVSET_ACCEL_Y_AXIS_SCN          MTR_Y "轴加速度"
#define MTR_ADVSET_ACCEL_Z_AXIS_SCN          MTR_Z "轴加速度"
#define MTR_ADVSET_ACCEL_E_AXIS_SCN          MTR_E "轴加速度"
#define MTR_ADVSET_MAX_SPEED_SCN             "最大速度设置"
#define MTR_ADVSET_MAX_SPEED_X_AXIS_SCN      MTR_X "轴最大速度"
#define MTR_ADVSET_MAX_SPEED_Y_AXIS_SCN      MTR_Y "轴最大速度"
#define MTR_ADVSET_MAX_SPEED_Z_AXIS_SCN      MTR_Z "轴最大速度"
#define MTR_ADVSET_MAX_SPEED_E_AXIS_SCN      MTR_E "轴最大速度"
#define MTR_ADVSET_JERK_SCN                  "突变速度设置"
#define MTR_ADVSET_JERK_X_AXIS_SCN           MTR_X "轴突变速度"
#define MTR_ADVSET_JERK_Y_AXIS_SCN           MTR_Y "轴突变速度"
#define MTR_ADVSET_JERK_Z_AXIS_SCN           MTR_Z "轴突变速度"
#define MTR_ADVSET_JERK_E_AXIS_SCN           MTR_E "轴突变速度"
#define MTR_ADVSET_STEP_SCN                  "脉冲当量设置"
#define MTR_ADVSET_STEP_X_AXIS_SCN           MTR_X "轴脉冲当量"
#define MTR_ADVSET_STEP_Y_AXIS_SCN           MTR_Y "轴脉冲当量"
#define MTR_ADVSET_STEP_Z_AXIS_SCN           MTR_Z "轴脉冲当量"
#define MTR_ADVSET_STEP_E_AXIS_SCN           MTR_E "轴脉冲当量"
#define MTR_ADVSET_PAUSE_POSITION_SCN        "打印暂停位置设置"
#define MTR_ADVSET_PAUSE_POSITION_X_SCN      MTR_X "轴暂停位置(相对位置)"
#define MTR_ADVSET_PAUSE_POSITION_Y_SCN      MTR_Y "轴暂停位置(相对位置)"
#define MTR_ADVSET_PAUSE_POSITION_Z_SCN      MTR_Z "轴暂停位置(相对位置)"
#define MTR_ADVSET_PAUSE_POSITION_TIPS_SCN   "(提示: -1无效.)"
#define MTR_ADVSET_FILAMENT_SCN              "换料设置"
#define MTR_ADVSET_FILAMENT_IN_LENGTH_SCN    "进料长度"
#define MTR_ADVSET_FILAMENT_IN_SPEED_SCN     "进料速度"
#define MTR_ADVSET_FILAMENT_OUT_LENGTH_SCN   "退料长度"
#define MTR_ADVSET_FILAMENT_OUT_SPEED_SCN    "退料速度"
#define MTR_ADVSET_EHEATING_MODE_SCN         "加热模式设置"
#define MTR_ADVSET_THERMALPROTECTION_SCN     "热保护设置"

#define MTR_EHEATMODE_NORMAL_SCN             "普通模式"
#define MTR_EHEATMODE_HIGH_SCN               "高温模式"
#define MTR_EHEATMODE_CUR_NORMAL_SCN         "当前加热模式: 普通模式"
#define MTR_EHEATMODE_CUR_HIGH_SCN           "当前加热模式: 高温模式"
#define MTR_EHEATMODE_TIPS_NORMAL_SCN        "   点击<确定>更改为普通模式.\n(此模式需配置普通喷头组件使用)"
#define MTR_EHEATMODE_TIPS_HIGH_SCN          "   点击<确定>更改为高温模式.\n(此模式需配置高温喷头组件使用)"
#define MTR_EHEATMODE_TIPS_SCN               "高温模式"
#define MTR_EHEATMODE_TIPS_ABB_SCN           "高温模式"

#define MTR_ABOUT_SCN                        "关于"

#define MTR_LANGUAGE_SCN                     "语言"

#define MTR_FILDET_SCN                       "材料检测"
#define MTR_FILDET_ON_SCN                    "材料检测: 开"
#define MTR_FILDET_OFF_SCN                   "材料检测: 关"
#define MTR_FILDET_TIPS_PAUSE_SCN            "耗材状态异常, 暂停打印中..."
#define MTR_FILDET_CLOG_SCN                  "材料检测: 耗材状态异常"
#define MTR_FILDET_TIPS_CLOG_SCN             "检测到耗材堵塞, 暂停打印\n请清理喷嘴, 按<确定>返回"
#define MTR_FILDET_TIPS_HOW_SCN              "挤出更多耗材或继续打印?"
#define MTR_FILDET_PURGE_SCN                 "挤出"
#define MTR_FILDET_PRINT_SCN                 "打印"

#define MTR_DEBUG_DEVICE_TITLE_SCN           "自检模式"
#define MTR_DEBUG_DEVICE_CONFIRM_SCN         "检 测"
#define MTR_DEBUG_DEVICE_WORKING_SCN         "正在检测, 请稍等."
#define MTR_DEBUG_DEVICE_TIPS_E_TEMP_SCN     "喷嘴温度"
#define MTR_DEBUG_DEVICE_TIPS_B_TEMP_SCN     "热床温度"
#define MTR_DEBUG_DEVICE_TIPS_E_HEAT_SCN     "喷嘴加热"
#define MTR_DEBUG_DEVICE_TIPS_B_HEAT_SCN     "热床加热"
#define MTR_DEBUG_DEVICE_TIPS_FANS_SCN       "风扇"
#define MTR_DEBUG_DEVICE_TIPS_X_AXIS_SCN     MTR_X "轴电机"
#define MTR_DEBUG_DEVICE_TIPS_Y_AXIS_SCN     MTR_Y "轴电机"
#define MTR_DEBUG_DEVICE_TIPS_Z_AXIS_SCN     MTR_Z "轴电机"
#define MTR_DEBUG_DEVICE_TIPS_E_AXIS_SCN     MTR_E "轴电机"
#define MTR_DEBUG_DEVICE_TIPS_SERVO_SCN      "3D Touch"
#define MTR_DEBUG_ZAXIS_TITLE_SCN            "Z轴调试"
#define MTR_DEBUG_ZAXIS_SLOWMODE_SCN         "低速测试"
#define MTR_DEBUG_ZAXIS_FASTMODE_SCN         "高速测试"
#define MTR_DEBUG_ZAXIS_WORKING_SCN          "正在进行Z轴调试"

#define MTR_PRINT_AGAIN_SCN                  "再次打印"
#define MTR_PRINT_TIPS_SAVEOFFSET_SCN        "            已使用微移动,\n可使用<保存并退出>保存参数."
#define MTR_PRINT_TIPS_SAVE_SCN              "保存并退出"
#define MTR_PRINT_PAUSE_SCN                  "暂停"
#define MTR_PRINT_RESUME_SCN                 "恢复"
#define MTR_PRINT_STOP_SCN                   "停止"
#define MTR_PRINT_NOFILE_TIPS_SCN            "错误:找不到文件,请插入u盘!"

#define MTR_PAGE_PREV_SCN                    "上一页"
#define MTR_PAGE_NEXT_SCN                    "下一页"

#define MTR_NO_KEY_BACK_SCN                  "退格"
#define MTR_NO_KEY_RESET_SCN                 "重置"
#define MTR_NO_KEY_CONFIRM_SCN               "确定"
