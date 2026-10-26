This sample is distributed as a compressed zip file which cannot be opened in 
e2studio. To use the sample, first import the uIP_Demo sample in to your
local workspace, then navigate to the project directory (where this text file 
will be located) and follow the instructions below.

1. Extract the provided an_r01an1190eu0100_rx63n_ether.zip file.
2. Right click the newly created e2studio uIP_Demo project and select Delete.
   Ensure the "Delete project contents on disk" checkbox is empty, and click OK.
3. In e2studio go to File->Import... and select HEW Project. Click Next. Browse to
   the file "an_r01an1190eu0100_rx63n_ether\workspace\RSKRX63N_uIP_Demo\RSKRX63N_uIP_Demo.hwp"
   A warning that Renesas_RXC 1.1.0.0 version is missing may be displayed, however can be safely
   ignored. Click Finish to complete the Import wizard. The project will be upgraded to use
   the latest RX toolchain.
4. Expand the newly created RSKRX63N_uIP_Demo. Go to src->uip->apps->webserver and
   right click httpd-fsdata.c. Go to Resource Configurations->Exclude from build.
   Click Select All and then OK.
5. Right click the RSKRX63N_uIP_Demo project and click Build Project.
6. When the build has completed, right click the RSKRX63N_uIP_Demo project and go to
   Debug As->Debug Configurations. Select Renesas GDB Hardware Debugging and click
   the New button. On the Debugger tab, change Debug hardware to E1 and Target
   Device to R5F563NB. Open the Connection Settings tab and change Permit Clock 
   Source Change on Writing Internal Flash Memory to Yes and Power Target From
   The Emulator to No.
7. Click Debug to start the sample.

You may see a warning indicating the workspace has been moved, click Yes to 
acknowledge this. 

On opening this project you may be asked to upgrade the toolchain version, or 
when building you may receive an L2201 error. In either case, upgrading the 
toolchain to version 1.2.0.0 or greater will resolve the issue.

The sample will start up with the Default debug session selected, to run the
sample change this to SessionRX_E1_E20_SYSTEM under Debug | Debug Sessions...
When connecting to the emulator, change the MCU group to RX63N Group and 
the Device to R5F563NB.

For more information please review the Application Note PDF inside the zipped 
archive.