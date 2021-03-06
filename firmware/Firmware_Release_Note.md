# Version 1.1.66 - 28-Dec-2020
PWM of the spindle has now a gradual ramp-up and a ramp-down only for CNC machining mode, for laser mode the behaviour is unchanged.
The effect is a gradual increase and decrease of spindle speed, avoiding abrupt changes that could lead to electrical issues in some cases.
The ramps can be regulated with the number of points stored in $44. The minimum is zero, that behaves like there is no ramp.
The maximum points is 100. The delay between points is fixed to 100 ms. The default number of points is 20.

# Version 1.1.65 - 17-Aug-2020
Improved robustness of serial communication moving part of the code in a new ramcode section.

# Version 1.1.64 - 13-Aug-2020
Improved robustness of serial communication.

# Version 1.1.63 - 18-Jul-2020
Removed blocking loop that could created timing issues.

# Version 1.1.62 - 19-May-2020
Fix ESC PWM spindle going to zero instead of going to the minimum pwm value.
This fix is not tested for the laser, at the moment, so don't use it to drive a laser head.

# Version 1.1.60 - 15-Mar-2020
First GRBL version 1.1 porting, done preliminary test with cnc and laser that seem to work properly.  

# Version 0.9.59 - 10-Mar-2020
Added the homing debug feature that prints on serial which is the movement that is being executed and when a limit switch is pressed.  
This debug is enabled with the setting $95 equal to 1, and disabled setting it to zero.  
The first time you install this version the settings will be reset to the default value and you may get an error indication, but it's  
just due to the CRC calculation made the first time, so there is no real problem.  
Here is an example of homing debug with some comments after the "<--" string:  

$H  
[Approaching]  
[X=0 Y=0 Z=1] <--Limit switch Z pressed   
[Retracting]  
[Approaching]  
[X=0 Y=0 Z=1] <--Limit switch Z pressed   
[Retracting]  
[Approaching]  
[X=0 Y=1 Z=0] <--Limit switch Y pressed   
[Retracting]  
[Approaching]  
[X=0 Y=1 Z=0] <--Limit switch Y pressed   
[Retracting]  
[Approaching]  
[X=0 Y=0 Z=0] <--Likely a bounce on X switch   
[X=1 Y=0 Z=0] <--Limit switch X pressed   
[Retracting]  
[Approaching]  
[X=0 Y=0 Z=0] <--Likely a bounce on X switch   
[X=0 Y=0 Z=0] <--You can have more than one bounce, it's normal   
[X=1 Y=0 Z=0] <--Limit switch X pressed   
[Retracting]  
ok  


# Version 0.9.58 - 11-Feb-2020
Added the gap-fill option to objcopy, to generate a binary with gap filled with 0xFF bytes instead of zeroes.  
This should leave the flash correctly erased in the settings space after the binary is loaded. As in the previous version  
0.9.57, reloading the bin resets the settings to their default values. 

# First Release Note 01-Feb-2020

## Scope

Release note for the "Compact PCB Maker - The Ant" machine firmware.  
This release note aims to summarize and document the evolution of the firmware images (the released binaries), although not all the details will be reported.  

The firmware is a porting of GRBL 0.9 on the NUCLEO-F401RE, an STM32 based microcontroller board. The low layer hardware access libraries used are the libopencm3.
For more detailed information read the [wiki](https://bitbucket.org/compactpcbmaker/cpcbm/wiki/Index.wiki). 

## Status  

At the time of the first issue of this release note, there are 3 firmware binaries released in the [Download section](https://bitbucket.org/compactpcbmaker/cpcbm/downloads/):

- main_f401_38.bin, that is the stable version used for the The Ant machine. This fw doesn't work properly to drive a laser head, since the PWM output is in high-impedance when the PWM Output is disabled(M5 command). This is not safe when driving a laser. 
- main_f401_39_BETA_more_safe_for_laser.bin, that is like the fw 38, but the PWM output is set to zero Volt, when the PWM Output is disabled(M5 command). This behaviour is safer when driving a laser head.
- gocm3_nucleo-f401re_CPCBM_AFRO20_COREXY_VER_0.9.57.bin, that has the same behaviour of the fw 39, but other experimental modifications:
	- different linking, i.e. the code and settings are stored in the STM32F401RE embedded FLASH in a different way respect to the other two binaries, there is a part of the code in the first sectors, then the settings, and then the rest of the code.  
	The advantage of this modification is that the settings are stored in smaller sectors, and changing and resetting them is faster. The drawbacks are that the binary is larger, also if the effective size used is about the same, and above all that every time that a new firmware binary is loaded the settings are reset to the default values.
	- the green led of the NUCLEO is used as an heart-beat to see if the firmware is up and running.
	- the building system is now based on python scripts and parametric, and the firmware version is set as MAJOR.minor.build. The MAJOR and minor numbers are set as 0.9, to reflect that this firmware is a porting of the version 0.9 of GRBL. The build number is automatically generated by the bitbucket pipelines and it is equal to the number of the commit.