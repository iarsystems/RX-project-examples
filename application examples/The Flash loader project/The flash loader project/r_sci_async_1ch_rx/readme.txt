PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_sci_async_1ch_rx
==================

Document Number 
---------------
Sample Code

Version
-------
v1.20

Overview
--------
This code allows the user to choose a SCI channel to use in asynchronous mode. Transmission is done through polling and
reception is interrupt driven. The user specifies a callback function and whenever a byte is received, that function 
is called with the received data as the input paramater.

Features
--------
* Sets up SCI channel in asynchronous mode
* Interrupt driven for reception

Supported MCUs
--------------
* RX621, RX62N Group
* RX62T Group
* RX62G Group
* RX630 Group
* RX631, RX63N Group 
* RX63T Group
* RX210 Group

Boards Tested On
----------------
* RSK+RX62N
* RDKRX62N
* RSKRX63N
* RDKRX63N
* RSKRX210

Limitations
-----------
* Currently only supports 1 channel at a time

Peripherals Used Directly
-------------------------
* SCI

Required Packages
-----------------
* None

How to add to your project
--------------------------
* Add src\r_sci_async_1ch_rx.c to your project.
* Add an include path to the 'r_sci_async_1ch_rx' directory. 
* Add an include path to the 'r_sci_async_1ch_rx\src' directory.
* Copy r_sci_async_1ch_rx_reference.h from 'ref' directory to your desired location and rename to 
  r_sci_async_1ch_rx_config.h.
* Configure middleware through r_sci_async_1ch_rx_config.h.

Toolchain(s) Used
-----------------
* Renesas RX v1.02

File Structure
--------------
r_sci_async_1ch_rx
|   readme.txt
|   r_sci_async_1ch_rx_if.h
|
+---doc
+---ref
|       r_sci_async_1ch_rx_config_reference.h
|
\---src
        r_sci_async_1ch_rx.c
        r_sci_async_1ch_rx.h                
