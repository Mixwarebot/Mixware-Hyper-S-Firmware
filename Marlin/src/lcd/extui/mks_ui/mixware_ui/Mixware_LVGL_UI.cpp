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
#include "../../../../inc/MarlinConfigPre.h"

#ifdef TFT_MIXWARE_LVGL_UI

#include "../draw_ui.h"
#include "../../../../feature/bltouch.h"
#include "../../../../gcode/gcode.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"
#include "../../../../module/probe.h"
#include "../../../../module/stepper.h"
#include "../../../../module/temperature.h"
#include "../../../../sd/cardreader.h"

#include "Mixware_LVGL_UI.h"
#include "language/language.h"

extern lv_group_t *g;
extern bool temps_update_flag;

static lv_obj_t *buttonStep,
                *labelStep;
static lv_obj_t *labelZOffset;
static float    bak_zOffset;

static lv_obj_t *l_debug;
static lv_obj_t *l_temp_nozzle,
                *l_heat_nozzle,
                #if HAS_HEATED_BED
                  *l_temp_bed,
                  *l_heat_bed,
                #endif
                *l_axis_x_motor,
                *l_axis_y_motor,
                *l_axis_z_motor,
                *l_axis_e_motor,
                *l_fan,
                *l_servo;

static millis_t h_timer;
static bool     b_motor_ok;

static uint8_t  debugState = MDEVICEDEBUGNULL,
                debugState_n = MDEVICEDEBUGNULL;

lv_point_t line_points_2[8][2] = {
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 1 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 1 + PARA_UI_SIZE_Y}},
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 2 + PARA_UI_SIZE_Y}},
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 3 + PARA_UI_SIZE_Y}},
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 4 + PARA_UI_SIZE_Y}},
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 5 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 5 + PARA_UI_SIZE_Y}},
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 6 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 6 + PARA_UI_SIZE_Y}},
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 7 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 7 + PARA_UI_SIZE_Y}},
    {{PARA_UI_POS_X, PARA_UI_POS_Y * 8 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y * 8 + PARA_UI_SIZE_Y}}
};

MixwareUI mixware_ui;

void MixwareUI::translatorInit()
{
  MTR.print                           = MTRLANG(PRINT);
  MTR.tool                            = MTRLANG(TOOL);
  MTR.set                             = MTRLANG(SETTINGS);
  MTR.add                             = MTRLANG(ADD);
  MTR.dec                             = MTRLANG(REDUCE);
  MTR.back                            = MTRLANG(REBACK);
  MTR.save                            = MTRLANG(SAVE);
  MTR.clickTips                       = MTRLANG(CLICK_TIPS);
  MTR.waitTips                        = MTRLANG(WAIT_TIPS);
  MTR.nozzle                          = MTRLANG(EXTRUCT);
  MTR.bed                             = MTRLANG(Bed);
  MTR.temp                            = MTRLANG(TEMPERATURE);
  MTR.preheat                         = MTRLANG(PREHEAT);
  MTR.Cool                            = MTRLANG(PREHEAT_COOL);
  MTR.more                            = MTRLANG(PREHEAT_MORE);
  MTR.fan                             = MTRLANG(FAN);
  MTR.fanOn                           = MTRLANG(FAN_ON);
  MTR.fanOff                          = MTRLANG(FAN_OFF);
  MTR.filament                        = MTRLANG(FILAMENT);
  MTR.filIn                           = MTRLANG(FILAMENT_IN);
  MTR.filInTips                       = MTRLANG(FILAMENT_TIPS_IN);
  MTR.filInTipsHeat                   = MTRLANG(FILAMENT_TIPS_IN_HEAT);
  MTR.filInTipsConfirm                = MTRLANG(FILAMENT_TIPS_IN_CONFIRM);
  MTR.filInTipsWork                   = MTRLANG(FILAMENT_TIPS_IN_WORK);
  MTR.filInTipsExit                   = MTRLANG(FILAMENT_TIPS_IN_EXIT);
  MTR.filOut                          = MTRLANG(FILAMENT_OUT);
  MTR.filOutTips                      = MTRLANG(FILAMENT_TIPS_OUT);
  MTR.filOutTipsHeat                  = MTRLANG(FILAMENT_TIPS_OUT_HEAT);
  MTR.filOutTipsWork                  = MTRLANG(FILAMENT_TIPS_OUT_WORK);
  MTR.filOutTipsExit                  = MTRLANG(FILAMENT_TIPS_OUT_EXIT);
  MTR.filTempAdjust                   = MTRLANG(FILAMENT_TEMP_ADJUST);
  MTR.move                            = MTRLANG(AXIS_MOVE);
  MTR.home                            = MTRLANG(AXIS_HOME);
  MTR.homeAll                         = MTRLANG(AXIS_HOME_ALL);
  MTR.axisDisabled                    = MTRLANG(AXIS_DISABLED);
  MTR.leveling                        = MTRLANG(LEVELING);
  MTR.levelingAuto                    = MTRLANG(LEVELING_AUTO);
  MTR.levelingAutoTipsStart           = MTRLANG(LEVELING_AUTO_TIPS_START);
  MTR.levelingAutoTipsWorking         = MTRLANG(LEVELING_AUTO_TIPS_WORKING);
  MTR.levelingAutoTipsExit            = MTRLANG(LEVELING_AUTO_TIPS_FINISHED);
  MTR.levelingAutoTipsFailed          = MTRLANG(LEVELING_AUTO_TIPS_FAILED);
  MTR.levelingManual                  = MTRLANG(LEVELING_MANUAL);
  MTR.levelingManualFL                = MTRLANG(LEVELING_MANUAL_FL);
  MTR.levelingManualFR                = MTRLANG(LEVELING_MANUAL_FR);
  MTR.levelingManualC                 = MTRLANG(LEVELING_MANUAL_C);
  MTR.levelingManualBL                = MTRLANG(LEVELING_MANUAL_BL);
  MTR.levelingManualBR                = MTRLANG(LEVELING_MANUAL_BR);
  MTR.offsetX                         = MTRLANG(OFFSET_X);
  MTR.offsetY                         = MTRLANG(OFFSET_Y);
  MTR.offsetZ                         = MTRLANG(OFFSET_Z);
  MTR.offsetZAdd                      = MTRLANG(OFFSET_Z_ADD);
  MTR.offsetZDec                      = MTRLANG(OFFSET_Z_DEC);
  MTR.flash                           = MTRLANG(FLASH_TITLE);
  MTR.flashReset                      = MTRLANG(FLASH_RESET);
  MTR.flashResetTips                  = MTRLANG(FLASH_RESET_TIPS);
  MTR.flashSave                       = MTRLANG(FLASH_STORE);
  MTR.flashSaveTips                   = MTRLANG(FLASH_STORE_TIPS);
  MTR.ADVSet                          = MTRLANG(ADVSET);
  MTR.ADVSetAccel                     = MTRLANG(ADVSET_ACCELERATION);
  MTR.ADVSetAccelPrint                = MTRLANG(ADVSET_ACCEL_PRINT);
  MTR.ADVSetAccelRetract              = MTRLANG(ADVSET_ACCEL_RETRACT);
  MTR.ADVSetAccelTravel               = MTRLANG(ADVSET_ACCEL_TRAVEL);
  MTR.ADVSetAccelX                    = MTRLANG(ADVSET_ACCEL_X_AXIS);
  MTR.ADVSetAccelY                    = MTRLANG(ADVSET_ACCEL_Y_AXIS);
  MTR.ADVSetAccelZ                    = MTRLANG(ADVSET_ACCEL_Z_AXIS);
  MTR.ADVSetAccelE                    = MTRLANG(ADVSET_ACCEL_E_AXIS);
  MTR.ADVSetMaxSpeed                  = MTRLANG(ADVSET_MAX_SPEED);
  MTR.ADVSetMaxSpeedX                 = MTRLANG(ADVSET_MAX_SPEED_X_AXIS);
  MTR.ADVSetMaxSpeedY                 = MTRLANG(ADVSET_MAX_SPEED_Y_AXIS);
  MTR.ADVSetMaxSpeedZ                 = MTRLANG(ADVSET_MAX_SPEED_Z_AXIS);
  MTR.ADVSetMaxSpeedE                 = MTRLANG(ADVSET_MAX_SPEED_E_AXIS);
  MTR.ADVSetJerk                      = MTRLANG(ADVSET_JERK);
  MTR.ADVSetJerkX                     = MTRLANG(ADVSET_JERK_X_AXIS);
  MTR.ADVSetJerkY                     = MTRLANG(ADVSET_JERK_Y_AXIS);
  MTR.ADVSetJerkZ                     = MTRLANG(ADVSET_JERK_Z_AXIS);
  MTR.ADVSetJerkE                     = MTRLANG(ADVSET_JERK_E_AXIS);
  MTR.ADVSetStep                      = MTRLANG(ADVSET_STEP);
  MTR.ADVSetStepX                     = MTRLANG(ADVSET_STEP_X_AXIS);
  MTR.ADVSetStepY                     = MTRLANG(ADVSET_STEP_Y_AXIS);
  MTR.ADVSetStepZ                     = MTRLANG(ADVSET_STEP_Z_AXIS);
  MTR.ADVSetStepE                     = MTRLANG(ADVSET_STEP_E_AXIS);
  MTR.ADVSetPausePos                  = MTRLANG(ADVSET_PAUSE_POSITION);
  MTR.ADVSetPausePosX                 = MTRLANG(ADVSET_PAUSE_POSITION_X);
  MTR.ADVSetPausePosY                 = MTRLANG(ADVSET_PAUSE_POSITION_Y);
  MTR.ADVSetPausePosZ                 = MTRLANG(ADVSET_PAUSE_POSITION_Z);
  MTR.ADVSetPausePosTips              = MTRLANG(ADVSET_PAUSE_POSITION_TIPS);
  MTR.ADVSetFil                       = MTRLANG(ADVSET_FILAMENT);
  MTR.ADVSetFilInLength               = MTRLANG(ADVSET_FILAMENT_IN_LENGTH);
  MTR.ADVSetFilInSpeed                = MTRLANG(ADVSET_FILAMENT_IN_SPEED);
  MTR.ADVSetFilOutLength              = MTRLANG(ADVSET_FILAMENT_OUT_LENGTH);
  MTR.ADVSetFilOutSpeed               = MTRLANG(ADVSET_FILAMENT_OUT_SPEED);
  MTR.ADVSeteHeatingMode              = MTRLANG(ADVSET_EHEATING_MODE);
  MTR.thermalProtection               = MTRLANG(ADVSET_THERMALPROTECTION);
  MTR.eHeatingModeNormal              = MTRLANG(EHEATMODE_NORMAL);
  MTR.eHeatingModeHigh                = MTRLANG(EHEATMODE_HIGH);
  MTR.eHeatingModeCurNormal           = MTRLANG(EHEATMODE_CUR_NORMAL);
  MTR.eHeatingModeCurHigh             = MTRLANG(EHEATMODE_CUR_HIGH);
  MTR.eHeatingModeTipsNormal          = MTRLANG(EHEATMODE_TIPS_NORMAL);
  MTR.eHeatingModeTipsHigh            = MTRLANG(EHEATMODE_TIPS_HIGH);
  MTR.eHeatingModeTips                = MTRLANG(EHEATMODE_TIPS);
  MTR.eHeatingModeTipsAbb             = MTRLANG(EHEATMODE_TIPS_ABB);
  MTR.about                           = MTRLANG(ABOUT);
  MTR.language                        = MTRLANG(LANGUAGE);
  MTR.filamentDetector                = MTRLANG(FILDET);
  MTR.filamentDetectorOn              = MTRLANG(FILDET_ON);
  MTR.filamentDetectorOff             = MTRLANG(FILDET_OFF);
  MTR.filamentDetectorTipsPause       = MTRLANG(FILDET_TIPS_PAUSE);
  MTR.filamentDetectorClog            = MTRLANG(FILDET_CLOG);
  MTR.filamentDetectorTipsClog        = MTRLANG(FILDET_TIPS_CLOG);
  MTR.filamentDetectorTipsHew         = MTRLANG(FILDET_TIPS_HOW);
  MTR.filamentDetectorPurge           = MTRLANG(FILDET_PURGE);
  MTR.filamentDetectorPrint           = MTRLANG(FILDET_PRINT);
  MTR.debugDevTitle                   = MTRLANG(DEBUG_DEVICE_TITLE);
  MTR.debugDevConfirm                 = MTRLANG(DEBUG_DEVICE_CONFIRM);
  MTR.debugDevWorking                 = MTRLANG(DEBUG_DEVICE_WORKING);
  MTR.debugDevTipsETemp               = MTRLANG(DEBUG_DEVICE_TIPS_E_TEMP);
  MTR.debugDevTipsBTemp               = MTRLANG(DEBUG_DEVICE_TIPS_B_TEMP);
  MTR.debugDevTipsEHeat               = MTRLANG(DEBUG_DEVICE_TIPS_E_HEAT);
  MTR.debugDevTipsBHeat               = MTRLANG(DEBUG_DEVICE_TIPS_B_HEAT);
  MTR.debugDevTipsFan                 = MTRLANG(DEBUG_DEVICE_TIPS_FANS);
  MTR.debugDevTipsX                   = MTRLANG(DEBUG_DEVICE_TIPS_X_AXIS);
  MTR.debugDevTipsY                   = MTRLANG(DEBUG_DEVICE_TIPS_Y_AXIS);
  MTR.debugDevTipsZ                   = MTRLANG(DEBUG_DEVICE_TIPS_Z_AXIS);
  MTR.debugDevTipsE                   = MTRLANG(DEBUG_DEVICE_TIPS_E_AXIS);
  MTR.debugDevTipsServo               = MTRLANG(DEBUG_DEVICE_TIPS_SERVO);
  MTR.debugZTitle                     = MTRLANG(DEBUG_ZAXIS_TITLE);
  MTR.debugZSlowMode                  = MTRLANG(DEBUG_ZAXIS_SLOWMODE);
  MTR.debugZFastMode                  = MTRLANG(DEBUG_ZAXIS_FASTMODE);
  MTR.debugZWorking                   = MTRLANG(DEBUG_ZAXIS_WORKING);
  MTR.printAgain                      = MTRLANG(PRINT_AGAIN);
  MTR.printTipsSaveOffset             = MTRLANG(PRINT_TIPS_SAVEOFFSET);
  MTR.printTipsSave                   = MTRLANG(PRINT_TIPS_SAVE);
  MTR.printPause                      = MTRLANG(PRINT_PAUSE);
  MTR.printResume                     = MTRLANG(PRINT_RESUME);
  MTR.printStop                       = MTRLANG(PRINT_STOP);
  MTR.previous                        = MTRLANG(PAGE_PREV);
  MTR.next                            = MTRLANG(PAGE_NEXT);
  MTR.keyBack                         = MTRLANG(NO_KEY_BACK);
  MTR.keyReset                        = MTRLANG(NO_KEY_RESET);
  MTR.keyConfirm                      = MTRLANG(NO_KEY_CONFIRM);
  MTR.printNoFileTips                 = MTRLANG(PRINT_NOFILE_TIPS);
  MTR.operation                       = MTRLANG(OPERATION);
  MTR.speed                           = MTRLANG(SPEED);
  MTR.babystep                        = MTRLANG(BABYSTEP);
}

void MixwareUI::textRevisionInit()
{
  MTR.main                            = MTR_MAIN;
  MTR.tempRange1c                     = MTR_TEMPERATURE_RANGE_1C;
  MTR.tempRange5c                     = MTR_TEMPERATURE_RANGE_5C;
  MTR.tempRange10c                    = MTR_TEMPERATURE_RANGE_10C;
  MTR.preheatPLA                      = MTR_PREHEAT_PLA;
  MTR.preheatTPU                      = MTR_PREHEAT_TPU;
  MTR.preheatPETG                     = MTR_PREHEAT_PETG;
  MTR.preheatPACF                     = MTR_PREHEAT_PACF;
  MTR.x                               = MTR_X;
  MTR.y                               = MTR_Y;
  MTR.z                               = MTR_Z;
  MTR.e                               = MTR_E;
  MTR.moveDistanceMM005               = MTR_AXIS_MOVEDISTANCE_MM_0_0_5;
  MTR.moveDistanceMM01                = MTR_AXIS_MOVEDISTANCE_MM_0_1;
  MTR.moveDistanceMM1                 = MTR_AXIS_MOVEDISTANCE_MM_1;
  MTR.moveDistanceMM5                 = MTR_AXIS_MOVEDISTANCE_MM_5;
  MTR.moveDistanceMM10                = MTR_AXIS_MOVEDISTANCE_MM_10;
  MTR.key1                            = MTR_NO_KEY_1;
  MTR.key2                            = MTR_NO_KEY_2;
  MTR.key3                            = MTR_NO_KEY_3;
  MTR.key4                            = MTR_NO_KEY_4;
  MTR.key5                            = MTR_NO_KEY_5;
  MTR.key6                            = MTR_NO_KEY_6;
  MTR.key7                            = MTR_NO_KEY_7;
  MTR.key8                            = MTR_NO_KEY_8;
  MTR.key9                            = MTR_NO_KEY_9;
  MTR.key0                            = MTR_NO_KEY_0;
  MTR.keyPoint                        = MTR_NO_KEY_POINT;
  MTR.keyNegative                     = MTR_NO_KEY_NEGATIVE;

  translatorInit();
}

void MixwareUI::imagePathInit()
{
  MIMG.homeAll                        = MIMAGEPATH(MIMG_N_HOME_ALL);
  MIMG.homeX                          = MIMAGEPATH(MIMG_N_HOME_X);
  MIMG.homeY                          = MIMAGEPATH(MIMG_N_HOME_Y);
  MIMG.homeZ                          = MIMAGEPATH(MIMG_N_HOME_Z);
  MIMG.motorOff                       = MIMAGEPATH(MIMG_N_MOTOR_OFF);

  MIMG.preheat                        = MIMAGEPATH(MIMG_N_PREHEAT);
  MIMG.fan                            = MIMAGEPATH(MIMG_N_FAN);
  MIMG.move                           = MIMAGEPATH(MIMG_N_MOVE);
  MIMG.level                          = MIMAGEPATH(MIMG_N_LEVEL);
  MIMG.filament                       = MIMAGEPATH(MIMG_N_FILAMENT);
  MIMG.home                           = MIMAGEPATH(MIMG_N_HOME);

  MIMG.add                            = MIMAGEPATH(MIMG_N_ADD);
  MIMG.dec                            = MIMAGEPATH(MIMG_N_DEC);
  MIMG.rate1                          = MIMAGEPATH(MIMG_N_SPEED_RATE_1);
  MIMG.rate5                          = MIMAGEPATH(MIMG_N_SPEED_RATE_5);
  MIMG.rate10                         = MIMAGEPATH(MIMG_N_SPEED_RATE_10);

  MIMG.bed                            = MIMAGEPATH(MIMG_N_BED);
  MIMG.temperatureRange1              = MIMAGEPATH(MIMG_N_TEMP_RANGE_1);
  MIMG.temperatureRange5              = MIMAGEPATH(MIMG_N_TEMP_RANGE_5);
  MIMG.temperatureRange10             = MIMAGEPATH(MIMG_N_TEMP_RANGE_10);

  MIMG.filamentIn                     = MIMAGEPATH(MIMG_N_FILAMENT_IN);
  MIMG.filamentOut                    = MIMAGEPATH(MIMG_N_FILAMENT_OUT);
  MIMG.extruct                        = MIMAGEPATH(MIMG_N_EXT);
  MIMG.speedHigh                      = MIMAGEPATH(MIMG_N_SPEED_HIGH);
  MIMG.speedSlow                      = MIMAGEPATH(MIMG_N_SPEED_SLOW);
  MIMG.speedNormal                    = MIMAGEPATH(MIMG_N_SPEED_NORMAL);
  MIMG.moveDistance_1_mm              = MIMAGEPATH(MIMG_N_STEP_MM_1);
  MIMG.moveDistance_5_mm              = MIMAGEPATH(MIMG_N_STEP_MM_5);
  MIMG.moveDistance_10_mm             = MIMAGEPATH(MIMG_N_STEP_MM_10);

  MIMG.dir                            = MIMAGEPATH(MIMG_N_DIR);
  MIMG.file                           = MIMAGEPATH(MIMG_N_FILE);

  MIMG.axisX                          = MIMAGEPATH(MIMG_N_AXIS_X);
  MIMG.axisY                          = MIMAGEPATH(MIMG_N_AXIS_Y);
  MIMG.axisZ                          = MIMAGEPATH(MIMG_N_AXIS_Z);
  MIMG.moveDistance_0_1_mm            = MIMAGEPATH(MIMG_N_STEP_MM_0_1);

  MIMG.speed                          = MIMAGEPATH(MIMG_N_SPEED);

  MIMG.printPause                     = MIMAGEPATH(MIMG_N_PRINT_PAUSE);
  MIMG.printResume                    = MIMAGEPATH(MIMG_N_PRINT_RESUME);
  MIMG.printStop                      = MIMAGEPATH(MIMG_N_PRINT_STOP);
  MIMG.stateX                         = MIMAGEPATH(MIMG_N_STATE_X);
  MIMG.stateY                         = MIMAGEPATH(MIMG_N_STATE_Y);
  MIMG.stateZ                         = MIMAGEPATH(MIMG_N_STATE_Z);
  MIMG.stateExtruct                   = MIMAGEPATH(MIMG_N_STATE_EXT);
  MIMG.stateBed                       = MIMAGEPATH(MIMG_N_STATE_BED);
  MIMG.stateFan                       = MIMAGEPATH(MIMG_N_STATE_FAN);
  MIMG.stateTime                      = MIMAGEPATH(MIMG_N_STATE_TIME);
  MIMG.operate                        = MIMAGEPATH(MIMG_N_OPERATE);

  MIMG.tool                           = MIMAGEPATH(MIMG_N_TOOL);
  MIMG.set                            = MIMAGEPATH(MIMG_N_SET);
  MIMG.print                          = MIMAGEPATH(MIMG_N_PRINT);

  MIMG.preheatPLA                     = MIMAGEPATH(MIMG_N_PREHEAT_PLA);
  MIMG.preheatPETG                    = MIMAGEPATH(MIMG_N_PREHEAT_PETG);
  MIMG.preheatPACF                    = MIMAGEPATH(MIMG_N_PREHEAT_PACF);
  MIMG.preheatBed                     = MIMAGEPATH(MIMG_N_PREHEAT_BED);
  MIMG.preheatCool                    = MIMAGEPATH(MIMG_N_PREHEAT_COOL);

  MIMG.level2                         = MIMAGEPATH(MIMG_N_LEVEL2);
  MIMG.levelPos1                      = MIMAGEPATH(MIMG_N_LEVEL_P1);
  MIMG.levelPos2                      = MIMAGEPATH(MIMG_N_LEVEL_P2);
  MIMG.levelPos3                      = MIMAGEPATH(MIMG_N_LEVEL_P3);
  MIMG.levelPos4                      = MIMAGEPATH(MIMG_N_LEVEL_P4);
  MIMG.levelPos5                      = MIMAGEPATH(MIMG_N_LEVEL_P5);
  MIMG.levelAuto                      = MIMAGEPATH(MIMG_N_LEVEL_AUTO);
  MIMG.levelManual                    = MIMAGEPATH(MIMG_N_LEVEL_MANUAL);
  MIMG.save                           = MIMAGEPATH(MIMG_N_SAVE);
  MIMG.babystep                       = MIMAGEPATH(MIMG_N_BABYSTEP);
  MIMG.zOffset                        = MIMAGEPATH(MIMG_N_Z_OFFSET);

  MIMG.EEPROM                         = MIMAGEPATH(MIMG_N_EEPROM);
  MIMG.about                          = MIMAGEPATH(MIMG_N_ABOUT);
  MIMG.configuration                  = MIMAGEPATH(MIMG_N_CONFIG);

  MIMG.zAxisDebugSlow                 = MIMAGEPATH(MIMG_N_DEBUG_Z_SLOW);
  MIMG.zAxisDebugFast                 = MIMAGEPATH(MIMG_N_DEBUG_Z_FAST);
  MIMG.deviceDebug                    = MIMAGEPATH(MIMG_N_DEBUG_DEV);

  MIMG.language                       = MIMAGEPATH(MIMG_N_LANG);
  MIMG.languageEN                     = MIMAGEPATH(MIMG_N_LANG_EN);
  MIMG.languageENSEL                  = MIMAGEPATH(MIMG_N_LANG_EN_SEL);
  MIMG.languageSCN                    = MIMAGEPATH(MIMG_N_LANG_SCN);
  MIMG.languageSCNSEL                 = MIMAGEPATH(MIMG_N_LANG_SCN_SEL);
  MIMG.languageTCN                    = MIMAGEPATH(MIMG_N_LANG_TCN);
  MIMG.languageTCNSEL                 = MIMAGEPATH(MIMG_N_LANG_TCN_SEL);

  MIMG.filamentDetector               = MIMAGEPATH(MIMG_N_FILAMENT_DET);
  MIMG.filamentDetectorOn             = MIMAGEPATH(MIMG_N_FILAMENT_DET_ON);
  MIMG.filamentDetectorOff            = MIMAGEPATH(MIMG_N_FILAMENT_DET_OFF);

  MIMG.arrow                          = MIMAGEPATH(MIMG_N_ARROW);
  MIMG.placeholder                    = MIMAGEPATH(MIMG_N_NULL);

  MIMG.eHeatingModeNormal             = MIMAGEPATH(MIMG_N_E_HEAT_M_NORMAL);
  MIMG.eHeatingModeHigh               = MIMAGEPATH(MIMG_N_E_HEAT_M_HIGH);
  MIMG.printHM                        = MIMAGEPATH(MIMG_N_PRINT_HM);
  MIMG.preheatHM                      = MIMAGEPATH(MIMG_N_PREHEAT_HM);
  MIMG.extructHM                      = MIMAGEPATH(MIMG_N_EXT_HM);
  MIMG.stateExtructHM                 = MIMAGEPATH(MIMG_N_STATE_EXT_HM);
  MIMG.eHeatingModeTip                = MIMAGEPATH(MIMG_N_E_HEAT_M_TIP_HM);

  MIMG.preview                        = MIMAGEPATH(MIMG_N_PREVIEW);
  MIMG.logo                           = MIMAGEPATH(MIMG_N_LOGO);
  MIMG.mainLogo                       = MIMAGEPATH(MIMG_N_MIXWARE);

  MIMG.enable                         = MIMAGEPATH(MIMG_N_ENABLE);
  MIMG.disable                        = MIMAGEPATH(MIMG_N_DISABLE);
}

void MixwareUI::preferenceInit()
{
  MPRE.zOffsetChanged = false;
  MPRE.zAxisDebugFastMode = false;
  MPRE.filamentIsBroken = false;
  MPRE.moveAxis = 0;
  MPRE.extrusionVolume = 1.0;

  MPRE.param.eHeatingMaxTemp = 260;
  MPRE.param.filamentDetectorEnabled = false;
  MPRE.param.thermalProtectionEnabled = false;
}

void MixwareUI::styleInit()
{
  lv_style_copy(&tft_style_label, &tft_style_label_rel);
  tft_style_label.text.color          = TFT_LV_PARA_BACK_BODY_COLOR;
  tft_style_label.text.sel_color      = TFT_LV_PARA_BACK_BODY_COLOR;

  lv_style_copy(&style_para_button, &lv_style_plain);
  style_para_button.body.border.color = LV_COLOR_BACKGROUND;
  style_para_button.body.border.width = 1;
  style_para_button.body.main_color   = LV_COLOR_BACKGROUND;
  style_para_button.body.grad_color   = LV_COLOR_BACKGROUND;
  style_para_button.body.shadow.width = 0;
  style_para_button.body.radius       = 5;
  style_para_button.body.border.color = TFT_LV_PARA_BACK_BODY_COLOR;
  style_para_button.body.border.width = 4;
  style_para_button.text.color        = LV_COLOR_WHITE;
  style_para_button.text.font         = &TERN(HAS_SPI_FLASH_FONT, gb2312_puhui32, lv_font_roboto_22);

  lv_style_copy(&style_para_tips, &lv_style_plain);
  style_para_tips.body.border.color   = LV_COLOR_BACKGROUND;
  style_para_tips.body.border.width   = 1;
  style_para_tips.body.main_color     = LV_COLOR_BACKGROUND;
  style_para_tips.body.grad_color     = LV_COLOR_BACKGROUND;
  style_para_tips.body.shadow.width   = 0;
  style_para_tips.body.radius         = 3;
  style_para_tips.body.border.color   = TFT_LV_PARA_BACK_BODY_COLOR;
  style_para_tips.body.border.width   = 2;
  style_para_tips.text.color          = LV_COLOR_WHITE;
  style_para_tips.text.font           = &TERN(HAS_SPI_FLASH_FONT, gb2312_puhui32, lv_font_roboto_22);
}

//
//  page button
//
void MixwareUI::ButtonAddClickTips(lv_obj_t *button)
{
  lv_obj_t *tips = lv_label_create(button, MTR.clickTips);
  lv_obj_align(tips, button, LV_ALIGN_IN_BOTTOM_MID, 0, 2);
}

void MixwareUI::ScreenPlaceholder()
{
  lv_obj_t *placeholder = lv_img_create(scr, nullptr); // Empty picture.
  lv_img_set_src(placeholder, MIMG.placeholder);
  lv_obj_set_pos(placeholder, 0, TFT_HEIGHT - 50);
}

void MixwareUI::ScreenMainLogo(lv_obj_t *par) {
  lv_obj_t *img = lv_img_create(par, nullptr);
  lv_obj_set_pos(img, 2, 6);
  lv_img_set_src(img, MIMG.mainLogo);
}

void MixwareUI::ScreenMainTips(lv_obj_t *par) {
  if (!getEHeatingMode()) {
    lv_obj_t* btn = lv_btn_create(par, nullptr);
    lv_obj_t* label = ScreenLabel(btn, MTR.eHeatingModeTips);
    lv_btn_set_style_both(btn, &style_para_tips);
    lv_obj_set_size(btn, 240, 36);
    lv_obj_align(label, btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(btn, nullptr, LV_ALIGN_CENTER, 0, -78);
  }
}

lv_obj_t *MixwareUI::ScreenLabel(lv_obj_t *par, const char *text)
{
  lv_obj_t *label = lv_label_create_empty(par);
  if (text)
    lv_label_set_text(label, text);
  lv_obj_set_style(label, &tft_style_label);

  return label;
}

void MixwareUI::ScreenBottomButton(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id)
{
  lv_obj_t *button = lv_btn_create(par, x, y, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE, cb, id);
  lv_obj_t *label = lv_label_create_empty(button);
  lv_btn_set_style_both(button, &style_para_back);
  lv_label_set_text(label, text);
  lv_obj_align(label, button, LV_ALIGN_CENTER, 0, 0);
}

void MixwareUI::ScreenBottomLeftButton(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id)
{
  ScreenBottomButton(par, text, BOTTOMBTN_P_LX, BOTTOMBTN_P_Y, cb, id);
}

void MixwareUI::ScreenBottomMiddleButton(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id)
{
  ScreenBottomButton(par, text, BOTTOMBTN_P_MX, BOTTOMBTN_P_Y, cb, id);
}

void MixwareUI::ScreenBottomRightButton(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id)
{
  ScreenBottomButton(par, text, BOTTOMBTN_P_RX, BOTTOMBTN_P_Y, cb, id);
}

void MixwareUI::ScreenReturnButton(lv_obj_t *par, lv_event_cb_t cb, const int id)
{
  ScreenBottomRightButton(par, MTR.back, cb, id);
}

lv_obj_t *MixwareUI::ScreenToolButton(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id)
{
  lv_obj_t *btn = lv_btn_create(par, x, y, TOOLBTN_WIDTH, TOOLBTN_HEIGHT, cb, id);
  lv_obj_t *label = lv_label_create_empty(btn);
  lv_btn_set_style_both(btn, &style_para_button);
  lv_label_set_text(label, text);
  lv_obj_align(label, btn, LV_ALIGN_CENTER, 0, 0);

  return btn;
}

lv_obj_t* MixwareUI::ScreenEnabledButton(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, const int index, const bool curValue)
{
  lv_screen_menu_item(par, text, x, y, cb, -1, index, false);
  lv_obj_t *btnValue = lv_imgbtn_create(par, curValue ? MIMG.enable : MIMG.disable, PARA_UI_BACK_POS_X + 25, y + PARA_UI_STATE_V, cb, id);
  return btnValue;
}

void MixwareUI::updateEnabledButton(lv_obj_t *btn, const bool curValue)
{
  lv_imgbtn_set_src_both(btn, curValue ? MIMG.enable : MIMG.disable);
}

#define DEBUG_LABEL_POS_X   (250)
#define DEBUG_LABEL_POS_Y(n) (32+26*n)
#define DEBUG_LABEL_SIZE_W  (50)
#define DEBUG_LABEL_SIZE_H  (24)

static void l_selfc_reset_label() {
  lv_label_set_text(l_temp_nozzle, "--");
  lv_label_set_text(l_heat_nozzle, "--");
  lv_label_set_text(l_fan, "--");
  lv_label_set_text(l_axis_x_motor, "--");
  lv_label_set_text(l_axis_y_motor, "--");
  lv_label_set_text(l_axis_z_motor, "--");
  lv_label_set_text(l_axis_e_motor, "--");
  lv_label_set_text(l_servo, "--");

  lv_obj_set_style(l_temp_nozzle, &tft_style_label_rel);
  lv_obj_set_style(l_heat_nozzle, &tft_style_label_rel);
  lv_obj_set_style(l_fan, &tft_style_label_rel);
  lv_obj_set_style(l_axis_x_motor, &tft_style_label_rel);
  lv_obj_set_style(l_axis_y_motor, &tft_style_label_rel);
  lv_obj_set_style(l_axis_z_motor, &tft_style_label_rel);
  lv_obj_set_style(l_axis_e_motor, &tft_style_label_rel);
  lv_obj_set_style(l_servo, &tft_style_label_rel);

  #if HAS_HEATED_BED
    lv_label_set_text(l_temp_bed, "--");
    lv_label_set_text(l_heat_bed, "--");
    lv_obj_set_style(l_temp_bed, &tft_style_label_rel);
    lv_obj_set_style(l_heat_bed, &tft_style_label_rel);
  #endif
}

static void eventHandler(lv_obj_t *obj, lv_event_t event)
{
  char str_1[16];
  if (event != LV_EVENT_RELEASED)
    return;
  switch (obj->mks_obj_id)
  {
  case ID_LEVEL:
    break;
  case ID_LEVEL_SELECT:
    uiCfg.para_ui_page = true;
    MUI.clearPage();
    MUI.drawPage_level();
    break;
  case ID_LEVEL_AUTO:
    uiCfg.para_ui_page = false;
#if HAS_ABL_NOT_UBL
    MUI.clearPage();
    lv_draw_dialog(DIALOG_AUTO_LEVEL_COMPLETED);
#endif
    break;
  case ID_LEVEL_MANUAL:
    uiCfg.leveling_first_time = 1;
    uiCfg.para_ui_page = 0;
    MUI.clearPage();
    lv_draw_manualLevel();
    break;
  case ID_LEVEL_REBACK:
    MUI.clearPage();
    if (uiCfg.para_ui_page)
    {
      uiCfg.para_ui_page = 0;
      MUI.drawPage_level();
    }
    else
    {
      disp_state_stack._disp_index = 2;
      lv_draw_tool();
    }
    break;
  case ID_ZOFFSET:
    uiCfg.leveling_first_time = 1;
    uiCfg.move_dist = 0.1;
    MUI.clearPage();
    lv_draw_dialog(DIALOG_ADJUST_Z_HEIGHT_WAIT_START);
    break;
  case ID_ZOFFSET_ADD:
    if (queue.ring_buffer.length <= (BUFSIZE - 3))
    {
      if (uiCfg.leveling_first_time)
      {
        if (!all_axes_trusted())
          queue.enqueue_now_P(PSTR("G28"));
        queue.enqueue_now_P(PSTR("G1 X150 Y150\nG1 Z0"));
        uiCfg.leveling_first_time = 0;
      }
      else
      {
        ZERO(public_buf_l);
        queue.enqueue_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 Z%s F300"), dtostrf(uiCfg.move_dist, 1, 2, str_1));
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_now_P(PSTR("G90"));
        probe.offset[Z_AXIS] += uiCfg.move_dist;
        MUI.zOffsetUpdate();
      }
    }
    break;
  case ID_ZOFFSET_DEC:
    if (queue.ring_buffer.length <= (BUFSIZE - 3))
    {
      if (uiCfg.leveling_first_time)
      {
        if (!all_axes_trusted())
          queue.enqueue_now_P(PSTR("G28"));
        queue.enqueue_now_P(PSTR("G1 X150 Y150\nG1 Z0"));
        uiCfg.leveling_first_time = 0;
      }
      else
      {
        ZERO(public_buf_l);
        queue.enqueue_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 Z-%s F300"), dtostrf(uiCfg.move_dist, 1, 2, str_1));
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_now_P(PSTR("G90"));
        probe.offset[Z_AXIS] -= uiCfg.move_dist;
        MUI.zOffsetUpdate();
      }
    }
    break;
  case ID_ZOFFSET_STEP:
    if ((int)(100 * uiCfg.move_dist) == 5)
      uiCfg.move_dist = 0.1;
    else
      uiCfg.move_dist = 0.05;

    MUI.moveDistanceUpdate();
    break;
  case ID_ZOFFSET_SAVE:
    MUI.clearPage();
    lv_draw_dialog(DIALOG_STORE_EEPROM_TIPS);
    break;
  case ID_ZOFFSET_REBACK:
    if (!MPRE.zOffsetChanged)
      probe.offset.z = bak_zOffset;

    queue.inject_P(PSTR("G91\nG1 Z10 F300\nG90\nM84"));
    MUI.clearPage();
    MUI.drawPage_level();
    break;
  case ID_ZAXISDEBUG_SLOW:
    MUI.clearPage();
    uiCfg.leveling_first_time = 1;
    MPRE.zAxisDebugFastMode = false;
    lv_draw_dialog(DIALOG_AXIS_Z_TEST_WAIT_START);
    break;
  case ID_ZAXISDEBUG_FAST:
    MUI.clearPage();
    uiCfg.leveling_first_time = 1;
    MPRE.zAxisDebugFastMode = true;
    lv_draw_dialog(DIALOG_AXIS_Z_TEST_WAIT_START);
    break;
  case ID_ZAXISDEBUG_REBACK:
    MUI.clearPage();
    MUI.drawPage_deviceDebug();
    break;
  case ID_DEVICEDEBUG_WORK:
    if (debugState_n == MDEVICEDEBUGNULL) {
      l_selfc_reset_label();
      gcode.process_subcommands_now(PSTR("M107"));
      debugState_n = MDEVICEDEBUGTEMP_E;
      lv_label_set_text(l_debug, MTR.debugDevWorking);
    }
    break;
  case ID_DEVICEDEBUG_TO_Z:
    MUI.clearPage();
    MUI.drawPage_zAxisDebug();
    break;
  case ID_DEVICEDEBUG_REBACK:
    if (debugState == MDEVICEDEBUGHEAT_E) {
      thermalManager.setTargetHotend(0, uiCfg.hotendTargetTempBak);
    }
    #if HAS_HEATED_BED
      else if (debugState == MDEVICEDEBUGHEAT_B) {
        thermalManager.setTargetBed(uiCfg.hotendTargetTempBak);
      }
    #endif
    else if (debugState == MDEVICEDEBUGHOME_X || debugState == MDEVICEDEBUGHOME_Y || debugState == MDEVICEDEBUGHOME_Z) {
      break;
    }

    b_motor_ok = false;
    gcode.process_subcommands_now(PSTR("M107"));

    MUI.clearPage();
    lv_draw_set();
    break;
  case ID_EHEATINGMODE_NORMAL:
    if (MUI.getEHeatingMode())
      break;
    MUI.clearPage();
    lv_draw_dialog(DIALOG_EHEATINGMODE_NORMAL);
    break;
  case ID_EHEATINGMODE_HIGH:
    if (!MUI.getEHeatingMode())
      break;
    MUI.clearPage();
    lv_draw_dialog(DIALOG_EHEATINGMODE_HIGH);
    break;
  case ID_EHEATINGMODE_REBACK:
    MUI.clearPage();
    lv_draw_advance_settings();
    break;

  case ID_EHEATINGTEMP_PAGE_SW:
    uiCfg.para_ui_page ^= true;
    MUI.clearPage();
    MUI.drawPage_eHeatingTemperature();
    break;
  case ID_EHEATINGTEMP_REBACK:
    uiCfg.filament_load_heat_flg = 0;
    uiCfg.filament_unload_heat_flg = 0;
    MUI.clearPage();
    lv_draw_filament_change();
    break;
  case ID_EHEATINGTEMP_170c ... ID_EHEATINGTEMP_350c:
    celsius_t t = obj->mks_obj_id;
    thermalManager.setTargetHotend(t, uiCfg.extruderIndex);
    MUI.setEHeating(t);

    MUI.clearPage();
    if (uiCfg.print_state == IDLE)
    {
      uiCfg.leveling_first_time = 1;
      lv_draw_dialog(DIALOG_TYPE_FILAMENT_WAIT_START);
    }
    else if (uiCfg.print_state == WORKING)
    {
#if ENABLED(SDSUPPORT)
      card.pauseSDPrint();
      stop_print_time();
      uiCfg.print_state = PAUSING;
#endif

      lv_draw_dialog(DIALOG_TYPE_FILAMENT_PAUSING);
    }
    else
    {
      if (uiCfg.filament_load_heat_flg)
      {
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
      }
      else if (uiCfg.filament_unload_heat_flg)
      {
        lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
      }
    }
    break;
  }
}

void MixwareUI::updateAxis(lv_obj_t *btn, lv_obj_t *lbl) {
  switch (MPRE.moveAxis) {
    case X_AXIS:
      MPRE.moveAxis = Y_AXIS;
      lv_imgbtn_set_src_both(btn, MIMG.axisY);
      lv_label_set_text(lbl, MTR.y);
    break;
    case Y_AXIS:
      MPRE.moveAxis = Z_AXIS;
      lv_imgbtn_set_src_both(btn, MIMG.axisZ);
      lv_label_set_text(lbl, MTR.z);
    break;
    case Z_AXIS:
      MPRE.moveAxis = X_AXIS;
      lv_imgbtn_set_src_both(btn, MIMG.axisX);
      lv_label_set_text(lbl, MTR.x);
    break;
  }
}

void MixwareUI::updateZAxisDebug() {
  if(temps_update_flag) {
    if (DIALOG_IS(AXIS_Z_TEST_WAIT_START)) {
      if (uiCfg.leveling_first_time) {
        uiCfg.leveling_first_time = false;
        if (!all_axes_trusted())
          gcode.process_subcommands_now(PSTR("G28"));
      }
      else {
        if (!planner.has_blocks_queued()) {
          lv_clear_dialog();
          uiCfg.leveling_first_time = 1;
          lv_draw_dialog(DIALOG_AXIS_Z_TEST);
        }
      }
      temps_update_flag = false;
    }
    else if (DIALOG_IS(AXIS_Z_TEST)) {
      if (uiCfg.leveling_first_time) {
        uiCfg.leveling_first_time = false;

        if (MPRE.zAxisDebugFastMode) {
          // planner.synchronize();
          gcode.process_subcommands_now("G1 X150 Y100 F2000\nG1 Z399 F1500\nG1 Z1 F1500");
          // queue.inject(PSTR("G1 X150 Y100 F2000\nG1 Z399 F1500\nG1 Z1 F1500"));
        } else {
          // planner.synchronize();
          gcode.process_subcommands_now("G1 X150 Y100 F2000\nG1 Z399 F200\nG1 Z1 F200");
          // queue.inject(PSTR("G1 X150 Y100 F2000\nG1 Z399 F200\nG1 Z1 F200"));
        }
      }
      else {
        if (planner.has_blocks_queued())
          return;

        uiCfg.leveling_first_time = 1;
      }
      temps_update_flag = false;
    }
  }
}

void MixwareUI::updateAutoLeveling() {
  if (DIALOG_IS(AUTO_LEVELING)) {
    if (MUI.getAutoLevelingState() == LEVEL_STATE_NULL) {
      queue.inject_P(PSTR("G28\nG29"));
      MUI.setAutoLevelingState(LEVEL_STATE_LEVELING);
    }
    else if (MUI.getAutoLevelingState() == LEVEL_STATE_LEVELERR) {
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_AUTO_LEVEL_LEVELERR);
    }
    else if (MUI.getAutoLevelingState() == LEVEL_STATE_FINISHED) {
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_AUTO_LEVEL_FINISHED);
    }
  }
  else if (DIALOG_IS(ADJUST_Z_HEIGHT_WAIT_START)) {
    if (uiCfg.leveling_first_time) {
      if (planner.has_blocks_queued())
        return;
      uiCfg.leveling_first_time = false;
      if (!all_axes_trusted()) queue.enqueue_now_P(PSTR("G28"));
      queue.enqueue_now_P(PSTR("G1 X150 Y150 F3000"));
      queue.enqueue_now_P(PSTR("G1 Z0 F500"));
    }
    else {
      if (current_position.z == 0) {
        lv_clear_dialog();
        MUI.drawPage_zOffsetSetting();
      }
    }
  }
}

void MixwareUI::updateRunout() {
  if (DIALOG_IS(RUNOUT_UNLOAD) && (abs(thermalManager.temp_hotend[0].target - thermalManager.temp_hotend[0].celsius) <= 2)) {
    if (uiCfg.filament_heat_completed_unload) {
      uiCfg.filament_heat_completed_unload = 0;
      if (thermalManager.temp_hotend[0].target >= 210 &&  MPRE.extrusionVolume == 1.0)
         MPRE.extrusionVolume = 0.8;
      planner.synchronize();
      uiCfg.filament_unloading_time_flg = 1;
      uiCfg.filament_unloading_time_cnt = 0;
      ZERO(public_buf_m);
      sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E10 F%d\nG1 E-%d F%d\nG90"), uiCfg.extruderIndex, (int)(100* MPRE.extrusionVolume), gCfgItems.filamentchange_unload_length, (int)(gCfgItems.filamentchange_unload_speed* MPRE.extrusionVolume));
      queue.inject_P(PSTR(public_buf_m));
    }
    if (uiCfg.filament_unloading_completed) {
      uiCfg.filament_rate = 0;
      uiCfg.filament_unloading_completed = false;
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_RUNOUT_LOADTIPS);
    }
  }

  if (DIALOG_IS(RUNOUT_LOAD) && (abs(thermalManager.temp_hotend[0].target - thermalManager.temp_hotend[0].celsius) <= 2)) {
    if (uiCfg.filament_heat_completed_load) {
      uiCfg.filament_heat_completed_load = 0;
      if (thermalManager.temp_hotend[0].target >= 210 &&  MPRE.extrusionVolume == 1.0)
         MPRE.extrusionVolume = 0.8;
      planner.synchronize();
      uiCfg.filament_loading_time_flg = 1;
      uiCfg.filament_loading_time_cnt = 0;
      ZERO(public_buf_m);
      sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E%d F%d\nG90"), uiCfg.extruderIndex, gCfgItems.filamentchange_load_length, (int)(gCfgItems.filamentchange_load_speed* MPRE.extrusionVolume));
      queue.inject_P(PSTR(public_buf_m));
    }

    if (uiCfg.filament_loading_completed) {
      uiCfg.filament_rate = 0;
      uiCfg.filament_loading_completed = false;
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_RUNOUT_FINISH);
    }
  }

  if (DIALOG_IS(RUNOUT_LOAD, RUNOUT_UNLOAD)) lv_filament_setbar();
}

//
// level mode page
//
void MixwareUI::drawPage_level()
{
  scr = lv_screen_create(AUROLEVEL_UI, MTR.leveling);

  if (!uiCfg.para_ui_page)
  {
#if ENABLED(AUTO_BED_LEVELING_BILINEAR) && defined(Z_MIN_PROBE_PIN)
    if (READ(Z_MIN_PROBE_PIN) == false)
      lv_big_button_create(scr, MIMG.level2, MTR.leveling, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), eventHandler, ID_LEVEL_SELECT);
    else
#endif
      lv_big_button_create(scr, MIMG.levelManual, MTR.levelingManual, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), eventHandler, ID_LEVEL_MANUAL);

    lv_big_button_create(scr, MIMG.zOffset, MTR.offsetZ, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), eventHandler, ID_ZOFFSET);
  }
  else
  {
    lv_big_button_create(scr, MIMG.levelManual, MTR.levelingManual, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), eventHandler, ID_LEVEL_MANUAL);
    lv_big_button_create(scr, MIMG.levelAuto, MTR.levelingAuto, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), eventHandler, ID_LEVEL_AUTO);
  }

  ScreenReturnButton(scr, eventHandler, ID_LEVEL_REBACK);
}

//
//  Nozzle heating mode page
//
void MixwareUI::drawPage_eHeatingModeSetting()
{
  scr = lv_screen_create(EHEATINGMODE_SETTING_UI, MTR.ADVSeteHeatingMode);

  lv_big_button_create(scr, MIMG.eHeatingModeNormal, MTR.eHeatingModeNormal, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), eventHandler, ID_EHEATINGMODE_NORMAL);
  lv_big_button_create(scr, MIMG.eHeatingModeHigh, MTR.eHeatingModeHigh, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), eventHandler, ID_EHEATINGMODE_HIGH);

  lv_obj_t *labelTips = lv_label_create(scr, getEHeatingMode() ? MTR.eHeatingModeCurNormal : MTR.eHeatingModeCurHigh);
  lv_obj_align(labelTips, nullptr, LV_ALIGN_CENTER, 0, -130);

  ScreenReturnButton(scr, eventHandler, ID_EHEATINGMODE_REBACK);
}

//
// Nozzle heating temperature page
//
void MixwareUI::drawPage_eHeatingTemperature()
{
  scr = lv_obj_create(nullptr, nullptr);
  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  disp_state = EHEATINGTEMP_SETTING_UI;

  // title
  lv_obj_t *titleLabel = lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, MTR.temp);
  lv_obj_set_style(titleLabel, &tft_style_label_rel);

  if (!getEHeatingMode())
  {
    lv_obj_t *mode = lv_label_create(scr, "");
    lv_label_set_text(mode, "HIGH");
    lv_obj_align(mode, nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
  }

  lv_refr_now(lv_refr_get_disp_refreshing());
  if (uiCfg.para_ui_page == false)
  {
    for (int i = 0; i < 10; i++)
    {
      celsius_t id = ID_EHEATINGTEMP_170c + i * 10;
      if (getEHeating() >= id)
        ScreenToolButton(scr, std::to_string(id).c_str(), TOOLBTN_P_X(i), TOOLBTN_P_Y(i), eventHandler, id);
    }
    if (getEHeating() >= ID_EHEATINGTEMP_270c)
      ScreenBottomMiddleButton(scr, machine_menu.next, eventHandler, ID_EHEATINGTEMP_PAGE_SW);
  }
  else
  {
    for (int i = 0; i < 9; i++)
    {
      celsius_t id = ID_EHEATINGTEMP_270c + i * 10;
      if (getEHeating() >= id)
        ScreenToolButton(scr, std::to_string(id).c_str(), TOOLBTN_P_X(i), TOOLBTN_P_Y(i), eventHandler, id);
    }
    ScreenBottomMiddleButton(scr, machine_menu.previous, eventHandler, ID_EHEATINGTEMP_PAGE_SW);
  }

  ScreenReturnButton(scr, eventHandler, ID_EHEATINGTEMP_REBACK);
}

//
//  z device debug page
//
void MixwareUI::l_selfc_label_create(lv_coord_t y, const char *text) {
  lv_obj_t *l_tips = lv_label_create(scr, 40, y, text);
  lv_obj_set_size(l_tips, 200, DEBUG_LABEL_SIZE_H);
}

void MixwareUI::updateDeviceDebug() {
  static float cur_temp;
  static bool b_bitouch;

  if (debugState != debugState_n) {
    debugState = debugState_n;
    switch (debugState) {
      case MDEVICEDEBUGNULL:
        break;
      case MDEVICEDEBUGTEMP_E:
        if (thermalManager.temp_hotend[0].celsius > -10) {
          lv_label_set_text(l_temp_nozzle, PSTR("OK"));
        }
        else {
          lv_label_set_text(l_temp_nozzle, PSTR("NG"));
          lv_obj_set_style(l_temp_nozzle, &tft_style_label);
        }

        debugState_n++;
        break;
      case MDEVICEDEBUGHEAT_E:
        cur_temp = thermalManager.degHotend(0);

        if (cur_temp < -10) {
          lv_label_set_text(l_heat_nozzle, PSTR("NG"));
          lv_obj_set_style(l_heat_nozzle, &tft_style_label);

          debugState_n++;
        }
        else {
          uiCfg.hotendTargetTempBak = thermalManager.degTargetHotend(0);
          if (uiCfg.hotendTargetTempBak < 245) {
            thermalManager.temp_hotend[0].target = cur_temp+15;
          }
          else
            thermalManager.temp_hotend[0].target = 0;

          if (uiCfg.hotendTargetTempBak < 75) {
            thermalManager.temp_hotend[0].target = 75;
          }
          thermalManager.start_watching_hotend(0);

          if (cur_temp < 60) {
            h_timer = millis() + ((int)(60-cur_temp)/3+20)*1000;
          }
          else
            h_timer = millis() + 20000;
        }
        break;
      #if HAS_HEATED_BED
        case MDEVICEDEBUGTEMP_B:
          if (thermalManager.temp_bed.celsius > -10) {
            lv_label_set_text(l_temp_bed, PSTR("OK"));
          }
          else {
            lv_label_set_text(l_temp_bed, PSTR("NG"));
            lv_obj_set_style(l_temp_bed, &tft_style_label);
          }

          debugState_n++;
          break;
        case MDEVICEDEBUGHEAT_B:
          cur_temp = thermalManager.degBed();

          if (cur_temp < -10) {
            lv_label_set_text(l_heat_bed, PSTR("NG"));
            lv_obj_set_style(l_heat_bed, &tft_style_label);

            debugState_n++;
          }
          else {
            uiCfg.hotendTargetTempBak = thermalManager.degTargetBed();
            if (uiCfg.hotendTargetTempBak < 90)
              thermalManager.setTargetBed(100);
            else
              thermalManager.setTargetBed(0);

            h_timer = millis() + 20000;
          }
          break;
      #endif
      case MDEVICEDEBUGFAN:
        gcode.process_subcommands_now(PSTR("M106 S255"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        lv_label_set_text(l_fan, PSTR("ON"));
        debugState_n++;
        break;
      case MDEVICEDEBUGHOME_X:
        gcode.process_subcommands_now(PSTR("G91\nG1 X10 F1200\nG90"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(X_MIN_PIN) == !X_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_x_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_x_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        gcode.process_subcommands_now(PSTR("G28X"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(X_MIN_PIN) == X_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_x_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_x_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        gcode.process_subcommands_now(PSTR("G0 X85 F1200"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(X_MIN_PIN) == !X_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_x_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_x_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        planner.synchronize();
        queue.inject(PSTR("G1 X255 F3000\nG1 X285 F1000\nG1 X185 F2400\nG1 X10 F4200"));

        b_motor_ok = true;
        h_timer = millis() + 10000;
        break;
      case MDEVICEDEBUGHOME_Y:
        gcode.process_subcommands_now(PSTR("G91\nG1 Y10 F1200\nG90"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(Y_MIN_PIN) == !Y_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_y_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_y_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        gcode.process_subcommands_now(PSTR("G28Y"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(Y_MIN_PIN) == Y_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_y_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_y_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        gcode.process_subcommands_now(PSTR("G0 Y85 F1200"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(Y_MIN_PIN) == !Y_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_y_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_y_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        planner.synchronize();
        queue.inject(PSTR("G1 Y255 F3000\nG1 Y285 F1000\nG1 Y185 F2400\nG1 Y10 F4200"));

        b_motor_ok = true;
        h_timer = millis() + 10000;
        break;
      case MDEVICEDEBUGHOME_Z:
        gcode.process_subcommands_now(PSTR("G91\nG1 Z10 F1200\nG90"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(Z_MIN_PIN) == !Z_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_z_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_z_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        gcode.process_subcommands_now(PSTR("G28Z\nG1 Z0"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(Z_MIN_PIN) == Z_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_z_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_z_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        gcode.process_subcommands_now(PSTR("G0 Z185 F1200"));
        while(planner.has_blocks_queued())
          watchdog_refresh();
        if (READ(Z_MIN_PIN) == !Z_MIN_ENDSTOP_INVERTING) {
          lv_label_set_text(l_axis_z_motor, PSTR("NG"));
          lv_obj_set_style(l_axis_z_motor, &tft_style_label);
          debugState_n++;
          break;
        }

        planner.synchronize();
        queue.inject(PSTR("G1 Z325 F2200\nG1 Z385 F600\nG1 Z235 F2000\nG1 Z10 F1200"));

        b_motor_ok = true;
        h_timer = millis() + 10000;
        break;
      case MDEVICEDEBUGHOME_E:
        gcode.process_subcommands_now(PSTR("G0 X150 Y150 Z100 F2000"));
        while(planner.has_blocks_queued())
          watchdog_refresh();

        planner.synchronize();
        queue.inject(PSTR("T0\nG91\nG1 E10 F100\nG1 E-80 F800\nG1 E40 F200\nG90"));

        b_motor_ok = true;
        h_timer = millis() + 10000;
        break;
      case MDEVICEDEBUGSERVO:
        b_bitouch = false;
        bltouch.init();
        if (!bltouch.deploy()) {
          lv_label_set_text(l_servo, PSTR("OK"));
          b_bitouch = true;
        }
        else {
          lv_label_set_text(l_servo, PSTR("NG"));
          lv_obj_set_style(l_servo, &tft_style_label);
        }

        if (!bltouch.stow() && b_bitouch) {
          lv_label_set_text(l_servo, PSTR("OK"));
        }
        else {
          lv_label_set_text(l_servo, PSTR("NG"));
          lv_obj_set_style(l_servo, &tft_style_label);
        }

        debugState = debugState_n = MDEVICEDEBUGNULL;
        lv_label_set_text(l_debug, MTR.debugDevConfirm);
        gcode.process_subcommands_now(PSTR("M107\nM84"));

        thermalManager.setTargetHotend(0, uiCfg.hotendTargetTempBak);
        thermalManager.setTargetBed(uiCfg.hotendTargetTempBak);
        break;
    }
  }

  if (debugState != MDEVICEDEBUGNULL && debugState_n != MDEVICEDEBUGNULL) {
    static char debug_buf[30] = { 0 };
    const char MDEVDEBUGing_buf[3][8] = { ".", "..", "..."};
    static uint8_t index = 0;
    sprintf_P(debug_buf, PSTR("%s%s"), MTR.debugDevWorking, MDEVDEBUGing_buf[index++]);
    if (index >= 3)
      index = 0;
    lv_label_set_text(l_debug, debug_buf);

    if (debugState == debugState_n) {
      if (debugState == MDEVICEDEBUGHEAT_E) {
        if (millis() < h_timer)
          return;

        if (abs(thermalManager.temp_hotend[0].celsius - cur_temp) >= 5) {
          lv_label_set_text(l_heat_nozzle, PSTR("OK"));
        }
        else {
          lv_label_set_text(l_heat_nozzle, PSTR("NG"));
          lv_obj_set_style(l_heat_nozzle, &tft_style_label);
        }

        thermalManager.temp_hotend[0].target = 200;
        thermalManager.start_watching_hotend(0);
        debugState_n++;
      }
      #if HAS_HEATED_BED
        else if (debugState == MDEVICEDEBUGHEAT_B) {
          if (millis() < h_timer)
            return;

          if ((thermalManager.temp_bed.celsius - cur_temp) >= 2) {
            lv_label_set_text(l_heat_bed, PSTR("OK"));
          }
          else {
            lv_label_set_text(l_heat_bed, PSTR("NG"));
            lv_obj_set_style(l_heat_bed, &tft_style_label);
          }

          debugState_n++;
        }
      #endif
      else if (debugState == MDEVICEDEBUGHOME_X || debugState == MDEVICEDEBUGHOME_Y || debugState == MDEVICEDEBUGHOME_Z || debugState == MDEVICEDEBUGHOME_E) {
        if (millis() < h_timer)
          return;

        if (!planner.has_blocks_queued()) {
          if (b_motor_ok) {
            b_motor_ok = false;
            if (debugState == MDEVICEDEBUGHOME_X) lv_label_set_text(l_axis_x_motor, PSTR("OK"));
            if (debugState == MDEVICEDEBUGHOME_Y) lv_label_set_text(l_axis_y_motor, PSTR("OK"));
            if (debugState == MDEVICEDEBUGHOME_Z) lv_label_set_text(l_axis_z_motor, PSTR("OK"));
            if (debugState == MDEVICEDEBUGHOME_E) lv_label_set_text(l_axis_e_motor, PSTR("OK"));
            debugState_n++;
          }
        }
      }
    }
  }
}

void MixwareUI::drawPage_deviceDebug()
{
  scr = lv_screen_create(DEBUG_SELFC_UI, MTR.debugDevTitle);

  debugState = debugState_n = MDEVICEDEBUGNULL;

  uint8_t index = 0;
  b_motor_ok = false;
  // Create an Image button
  l_temp_nozzle = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsETemp);
  lv_obj_set_size(l_temp_nozzle,  DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  #if HAS_HEATED_BED
    l_temp_bed = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
    l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsBTemp);
    lv_obj_set_size(l_temp_bed,   DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);
  #endif

  l_heat_nozzle = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsEHeat);
  lv_obj_set_size(l_heat_nozzle,  DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  #if HAS_HEATED_BED
    l_heat_bed = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
    l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsBHeat);
    lv_obj_set_size(l_heat_bed,   DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);
  #endif

  l_fan = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsFan);
  lv_obj_set_size(l_fan,          DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  l_axis_x_motor  = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsX);
  lv_obj_set_size(l_axis_x_motor, DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  l_axis_y_motor  = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsY);
  lv_obj_set_size(l_axis_y_motor, DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  l_axis_z_motor  = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsZ);
  lv_obj_set_size(l_axis_z_motor, DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  l_axis_e_motor  = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsE);
  lv_obj_set_size(l_axis_e_motor, DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  l_servo         = lv_label_create(scr, DEBUG_LABEL_POS_X, DEBUG_LABEL_POS_Y(index), nullptr);
  l_selfc_label_create(DEBUG_LABEL_POS_Y(index++), MTR.debugDevTipsServo);
  lv_obj_set_size(l_servo,        DEBUG_LABEL_SIZE_W, DEBUG_LABEL_SIZE_H);

  l_selfc_reset_label();

  lv_obj_t* btn_check = lv_btn_create(scr, 20, 290, 280, 64, eventHandler, ID_DEVICEDEBUG_WORK);
  l_debug = lv_label_create(btn_check, MTR.debugDevConfirm);
  lv_btn_set_style_both(btn_check, &style_para_button);
  lv_obj_align(l_debug, btn_check, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t* btn_zaxis = lv_btn_create(scr, 20, 290+69, 280, 64, eventHandler, ID_DEVICEDEBUG_TO_Z);
  lv_obj_t* l_zaxis = lv_label_create(btn_zaxis, MTR.debugZTitle);
  lv_btn_set_style_both(btn_zaxis, &style_para_button);
  lv_obj_align(l_zaxis, btn_zaxis, LV_ALIGN_CENTER, 0, 0);

  ScreenReturnButton(scr, eventHandler, ID_DEVICEDEBUG_REBACK);
}

//
//  axis move page
//
void MixwareUI::drawPage_move()
{
}

//
//  z axis debug page
//
void MixwareUI::drawPage_zAxisDebug()
{
  scr = lv_screen_create(ZAXISDEBUG_UI, MTR.debugZTitle);

  // Create an Image button
  lv_big_button_create(scr, MIMG.zAxisDebugSlow, MTR.debugZSlowMode, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), eventHandler, ID_ZAXISDEBUG_SLOW);
  lv_big_button_create(scr, MIMG.zAxisDebugFast, MTR.debugZFastMode, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), eventHandler, ID_ZAXISDEBUG_FAST);

  ScreenReturnButton(scr, eventHandler, ID_ZAXISDEBUG_REBACK);
}

//
// Z Axis offset page
//
void MixwareUI::moveDistanceUpdate()
{
  if ((int)(100 * uiCfg.move_dist) == 5)
  {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, MIMG.moveDistance_1_mm);
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, MIMG.moveDistance_1_mm);
  }
  else if ((int)(100 * uiCfg.move_dist) == 10)
  {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, MIMG.moveDistance_10_mm);
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, MIMG.moveDistance_10_mm);
  }
  if (gCfgItems.multiple_language != 0)
  {
    if ((int)(100 * uiCfg.move_dist) == 5)
    {
      lv_label_set_text(labelStep, MTR.moveDistanceMM005);
    }
    else if ((int)(100 * uiCfg.move_dist) == 10)
    {
      lv_label_set_text(labelStep, MTR.moveDistanceMM01);
    }
    lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET-10);
  }
}

void MixwareUI::zOffsetUpdate()
{
  char str_1[16];
  sprintf_P(public_buf_l, PSTR("%s mm"), dtostrf(probe.offset.z, 1, 2, str_1));
  lv_label_set_text(labelZOffset, public_buf_l);
}

void MixwareUI::drawPage_zOffsetSetting()
{
  scr = lv_screen_create(ZOFFSET_SETTING_UI, MTR.offsetZ);
  bak_zOffset = probe.offset.z;
  MPRE.zOffsetChanged = false;

  lv_big_button_create(scr, MIMG.save, MTR.save, IMAGEBTN_P_X(2), IMAGEBTN_P_Y(2), eventHandler, ID_ZOFFSET_SAVE);
  buttonStep = lv_imgbtn_create(scr, nullptr, IMAGEBTN_P_X(3), IMAGEBTN_P_Y(3), eventHandler, ID_ZOFFSET_STEP);
  lv_big_button_create(scr, MIMG.add, MTR.offsetZAdd, IMAGEBTN_P_X(4), IMAGEBTN_P_Y(4), eventHandler, ID_ZOFFSET_ADD);
  lv_big_button_create(scr, MIMG.dec, MTR.offsetZDec, IMAGEBTN_P_X(5), IMAGEBTN_P_Y(5), eventHandler, ID_ZOFFSET_DEC);

  ScreenReturnButton(scr, eventHandler, ID_ZOFFSET_REBACK);

  labelStep = lv_label_create_empty(buttonStep);

  /*Create an Image button*/
  lv_obj_t *buttonZ_offset = lv_img_create(scr, NULL);
  lv_img_set_src(buttonZ_offset, MIMG.stateZ);
  lv_obj_set_pos(buttonZ_offset, 125, 85);

  labelZOffset = lv_label_create(scr, 170, 95, nullptr);

  moveDistanceUpdate();
  zOffsetUpdate();

  ButtonAddClickTips(buttonStep);
}

void MixwareUI::clearPage()
{
  lv_obj_del(scr);
}

void MixwareUI::printPauseUIForFilament()
{
  // planner.synchronize();  //can not add.
  if (uiCfg.dialogType == DIALOG_RUNOUT_PAUSING) {
    uiCfg.moveSpeed_bak = feedrate_mm_s;
    uiCfg.hotendTargetTempBak = thermalManager.temp_hotend[0].target;
    clear_cur_ui();
    lv_draw_dialog(DIALOG_RUNOUT_UNLOAD);
  }
  else if (uiCfg.dialogType == DIALOG_TYPE_FILAMENT_PAUSING) {
    lv_clear_dialog();
    if (uiCfg.filament_load_heat_flg) {
      lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_HEAT);
    }
    else if (uiCfg.filament_unload_heat_flg) {
      lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_HEAT);
    }
  }
}

FilamentDetector detector;

#ifndef FILAMENT_DETECTOR_ALARM_GAP
#define FILAMENT_DETECTOR_ALARM_GAP 16
#endif

#ifndef FILAMENT_DETECTOR_STOP_GAP
#define FILAMENT_DETECTOR_STOP_GAP 22
#endif

#ifndef FILAMENT_DETECTOR_BLOCK_NUMBER
#define FILAMENT_DETECTOR_BLOCK_NUMBER 15
#endif

#ifndef FILAMENT_DETECTOR_BLOCK_LENGHT
#define FILAMENT_DETECTOR_BLOCK_LENGHT 2400
#endif

#ifndef FILAMENT_DETECTOR_FILE_START_POS
#define FILAMENT_DETECTOR_FILE_START_POS 512
#endif

#ifndef FILAMENT_DETECTOR_EFFECTIVE_LENGTH
#define FILAMENT_DETECTOR_EFFECTIVE_LENGTH 64
#define FILAMENT_DETECTOR_EFFECTIVE_GAP (planner.settings.axis_steps_per_mm[E_AXIS] * FILAMENT_DETECTOR_EFFECTIVE_LENGTH)
#endif

int32_t FilamentDetector::last_pos;
int32_t FilamentDetector::cur_pos;
int32_t FilamentDetector::alarm_gap;
int32_t FilamentDetector::stop_gap;
uint8_t FilamentDetector::block_count;
int32_t FilamentDetector::cur_gap;
millis_t FilamentDetector::last_time;
millis_t FilamentDetector::update_time;

void FilamentDetector::reset()
{
  WRITE(BEEPER_PIN, LOW);
  MPRE.filamentIsBroken = false;
  last_pos = block_count = 0;
  last_time = millis();
  alarm_gap = planner.settings.axis_steps_per_mm[E_AXIS] * FILAMENT_DETECTOR_ALARM_GAP;
  stop_gap = planner.settings.axis_steps_per_mm[E_AXIS] * FILAMENT_DETECTOR_STOP_GAP;
}

void FilamentDetector::update()
{
  if (!MUI.getEHeatingMode() || card.getIndex() < FILAMENT_DETECTOR_FILE_START_POS || stepper.position(E_AXIS) < FILAMENT_DETECTOR_EFFECTIVE_GAP) // || current_position[Z_AXIS] < 0.5
    return;

  static uint8_t last_state = 0;
  const uint8_t cur_state = (READ(MT_DET_1_PIN) ? _BV(0) : 0),
                change = last_state ^ cur_state;
  last_state = cur_state;

  const int32_t cur_pos = stepper.position(E_AXIS);
  if (!cur_pos)
    last_pos = 0;

  if (change || !last_pos)
  {
    uint32_t gap = cur_pos - last_pos;
    const millis_t ms = millis();
    update_time = ms - last_time;
    const millis_t space_time = 1000 * gap / planner.settings.axis_steps_per_mm[E_AXIS] / (feedrate_mm_s * feedrate_percentage / 100);
    if (gap > FILAMENT_DETECTOR_BLOCK_LENGHT && update_time > space_time)
      block_count++;
    else
      block_count = 0;

    last_time = ms;
    last_pos = cur_pos;
  }

  cur_gap = cur_pos - last_pos;
  if (IS_SD_PRINTING())
    alarm();
}

void FilamentDetector::updateUI()
{
  update();

  if (detector.has_break()) {
    uiCfg.moveSpeed_bak = feedrate_mm_s;
    MPRE.filamentIsBroken = true;

    card.pauseSDPrint();
    stop_print_time();
    uiCfg.print_state = PAUSING;

    WRITE(BEEPER_PIN, LOW);
    clear_cur_ui();
    if (detector.has_block()) {
      lv_draw_dialog(DIALOG_TYPE_FILAMENT_CLOG);
    }
    else {
      uiCfg.filament_heat_completed_unload = 1;
      lv_draw_dialog(DIALOG_RUNOUT_PAUSING);
    }
  }
}

void FilamentDetector::alarm()
{
  const bool a = (MUI.getEHeatingMode() && stepper.axis_is_moving(E_AXIS) && last_pos && stepper.position(E_AXIS)) ? (bool)(cur_gap >= alarm_gap) : false;

  if (a)
    WRITE(BEEPER_PIN, HIGH);
  else
    WRITE(BEEPER_PIN, LOW);
}

bool FilamentDetector::has_break()
{
  return (MUI.getEHeatingMode() && stepper.axis_is_moving(E_AXIS) && last_pos && stepper.position(E_AXIS)) ? (bool)(cur_gap >= stop_gap) : false;
}

bool FilamentDetector::has_block()
{
  return (bool)(MUI.getEHeatingMode() && block_count > FILAMENT_DETECTOR_BLOCK_NUMBER);
}

Printing_Babystep p_babystep;

Printing_Babystep::Printing_Babystep()
{
  init();
}

void Printing_Babystep::init()
{
  save_probe = start_probe = end_probe = probe.offset.z;
}

void Printing_Babystep::reset()
{
  start_probe = end_probe = probe.offset.z;
}

void Printing_Babystep::update()
{
  end_probe = probe.offset.z;
}

bool Printing_Babystep::isChanged()
{
  return (bool)(!(start_probe == end_probe && save_probe == end_probe));
}
// }

#endif // HAS_TFT_LVGL_UI
