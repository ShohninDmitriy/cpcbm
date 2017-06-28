/*
  main.c - An embedded CNC Controller with rs274/ngc (g-code) support
  Part of Grbl
  
  Copyright (c) 2011-2015 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud
  
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

#include "grbl.h"


// Declare system global variable structure
system_t sys; 

#ifndef TEST_NUCLEO

int main(void)
{
#ifdef NUCLEO
#ifndef BASIC_CPU_SPEED
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
#endif
#endif
  // Initialize system upon power-up.
  serial_init();   // Setup serial baud rate and interrupts
  settings_init(); // Load Grbl settings from EEPROM
  stepper_init();  // Configure stepper pins and interrupt timers
  system_init();   // Configure pinout pins and pin-change interrupt

  memset(&sys, 0, sizeof(system_t));  // Clear all system variables
  sys.abort = true;   // Set abort to complete initialization
#ifdef NUCLEO
  __enable_irq(); // Global enable interrupts
#else
  sei(); // Enable interrupts
#endif

  // Check for power-up and set system alarm if homing is enabled to force homing cycle
  // by setting Grbl's alarm state. Alarm locks out all g-code commands, including the
  // startup scripts, but allows access to settings and internal commands. Only a homing
  // cycle '$H' or kill alarm locks '$X' will disable the alarm.
  // NOTE: The startup script will run after successful completion of the homing cycle, but
  // not after disabling the alarm locks. Prevents motion startup blocks from crashing into
  // things uncontrollably. Very bad.
  #ifdef HOMING_INIT_LOCK
    if (bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE)) { sys.state = STATE_ALARM; }
  #endif
  
  // Force Grbl into an ALARM state upon a power-cycle or hard reset.
  #ifdef FORCE_INITIALIZATION_ALARM
    sys.state = STATE_ALARM;
  #endif
  
  // Grbl initialization loop upon power-up or a system abort. For the latter, all processes
  // will return to this loop to be cleanly re-initialized.
  for(;;) {

    // TODO: Separate configure task that require interrupts to be disabled, especially upon
    // a system abort and ensuring any active interrupts are cleanly reset.
  
    // Reset Grbl primary systems.
    serial_reset_read_buffer(); // Clear serial read buffer
    gc_init(); // Set g-code parser to default state
    spindle_init();
    coolant_init();
    limits_init(); 
    probe_init();
    plan_reset(); // Clear block buffer and planner variables
    st_reset(); // Clear stepper subsystem variables.

    // Sync cleared gcode and planner positions to current system position.
    plan_sync_position();
    gc_sync_position();

    // Reset system variables.
    sys.abort = false;
    sys_rt_exec_state = 0;
    sys_rt_exec_alarm = 0;
    sys.suspend = false;
    sys.soft_limit = false;
              
    // Start Grbl main loop. Processes program inputs and executes them.
    protocol_main_loop();
    
  }
  return 0;   /* Never reached */
}

#else
int main(void)
{
#ifdef TEST_FLASH_GLOBAL_PARAMS
	flash_put_char( ((unsigned int)EFLASH_ADDR_GLOBAL_MAIN), ((unsigned char)0x0));
	flash_put_char( ((unsigned int)EFLASH_ADDR_GLOBAL_MAIN+1), ((unsigned char)0x0));
	flash_put_char( ((unsigned int)EFLASH_ADDR_GLOBAL_MAIN+2), ((unsigned char)0x0));
	flash_put_char( ((unsigned int)EFLASH_ADDR_GLOBAL_MAIN+3), ((unsigned char)0x0));
#endif

	settings_init(); // Load Grbl settings from memory
	settings_restore(SETTINGS_RESTORE_ALL);

	serial_init();
	report_init_message();

	stepper_init();  // Configure stepper pins and interrupt timers

	st_reset();

	// Reset system variables.
	sys.abort = false;
	sys_rt_exec_state = 0;
	sys_rt_exec_alarm = 0;
	sys.suspend = false;
	sys.soft_limit = false;

	sys.state = STATE_CYCLE;


	system_init();   // Configure pinout pins and pin-change interrupt
	limits_init();

	uint32_t counter = 0;
	uint16_t temp_dir_bits;
	uint32_t steps_x;
	uint32_t steps_y;
	uint32_t steps_z;
	uint32_t steps_event_count;
	while(1){
		temp_dir_bits = 0x510;
		steps_x = 0;
		steps_y = 0;
		steps_z = 0;

		switch(counter%3)
		{
		case 0:
			steps_x = 100;
			break;
		case 1:
			steps_y = 100;
			break;
		case 2:
			steps_z = 100;
			break;
		default:
			break;
		}
		steps_event_count = 0;//counter % 5;
		counter++;

		fill_fake_prep_buffer(temp_dir_bits,steps_x,steps_y,steps_z,steps_event_count);
		//st_prep_buffer(); // Initialize step segment buffer before beginning cycle.
		st_wake_up();
	}

   // while(1);
    return 0;
}
#endif

