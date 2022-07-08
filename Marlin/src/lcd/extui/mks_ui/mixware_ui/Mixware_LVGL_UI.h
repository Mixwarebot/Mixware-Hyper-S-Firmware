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

#include "../../../../inc/MarlinConfig.h"

#include <lvgl.h>
#include <lv_conf.h>

#ifdef TFT_MIXWARE_LVGL_UI
#include "image_def.h"
// namespace MixwareUI
// {
//
//
//
enum
{
  ID_LEVEL = 1,
  ID_LEVEL_SELECT,
  ID_LEVEL_AUTO,
  ID_LEVEL_MANUAL,
  ID_LEVEL_REBACK,

  ID_ZOFFSET,
  ID_ZOFFSET_ADD,
  ID_ZOFFSET_DEC,
  ID_ZOFFSET_STEP,
  ID_ZOFFSET_SAVE,
  ID_ZOFFSET_REBACK,

  ID_ZAXISDEBUG_SLOW,
  ID_ZAXISDEBUG_FAST,
  ID_ZAXISDEBUG_REBACK,

  ID_DEVICEDEBUG_WORK,
  ID_DEVICEDEBUG_TO_Z,
  ID_DEVICEDEBUG_REBACK,

  ID_EHEATINGMODE_NORMAL,
  ID_EHEATINGMODE_HIGH,
  ID_EHEATINGMODE_REBACK,
  ID_EHEATINGTEMP_PAGE_SW,
  ID_EHEATINGTEMP_REBACK,
  ID_EHEATINGTEMP_170c = 170,
  ID_EHEATINGTEMP_180c = 180,
  ID_EHEATINGTEMP_190c = 190,
  ID_EHEATINGTEMP_200c = 200,
  ID_EHEATINGTEMP_210c = 210,
  ID_EHEATINGTEMP_220c = 220,
  ID_EHEATINGTEMP_230c = 230,
  ID_EHEATINGTEMP_240c = 240,
  ID_EHEATINGTEMP_250c = 250,
  ID_EHEATINGTEMP_260c = 260,
  ID_EHEATINGTEMP_270c = 270,
  ID_EHEATINGTEMP_280c = 280,
  ID_EHEATINGTEMP_290c = 290,
  ID_EHEATINGTEMP_300c = 300,
  ID_EHEATINGTEMP_310c = 310,
  ID_EHEATINGTEMP_320c = 320,
  ID_EHEATINGTEMP_330c = 330,
  ID_EHEATINGTEMP_340c = 340,
  ID_EHEATINGTEMP_350c = 350
};

enum
{
  MDEVICEDEBUGNULL = 1,
  MDEVICEDEBUGTEMP_E,
#if HAS_HEATED_BED
  MDEVICEDEBUGTEMP_B,
#endif
  MDEVICEDEBUGHEAT_E,
#if HAS_HEATED_BED
  MDEVICEDEBUGHEAT_B,
#endif
  MDEVICEDEBUGFAN,
  MDEVICEDEBUGHOME_X,
  MDEVICEDEBUGHOME_Y,
  MDEVICEDEBUGHOME_Z,
  MDEVICEDEBUGHOME_E,
  MDEVICEDEBUGSERVO
};

typedef enum
{
  LEVEL_STATE_NULL,
  LEVEL_STATE_HOMEING,
  LEVEL_STATE_LEVELING,
  LEVEL_STATE_LEVELERR,
  LEVEL_STATE_FINISHED
} MixwareAutoLevelState;

typedef struct
{
  const char *main;
  const char *tempRange1c;
  const char *tempRange5c;
  const char *tempRange10c;
  const char *preheatPLA;
  const char *preheatTPU;
  const char *preheatPETG;
  const char *preheatPACF;
  const char *x;
  const char *y;
  const char *z;
  const char *e;
  const char *moveDistanceMM005;
  const char *moveDistanceMM01;
  const char *moveDistanceMM1;
  const char *moveDistanceMM5;
  const char *moveDistanceMM10;
  const char *print;
  const char *tool;
  const char *set;
  const char *add;
  const char *dec;
  const char *back;
  const char *save;
  const char *clickTips;
  const char *waitTips;
  const char *nozzle;
  const char *bed;
  const char *temp;
  const char *preheat;
  const char *Cool;
  const char *more;
  const char *fan;
  const char *fanOn;
  const char *fanOff;
  const char *filament;
  const char *filIn;
  const char *filInTips;
  const char *filInTipsHeat;
  const char *filInTipsConfirm;
  const char *filInTipsWork;
  const char *filInTipsExit;
  const char *filOut;
  const char *filOutTips;
  const char *filOutTipsHeat;
  const char *filOutTipsWork;
  const char *filOutTipsExit;
  const char *filTempAdjust;
  const char *move;
  const char *home;
  const char *homeAll;
  const char *axisDisabled;
  const char *leveling;
  const char *levelingAuto;
  const char *levelingAutoTipsStart;
  const char *levelingAutoTipsWorking;
  const char *levelingAutoTipsExit;
  const char *levelingAutoTipsFailed;
  const char *levelingManual;
  const char *levelingManualFL;
  const char *levelingManualFR;
  const char *levelingManualC;
  const char *levelingManualBL;
  const char *levelingManualBR;
  const char *offsetX;
  const char *offsetY;
  const char *offsetZ;
  const char *offsetZAdd;
  const char *offsetZDec;
  const char *flash;
  const char *flashReset;
  const char *flashResetTips;
  const char *flashSave;
  const char *flashSaveTips;
  const char *ADVSet;
  const char *ADVSetAccel;
  const char *ADVSetAccelPrint;
  const char *ADVSetAccelRetract;
  const char *ADVSetAccelTravel;
  const char *ADVSetAccelX;
  const char *ADVSetAccelY;
  const char *ADVSetAccelZ;
  const char *ADVSetAccelE;
  const char *ADVSetMaxSpeed;
  const char *ADVSetMaxSpeedX;
  const char *ADVSetMaxSpeedY;
  const char *ADVSetMaxSpeedZ;
  const char *ADVSetMaxSpeedE;
  const char *ADVSetJerk;
  const char *ADVSetJerkX;
  const char *ADVSetJerkY;
  const char *ADVSetJerkZ;
  const char *ADVSetJerkE;
  const char *ADVSetStep;
  const char *ADVSetStepX;
  const char *ADVSetStepY;
  const char *ADVSetStepZ;
  const char *ADVSetStepE;
  const char *ADVSetPausePos;
  const char *ADVSetPausePosX;
  const char *ADVSetPausePosY;
  const char *ADVSetPausePosZ;
  const char *ADVSetPausePosTips;
  const char *ADVSetFil;
  const char *ADVSetFilInLength;
  const char *ADVSetFilInSpeed;
  const char *ADVSetFilOutLength;
  const char *ADVSetFilOutSpeed;
  const char *ADVSeteHeatingMode;
  const char *eHeatingModeNormal;
  const char *eHeatingModeHigh;
  const char *eHeatingModeCurNormal;
  const char *eHeatingModeCurHigh;
  const char *eHeatingModeTipsNormal;
  const char *eHeatingModeTipsHigh;
  const char *eHeatingModeTips;
  const char *eHeatingModeTipsAbb;
  const char *about;
  const char *language;
  const char *filamentDetector;
  const char *filamentDetectorOn;
  const char *filamentDetectorOff;
  const char *filamentDetectorTipsPause;
  const char *filamentDetectorClog;
  const char *filamentDetectorTipsClog;
  const char *filamentDetectorTipsHew;
  const char *filamentDetectorPurge;
  const char *filamentDetectorPrint;
  const char *debugDevTitle;
  const char *debugDevConfirm;
  const char *debugDevWorking;
  const char *debugDevTipsETemp;
  const char *debugDevTipsBTemp;
  const char *debugDevTipsEHeat;
  const char *debugDevTipsBHeat;
  const char *debugDevTipsFan;
  const char *debugDevTipsX;
  const char *debugDevTipsY;
  const char *debugDevTipsZ;
  const char *debugDevTipsE;
  const char *debugDevTipsServo;
  const char *debugZTitle;
  const char *debugZSlowMode;
  const char *debugZFastMode;
  const char *debugZWorking;
  const char *printAgain;
  const char *printTipsSaveOffset;
  const char *printTipsSave;
  const char *printPause;
  const char *printResume;
  const char *printStop;
  const char *previous;
  const char *next;
  const char *keyBack;
  const char *keyReset;
  const char *keyConfirm;
  const char *printNoFileTips;
  const char *operation;
  const char *speed;
  const char *babystep;
  const char *thermalProtection;
  const char *key1;
  const char *key2;
  const char *key3;
  const char *key4;
  const char *key5;
  const char *key6;
  const char *key7;
  const char *key8;
  const char *key9;
  const char *key0;
  const char *keyPoint;
  const char *keyNegative;
} MixwareUIText;

typedef struct
{
  const char *homeAll;
  const char *homeX;
  const char *homeY;
  const char *homeZ;
  const char *motorOff;

  const char *preheat;
  const char *fan;
  const char *move;
  const char *level;
  const char *filament;
  const char *home;

  const char *add;
  const char *dec;
  const char *rate1;
  const char *rate5;
  const char *rate10;

  const char *bed;
  const char *temperatureRange1;
  const char *temperatureRange5;
  const char *temperatureRange10;

  const char *filamentIn;
  const char *filamentOut;
  const char *extruct;
  const char *speedHigh;
  const char *speedSlow;
  const char *speedNormal;
  const char *moveDistance_1_mm;
  const char *moveDistance_5_mm;
  const char *moveDistance_10_mm;

  const char *dir;
  const char *file;

  const char *axisX;
  const char *axisY;
  const char *axisZ;
  const char *moveDistance_0_1_mm;

  const char *speed;

  const char *printPause;
  const char *printResume;
  const char *printStop;
  const char *stateX;
  const char *stateY;
  const char *stateZ;
  const char *stateExtruct;
  const char *stateBed;
  const char *stateFan;
  const char *stateTime;
  const char *operate;

  const char *tool;
  const char *set;
  const char *print;

  const char *preheatPLA;
  const char *preheatPETG;
  const char *preheatPACF;
  const char *preheatBed;
  const char *preheatCool;

  const char *level2;
  const char *levelPos1;
  const char *levelPos2;
  const char *levelPos3;
  const char *levelPos4;
  const char *levelPos5;
  const char *levelAuto;
  const char *levelManual;
  const char *save;
  const char *babystep;
  const char *zOffset;

  const char *EEPROM;
  const char *about;
  const char *configuration;

  const char *zAxisDebugSlow;
  const char *zAxisDebugFast;
  const char *deviceDebug;

  const char *language;
  const char *languageEN;
  const char *languageENSEL;
  const char *languageSCN;
  const char *languageSCNSEL;
  const char *languageTCN;
  const char *languageTCNSEL;

  const char *filamentDetector;
  const char *filamentDetectorOn;
  const char *filamentDetectorOff;

  const char *arrow;
  const char *placeholder;

  const char *eHeatingModeNormal;
  const char *eHeatingModeHigh;
  const char *printHM;
  const char *preheatHM;
  const char *extructHM;
  const char *stateExtructHM;
  const char *eHeatingModeTip;

  const char *preview;
  const char *logo;
  const char *mainLogo;

  const char *enable;
  const char *disable;
} MixwareUIImage;

typedef struct
{
  celsius_t eHeatingMaxTemp;                // normal mode: 260c high mode: 350c
  bool filamentDetectorEnabled;             // Enabled: true
  bool thermalProtectionEnabled;            // Enabled: true
}MixwareUIFalshParam;

typedef struct
{
  bool zOffsetChanged;                      // babystep changed: true
  bool zAxisDebugFastMode;
  bool filamentIsBroken;
  int16_t moveAxis;
  float extrusionVolume;

  MixwareUIFalshParam param;
} MixwareUIPreference;

typedef struct
{
  MixwareUIText tr;
  MixwareUIImage image;
  MixwareUIPreference preference;
  MixwareAutoLevelState autoLevelStatus;
} MixwareUIInfo;

extern lv_point_t line_points_2[8][2];

class MixwareUI
{
private:
  lv_obj_t *scr;

  lv_style_t tft_style_label;
  lv_style_t style_para_button;
  lv_style_t style_para_tips;

  void ScreenBottomButton(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id);
  void l_selfc_label_create(lv_coord_t y, const char *text);

public:
  MixwareUIInfo info;

  inline void setAutoLevelingState(MixwareAutoLevelState s) { info.autoLevelStatus = s; }
  inline MixwareAutoLevelState getAutoLevelingState() { return info.autoLevelStatus; }

  inline void setEHeating(celsius_t t) { info.preference.param.eHeatingMaxTemp = t; }
  inline celsius_t getEHeating() { return info.preference.param.eHeatingMaxTemp; }
  inline bool getEHeatingMode() { return (getEHeating() < 300); }
  inline const char *getEHeatingModeString() { return (getEHeatingMode() ? info.tr.eHeatingModeNormal : info.tr.eHeatingModeHigh); }
  inline const char *getEHeatingModeImage() { return (getEHeatingMode() ? info.image.eHeatingModeNormal : info.image.eHeatingModeHigh); }

  inline bool getFilamentDetectorstate() { return (info.preference.param.filamentDetectorEnabled); }
  inline void toggleFilamentDetector() { info.preference.param.filamentDetectorEnabled ^= true; }
  inline const char *getFilamentDetectorString() { return (getFilamentDetectorstate() ? info.tr.filamentDetectorOn : info.tr.filamentDetectorOff); }
  inline const char *getFilamentDetectorImage() { return (getFilamentDetectorstate() ? info.image.filamentDetectorOn : info.image.filamentDetectorOff); }

  inline void setThermalProtection(bool b) { info.preference.param.thermalProtectionEnabled = b; }
  inline void toggleThermalProtection() { info.preference.param.thermalProtectionEnabled ^= true; }
  inline bool getThermalProtection() { return info.preference.param.thermalProtectionEnabled; }

  void translatorInit();
  void textRevisionInit();
  void imagePathInit();
  void preferenceInit();
  void styleInit();

  //
  //  page button
  //
  void ButtonAddClickTips(lv_obj_t *button);
  void ScreenPlaceholder();
  void ScreenMainLogo(lv_obj_t *par);
  void ScreenMainTips(lv_obj_t *par);
  lv_obj_t *ScreenLabel(lv_obj_t *par, const char *text);
  void ScreenBottomLeftButton(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id);
  void ScreenBottomMiddleButton(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id);
  void ScreenBottomRightButton(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id);
  void ScreenReturnButton(lv_obj_t *par, lv_event_cb_t cb, const int id);
  lv_obj_t *ScreenToolButton(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id);

  lv_obj_t *ScreenEnabledButton(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, const int index, const bool curValue);
  void updateEnabledButton(lv_obj_t *btn, const bool curValue);
  void updateAxis(lv_obj_t *btn, lv_obj_t *lbl);

  void updateZAxisDebug();
  void updateAutoLeveling();
  void updateRunout();
  //
  // level mode page
  //
  void drawPage_level();

  //
  //  Nozzle heating mode page
  //
  void drawPage_eHeatingModeSetting();

  //
  // Nozzle heating temperature page
  //
  void drawPage_eHeatingTemperature();

  //
  //  z device debug page
  //
  void updateDeviceDebug();
  void drawPage_deviceDebug();

  //
  //  axis move page
  //
  void drawPage_move();

  //
  //  z axis debug page
  //
  void drawPage_zAxisDebug();

  //
  // Z Axis offset page
  //
  void moveDistanceUpdate();
  void zOffsetUpdate();
  void drawPage_zOffsetSetting();

  void clearPage();

  void printPauseUIForFilament();
};
extern MixwareUI mixware_ui;

class FilamentDetector
{
private:
  static int32_t last_pos;
  static int32_t cur_pos;
  static int32_t alarm_gap;
  static int32_t stop_gap;
  static uint8_t block_count;
  static int32_t cur_gap;
  static millis_t last_time;
  static millis_t update_time;

public:
  static void reset();
  static void alarm();
  static bool has_break();
  static bool has_block();

  static void update();
  static void updateUI();
};

extern FilamentDetector detector;

class Printing_Babystep
{
private:
  float save_probe;
  float start_probe;
  float end_probe;

public:
  Printing_Babystep();

  void init();
  void reset();
  void update();
  bool isChanged();
};

extern Printing_Babystep p_babystep;
// }
#endif
