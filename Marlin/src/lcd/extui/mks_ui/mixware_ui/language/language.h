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

#include "language_scn.h"
#include "language_tcn.h"
#include "language_en.h"

#define MTR_MAIN                            "Hyper S"

#define MTR_TEMPERATURE_RANGE_1C            "1℃"
#define MTR_TEMPERATURE_RANGE_5C            "5℃"
#define MTR_TEMPERATURE_RANGE_10C           "10℃"

#define MTR_PREHEAT_PLA                     "PLA"
#define MTR_PREHEAT_TPU                     "TPU"
#define MTR_PREHEAT_PETG                    "PETG"
#define MTR_PREHEAT_PACF                    "PA-CF"

#define MTR_X                               "X"
#define MTR_Y                               "Y"
#define MTR_Z                               "Z"
#define MTR_E                               "E"
#define MTR_AXIS_X                          MTR_X "-axis"
#define MTR_AXIS_Y                          MTR_Y "-axis"
#define MTR_AXIS_Z                          MTR_Z "-axis"
#define MTR_AXIS_E                          MTR_E "-axis"

#define MTR_AXIS_MOVEDISTANCE_MM_0_0_5      "0.05 mm"
#define MTR_AXIS_MOVEDISTANCE_MM_0_1        "0.1 mm"
#define MTR_AXIS_MOVEDISTANCE_MM_1          "1 mm"
#define MTR_AXIS_MOVEDISTANCE_MM_5          "5 mm"
#define MTR_AXIS_MOVEDISTANCE_MM_10         "10 mm"

#define MTR_NO_KEY_1                        "1"
#define MTR_NO_KEY_2                        "2"
#define MTR_NO_KEY_3                        "3"
#define MTR_NO_KEY_4                        "4"
#define MTR_NO_KEY_5                        "5"
#define MTR_NO_KEY_6                        "6"
#define MTR_NO_KEY_7                        "7"
#define MTR_NO_KEY_8                        "8"
#define MTR_NO_KEY_9                        "9"
#define MTR_NO_KEY_0                        "0"
#define MTR_NO_KEY_POINT                    "."
#define MTR_NO_KEY_NEGATIVE                 "-"

#define MTRGETDEF(T, L)                     MTR_##T##_##L
#define _MTRLANG1(T)                        MTRGETDEF(T, SCN)
#define _MTRLANG2(T)                        MTRGETDEF(T, TCN)
#define _MTRLANG3(T)                        MTRGETDEF(T,  EN)

// #define MTRLANG1(T) _MTRLANG1(T)
// #define MTRLANG2(T) IF(gCfgItems.language == 2, _MTRLANG2(T), MTRLANG1(T)) // LANG_COMPLEX_CHINESE 2
// #define MTRLANG3(T) IF(gCfgItems.language == 3, _MTRLANG3(T), MTRLANG2(T)) // LANG_ENGLISH         3
// #define MTRLANG(T) MTRLANG3(T)
// #define _MTRLANG(T, N) MTRLANG_##N(T)
// #define MTRLANG(T) MTRLANG_(T, NUM_LANGUAGES)

// LANG_SIMPLE_CHINESE  1
// LANG_COMPLEX_CHINESE 2
// LANG_ENGLISH         3
#define MTRLANG(T) ( \
  gCfgItems.language == 1 ? _MTRLANG1(T) : \
  gCfgItems.language == 2 ? _MTRLANG2(T) : \
                            _MTRLANG3(T)   )
