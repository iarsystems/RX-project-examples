PLEASE REFER TO THE APPLICATION NOTE FOR THIS MODULE FOR MORE INFORMATION

r_flash_api_rx64m
=========

Document Number 
---------------
r01an2072eu0100

Version
-------
v1.00

Overview
--------
This module implements the Flash API for the RX64M MCU

Features
--------
* Data Flash Erase
* Data Flash Blank Check
* Data Flash Write
* Code Flash Erase
* Code Flash Lock Bit Set
* Code Flash Lock Bit Clear

Supported MCUs
--------------
* RX64M Group

Boards Tested On
----------------
* RDKRX64M

Limitations
-----------
* Currently the Flash API does not support BGO mode for the Code Flash

Peripherals Used Directly
-------------------------
* FCU

Required Packages
-----------------
* r_bsp      V2.40

How to add to your project
--------------------------
* Add src\r_example.c to your project.
* Add an include path to the 'r_example' directory. 
* Add an include path to the 'r_example\src' directory.
* Copy the reference configuration file 'r_flash_config_reference.h' to your project and rename it r_flash_config.h.
* Configure middleware for your system through just copied r_flash_config.h.
* Add a #include for r_flash_if.h to any source files that need to use the API functions.

Toolchain(s) Used
-----------------
* Renesas RX v2.01

File Structure
--------------
r_example
|   readme.txt
|   r_flash_api_rx_if.h
|
+---!publishing
|       do_not_distribute_with_this_folder
|       r_flash_v1.00.xml
|
+---demo
|       r_flash_demo_main.c
|       r_flash_demo_main.h
|
+---doc
|       fit_template.docx
|
+---lib
|      
|
+---ref
|       r_flash_api_rx_config_reference.h
|
+---src
   |   r_flash_api_rx.c
   |   r_flash_api_rx.h
   |
   \---ports
       |   Have one folder per port
       |
       +---rx64M
              r_flash_rx64M.c
              r_flash_rx64M.h
       
   


