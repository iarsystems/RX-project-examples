Add files to your project according to which device you are using.

The directory ..\FreeRTOSSource\portable\ contains MCU specific files. 

(1)	Port.c and Portmacro.h
Port.c and Portmacro.h are common to all H8S and H8SX devices. Add Port.c to your project source files, and Portmacro.h traditionally is put in the ..\FreeRTOSSource\include directory. That directory should be included in your include path. 

(2)	FreeRTOSPort.h
FreeRTOSPort.h is a file added by RTA for keeping MCU variant specific code apart dfrom all other code. 

The timer tick configuration for different MCUs reside in this file. Here are the different macros  and what they do. You can change these macros to better suit your projects needs.
•	TRAP_VECT		Define which interrupt you want to be executed if the trap instruction is called.
•	TMR_VECT       		Define which interrupt is the timer tick interrupt vector . The scheduler is clocked by this interrupt.
•	TMR_INT_PRIORITY() 	Define what interrupt level the timer tick interrupt should have.
•	CLR_TMR_INT()      	Define how the timer tick interrupt flag is cleared.
•	CONFIG_TMR()       	Defines which timer will cause the timer tick interrupt, and how this timer is set up.

If you are using the 
- H8S/2378, put FreeRTOSPort(H8S2378_TMR).h in your project directory and rename it FreeRTOSPort.h
- H8SX/1668, put FreeRTOSPort(H8SX1668_TMR).h in your project directory and rename it FreeRTOSPort.h
- H8SX/1582, put FreeRTOSPort(H8SX1582_TMR).h in your project directory and rename it FreeRTOSPort.h

