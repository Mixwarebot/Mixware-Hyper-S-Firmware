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

//
#define MTR_PRINT_EN                        "Printing"
#define MTR_TOOL_EN                         "Tool"
#define MTR_SETTINGS_EN                     "Settings"
#define MTR_OPERATION_EN                    "Operation"
#define MTR_SPEED_EN                        "Speed"
#define MTR_BABYSTEP_EN                     "Babystep"

#define MTR_ADD_EN                          "Add"
#define MTR_REDUCE_EN                       "Dec"
#define MTR_REBACK_EN                       "Back"
#define MTR_SAVE_EN                         "Save"
#define MTR_CLICK_TIPS_EN                   "(Click Switch)"
#define MTR_WAIT_TIPS_EN                    "Starting, please wait..."

#define MTR_EXTRUCT_EN                      "Nozzle"
#define MTR_Bed_EN                          "Bed"
#define MTR_TEMPERATURE_EN                  "Temp."
#define MTR_PREHEAT_EN                      "Preheat"
#define MTR_PREHEAT_COOL_EN                 "Cool"
#define MTR_PREHEAT_MORE_EN                 "More"

#define MTR_FAN_EN                          "Fan"
#define MTR_FAN_ON_EN                       "Open " MTR_FAN_EN
#define MTR_FAN_OFF_EN                      "Close " MTR_FAN_EN

#define MTR_FILAMENT_EN                     "Filament"
#define MTR_FILAMENT_IN_EN                  "Load Fil."
#define MTR_FILAMENT_TIPS_IN_EN             "Please select the filament to load."
#define MTR_FILAMENT_TIPS_IN_HEAT_EN        "The nozzle is heating,\n       please wait..."
#define MTR_FILAMENT_TIPS_IN_CONFIRM_EN     "   Please load filament to extruder,\n   and click <confirm> to start loading."
#define MTR_FILAMENT_TIPS_IN_WORK_EN        "Loading, please wait!"
#define MTR_FILAMENT_TIPS_IN_EXIT_EN        "Load filament completed,\nclick <confirm> to return!"
#define MTR_FILAMENT_OUT_EN                 "Unload Fil."
#define MTR_FILAMENT_TIPS_OUT_EN            "Please select the filament to unload."
#define MTR_FILAMENT_TIPS_OUT_HEAT_EN       MTR_FILAMENT_TIPS_IN_HEAT_EN
#define MTR_FILAMENT_TIPS_OUT_WORK_EN       "Unloading, please wait!"
#define MTR_FILAMENT_TIPS_OUT_EXIT_EN       "Unload filament completed,\n click <confirm> to return!"
#define MTR_FILAMENT_TEMP_ADJUST_EN         "Adjust Temp."

#define MTR_AXIS_MOVE_EN                    "Move"
#define MTR_AXIS_HOME_EN                    "Home"
#define MTR_AXIS_HOME_ALL_EN                "All"
#define MTR_AXIS_DISABLED_EN                "Motor off"

#define MTR_LEVELING_EN                     "Leveling"
#define MTR_LEVELING_AUTO_EN                "Auto-leveling"
#define MTR_LEVELING_AUTO_TIPS_START_EN     "Click <confirm> to start Auto-leveling."
#define MTR_LEVELING_AUTO_TIPS_WORKING_EN   "Auto-leveling, please wait!"
#define MTR_LEVELING_AUTO_TIPS_FINISHED_EN  " Auto-leveling completed.\nClick <confirm> to return!\nClick <adjust> to Z offset!"
#define MTR_LEVELING_AUTO_TIPS_FAILED_EN    "    Auto-leveling failed.\nClick <confirm> to return!"
#define MTR_LEVELING_MANUAL_EN              "Manual leveling"
#define MTR_LEVELING_MANUAL_FL_EN           "Front Left"
#define MTR_LEVELING_MANUAL_FR_EN           "Front Right"
#define MTR_LEVELING_MANUAL_C_EN            "Center"
#define MTR_LEVELING_MANUAL_BL_EN           "Back Left"
#define MTR_LEVELING_MANUAL_BR_EN           "Back Right"

#define MTR_OFFSET_X_EN                     "X-axis Offset"
#define MTR_OFFSET_Y_EN                     "Y-axis Offset"
#define MTR_OFFSET_Z_EN                     "Z-axis Offset"
#define MTR_OFFSET_Z_ADD_EN                 "Lift Nozzle"
#define MTR_OFFSET_Z_DEC_EN                 "Drop Nozzle"

#define MTR_FLASH_TITLE_EN                  "Factory Setup"
#define MTR_FLASH_RESET_EN                  "Restore factory settings"
#define MTR_FLASH_RESET_TIPS_EN             "Restore factory settings?"
#define MTR_FLASH_STORE_EN                  "Save settings to the printer"
#define MTR_FLASH_STORE_TIPS_EN             "Save settings to the printer?"

#define MTR_ADVSET_EN                       "Advanced Set."
#define MTR_ADVSET_ACCELERATION_EN          "Acceleration Settings"
#define MTR_ADVSET_ACCEL_PRINT_EN           "Print Acceleration"
#define MTR_ADVSET_ACCEL_RETRACT_EN         "Retract Acceleration"
#define MTR_ADVSET_ACCEL_TRAVEL_EN          "Travel Acceleration"
#define MTR_ADVSET_ACCEL_X_AXIS_EN          MTR_AXIS_X " Acceleration"
#define MTR_ADVSET_ACCEL_Y_AXIS_EN          MTR_AXIS_Y " Acceleration"
#define MTR_ADVSET_ACCEL_Z_AXIS_EN          MTR_AXIS_Z " Acceleration"
#define MTR_ADVSET_ACCEL_E_AXIS_EN          MTR_AXIS_E " Acceleration"
#define MTR_ADVSET_MAX_SPEED_EN             "Maximum Speed Settings"
#define MTR_ADVSET_MAX_SPEED_X_AXIS_EN      MTR_AXIS_X " Maximum Speed"
#define MTR_ADVSET_MAX_SPEED_Y_AXIS_EN      MTR_AXIS_Y " Maximum Speed"
#define MTR_ADVSET_MAX_SPEED_Z_AXIS_EN      MTR_AXIS_Z " Maximum Speed"
#define MTR_ADVSET_MAX_SPEED_E_AXIS_EN      MTR_AXIS_E " Maximum Speed"
#define MTR_ADVSET_JERK_EN                  "Jerk Settings"
#define MTR_ADVSET_JERK_X_AXIS_EN           MTR_AXIS_X " Jerk"
#define MTR_ADVSET_JERK_Y_AXIS_EN           MTR_AXIS_Y " Jerk"
#define MTR_ADVSET_JERK_Z_AXIS_EN           MTR_AXIS_Z " Jerk"
#define MTR_ADVSET_JERK_E_AXIS_EN           MTR_AXIS_E " Jerk"
#define MTR_ADVSET_STEP_EN                  "Steps Settings"
#define MTR_ADVSET_STEP_X_AXIS_EN           MTR_AXIS_X " Steps"
#define MTR_ADVSET_STEP_Y_AXIS_EN           MTR_AXIS_Y " Steps"
#define MTR_ADVSET_STEP_Z_AXIS_EN           MTR_AXIS_Z " Steps"
#define MTR_ADVSET_STEP_E_AXIS_EN           MTR_AXIS_E " Steps"
#define MTR_ADVSET_PAUSE_POSITION_EN        "Print Pause Position Settings"
#define MTR_ADVSET_PAUSE_POSITION_X_EN      MTR_AXIS_X "(Absolute Position)"
#define MTR_ADVSET_PAUSE_POSITION_Y_EN      MTR_AXIS_Y "(Absolute Position)"
#define MTR_ADVSET_PAUSE_POSITION_Z_EN      MTR_AXIS_Z "(Relative Position)"
#define MTR_ADVSET_PAUSE_POSITION_TIPS_EN   "(Tips: -1 invalid.)"
#define MTR_ADVSET_FILAMENT_EN              "Filsment Settings"
#define MTR_ADVSET_FILAMENT_IN_LENGTH_EN    "Load Lenght"
#define MTR_ADVSET_FILAMENT_IN_SPEED_EN     "Load Speed"
#define MTR_ADVSET_FILAMENT_OUT_LENGTH_EN   "Unload Lenght"
#define MTR_ADVSET_FILAMENT_OUT_SPEED_EN    "Unload Speed"
#define MTR_ADVSET_EHEATING_MODE_EN         "Heating Mode Settings"
#define MTR_ADVSET_THERMALPROTECTION_EN     "Thermal Protection Settings"

#define MTR_EHEATMODE_NORMAL_EN             "Normal\n Mode"
#define MTR_EHEATMODE_HIGH_EN               "High Temp.\n    Mode"
#define MTR_EHEATMODE_CUR_NORMAL_EN         "Current Heating Mode:\n     Normal Mode"
#define MTR_EHEATMODE_CUR_HIGH_EN           "Current Heating Mode:\nHigh Temperature Mode"
#define MTR_EHEATMODE_TIPS_NORMAL_EN        "  click <confirm> for switching to\n        Normal Heating Mode.\n(This mode needs to be configured\n     with normal nozzle module)"
#define MTR_EHEATMODE_TIPS_HIGH_EN          "    click <confirm> for switching to\n         High Temperature Mode.\n  (This mode needs to be configured\nwith high temperature nozzle module)"
#define MTR_EHEATMODE_TIPS_EN               "High Temperature Mode"
#define MTR_EHEATMODE_TIPS_ABB_EN           "HIGH Temp."

#define MTR_ABOUT_EN                        "About"

#define MTR_LANGUAGE_EN                     "Language"

#define MTR_FILDET_EN                       "Filament Detector"
#define MTR_FILDET_ON_EN                    "Filament\nDetector:On"
#define MTR_FILDET_OFF_EN                   "Filament\nDetector:Off"
#define MTR_FILDET_TIPS_PAUSE_EN            "Filament abnormal, pausing..."
#define MTR_FILDET_CLOG_EN                  "Filament Detector: Filament abnormal"
#define MTR_FILDET_TIPS_CLOG_EN             "  The filament is clogged,\n   please clear the nozzle.\nClick <confirm> to return!"
#define MTR_FILDET_TIPS_HOW_EN              "Extrude the filament\nor continue printing?"
#define MTR_FILDET_PURGE_EN                 "Extrude"
#define MTR_FILDET_PRINT_EN                 "Print"

#define MTR_DEBUG_DEVICE_TITLE_EN           "Self Check"
#define MTR_DEBUG_DEVICE_CONFIRM_EN         "Check"
#define MTR_DEBUG_DEVICE_WORKING_EN         "Checking, please wait."
#define MTR_DEBUG_DEVICE_TIPS_E_TEMP_EN     "Nozzle Temperature"
#define MTR_DEBUG_DEVICE_TIPS_B_TEMP_EN     "Hot Bed Temperature"
#define MTR_DEBUG_DEVICE_TIPS_E_HEAT_EN     "Nozzle Heating"
#define MTR_DEBUG_DEVICE_TIPS_B_HEAT_EN     "Hot Bed Heating"
#define MTR_DEBUG_DEVICE_TIPS_FANS_EN       "Fans"
#define MTR_DEBUG_DEVICE_TIPS_X_AXIS_EN     MTR_AXIS_X " Motor"
#define MTR_DEBUG_DEVICE_TIPS_Y_AXIS_EN     MTR_AXIS_Y " Motor"
#define MTR_DEBUG_DEVICE_TIPS_Z_AXIS_EN     MTR_AXIS_Z " Motor"
#define MTR_DEBUG_DEVICE_TIPS_E_AXIS_EN     MTR_AXIS_E " Motor"
#define MTR_DEBUG_DEVICE_TIPS_SERVO_EN      "3D Touch"
#define MTR_DEBUG_ZAXIS_TITLE_EN            "Z-axis Debug"
#define MTR_DEBUG_ZAXIS_SLOWMODE_EN         " Slow-Speed\n Z-axis Debug"
#define MTR_DEBUG_ZAXIS_FASTMODE_EN         " High-Speed\n Z-axis Debug"
#define MTR_DEBUG_ZAXIS_WORKING_EN          "Z-axis debugging..."

#define MTR_PRINT_AGAIN_EN                  "Print Again"
#define MTR_PRINT_TIPS_SAVEOFFSET_EN        "Babystep has been used, you can use\n<save and exit> to save parameters."
#define MTR_PRINT_TIPS_SAVE_EN              "Save & Exit"
#define MTR_PRINT_PAUSE_EN                  "Pause"
#define MTR_PRINT_RESUME_EN                 "Remuse"
#define MTR_PRINT_STOP_EN                   "Stop"
#define MTR_PRINT_NOFILE_TIPS_EN            "Error:no file, please check it again."

#define MTR_PAGE_PREV_EN                    "Previous"
#define MTR_PAGE_NEXT_EN                    "Next"

#define MTR_NO_KEY_BACK_EN                  "Delete"
#define MTR_NO_KEY_RESET_EN                 "Reset"
#define MTR_NO_KEY_CONFIRM_EN               "Confirm"
