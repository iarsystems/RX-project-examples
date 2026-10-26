PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_delay
=======

Document Number 
---------------
R########

Version
-------
v1.00

Overview
--------
This module provides a convenient API for users to implement delays in their system. This module is flexible in that 
delays can be implemented using hardware or software. The user also has the option of configuring this module so that a 
user defined function is called for implementing delays. This allows the user to replace this module's implementation 
with their own (e.g. use their RTOS' functionality) without impacting their already written code that uses this module.

Features
--------
* Easy to use API for implementing delays in your code
* Hardware and software implementations are available
* User-defined function option allows RTOS to be called instead without affecting existing code


Supported MCUs
--------------
* RX610 Group
* RX621, RX62N Group
* RX62T Group
* RX630 Group
* RX631, RX63N Group 

Boards Tested On
----------------
* RDKRX63N

Limitations
-----------
* None 

Peripherals Used Directly
-------------------------
* None

Required Packages
-----------------
* r_cmt_rx      [Optional]

How to add to your project
--------------------------
* Add src\r_delay.c to your project.
* Add an include path to the 'r_delay' directory. 
* Add an include path to the 'r_delay\src' directory.
* Copy the reference configuration file 'r_delay_config_reference.h' to your project and rename it r_delay_config.h.
* Configure middleware for your system through just copied r_delay_config.h.
* Add a #include for r_delay_if.h to any source files that need to use the API functions.

Toolchain(s) Used
-----------------
* Renesas RX v1.02

File Structure
--------------
r_delay
|   readme.txt
|   r_delay_if.h
|
+---demo
|       r_delay_demo_main.c
|
+---doc
|       r_delay.docx
|       r_delay.pdf
|
+---ref
|       r_delay_config_reference.h
|
\---src
        r_delay.c
        r_delay.h


