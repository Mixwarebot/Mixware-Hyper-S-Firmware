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
#include "mixware_ui_def.h"
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

  ID_HEATINGMODE_NORMAL,
  ID_HEATINGMODE_HIGH,
  ID_HEATINGMODE_REBACK,
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
  const char *move_distance_mm_0_0_5;
  const char *move_distance_mm_0_1;
  const char *move_distance_mm_1;
  const char *move_distance_mm_5;
  const char *move_distance_mm_10;
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
  const char *heating_mode_mormal;
  const char *heating_mode_high;
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
  const char *print_again;
  const char *printTipsSaveOffset;
  const char *printTipsSave;
  const char *printPause;
  const char *printResume;
  const char *printStop;
  const char *previous;
  const char *next;
  const char *key_back;
  const char *key_reset;
  const char *key_confirm;
  const char *no_file_tips;
  const char *operation;
  const char *speed;
  const char *babystep;
  const char *thermal_protection;
  const char *key_1;
  const char *key_2;
  const char *key_3;
  const char *key_4;
  const char *key_5;
  const char *key_6;
  const char *key_7;
  const char *key_8;
  const char *key_9;
  const char *key_0;
  const char *key_point;
  const char *key_negative;
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
  const char *move_distance_mm_1;
  const char *move_distance_mm_5;
  const char *move_distance_mm_10;

  const char *dir;
  const char *file;

  const char *axisX;
  const char *axisY;
  const char *axisZ;
  const char *move_distance_mm_0_1;

  const char *speed;

  const char *printPause;
  const char *printResume;
  const char *printStop;
  const char *stateX;
  const char *stateY;
  const char *stateZ;
  const char *state_extruct;
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

  const char *heating_mode_mormal;
  const char *heating_mode_high;
  const char *heating_mode_print;
  const char *heating_mode_preheat;
  const char *heating_mode_extruct;
  const char *heating_mode_state_extruct;
  const char *heating_mode_tips;

  const char *preview;
  const char *logo;
  const char *logo_main;

  const char *enable;
  const char *disable;
} mui_image_path_t;

typedef struct
{
  celsius_t max_heating_mode_temp;                // normal mode: 260c high mode: 350c
  bool enabled_filament_detector;             // Enabled: true
  bool enabled_thermal_protection;            // Enabled: true
}mui_falsh_param_t;

typedef struct
{
  bool is_z_offset_changed;                      // babystep changed: true
  bool is_z_axis_debug_fast_mode;
  bool is_filament_broken;
  int16_t move_axis;
  float extrusion_volume;

  mui_falsh_param_t param;
} mui_preference_t;

typedef struct
{
  MixwareUIText tr;
  mui_image_path_t image;
  mui_preference_t preference;
  MixwareAutoLevelState auto_leveling_state;
} MixwareUI_Info;

class MixwareUI
{
public:
  lv_obj_t *page_scr;

  lv_style_t page_label_style;
  lv_style_t page_button_style;
  lv_style_t page_tips_style;

  static char HEATINGMODE_NORMAL_CMD[], HEATINGMODE_HIGH_CMD[];

  static lv_point_t page_line_pos[8][2];

  void page_bottom_button(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id);

  MixwareUI_Info info;

  static inline lv_point_t *get_page_line_pos(const int index) { return page_line_pos[index]; }

  inline void set_auto_leveling_state(MixwareAutoLevelState state) { info.auto_leveling_state = state; }
  inline MixwareAutoLevelState get_auto_leveling_state() { return info.auto_leveling_state; }

  inline void set_heating_mode_temperature(celsius_t temp) { info.preference.param.max_heating_mode_temp = temp; }
  inline celsius_t get_heating_mode_temperature() { return info.preference.param.max_heating_mode_temp; }
  inline bool get_heating_mode() { return (get_heating_mode_temperature() < 300); }
  inline const char *get_heating_mode_tr() { return (get_heating_mode() ? info.tr.heating_mode_mormal : info.tr.heating_mode_high); }
  inline const char *get_heating_mode_image_path() { return (get_heating_mode() ? info.image.heating_mode_mormal : info.image.heating_mode_high); }

  inline bool get_filament_detector_state() { return (info.preference.param.enabled_filament_detector); }
  inline void toggle_filament_detector() { info.preference.param.enabled_filament_detector ^= true; }
  inline const char *get_filament_detector_tr() { return (get_filament_detector_state() ? info.tr.filamentDetectorOn : info.tr.filamentDetectorOff); }
  inline const char *get_filament_detector_image_path() { return (get_filament_detector_state() ? info.image.filamentDetectorOn : info.image.filamentDetectorOff); }

  inline void set_thermal_protection(bool en) { info.preference.param.enabled_thermal_protection = en; }
  inline void toggle_thermal_protection() { info.preference.param.enabled_thermal_protection ^= true; }
  inline bool get_thermal_protection() { return info.preference.param.enabled_thermal_protection; }

  void translator_init();
  void text_revision_init();
  void image_path_init();
  void preference_init();
  void style_init();

  //
  //  page button
  //
  void page_button_add_tips(lv_obj_t *button);

  void page_placeholder(lv_obj_t *par, lv_coord_t x, lv_coord_t y);

  void page_logo_main(lv_obj_t *par);
  void page_tips_main(lv_obj_t *par);

  lv_obj_t *page_label(lv_obj_t *par, const char *text);

  void screen_bottom_button_left(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id);
  void page_bottom_button_middle(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id);
  void page_bottom_button_right(lv_obj_t *par, const char *text, lv_event_cb_t cb, const int id);
  void page_button_return(lv_obj_t *par, lv_event_cb_t cb, const int id);

  lv_obj_t *page_push_button(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id);

  void update_button_enabled(lv_obj_t *btn, const bool curValue);
  lv_obj_t *page_button_enabled(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, const int index, const bool curValue);

  //
  // level mode page
  //
  void update_auto_leveling();
  void page_draw_leveling();

  //
  //  Nozzle heating mode page
  //
  void page_draw_heating_mode_setup();

  //
  // Nozzle heating temperature page
  //
  void page_draw_temperature_adjust();

  //
  //  device debug page
  //
  void page_device_debug_reset_label();
  void page_device_debug_label(lv_coord_t y, const char *text);
  void update_device_debug();
  void page_draw_device_debug();

  //
  //  z axis debug page
  //
  void update_z_axis_debug();
  void page_draw_z_axis_debug();

  //
  // Z Axis offset page
  //
  void update_move_axis(lv_obj_t *btn, lv_obj_t *lbl);
  void update_move_distance();
  void update_offset();
  void page_draw_offset_setup();

  void page_clear();

  void update_filament_detector();
  void update_pause_print_ui();
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
  static void check();
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
  bool is_changed();
};

extern Printing_Babystep p_babystep;
// }
#endif
