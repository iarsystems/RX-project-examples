----------------------------------------------
RX65N Envision Kit Demo Firmware (20171113)
----------------------------------------------
Adapted for IAR Embedded Workbench for RX
----------------------------------------------
-release data
  benchmark         ("Benchmark Demo adapted for IAR Embedded workbench for RX")
  rx65n_secure_boot ("Secure Boot adapted for IAR Embedded workbench for RX")
  standard          ("Standard Demo" adapted for IAR Embedded workbench for RX")
  convert           (Converter for from motrola file to renesas secure update (*.rsu))
   exe
    convert.exe

-environment
 Board: RX65N Envision Kit.
 IDE: IAR Embedded Workbench for RX 4.10 or later
 Devices: USB cable(power supply/debug) - 1pcs
          USB memory - 1pcs
          Windows PC or mobile battery (power source) - 1pcs

-Setup:
 (1)Open the "rx65n_secure_boot" project.
 (2)set board SW1-1 ON and, rebuild all, download and debug, disconnect the debugger, set board SW1-1 OFF. 
    If the secure boot program should be debugged keep SW1-1 at ON and do not desconnect.

 (3)Open the "rx65n_envision_standard" project and select the Motorola output configuration.
 (4)rebuild all
 (5)execute "/rx65n_envisionkit/convert/exe/convert.exe"
 (6)specify "/rx65n_envisionkit/standard/Motorola output/Exe/rx65n_envision_standard.mot" to "User Program File Path" using Browse button.
 (7)select "RX65N(ROM 2048KB)" to Select MCU.
 (8)push Generate button. "userprog.rsu" is generated. Please rename to "std.rsu". Please input this file into USB Memory. 

 (9)import the "rx65n_envision_benchmark" project and select the Motorola output configuration.
 (10)rebuild all
 (11)execute "/rx65n_envisionkit/convert/exe/convert.exe"
 (12)specify "/rx65n_envisionkit/benchmark/Motorola output/Exe/rx65n_envision_benchmark.mot" to "User Program File Path" using Browse button.
 (13)select "RX65N(ROM 2048KB)" to Select MCU.
 (14)push Generate button. "userprog.rsu" is generated. Please rename to "bench.rsu". Please input this file into USB Memory. 

 (15)connect USB Memory to the CN11 on board. Secure Boot would write Standard Demo firmware to temporary area. (filename: std.rsu)
 (16)When write succeed, display shows the message like "swap bank ...", 
 (17)After 3 seconds, Secure Boot executes bank swap and reset. Standard Demo will start.
 (18)Please push "Bank Swap" Button. Display Shows Bank Swap Demo. 
 (19)(on board) Keep pushing SW2 in 3 seconds. Starts download and writes Benchmark Demo firmware to temporary area. (filename: bench.rsu)
 (20)When write succeed, Please push "Swap!!" button. Standard Demo will swap bank and software reset.
 (21)Benchmark Demo will start.
 
-How to debug user program (standard/benchmark)
 User program (standard/benchmark) are allocated into 0xfff00000-0xffff0000,
 because of secure boot is allocated into 0xffff0000-0xffffffff includes reset vector
 to verify the user programs.
 User program's reset vector (.resetvect) is allocated into 0xfffefffc,
 and User program's excepted vector (.exceptvect) is allocated into 0xfffeff80.
 Therefore, distributed user programs source code cannot debug current allocation.
 User can debug when changing allocation as below using Linker setting.
 .resetvect 0xfffefffc -> 0xfffffffc
 .exceptvect 0xfffeff80 -> 0xffffff80
 If the debug configuration is selected for standard or benchmark projects a 
 linker file with those addresses are used.

-Technical Info
 RX65N Envition Kit Demo has following memory map.

 +---------------------------------+ 0xFFE00000 -
 |   temporary area                |            |
 +---------------------------------+ 0xFFEF0000 | bank1
 |   Secure Boot area(mirror)      |            |
 +---------------------------------+ 0xFFF00000 -
 |   execute area                  |            |
 +---------------------------------+ 0xFFFF0000 | bank0
 |   Secure Boot area              |            |
 +---------------------------------+ 0xFFFFFFFF -

 Phase Setup-(2) status <just download secure boot>:
 +---------------------------------+ 0xFFE00000 -
 |   blank                         |            |
 +---------------------------------+ 0xFFEF0000 | bank1
 |   blank                         |            |
 +---------------------------------+ 0xFFF00000 -
 |   blank                         |            |
 +---------------------------------+ 0xFFFF0000 | bank0 <start bank>
 |   Secure Boot                   |            |
 +---------------------------------+ 0xFFFFFFFF -

 Phase Setup-(2) status <after first execution of Secure Boot>:
 +---------------------------------+ 0xFFE00000 -
 |   blank                         |            |
 +---------------------------------+ 0xFFEF0000 | bank1
 |   Secure Boot(mirror)           |            |
 +---------------------------------+ 0xFFF00000 -
 |   blank                         |            |
 +---------------------------------+ 0xFFFF0000 | bank0 <start bank>
 |   Secure Boot                   |            |
 +---------------------------------+ 0xFFFFFFFF -
 
 Notice:
  Secure Boot copies itself to Secure Boot area(mirror) in first execution phase.

 Phase Setup-(14) status <installing Standard Demo>:
 +---------------------------------+ 0xFFE00000 -
 |   Standard Demo                 |            |
 +---------------------------------+ 0xFFEF0000 | bank1
 |   Secure Boot(mirror)           |            |
 +---------------------------------+ 0xFFF00000 -
 |   blank                         |            |
 +---------------------------------+ 0xFFFF0000 | bank0 <start bank>
 |   Secure Boot                   |            |
 +---------------------------------+ 0xFFFFFFFF -

 Phase Setup-(15) status <after swap bank>:
 +---------------------------------+ 0xFFE00000 -
 |   blank                         |            |
 +---------------------------------+ 0xFFEF0000 | bank0
 |   Secure Boot                   |            |
 +---------------------------------+ 0xFFF00000 -
 |   Standard Demo                 |            |
 +---------------------------------+ 0xFFFF0000 | bank1 <start bank>
 |   Secure Boot(mirror)           |            |
 +---------------------------------+ 0xFFFFFFFF -

 Phase Setup-(19) status <installing Benchmark Demo>:
 +---------------------------------+ 0xFFE00000 -
 |   Benchmark Demo                |            |
 +---------------------------------+ 0xFFEF0000 | bank0
 |   Secure Boot                   |            |
 +---------------------------------+ 0xFFF00000 -
 |   Standard Demo                 |            |
 +---------------------------------+ 0xFFFF0000 | bank1 <start bank>
 |   Secure Boot(mirror)           |            |
 +---------------------------------+ 0xFFFFFFFF -

 Phase Setup-(20) status <after swap bank>:
 +---------------------------------+ 0xFFE00000 -
 |   Standard Demo                 |            |
 +---------------------------------+ 0xFFEF0000 | bank1
 |   Secure Boot(mirror)           |            |
 +---------------------------------+ 0xFFF00000 -
 |   Benchmark Demo                |            |
 +---------------------------------+ 0xFFFF0000 | bank0 <start bank>
 |   Secure Boot                   |            |
 +---------------------------------+ 0xFFFFFFFF -




