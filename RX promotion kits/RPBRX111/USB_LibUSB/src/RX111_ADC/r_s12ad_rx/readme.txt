PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_example
=========

Document Number 
---------------
R########

Version
-------
v1.40

Overview
--------
Description. Long description. Long description. Long description. Long description. Long description. Really long 
description. Wraps at 120 characters.

Features
--------
* Feature #1
* Feature #2

Supported MCUs
--------------
* RX610 Group
* RX621, RX62N Group
* RX62T Group
* RX630 Group
* RX631, RX63N Group 

Boards Tested On
----------------
* RSKRX610
* RSK+RX62N
* RSKRX62T
* RDKRX62N
* RSKRX630
* RSKRX63N
* RDKRX63N

Limitations
-----------
* This is an example, it will not build.
* None <-- Use this when there are none

Peripherals Used Directly
-------------------------
* S12AD

Required Packages
-----------------
* r_rspi_rx     v1.30-v1.40
* r_cmt_rx      v1.10

How to add to your project
--------------------------
* Add src\r_example.c to your project.
* Add an include path to the 'r_example' directory. 
* Add an include path to the 'r_example\src' directory.
* Copy the reference configuration file 'r_example_config_reference.h' to your project and rename it r_example_config.h.
* Configure middleware for your system through just copied r_example_config.h.
* Add a #include for r_example_if.h to any source files that need to use the API functions.

Toolchain(s) Used
-----------------
* Renesas RX v1.02

File Structure
--------------
r_example
|   readme.txt
|   r_example_if.h
|
+---!publishing
|       do_not_distribute_with_this_folder
|       r_example_v1.00.xml
|
+---demo
|       r_example_demo_main.c
|       r_example_demo_main.h
|
+---doc
|       fit_template.docx
|
+---lib
|       r_example_v1.00.lib
|
+---ref
|       r_example_config_reference.h
|
+---src
|   |   r_example.c
|   |   r_example.h
|   |
|   \---ports
|       |   Have one folder per port
|       |
|       +---rx62x
|       |       r_example_rx62x.c
|       |       r_example_rx62x.h
|       |
|       \---rx63x
|               r_example_rx63x.c
|               r_example_rx63x.h
|
\---utilities
    |   put_any_tools_or_programs_associated_with_this_module_here
    |
    +---batch_files
    \---scripts

