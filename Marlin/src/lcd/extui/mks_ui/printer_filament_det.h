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

#include "../../../inc/MarlinConfig.h"

class FilamentDetector {
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
};

extern FilamentDetector detector;

class Printing_Babystep {
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
