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
#pragma once

//****************英文***************************//
#define MACHINE_CONFIG_EN              "Config"

#define NEXT_EN                 "Next"
#define PREVIOUS_EN             "Previous"
#define DEFAULT_EN              "Default"
#define KEY_BACK_EN             "Delete"
#define KEY_REST_EN             "Reset"
#define KEY_CONFIRM_EN          "Confirm"

#define KEYBOARD_KEY0_EN "0"
#define KEYBOARD_KEY1_EN "1"
#define KEYBOARD_KEY2_EN "2"
#define KEYBOARD_KEY3_EN "3"
#define KEYBOARD_KEY4_EN "4"
#define KEYBOARD_KEY5_EN "5"
#define KEYBOARD_KEY6_EN "6"
#define KEYBOARD_KEY7_EN "7"
#define KEYBOARD_KEY8_EN "8"
#define KEYBOARD_KEY9_EN "9"
#define KEYBOARD_KEY_POINT_EN "."
#define KEYBOARD_KEY_NEGATIVE_EN "-"

#define MACHINE_PARA_TITLE_EN       "Config"
#define MACHINE_TYPE_CNOFIG_EN      "Machine Settings"
#define MOTOR_CONFIG_EN             "Motor Settings"
#define MACHINE_LEVELING_CONFIG_EN  "Leveling Settings"
#define ADVANCE_CONFIG_EN           "Advanced Settings"

#define MACHINE_CONFIG_TITLE_EN     "Machine Settings"
#define MACHINE_TYPE_EN             "Machine Type"
#define MACHINE_STROKE_EN           "Machine Size"
#define MACHINE_HOMEDIR_EN          "Home Direction"
#define MACHINE_ENDSTOP_TYPE_EN     "Endstop Type"
#define MACHINE_FILAMENT_CONFIG_EN  "Filament Settings"

#define MACHINE_TYPE_CONFIG_TITLE_EN    "Machine Type"
#define MACHINE_TYPE_XYZ_EN             "XYZ Machine"
#define MACHINE_TYPE_DELTA_EN           "Delta Machine"
#define MACHINE_TYPE_COREXY_EN          "Corexy Machine"

#define MACHINE_STROKE_CONF_TITLE_EN    "Machine Size"
#define X_MAX_LENGTH_EN                 "X-axis Maximum Stroke"
#define Y_MAX_LENGTH_EN                 "Y-axis Maximum Stroke"
#define Z_MAX_LENGTH_EN                 "Z-axis Maximum Stroke"

#define X_MIN_LENGTH_EN                 "X-axis Minimum Stroke"
#define Y_MIN_LENGTH_EN                 "Y-axis Minimum Stroke"
#define Z_MIN_LENGTH_EN                 "Z-axis Minimum Stroke"

#define HOME_DIR_CONF_TITLE_EN          "Home Direction"
#define HOME_DIR_X_EN                   "X-axis Home Direction"
#define HOME_DIR_Y_EN                   "Y-axis Home Direction"
#define HOME_DIR_Z_EN                   "Z-axis Home Direction"
#define HOME_MIN_EN                     "MIN"
#define HOME_MAX_EN                     "MAX"

#define ENDSTOP_CONF_TITLE_EN           "Endstop Type"
#define MIN_ENDSTOP_X_EN                "X-axis Minimum Endstop"
#define MIN_ENDSTOP_Y_EN                "Y-axis Minimum Endstop"
#define MIN_ENDSTOP_Z_EN                "Z-axis Minimum Endstop"
#define MAX_ENDSTOP_X_EN                "X-axis Maximum Endstop"
#define MAX_ENDSTOP_Y_EN                "Y-axis Maximum Endstop"
#define MAX_ENDSTOP_Z_EN                "Z-axis Maximum Endstop"
#define ENDSTOP_FIL_EN                  "Filament Sensor"
#define ENDSTOP_LEVEL_EN                "Leveling Sensor"
#define ENDSTOP_OPENED_EN               "Open"
#define ENDSTOP_CLOSED_EN               "Close"

#define FILAMENT_CONF_TITLE_EN          "Filament Settings"
#define FILAMENT_IN_LENGTH_EN           "Load Length"
#define FILAMENT_IN_SPEED_EN            "Load Speed"
#define FILAMENT_TEMPERATURE_EN         "Filament Temperature"
#define FILAMENT_OUT_LENGTH_EN          "Unload Length"
#define FILAMENT_OUT_SPEED_EN           "Unload Speed"

#define LEVELING_CONF_TITLE_EN          "Leveling Settings"
#define LEVELING_PARA_CONF_EN           "Leveling Settings"
#define LEVELING_MANUAL_POS_EN          "Manual Leveling Settings"
#define LEVELING_AUTO_COMMAND_EN        "Auto-leveling Settings"
#define LEVELING_AUTO_ZOFFSET_EN        "Auto-leveling Settings"

#define LEVELING_PARA_CONF_TITLE_EN     "Leveling Setting"
#define AUTO_LEVELING_ENABLE_EN         "Enable Auto-leveling"
#define BLTOUCH_LEVELING_ENABLE_EN      "Enable BLtouch"
#define PROBE_PORT_EN                   "Probe Connector"
#define PROBE_X_OFFSET_EN               "Probe X-axis Offset"
#define PROBE_Y_OFFSET_EN               "Probe Y-axis Offset"
#define PROBE_Z_OFFSET_EN               "Probe Z-axis Offset"
#define PROBE_XY_SPEED_EN               "Probe XY-axis Apeed"
#define PROBE_Z_SPEED_EN                "Probe Z-axis Apeed"
#define ENABLE_EN                       "Yes"
#define DISABLE_EN                      "No"
#define LOCKED_EN                       "N/A"
#define Z_MIN_EN                        "ZMin"
#define Z_MAX_EN                        "ZMax"

#define DELTA_LEVEL_CONF_TITLE_EN       "Delta Machine Settings"
#define DELTA_LEVEL_CONF_EN             "Delta Machine Leveling"
#define DELTA_MACHINE_RADIUS_EN         "Machine Radius"
#define DELTA_DIAGONAL_ROD_EN           "Machine Rod Length"
#define DELTA_PRINT_RADIUS_EN           "Print Radius"
#define DELTA_HEIGHT_EN                 "Print Height"
#define SMOOTH_ROD_OFFSET_EN            "Slider Offset"
#define EFFECTOR_OFFSET_EN              "Effector Offset"
#define CALIBRATION_RADIUS_EN           "Leveling Radius"

#define XYZ_LEVEL_CONF_TITLE_EN         "Cartesian Machine Settings"
#define PROBE_REACH_MAX_LEFT_EN         "Probe Reaches Leftmost Position"
#define PROBE_REACH_MAX_RIGHT_EN        "Probe Reaches Rightmost Position"
#define PROBE_REACH_MAX_FRONT_EN        "Probe Reaches Front Position"
#define PROBE_REACH_MAX_BACK_EN         "Probe Reaches Final Position"

#define TEMPERATURE_CONF_TITLE_EN       "Temperature Settings"
#define NOZZLE_CONF_EN                  "Nozzle Settings"
#define HOTBED_CONF_EN                  "Hotbed Settings"
#define PREHEAT_TEMPER_EN               "Preset Temperature"

#define NOZZLE_CONF_TITLE_EN            "Nozzle Settings"
#define NOZZLECNT_EN                    "Number of Nozzles"
#define NOZZLE_TYPE_EN                  "E0 Temperature Type"
#define NOZZLE_ADJUST_TYPE_EN           "PID Thermostat"
#define NOZZLE_MIN_TEMPERATURE_EN       "Lowest Temperature"
#define NOZZLE_MAX_TEMPERATURE_EN       "Maximum Temperature"
#define EXTRUD_MIN_TEMPER_EN            "Minimum Extrusion Temperature"

#define HOTBED_CONF_TITLE_EN            "Hotbed Settings"
#define HOTBED_ADJUST_EN                "PID Thermostat"
#define HOTBED_MIN_TEMPERATURE_EN       "Lowest Temperature"
#define HOTBED_MAX_TEMPERATURE_EN       "Maximum Temperature"

#define MOTOR_CONF_TITLE_EN             "Motor Settings"
#define MAXFEEDRATE_CONF_EN             "Maximum Speed Settings"
#define ACCELERATION_CONF_EN            "Acceleration Settings"
#define JERKCONF_EN                     "Jerk Speed Settings"
#define STEPSCONF_EN                    "Steps Settings"
#define TMC_CURRENT_EN                  "TMC Current Settings"
#define TMC_STEP_MODE_EN                "TMC Step Mode Settings"
#define MOTORDIRCONF_EN                 "Motor Direction Settings"
#define HOMEFEEDRATECONF_EN             "Home Speed Setting"

#define MAXFEEDRATE_CONF_TITLE_EN       "Maximum Speed"
#define X_MAXFEEDRATE_EN                "X-axis Maximum Speed"
#define Y_MAXFEEDRATE_EN                "Y-axis Maximum Speed"
#define Z_MAXFEEDRATE_EN                "Z-axis Maximum Speed"
#define E0_MAXFEEDRATE_EN               "E-axis Maximum Speed"
#define E1_MAXFEEDRATE_EN               "E-axis Maximum Speed"

#define ACCELERATION_CONF_TITLE_EN      "Acceleration"
#define PRINT_ACCELERATION_EN           "Print Acceleration"
#define RETRACT_ACCELERATION_EN         "Retraction Acceleration"
#define TRAVEL_ACCELERATION_EN          "Travel Acceleration"
#define X_ACCELERATION_EN               "X-axis Acceleration"
#define Y_ACCELERATION_EN               "Y-axis Acceleration"
#define Z_ACCELERATION_EN               "Z-axis Acceleration"
#define E0_ACCELERATION_EN              "E-axis Acceleration"
#define E1_ACCELERATION_EN              "E-axis Acceleration"

#define JERK_CONF_TITLE_EN              "Jerk Speed"
#define X_JERK_EN                       "X-axis Jerk Speed"
#define Y_JERK_EN                       "Y-axis Jerk Speed"
#define Z_JERK_EN                       "Z-axis Jerk Speed"
#define E_JERK_EN                       "E-axis Jerk Speed"

#define STEPS_CONF_TITLE_EN             "Steps Settings"
#define X_STEPS_EN                      "X-axis Steps"
#define Y_STEPS_EN                      "Y-axis Steps"
#define Z_STEPS_EN                      "Z-axis Steps"
#define E0_STEPS_EN                     "E-axis Steps"
#define E1_STEPS_EN                     "E-axis Steps"

#define TMC_CURRENT_CONF_TITLE_EN       "TMC Current Settings"
#define X_TMC_CURRENT_EN                "X-axis Current (mA)"
#define Y_TMC_CURRENT_EN                "Y-axis Current (mA)"
#define Z_TMC_CURRENT_EN                "Z-axis Current (mA)"
#define E0_TMC_CURRENT_EN               "E-axis Current (mA)"
#define E1_TMC_CURRENT_EN               "E-axis Current (mA)"

#define TMC_MODE_CONF_TITLE_EN          "TMC step mode settings"
#define X_TMC_MODE_EN                   "X-axis Ultra Quiet Stepping Mode"
#define Y_TMC_MODE_EN                   "Y-axis Ultra Quiet Stepping Mode"
#define Z_TMC_MODE_EN                   "Z-axis Ultra Quiet Stepping Mode"
#define E0_TMC_MODE_EN                  "E-axis Ultra Quiet Stepping Mode"
#define E1_TMC_MODE_EN                  "E-axis Ultra Quiet Stepping Mode"

#define MOTORDIR_CONF_TITLE_EN          "Motor Direction"
#define X_MOTORDIR_EN                   "X-axis Motor Direction Invert"
#define Y_MOTORDIR_EN                   "Y-axis Motor Direction Invert"
#define Z_MOTORDIR_EN                   "Z-axis Motor Direction Invert"
#define E0_MOTORDIR_EN                  "E-axis Motor Direction Invert"
#define E1_MOTORDIR_EN                  "E-axis Motor Direction Invert"
#define INVERT_P_EN                     "Yes"
#define INVERT_N_EN                     "No"

#define HOMEFEEDRATE_CONF_TITLE_EN      "Home Speed"
#define X_HOMESPEED_EN                  "XY-axis Home Speed"
#define Y_HOMESPEED_EN                  "Y-axis Home Speed"
#define Z_HOMESPEED_EN                  "Z-axis Home Speed"

#define ADVANCED_CONF_TITLE_EN          "Advance"
#define PWROFF_DECTION_EN               "Power off Dection Module"
#define PWROFF_AFTER_PRINT_EN           "Auto Shutdown After Print"
#define HAVE_UPS_EN                     "Has UPS Power Supply"
#define Z2_AND_Z2ENDSTOP_CONF_EN        "Z2 Settings"
#define ENABLE_PINS_CONF_EN             "Enable Pins Level Settings"
#define WIFI_SETTINGS_EN                "Wi-Fi Parameter Settings"
#define HOMING_SENSITIVITY_CONF_EN      "Homing Sensitivity Settings"
#define ENCODER_SETTINGS_EN             "Rotary Encoder Settings"

#define Z2_AND_Z2ENDSTOP_CONF_TITLE_EN  "Z2 Settings"
#define Z2_ENABLE_EN                    "Z2 Enable"
#define Z2_ENDSTOP_EN                   "Z2_EndStop Enable"
#define Z2_PORT_EN                      "Z2 Connector"

#define ENABLE_PINS_CONF_TITLE_EN       "ENABLE_PINS_LEVEL"
#define X_ENABLE_PINS_INVERT_EN         "X_ENABLE_PIN_INVERT"
#define Y_ENABLE_PINS_INVERT_EN         "Y_ENABLE_PIN_INVERT"
#define Z_ENABLE_PINS_INVERT_EN         "Z_ENABLE_PIN_INVERT"
#define E_ENABLE_PINS_INVERT_EN         "E_ENABLE_PIN_INVERT"

#define PAUSE_POSITION_EN    "Printing Pause Settings"
#define PAUSE_POSITION_X_EN  "X-axis Position (Absolute position,-1 invalid)"
#define PAUSE_POSITION_Y_EN  "Y-axis Position (Absolute position,-1 invalid)"
#define PAUSE_POSITION_Z_EN  "Z-axis Position (Relative position,-1 invalid)"

#define WIFI_SETTINGS_TITLE_EN    "WiFi Parameter"
#define WIFI_SETTINGS_MODE_EN     "WiFi Mode"
#define WIFI_SETTINGS_NAME_EN     "WiFi Name: "
#define WIFI_SETTINGS_PASSWORD_EN "WiFi Password: "
#define WIFI_SETTINGS_CLOUD_EN    "Do you use cloud services?"
#define WIFI_SETTINGS_CONFIG_EN   "Config"
#define WIFI_SETTINGS_EDIT_EN     "Edit"
#define WIFI_CONFIG_TIPS_EN       "WiFi Configuration?"

#define OFFSET_TITLE_EN  "Offset"
#define OFFSET_X_EN      "X-axis Offset"
#define OFFSET_Y_EN      "Y-axis Offset"
#define OFFSET_Z_EN      "Z-axis Offset"

#define HOMING_SENSITIVITY_CONF_TITLE_EN      "Sensitivity"
#define X_SENSITIVITY_EN                      "X-axis Sensitivity"
#define Y_SENSITIVITY_EN                      "Y-axis Sensitivity"
#define Z_SENSITIVITY_EN                      "Z-axis Sensitivity"
#define Z2_SENSITIVITY_EN                     "Z2-axis Sensitivity"

#define ENCODER_CONF_TITLE_EN                 "Rotary Encoder Settings"
#define ENCODER_CONF_TEXT_EN                  "Is the encoder function used?"

#define TOOL_TEXT_EN            "Tool"
#define PREHEAT_TEXT_EN         "Preheat"
#define MOVE_TEXT_EN            "Move"
#define HOME_TEXT_EN            "Home"
#define PRINT_TEXT_EN           "Print"
#define EXTRUDE_TEXT_EN         "Extruder"
#define LEVELING_TEXT_EN        "Leveling"
#define AUTO_LEVELING_TEXT_EN   "Auto-leveling"
#define SET_TEXT_EN             "Settings"
#define MORE_TEXT_EN            "More"

#define ADD_TEXT_EN             "Add"
#define DEC_TEXT_EN             "Dec"
#define EXTRUDER_1_TEXT_EN      "Extruder"
#define EXTRUDER_2_TEXT_EN      "Extruder"
#define HEATBED_TEXT_EN         "Hotbed"
#define TEXT_1C_EN              "1℃"
#define TEXT_5C_EN              "5℃"
#define TEXT_10C_EN             "10℃"
#define CLOSE_TEXT_EN           "Close"

#define BACK_TEXT_EN            "Back"

#define TOOL_PREHEAT_EN         "Preheat"
#define TOOL_EXTRUDE_EN         "Extruder"
#define TOOL_MOVE_EN            "Move"
#define TOOL_HOME_EN            "Home"
#define TOOL_LEVELING_EN        "Leveling"
#define TOOL_AUTO_LEVELING_EN   "Auto-leveling"
#define TOOL_FILAMENT_EN        "Filament"
#define TOOL_MORE_EN            "More"

#define AXIS_X_ADD_TEXT_EN      "X+"
#define AXIS_X_DEC_TEXT_EN      "X-"
#define AXIS_Y_ADD_TEXT_EN      "Y+"
#define AXIS_Y_DEC_TEXT_EN      "Y-"
#define AXIS_Z_ADD_TEXT_EN      "Z+"
#define AXIS_Z_DEC_TEXT_EN      "Z-"
#define TEXT_01MM_EN            "0.1mm"
#define TEXT_1MM_EN             "1mm"
#define TEXT_10MM_EN            "10mm"

#define HOME_X_TEXT_EN          "X"
#define HOME_Y_TEXT_EN          "Y"
#define HOME_Z_TEXT_EN          "Z"
#define HOME_ALL_TEXT_EN        "Home"
#define HOME_STOPMOVE_EN        "Quickstop"

#define PAGE_UP_TEXT_EN         "Page Up"
#define PAGE_DOWN_TEXT_EN       "Page Down"

#define EXTRUDER_IN_TEXT_EN           "In"
#define EXTRUDER_OUT_TEXT_EN          "Out"
#define EXTRUDE_1MM_TEXT_EN           "1 mm"
#define EXTRUDE_5MM_TEXT_EN           "5 mm"
#define EXTRUDE_10MM_TEXT_EN          "10 mm"
#define EXTRUDE_LOW_SPEED_TEXT_EN     "Low"
#define EXTRUDE_MEDIUM_SPEED_TEXT_EN  "Normal"
#define EXTRUDE_HIGH_SPEED_TEXT_EN    "High"

#define LEVELING_POINT1_TEXT_EN       "Point 1"
#define LEVELING_POINT2_TEXT_EN       "Point 2"
#define LEVELING_POINT3_TEXT_EN       "Point 3"
#define LEVELING_POINT4_TEXT_EN       "Point 4"
#define LEVELING_POINT5_TEXT_EN       "Point 5"

#define FILESYS_TEXT_EN               "File System"
#define WIFI_TEXT_EN                  "WiFi"
#define FAN_TEXT_EN                   "Fan"
#define ABOUT_TEXT_EN                 "About"
#define BREAK_POINT_TEXT_EN           "Continue"
#define FILAMENT_TEXT_EN              "Filament"
#define LANGUAGE_TEXT_EN              "Language"
#define MOTOR_OFF_TEXT_EN             "Motor-off"
#define MOTOR_OFF_XY_TEXT_EN          "Off-XY"
#define SHUTDOWN_TEXT_EN              "Shutdown"
#define MACHINE_PARA_EN               "Config"
#define EEPROM_SETTINGS_EN            "Factory Setting"//"Eeprom\nSettings"

#define U_DISK_TEXT_EN                "USB"
#define SD_CARD_TEXT_EN               "SD"
#define WIFI_NAME_TEXT_EN             "WiFi: "
#define WIFI_KEY_TEXT_EN              "Key: "
#define WIFI_IP_TEXT_EN               "IP: "
#define WIFI_AP_TEXT_EN               "State: AP"
#define WIFI_STA_TEXT_EN              "State: STA"
#define WIFI_CONNECTED_TEXT_EN        "Connected"
#define WIFI_DISCONNECTED_TEXT_EN     "Disconnected"
#define WIFI_EXCEPTION_TEXT_EN        "Exception"
#define WIFI_RECONNECT_TEXT_EN        "Reconnect"
#define CLOUD_TEXT_EN                 "Cloud"
#define CLOUD_BIND_EN                 "Bind"
#define CLOUD_UNBIND_EN               "Unbind"
#define CLOUD_UNBINDING_EN            "Unbinding"
#define CLOUD_DISCONNECTED_EN         "Disconnected"
#define CLOUD_UNBINDED_EN             "Unbinded"
#define CLOUD_BINDED_EN               "Binded"
#define CLOUD_DISABLE_EN              "Disable"

#define FAN_ADD_TEXT_EN               "Add"
#define FAN_DEC_TEXT_EN               "Dec"
#define FAN_OPEN_TEXT_EN              "100%"
#define FAN_HALF_TEXT_EN              "50%"
#define FAN_CLOSE_TEXT_EN             "Close"
#define FAN_TIPS1_TEXT_EN             "Fan"
#define FAN_TIPS2_TEXT_EN             "Fan\nClose"

#define FILAMENT_IN_TEXT_EN           "Load"
#define FILAMENT_OUT_TEXT_EN          "Unload"
#define FILAMENT_EXT0_TEXT_EN         "Extruder"
#define FILAMENT_EXT1_TEXT_EN         "Extruder"
#define FILAMENT_HEAT_TEXT_EN         "Preheat"
#define FILAMENT_STOP_TEXT_EN         "Stop"
#define FILAMENT_TIPS2_TEXT_EN        "T:"
#define FILAMENT_TIPS3_TEXT_EN        "Loading..."
#define FILAMENT_TIPS4_TEXT_EN        "Unloading..."
#define FILAMENT_TIPS5_TEXT_EN        "Low temperature, please heat up"
#define FILAMENT_TIPS6_TEXT_EN        "Completed"

#define FILAMENT_CHANGE_TEXT_EN                 "Please click <Load> \nor <unload>, After \npinter pause."
#define FILAMENT_DIALOG_LOAD_HEAT_TIPS_EN       "The nozzle is heating,\n       please wait..."
#define FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_EN     "The nozzle is heating,\n       please wait..."
#define FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_EN   "    Heating is completed, please\n   load filament to extruder, and\n click <confirm> to start loading."
#define FILAMENT_DIALOG_LOAD_CONFIRM2_TIPS_EN   "   Please load filament to extruder,\n   and click <confirm> to start loading."
#define FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_EN  "Heat completed,please \nclick <confirm> to start unloading.!"
#define FILAMENT_DIALOG_LOADING_TIPS_EN         "Loading, please wait!"
#define FILAMENT_DIALOG_UNLOADING_TIPS_EN       "Unloading, please wait!"
#define FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_EN   "Load filament completed,\nclick <confirm> to return!"
#define FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_EN "Unload filament completed,\n click <confirm> to return!"


#define PRE_HEAT_EXT_TEXT_EN            "E"
#define PRE_HEAT_BED_TEXT_EN            "Bed"

#define FILE_LOADING_EN                 "Loading..."
#define NO_FILE_AND_CHECK_EN          "  No files found!\n            Check the file system configuration!"

#define NO_FILE_EN                          "No files found!"

#define EXTRUDER_TEMP_TEXT_EN               "Temp."
#define EXTRUDER_E_LENGTH1_TEXT_EN          "Extruder"
#define EXTRUDER_E_LENGTH2_TEXT_EN          "Extruder"
#define EXTRUDER_E_LENGTH3_TEXT_EN          "Extruder"

#define ABOUT_TYPE_TEXT_EN                  "Type: "
#define ABOUT_VERSION_TEXT_EN               "Firmware: "
#define ABOUT_WIFI_TEXT_EN                  "WiFi: "

#define PRINTING_OPERATION_EN               "Operation"
#define PRINTING_PAUSE_EN                   "Pause"
#define PRINTING_TEMP_EN                    "Temp."
#define PRINTING_CHANGESPEED_EN             "Speed"
#define PRINTING_RESUME_EN                  "Resume"
#define PRINTING_STOP_EN                    "Stop"
#define PRINTING_MORE_EN                    "More"
#define PRINTING_EXTRUDER_EN                "Extrusion"
#define PRINTING_MOVE_EN                    "Move"

#define EXTRUDER_SPEED_EN                   "Extrusion"
#define MOVE_SPEED_EN                       "Move"
#define EXTRUDER_SPEED_STATE_EN             "Extruded Speed"
#define MOVE_SPEED_STATE_EN                 "Move Speed"
#define STEP_1PERCENT_EN                    "1%"
#define STEP_5PERCENT_EN                    "5%"
#define STEP_10PERCENT_EN                   "10%"

#define TITLE_READYPRINT_EN                 "Ready to print"
#define TITLE_PREHEAT_EN                    "Preheat"
#define TITLE_MOVE_EN                       "Move"
#define TITLE_HOME_EN                       "Home"
#define TITLE_EXTRUDE_EN                    "Extruder"
#define TITLE_LEVELING_EN                   "Leveling"
#define TITLE_SET_EN                        "Settings"
#define TITLE_MORE_EN                       "More"
#define TITLE_CHOOSEFILE_EN                 "Choose File"
#define TITLE_PRINTING_EN                   "Printing"
#define TITLE_OPERATION_EN                  "Operation"
#define TITLE_ADJUST_EN                     "Adjustment"
#define TITLE_WIRELESS_EN                   "Wireless"
#define TITLE_FILAMENT_EN                   "Filament"
#define TITLE_ABOUT_EN                      "About"
#define TITLE_FAN_EN                        "Fan"
#define TITLE_LANGUAGE_EN                   "Language"
#define TITLE_PAUSE_EN                      "Pause"
#define TITLE_CHANGESPEED_EN                "Speed"
#define TITLE_CLOUD_TEXT_EN                 "Cloud"
#define TITLE_DIALOG_CONFIRM_EN             "Confirm"
#define TITLE_FILESYS_EN                    "File System"

#define AUTO_SHUTDOWN_EN                    "Auto"
#define MANUAL_SHUTDOWN_EN                  "Manual"

#define DIALOG_CONFIRM_EN                   "Confirm"
#define DIALOG_CANCLE_EN                    "Cancel"
#define DIALOG_OK_EN                        "OK"
#define DIALOG_RESET_EN                     "Reset"
#define DIALOG_DISABLE_EN                   "Disable"
#define DIALOG_PRINT_MODEL_EN               "Print this model?"
#define DIALOG_CANCEL_PRINT_EN              "Stop printing?"
#define DIALOG_RETRY_EN                     "Retry"
#define DIALOG_STOP_EN                      "Stop"
#define DIALOG_REPRINT_FROM_BREAKPOINT_EN   " ?"
#define DIALOG_ERROR_TIPS1_EN               "Error:no file, please check it again."
#define DIALOG_ERROR_TIPS2_EN               "Error:transaction failed. please make sure the display baud rate\nsame as mainboard!"
#define DIALOG_ERROR_TIPS3_EN               "Error:file name or path is too long!"
#define DIALOG_CLOSE_MACHINE_EN             "Closing machine......"
#define DIALOG_UNBIND_PRINTER_EN            "Unbind the printer?"
#define DIALOG_FILAMENT_NO_PRESS_EN         "Filament detection switch is not pressed"
#define DIALOG_PRINT_FINISH_EN              "Done!"
#define DIALOG_PRINT_TIME_EN                "Printing time: "
#define DIALOG_REPRINT_EN                   "Print again"
#define DIALOG_WIFI_ENABLE_TIPS_EN          "The wifi module is being configured\nplease wait a moment....."
#define DIALOG_PAUSING_TIPS_EN              "Pausing..."

#define HOTBED_ENABLE_EN        "Enable Hotbed"
#define MOTOR_EN_HIGH_LEVEL_EN  "High"
#define MOTOR_EN_LOW_LEVEL_EN   "Low"

#define TEXT_WIFI_MENU_TITLE_EN "WIFI"
#define TEXT_WIFI_SAPCE_EN      "space"
#define TEXT_WIFI_LETTER_EN     "abc"
#define TEXT_WIFI_DIGITAL_EN    "123"
#define TEXT_WIFI_SYMBOL_EN     "#+="
#define TEXT_WIFI_PASSWORD_EN   "Password"

#define TEXT_WIFI_JOINING_EN  "Joining Network..."
#define TEXT_WIFI_FAILED_JOIN_EN  "Failed to Join WiFi"
#define TEXT_WIFI_WIFI_CONECTED_EN "WiFi Connected"

#define TEXT_BUTTON_DISCONECTED_EN  "Disconnect"
#define TEXT_WIFI_FORGET_EN         "Forget Network"
#define TEXT_DISCONECTED_EN         "WiFi Connected"

//wifi-list
#define MAIN_BUILT_EN       "Build"
#define MAIN_FILAMENT_EN    "Filament"
#define MAIN_SETUP_EN       "Setup"
#define MAIN_ABOUT_EN       "About"
#define MAIN_MENU_EN        "Menu"
#define FILE_MENU_BUILD_EN  "Build"
#define FILE_MENU_MENU_EN   " < Menu"

//about
#define ABOUT_TITLE_EN                    "About"
#define ABOUT_BUILT_MACHINES_EN           "Built Machines"
#define ABOUT_SPARK_EN                    "Spark"
#define ABOUT_VERSION_EN                  "Version 1.1.0"
#define ABOUT_SERIAL_NUMBER_EN            "Serial Number:"
#define ABOUT_S_NUMBER_EN                 "DCPLX02KFC6P"

//set
#define SETUP_TITLE_EN                    "Setup"
#define SETUP_WIFI_EN                     "WiFi"
#define SETUP_MANUAL_IP_EN                "Manual IP"
#define SETUP_WIFI_NOT_CONNECTED_EN       "Not Connected"
#define SETUP_WIFI_NETWORK_EN             "WiFi_Network"

//build
#define BUILD_TITLE_EN                    "Build"
#define BUILD_SD_CARD_EN                  "SD Card"
#define BUILD_USB_DRIVE_EN                "USB Drive"

//SD card
#define SD_CARD_TITLE_EN                  "SD Card"
#define SD_CARD_BACK_EN                   "< Back"
//USB Drive
#define USB_DRIVE_TITLE_EN                "USB Drive"
#define USB_DRIVE_BACK_EN                 "< Back"
#define FILE_PAGES_EN                     "%d/%d"
#define FILE_NEXT_PAGE_EN                 "Next Page"

//BUILD PLATE
#define PLATE_TITLE_EN                    "Build Plate"
#define PLATE_BACK_EN                     "< Back"
#define PLATE_CONFIRM_EN                  "Confirm >"
#define PLATE_TIPS_EN                     "Confirm that there is a Clear\nBuild Plate installed in the\nmachine."

//build model
#define MODEL_TITLE_EN                    "Build Model"
#define MODEL_START_BUILD_EN              "Start Build"
#define MODEL_BACK_EN                     "< Back"

//building
#define BUILDING_TITLE_EN                 "Building"
#define BUILDING_MENU_EN                  "Build Menu"
#define BUILDING_COMPLETED                "Build\nComplete"

//building menu
#define BUILDING_MENU_TITLE_EN            "Build Menu"
#define BUILDING_MENU_SETTINGS_EN         "Build Settings"
#define BUILDING_MENU_PAUSE_EN            "Pause Build"
#define BUILDING_MENU_CANCEL_EN           "Cancel Build"
#define BUILDING_MENU_BACK_EN             "< Back"

//build settings
#define SETTINGS_TITLE_EN                 "Build Settings"
#define SETTINGS_NOZZLE_TEMPER_EN         "Nozzle Temp:"
#define SETTINGS_NOZZLE_VALUE_EN          "%d"
#define SETTINGS_BED_TEMPER_EN            "Bed Temp:"
#define SETTINGS_BED_VALUE_EN             "%d"
#define SETTINGS_BUILD_SPEED_EN           "Build Speed:"
#define SETTINGS_SPEED_VALUE_EN           "Standard"
#define SETTINGS_BACK_EN                  "< Back"

//build paused
#define PAUSED_TITLE_EN                   "Build Paused"
#define PAUSED_RESUME_EN                  "Resume Build"
#define PAUSED_CANCEL_EN                  "Cancel Build"
#define PAUSED_BACK_EN                    "< Back"

//build cancel
#define CANCEL_TITLE_EN                   "Cancel Build"
#define CANCEL_BUILD_EN                   "Cancel Build"
#define CANCEL_TIPS_EN                    "Are you sure you want to\ncancel this build? The model\nwill be deleted from this\nmachine. It will need to be\nresent from your computer\nbefore it can be built in the\nfuture."
#define CANCEL_BACK_EN                    "< Back"
#define CANCEL_BUILD_DISPLAY_EN           "Build\nCanceled"
#define CANCEL_OVER_PLATE_TIPS_EN         "Confirm that the Build Plate\nhas been removed from the\nmachine."

//filament model enter
#define FILAMENT_MODEL_ENTER_TITLE_EN     "Model-PLA"
#define FILAMENT_MODEL_ENTER_BACK_EN      "< Back"
#define FILAMENT_MODEL_ENTER_BEGIN_EN     "Begin >"
#define FILAMENT_MODEL_ENTER_TIPS_EN      "The Model Filament spool\ncompartment is located on\nthe right side of the machine."

//filament model PLA
#define FILAMENT_MODEL_PLA_TITLE_EN       "Model-PLA"
#define FILAMENT_PLA_LOAD_TITLE_EN        "Load Filament"
#define FILAMENT_PLA_UNLOAD_TITLE_EN      "Unload Filament"
#define FILAMENT_MODEL_PLA_LOAD_EN        "Load Filament"
#define FILAMENT_MODEL_PLA_UNLOAD_EN      "Unload Filament"
//filament support enter
#define FILAMENT_SUPPORT_ENTER_TITLE_EN   "Support-PVA"
#define FILAMENT_SUPPORT_ENTER_BACK_EN    "< Back"
#define FILAMENT_SUPPORT_ENTER_BEGIN_EN   "Begin >"
#define FILAMENT_SUPPORT_ENTER_TIPS_EN    "The Support Filament spool\ncompartment is located on\nthe left side of the machine."
//filament heating
#define FILAMENT_HEATING_LOAD_TITLE_EN    "Load Filament"
#define FILAMENT_HEATING_UNLOAD_TITLE_EN  "Unload Filament"
#define FILAMENT_HEATING_CANCEL_EN        "< Cancel"
#define FILAMENT_HEATING_MATERIAL_EN      "Material:"
#define FILAMENT_HEATING_PLA_EN           "Model-PLA"
#define FILAMENT_HEATING_TIPS_EN          "Print head is heating..."
//rotate left
#define ROTATE_LEFT_LOAD_TITLE_EN         "Load Filament"
#define ROTATE_LEFT_UNLOAD_TITLE_EN       "Unload Filament"
#define ROTATE_LEFT_CANCEL_EN             "< Cancel"
#define ROTATE_LEFT_MATERIAL_EN           "Material:"
#define ROTATE_LEFT_PLA_EN                "Model-PLA"
#define ROTATE_LEFT_NEXT_EN               "Next >"
#define ROTATE_LEFT_TIPS_EN               "Rotate extruder selection\ndial to the left."

//hang spool
#define HANG_SPOOL_TITLE_EN         "Load Filament"
#define HANG_SPOOL_PREVIOUS_EN      "< Previous"
#define HANG_SPOOL_MATERIAL_EN      "Material:"
#define HANG_SPOOL_PLA_EN           "Model-PLA"
#define HANG_SPOOL_NEXT_EN          "Next >"
#define HANG_SPOOL_TIPS_EN          "Hang the spool in the spool\ncompartment as shown."

//feed filament
#define FEED_FILAMENT_TITLE_EN      "Load Filament"
#define FEED_FILAMENT_PREVIOUS_EN   "< Previous"
#define FEED_FILAMENT_MATERIAL_EN   "Material:"
#define FEED_FILAMENT_PLA_EN        "Model-PLA"
#define FEED_FILAMENT_NEXT_EN       "Next >"
#define FEED_FILAMENT_TIPS_EN       "Feed filament into extruder\nup beyond the gears."

//feed filament
#define ROTATE_UP_TITLE_EN          "Load Filament"
#define ROTATE_UP_PREVIOUS_EN        "< Previous"
#define ROTATE_UP_MATERIAL_EN       "Material:"
#define ROTATE_UP_PLA_EN            "Model-PLA"
#define ROTATE_UP_NEXT_EN           "Next >"
#define ROTATE_UP_TIPS_EN           "Rotate extruder selection\ndial up."

//filament begin
#define FEED_BEGIN_TITLE_EN         "Load Filament"
#define FEED_BEGIN_MATERIAL_EN      "Material:"
#define FEED_BEGIN_PLA_EN           "Model-PLA"
#define FEED_BEGIN_NEXT_EN          "Next >"
#define FEED_BEGIN_TIPS_EN          "Press Next when filament\nbegins to extrude."

//filament finish
#define FEED_FINISH_TITLE_EN        "Load Filament"
#define FEED_FINISH_MATERIAL_EN     "Material:"
#define FEED_FINISH_PLA_EN          "Model-PLA"
#define FEED_FINISH_NEXT_EN         "Finish >"
#define FEED_FINISH_TIPS_EN         "Remove filament from the\nnozzle and discard."
//fiament remove
#define REMOVE_SPOOL_TITLE_EN       "Unload Filament"
#define REMOVE_SPOOL_PREVIOUS_EN     "< Previous"
#define REMOVE_SPOOL_FINISH_EN      "Finish >"
#define REMOVE_SPOOL_MATERIAL_EN    "Material:"
#define REMOVE_SPOOL_PLA_EN         "Model-PLA"
#define REMOVE_SPOOL_TIPS_EN        "Remove the spool and pull\nfilament out of the machine."

#define FILAMENT_SUPPORT_PVA_EN     "Support-PVA"
#define LOAD_FINISH_EN              "Loading\nFilament\nCompleted"
#define UNLOAD_FINISH_EN            "Unloading\nFilament\nCompleted"

//manual ip
#define MANUAL_IP_TITLE_EN          "Manual IP"
#define MANUAL_IP_CANCEL_EN          "< Cancel"
#define MANUAL_IP_APPLY_EN          "Join >"
#define MANUAL_IP_ADDRESS_EN        "IP Address"
#define MANUAL_IP_MASK_EN           "Subnet Mask"
#define MANUAL_IP_GATEWAY_EN        "Default Gateway"
#define MANUAL_IP_SERVER_EN         "Name Server"
#define MANUAL_IP_INIT_DATA_EN      "0.0.0.0"
#define MANUAL_TEXT_POINT_EN         "."
#define MANUAL_TEXT_ENTER_EN        "Enter"

#define TEXT_FORGET_TIPS_TITLE_EN "Forget Network"
#define TEXT_FORGET_NETWORK_TIPS1_EN "Are you sure you want to\nforget this network?"
#define TEXT_FORGET_NETWORK_TIPS2_EN "This machine will no longer\njoin this Wi-Fi Network."

#define TEXT_IPADDRESS_EN "IP Address: "

#define TEXT_BUILD_FROM_CURA_CANCEL_TIPS1_EN "Are you sure you want to\ncancel this build?"
#define TEXT_BUILD_FROM_CURA_CANCEL_TIPS2_EN "The model will be deleted\nfrom this machine.It will\nneed to be resent from your\ncomputer before it can be\nbuilt in the future."

#define DIALOG_CONFIRM_EN2              "Confirm"

#define HEATING_TITLE_EN  "Heating"
#define LEVELING_TITLE_EN  "Leveling"

#define ABOUT_SPARK_ADD_EN "Spark+"

#define TEXT_RECEIVING_DATA_EN "Receiving Data"

#define TEXT_BABY_STEP_EN "Babystep"

#define PRINTING_OTHER_LANGUGE              "Printing"
#define PRINTING_OPERATION_OTHER_LANGUGE    "Operation"
#define PRINTING_PAUSE_OTHER_LANGUGE        "Pause"

#define MESSAGE_PAUSING_EN        "Pausing..."
#define MESSAGE_CHANGING_EN       "Waiting for filament changing."
#define MESSAGE_UNLOAD_EN         "Waiting for filament unloading."
#define MESSAGE_WAITING_EN        "Press the button to resume printing."
#define MESSAGE_INSERT_EN         "After loading the filament,\nclick the button to start printing."
#define MESSAGE_LOAD_EN           "Waiting for filament loading."
#define MESSAGE_PURGE_EN          "Waiting for extruding."
#define MESSAGE_RESUME_EN         "Waiting for printing resume..."
#define MESSAGE_HEAT_EN           "Press for heating up the nozzle."
#define MESSAGE_HEATING_EN        "Please wait for nozzle heating..."
#define MESSAGE_OPTION_EN         "Extrude the filament\nor continue printing?"
#define MESSAGE_PURGE_MORE_EN     "Extrude"
#define MESSAGE_CONTINUE_PRINT_EN "Print"
#define EEPROM_SETTINGS_TITLE_EN  "Factory Setting"//"EEPROM Settings"
#define EEPROM_SETTINGS_STORE_EN  "Save settings to EEPROM"
#define EEPROM_SETTINGS_READ_EN   "Read settings from EEPROM"
#define EEPROM_SETTINGS_REVERT_EN "Restore factory settings"

#define EEPROM_STORE_TIPS_EN  "Store settings to EEPROM?"
#define EEPROM_READ_TIPS_EN   "Read settings from EEPROM?"
#define EEPROM_REVERT_TIPS_EN "Restore factory settings?"

#define MORE_CUSTOM1_TEXT_EN  USER_DESC_1
#define MORE_CUSTOM2_TEXT_EN  USER_DESC_2
#define MORE_CUSTOM3_TEXT_EN  USER_DESC_3
#define MORE_CUSTOM4_TEXT_EN  USER_DESC_4
#define MORE_CUSTOM5_TEXT_EN  USER_DESC_5
#define MORE_CUSTOM6_TEXT_EN  USER_DESC_6
#define MORE_CUSTOM7_TEXT_EN  USER_DESC_7

#if ENABLED(MIXWARE_MODEL_V)
  #undef EXTRUDER_1_TEXT_EN
  #undef EXTRUDER_2_TEXT_EN
  #undef FILAMENT_EXT0_TEXT_EN
  #undef FILAMENT_EXT1_TEXT_EN
  #undef EEPROM_SETTINGS_STORE_EN
  #undef EEPROM_STORE_TIPS_EN
  #undef PAUSE_POSITION_X_EN
  #undef PAUSE_POSITION_Y_EN
  #undef PAUSE_POSITION_Z_EN
  #undef TITLE_LEVELING_EN

  #define ADJUST_Z_MENU_SAVE_EN                 "Save"
  #define ADJUST_Z_MENU_ADD_EN                  "Lift Nozzle"
  #define ADJUST_Z_MENU_DEC_EN                  "Drop Nozzle"
  #define MENU_WAIT_START_EN                    "Starting, please wait..."
  #define RUNOUT_CONF_TEXT_EN                   "Filament Detector"
  #define EXTRUDER_1_TEXT_EN                    "Nozzle"
  #define EXTRUDER_2_TEXT_EN                    "Nozzle"
  #define FILAMENT_EXT0_TEXT_EN                 "Nozzle"
  #define FILAMENT_EXT1_TEXT_EN                 "Nozzle"
  #define FILAMENT_DIALOG_LOAD_SELECT_TIPS_EN   "Please select the filament to load."
  #define FILAMENT_DIALOG_UNLOAD_SELECT_TIPS_EN "Please select the filament to unload."
  #define FILAMENT_DIALOG_CLOGGING_EN           "  The filament is clogged,\n   please clear the nozzle.\nClick <confirm> to return!"
  #define FILAMENT_SENSOR_ON_EN                 "Filament\nDetector:On"
  #define FILAMENT_SENSOR_OFF_EN                "Filament\nDetector:Off"
  #define FILAMENT_DIALOG_BREAKING_EN           "Filament abnormal, pausing..."
  #define FILAMENT_DIALOG_BREAK_TITLE_EN        "Filament Detector: Filament abnormal"
  #define AUTO_LEVELING_TITLE_EN                "Auto-leveling"
  #define AUTO_LEVELING_COMPLETED_EN            "Click <confirm> to start auto-leveling."
  #define AUTO_LEVELING_LEVELING_EN             "Auto-leveling, please wait!"
  #define AUTO_LEVELING_FINISHED_EN             " Auto-leveling completed.\nClick <confirm> to return!\nClick <adjust> to Z offset!"
  #define AUTO_LEVELING_FAILED_EN               "    Auto-leveling failed.\nClick <confirm> to return!"
  #define EEPROM_SETTINGS_STORE_EN              "Save settings to the printer"
  #define EEPROM_STORE_TIPS_EN                  "Save settings to the printer?"

  #define PAUSE_POSITION_X_EN                   "X-axis(Absolute Position)"
  #define PAUSE_POSITION_Y_EN                   "Y-axis(Absolute Position)"
  #define PAUSE_POSITION_Z_EN                   "Z-axis(Relative Position)"
  #define PAUSE_POSITION_TIPS_EN                "(Tips: -1 invalid.)"
  #define TITLE_LEVELING_EN                     "Manual leveling"

  #define AXIS_TEST_Z_EN                        "Z-axis Test"
  #define AXIS_TESTING_Z_EN                     "Z-axis Testing"

  #define PIDTEMP_SW_TITLE_EN                   "Heating Mode Settings"
  #define PIDTEMP_SW_NMODEL_EN                  "Normal\n Mode"
  #define PIDTEMP_SW_HMODEL_EN                  "High Temp.\n    Mode"
  #define PIDTEMP_SW_NMODEL_CUR_EN              "Current Heating Mode:\n     Normal Mode"
  #define PIDTEMP_SW_HMODEL_CUR_EN              "Current Heating Mode:\nHigh Temperature Mode"
  #define PIDTEMP_SW_NMODEL_TIP_EN              "  click <confirm> for switching to\n        Normal Heating Mode.\n(This mode needs to be configured\n     with normal nozzle module)"
  #define PIDTEMP_SW_HMODEL_TIP_EN              "    click <confirm> for switching to\n         High Temperature Mode.\n  (This mode needs to be configured\nwith high temperature nozzle module)"

  #define HIGH_HEAT_MODE_EN                     "HIGH Temp."
  #define HIGH_HEAT_MODE_TIPS_EN                "High Temperature Mode"
  #define FILAMENT_ADJUST_TEMP_EN               "Adjust Temp."
  #define DIALOG_PRINT_AGAIN_EN                 "Print Again"

  #define DEBUG_SELF_CHECK_TITLE_EN             "Self Check"
  #define DEBUG_SELF_CHECK_CONFIRM_EN           "Check"
  #define DEBUG_SELF_CHECK_CHECKING_EN          "Checking, Please wait."
  #define DEBUG_SELF_CHECK_AXIS_SLOW_EN         "Slow-Speed\n Z-axis Test"
  #define DEBUG_SELF_CHECK_AXIS_FAST_EN         "High-Speed\n Z-axis Test"

  #define DEBUG_SELF_CHECK_TIPS_E_TEMP_EN       "Nozzle Temperature"
  #define DEBUG_SELF_CHECK_TIPS_B_TEMP_EN       "Hot Bed Temperature"
  #define DEBUG_SELF_CHECK_TIPS_E_HEAT_EN       "Nozzle Heating"
  #define DEBUG_SELF_CHECK_TIPS_B_HEAT_EN       "Hot Bed Heating"
  #define DEBUG_SELF_CHECK_TIPS_X_MOTOR_EN      "X-axis Motor"
  #define DEBUG_SELF_CHECK_TIPS_Y_MOTOR_EN      "Y-axis Motor"
  #define DEBUG_SELF_CHECK_TIPS_3DTPUCH_EN      "3D Touch"

  #define MENU_BUTTON_TIPS_EN                   "(Click Change)"
  #define DIALOG_PRINT_SAVEEXIT_EN              "Save & Exit"
  #define DIALOG_PRINT_SAVEEXITTIPS_EN          "Babystep has been used, you can use\n<save and exit> to save parameters."
#endif
