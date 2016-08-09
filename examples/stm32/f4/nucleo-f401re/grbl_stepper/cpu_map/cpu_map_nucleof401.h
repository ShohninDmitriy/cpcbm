/*
  cpu_map_atmega2560.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2015 Sungeun K. Jeon

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* This cpu_map file serves as a central pin mapping settings file for AVR Mega 2560 */

#include <libopencm3/stm32/gpio.h>

#ifdef GRBL_PLATFORM
#error "cpu_map already defined: GRBL_PLATFORM=" GRBL_PLATFORM
#endif


#define GRBL_PLATFORM "NucleoF401"

// Serial port pins
// #define SERIAL_RX USART0_RX_vect
// #define SERIAL_UDRE USART0_UDRE_vect

// Increase Buffers to make use of extra SRAM
//#define RX_BUFFER_SIZE		256
//#define TX_BUFFER_SIZE		128
//#define BLOCK_BUFFER_SIZE	36
//#define LINE_BUFFER_SIZE	100

// Define step pulse output pins. NOTE: All step bit pins must be on the same port.
#define STEP_X_DDR        GPIOA_MODER
#define STEP_X_PORT       GPIOA_ODR
#define X_STEP_BIT        10 // NucleoF401 Digital PA10
#define STEP_MASK_X_DDR   (1<<(X_STEP_BIT*2)) // All (step bits*2) because the direction/mode has 2 bits
#define STEP_MASK_X       (1<<X_STEP_BIT)     // X step mask bit

#define STEP_YZ_DDR       GPIOB_MODER
#define STEP_YZ_PORT      GPIOB_ODR
#define Y_STEP_BIT        3 // NucleoF401 Digital PB3
#define Z_STEP_BIT        5 // NucleoF401 Digital PB5
#define STEP_MASK_YZ_DDR  ((1<<(Y_STEP_BIT*2))|(1<<(Z_STEP_BIT*2))) // All (step bits*2) because the direction/mode has 2 bits
#define STEP_MASK_YZ      ((1<<Y_STEP_BIT)|(1<<Z_STEP_BIT))         // Y-Z step mask bits

// Define step direction output pins. NOTE: All direction pins must be on the same port.
#define DIRECTION_Z_DDR       GPIOA_MODER
#define DIRECTION_Z_PORT      GPIOA_ODR
#define Z_DIRECTION_BIT       8   // NucleoF401 Digital PA8
#define DIRECTION_MASK_Z_DDR  (1<<(X_DIRECTION_BIT*2)) // Z DIR Mask direction bits
#define DIRECTION_MASK_Z      ((1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // Z DIR MASK bit

#define DIRECTION_XY_DDR       GPIOB_MODER
#define DIRECTION_XY_PORT      GPIOB_ODR
#define X_DIRECTION_BIT        4  // NucleoF401 Digital PB4
#define Y_DIRECTION_BIT        10 // NucleoF401 Digital PB10
#define DIRECTION_MASK_XY_DDR  ((1<<(X_DIRECTION_BIT*2))|(1<<(Y_DIRECTION_BIT*2))) // All direction bits
#define DIRECTION_MASK_XY      ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT))         // XY DIR MASK bits

// Define stepper driver enable/disable output pin.
#define STEPPERS_DISABLE_DDR        GPIOA_MODER
#define STEPPERS_DISABLE_PORT       GPIOA_ODR
#define STEPPERS_DISABLE_BIT        9 // NucleoF401 Digital PA9
#define STEPPERS_DISABLE_MASK_DDR   (1<<(STEPPERS_DISABLE_BIT*2))
#define STEPPERS_DISABLE_MASK       (1<<STEPPERS_DISABLE_BIT)

/* TO BE DONE: CORRECT THE FOLLOWING CONFIGURATION FOR NUCLEO PINOUT */

// Define homing/hard limit switch input pins and limit interrupt vectors. 
// NOTE: All limit bit pins must be on the same port
#define LIMIT_DDR       DDRB
#define LIMIT_PORT      PORTB
#define LIMIT_PIN       PINB
#define X_LIMIT_BIT     4 // MEGA2560 Digital Pin 10
#define Y_LIMIT_BIT     5 // MEGA2560 Digital Pin 11
#define Z_LIMIT_BIT     6 // MEGA2560 Digital Pin 12
#define LIMIT_INT       PCIE0  // Pin change interrupt enable pin
#define LIMIT_INT_vect  PCINT0_vect 
#define LIMIT_PCMSK     PCMSK0 // Pin change interrupt register
#define LIMIT_MASK ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits

// Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_DDR      DDRH
#define SPINDLE_ENABLE_PORT     PORTH
#define SPINDLE_ENABLE_BIT      3 // MEGA2560 Digital Pin 6
#define SPINDLE_DIRECTION_DDR   DDRE
#define SPINDLE_DIRECTION_PORT  PORTE
#define SPINDLE_DIRECTION_BIT   3 // MEGA2560 Digital Pin 5

// Define flood and mist coolant enable output pins.
// NOTE: Uno analog pins 4 and 5 are reserved for an i2c interface, and may be installed at
// a later date if flash and memory space allows.
#define COOLANT_FLOOD_DDR     DDRH
#define COOLANT_FLOOD_PORT    PORTH
#define COOLANT_FLOOD_BIT     5 // MEGA2560 Digital Pin 8
#ifdef ENABLE_M7 // Mist coolant disabled by default. See config.h to enable/disable.
#define COOLANT_MIST_DDR    DDRH
#define COOLANT_MIST_PORT   PORTH
#define COOLANT_MIST_BIT    6 // MEGA2560 Digital Pin 9
#endif  

// Define user-control CONTROLs (cycle start, reset, feed hold) input pins.
// NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
#define CONTROL_DDR       DDRK
#define CONTROL_PIN       PINK
#define CONTROL_PORT      PORTK
#define RESET_BIT         0  // MEGA2560 Analog Pin 8
#define FEED_HOLD_BIT     1  // MEGA2560 Analog Pin 9
#define CYCLE_START_BIT   2  // MEGA2560 Analog Pin 10
#define SAFETY_DOOR_BIT   3  // MEGA2560 Analog Pin 11
#define CONTROL_INT       PCIE2  // Pin change interrupt enable pin
#define CONTROL_INT_vect  PCINT2_vect
#define CONTROL_PCMSK     PCMSK2 // Pin change interrupt register
#define CONTROL_MASK ((1<<RESET_BIT)|(1<<FEED_HOLD_BIT)|(1<<CYCLE_START_BIT)|(1<<SAFETY_DOOR_BIT))
#define CONTROL_INVERT_MASK CONTROL_MASK // May be re-defined to only invert certain control pins.

// Define probe switch input pin.
#define PROBE_DDR       DDRK
#define PROBE_PIN       PINK
#define PROBE_PORT      PORTK
#define PROBE_BIT       7  // MEGA2560 Analog Pin 15
#define PROBE_MASK      (1<<PROBE_BIT)

// Start of PWM & Stepper Enabled Spindle
#ifdef VARIABLE_SPINDLE
  // Advanced Configuration Below You should not need to touch these variables
  // Set Timer up to use TIMER4B which is attached to Digital Pin 7
  #define PWM_MAX_VALUE       65535.0
  #define TCCRA_REGISTER		TCCR4A
  #define TCCRB_REGISTER		TCCR4B
  #define OCR_REGISTER		OCR4B
  
  #define COMB_BIT			COM4B1
  #define WAVE0_REGISTER		WGM40
  #define WAVE1_REGISTER		WGM41
  #define WAVE2_REGISTER		WGM42
  #define WAVE3_REGISTER		WGM43
  
  #define SPINDLE_PWM_DDR		DDRH
  #define SPINDLE_PWM_PORT    PORTH
  #define SPINDLE_PWM_BIT		4 // MEGA2560 Digital Pin 97
#endif // End of VARIABLE_SPINDLE

#define SET_STEP_DDR \
  do { \
    STEP_X_DDR |= STEP_MASK_X_DDR; \
    STEP_YZ_DDR |= STEP_MASK_YZ_DDR; \
  } while (0)

#define SET_DIRECTION_DDR \
  do { \
    DIRECTION_XY_DDR |= DIRECTION_MASK_XY_DDR; \
    DIRECTION_Z_DDR |= DIRECTION_MASK_Z_DDR; \
  } while (0)    

#define SET_STEP_BITS(stepbits) \
  do { \
    STEP_X_PORT = (STEP_X_PORT & ~STEP_MASK_X) | (stepbits & STEP_MASK_X); \
    STEP_YZ_PORT = (STEP_YZ_PORT & ~STEP_MASK_YZ) | (stepbits & STEP_MASK_YZ); \
  } while (0)  

#define SET_STEPS(stepbits) \
  do { \
    STEP_X_PORT  = (st.step_bits & STEP_MASK_X); \
    STEP_YZ_PORT = (st.step_bits & STEP_MASK_YZ); \
  } while (0)
	  
#define SAVE_STEP_BITS(stepbits) \
  do { st.step_bits = (STEP_X_PORT & ~STEP_MASK_X) | (STEP_YZ_PORT & ~STEP_MASK_YZ) | stepbits;} while (0)  	  

#define SET_DIRECTION_BITS(dirbits) \
  do { \
    DIRECTION_Z_PORT  = (DIRECTION_Z_PORT & ~DIRECTION_MASK_Z) | (dirbits & DIRECTION_MASK_Z); \
    DIRECTION_XY_PORT = (DIRECTION_XY_PORT & ~DIRECTION_MASK_XY) | (dirbits & DIRECTION_MASK_XY); \
  } while (0)  

	  