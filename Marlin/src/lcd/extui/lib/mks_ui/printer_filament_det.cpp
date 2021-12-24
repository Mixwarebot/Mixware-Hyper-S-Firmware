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
#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#if ENABLED(MIXWARE_MODEL_V)

#include "printer_filament_det.h"
FilamentDetector detector;

#include "../../../../module/stepper.h"
#include "../../../../module/probe.h"
#include "../../../../sd/cardreader.h"

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

void FilamentDetector::reset() {
  WRITE(BEEPER_PIN, LOW);
  uiCfg.filament_broken = false;
  last_pos = block_count = 0;
  last_time = millis();
  alarm_gap = planner.settings.axis_steps_per_mm[E_AXIS] * FILAMENT_DETECTOR_ALARM_GAP;
  stop_gap = planner.settings.axis_steps_per_mm[E_AXIS] * FILAMENT_DETECTOR_STOP_GAP;
}

void FilamentDetector::update() {
  if (!gCfgItems.filament_det_enable || card.getIndex() < FILAMENT_DETECTOR_FILE_START_POS || stepper.position(E_AXIS) < FILAMENT_DETECTOR_EFFECTIVE_GAP)// || current_position[Z_AXIS] < 0.5
    return;

  static uint8_t last_state = 0;
  const uint8_t cur_state = (READ(MT_DET_1_PIN) ? _BV(0) : 0),
                change = last_state ^ cur_state;
  last_state = cur_state;

  const int32_t cur_pos = stepper.position(E_AXIS);
  if (!cur_pos) last_pos = 0;

  if (change || !last_pos) {
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
  if (IS_SD_PRINTING()) alarm();
}

void FilamentDetector::alarm() {
  const bool a = (gCfgItems.filament_det_enable && stepper.axis_is_moving(E_AXIS) && last_pos && stepper.position(E_AXIS)) ? (bool)(cur_gap >= alarm_gap) : false;

  if (a) WRITE(BEEPER_PIN, HIGH);
  else   WRITE(BEEPER_PIN, LOW);
}

bool FilamentDetector::has_break() {
  return (gCfgItems.filament_det_enable && stepper.axis_is_moving(E_AXIS) && last_pos && stepper.position(E_AXIS)) ? (bool)(cur_gap >= stop_gap) : false;
}

bool FilamentDetector::has_block() {
  return (bool)(gCfgItems.filament_det_enable && block_count > FILAMENT_DETECTOR_BLOCK_NUMBER);
}

Printing_Babystep p_babystep;

Printing_Babystep::Printing_Babystep() {
  init();
}

void Printing_Babystep::init() {
  save_probe = start_probe = end_probe = probe.offset.z;
}

void Printing_Babystep::reset() {
  start_probe = end_probe = probe.offset.z;
}

void Printing_Babystep::update() {
  end_probe = probe.offset.z;
}

bool Printing_Babystep::isChanged() {
  return (bool)(!(start_probe == end_probe && save_probe == end_probe));
}

#endif // MIXWARE_MODEL_V
#endif // HAS_TFT_LVGL_UI
