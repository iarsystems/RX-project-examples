PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_usb_pmsc
=======================

Document Number 
---------------
R01AN2029EJ

Version
-------
v1.11

Overview
--------
USB Peripheral Mass Storage Class Driver (PMSC)

Features
--------
The USB Peripheral Mass Storage lass driver comprises a USB Mass Storage class 
bulk-only transport (BOT) protocol. When combined with a USB peripheral control
driver and storage device driver, it enables communication with a USB host as 
a BOT-compatible storage device.


Supported MCUs
--------------
* RX64M Group
* RX71M Group
* RX63N Group

Boards Tested On
----------------
* RSKRX64M
* RSKRX71M
* RSKRX63N
 
Limitations
-----------

Peripherals Used Directly
-------------------------


Required Packages
-----------------
* r_bsp
* r_usb_basic

How to add to your project
--------------------------

Toolchain(s) Used
-----------------
* Renesas RX v.2.03.00

File Structure
--------------
r_usb_pmsc
|   readme.txt
|   r_usb_pmsc_if.h
|   r_media_driver_if.h
|
+---doc
|       r01an2029ej0111_usb.pdf
|
+---ref
|       r_usb_pmsc_config_reference.h
|       r_usb_atapi_driver_config_reference.h
|       r_media_driver_api_config_reference.h
|
\---src
     |  r_media_driver_api.c
     |  r_usb_atapi_driver.c
     |  r_usb_pmsc_api.c
     |  r_usb_pmsc_ddi.c
     |  r_usb_pmsc_driver.c
     |  r_usb_pmsc_pci.c
     |
     \---inc
             r_usb_patapi_define.h
             r_usb_pmsc_api.h
             r_usb_pmsc_define.h
             r_usb_pmsc_extern.h
