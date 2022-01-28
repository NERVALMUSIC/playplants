/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_PLANTER_
#define _VARIANT_PLANTER_

#include "nrf.h"
#include "nrf_peripherals.h"

/** Master clock frequency */
#if defined(NRF52_SERIES)
#define VARIANT_MCK       (64000000ul)
#else
#define VARIANT_MCK       (16000000ul)
#endif

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Number of pins defined in PinDescription array
#if GPIO_COUNT == 1
#define PINS_COUNT           (32u)
#define NUM_DIGITAL_PINS     (32u)
#elif GPIO_COUNT == 2
#define PINS_COUNT           (64u)
#define NUM_DIGITAL_PINS     (64u)
#else
#error "Unsupported GPIO_COUNT"
#endif
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (0u)

// LEDs
#define RED_LED              (16) // P0.16
#define GRN_LED              (14) // P0.14
#define BLU_LED              (15) // P0.15

// Digital I/O
#define CHG             03    // Charge indication
#define BUTTON_PIN      04    // User button
#define POW_EN          05    // Enable battery readings
#define INT             06    // Sensor Interrupt pin

/*
 * Analog pins
 */
#define BAT_MON              (2)  // P0.02

#if defined(NRF52_SERIES)
#define ADC_RESOLUTION    14
#else
#define ADC_RESOLUTION    10
#endif

/*
 * Serial interfaces
 */
// Serial
#define PIN_SERIAL_RX       (26) // P0.26
#define PIN_SERIAL_TX       (27) // P0.27

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (0) // P0.00
#define PIN_WIRE_SCL         (1) // P0.01

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#ifdef __cplusplus
}
#endif

#endif
