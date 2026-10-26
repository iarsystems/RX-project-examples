# #####################################################
#
#                      RENESAS LibUSB Tester  GUI
#
#                     Renesas Electronics America
#
# #####################################################
# For test of Renesas LibUSB R0104592 firmware.
#
# Find a device by specifying PID and VID. Read descriptors, then read/
# write to any endpoint listed in the output generated. This file is the
# corresponding PC host application.
# #####################################################
# LibUSB can be used to quickly get a prototype target running and exchanging 
# info with the host without having to follow the rigid spec's of a class. 
# The package R010492 (or REE RSK LibUSB) were not flexible in that the host 
# side program provided is an .exe file written in MS Visual Studio/MSC++. 
# When new experiments or new prototype code on target is created the host 
# code also obviously needs to change. This is easier to do with this code 
# since for example there is no need to learn, or for a particular license, 
# for a particular MS tool .
#
# PYTHON: Download and install python 2.6.6 (www.python.org) from
#   http://www.python.org/download/releases/2.6.6. On that page download
#   "Windows x86 MSI Installer (2.6.6)". Install everything to get 
#   documentation, Idle, etc.
#
# PYUSB. Download and install PyUSB from sourceforge.net/apps/trac/pyusb. 
#   After install, PyUSB modules should be in the site-packages folder, e.g. 
#   C:\Python\Python27\Lib\site-packages\usb`. You can put Renesas_LibUSB_0492.py 
#   anywhere because you should have Python in your system path, that is, System 
#   variables "Path" should include e.g.
#   "C:\Python\Python27;C:\Python\Python27\libs;C:\Python\Python27\Lib"
#   The following modules can then be imported and used:
#       Content   Description
#       ------    -------------
#       core     Main USB module.
#       util     USB utility functions.
#       control  Standard USB control requests.
#       backend  Contains the Libusb library!
#       time     To pace the test so you have time to see what happens.
# 
# C/C++. Python code uses pyusb.py located (in my case) in C:\Python27\Lib\site-packages\usb,
#   which in turn calls the "backend" C:\Python27\Lib\site-packages\usb\backend\libusb01.py.
#   lusb0_usb.h then (yes, that is how it is spelled) is the c-language equivalent to the APIs found
#   in this libusb01.py.  When you compile the c-code, the calls made according to lusb0_usb.h 
#   should end up calling libusb0.dll (or libusb0.sys). NOTE: This has not been verified, this is 
#   just how I understand it!
#
# LINUX is the easiest to use when it comes to LibUSB since there is a generic
#   USB kernel driver called usbfs. When no other kernel driver is active 
#   for a USB device, or you detach the original kernel driver, then usbfs 
#   driver will be attached to the device and you can use libusb for it.
#
# WINDOWS. There is no generic USB driver like usbfs, so you need to use 
#   WinUSB or libusb-win32 which will service the LibUSB "backend".
#   Libusb-win32 is a library that allows userspace application to access USB 
#   devices on Windows operation systems (Win2k, WinXP, Vista, Win7) without 
#   writing kernel driver code. There are two ways to do this
#  1. Run "install-filter-win.exe". 
#  2. Run "INF-wizard.exe".
#   Both these come with the download libusb-win32-bin-x.x.zip from 
#   http://sourceforge.net/projects/libusb-win32/files/libusb-win32-releases.
#   Read more at http://sourceforge.net/apps/trac/libusb-win32/wiki.
#       Also make sure the Windows system PATH is correct. If not, Right-click on 
#   My Computer and add this via the Advanced tab in System properties.
#
# For Mac OS X, it is more difficult if there are existing drivers attached 
#   to the device.
#
# RUN by starting a PC terminal and typing:
#   >cd <path to directory of Renesas_LibUSB_0492_Tester.py>
#   >python Renesas_LibUSB_0492_Tester_v1.00.py
#   Or, right-click on Renesas_LibUSB_0492_Tester_v1.01.py and select Idle. 
#   Run with 'F5' inside Idle (when RSK is running LibUSB demo). 
#
# SEE ALSO pyusb-1.0.0-a1\docs\tutorial.txt 
#   (or http://pyusb.sourceforge.net/docs/1.0/tutorial.html)
#
# PROBLEMS: techsupport.america@renesas.com
# #####################################################                                                
# HISTORY       Notes                               
# --------      ----  -----      
# August 2012.  First released WW. (REA)
# May 2013.     Revised when adding RX111. Replaced class RenesasLibUSBApplication with new 
#               code in __main__ as using the app as a class is unnecessary. 
#               Improved commenting, added NULL to ADC command.
#               Changes added for testing of RX600 v 2.1.
# #####################################################
#   C   O   D   E 
# #####################################################
# IMPORTS.  Directory of imported files must have a file __init__.py (which
#   can be empty. The USB modules will be imported from e.g.
#   C:\Python26\Lib\site-packages\usb..
# TIP: To find all module functions within module at console, type
# >dir(module)
import usb.core
import usb.control, usb.util, time
import sys, os, operator, string
# TKINTER GUI ------------------------------------------------
# http://www.pythonware.com/library
from Tkinter import *   # Better. <Tkinter.Button...> now referenced <Button..>
# ------------------------------------------------------------

# Write "help(usb.control)" at the python terminal to get info on member 
#   functions of control.py. Standard Host Device Control Commands from 
#   usb.control:
    # 'clear_feature',
    # 'set_feature',
    # 'get_descriptor',
    # 'set_descriptor',
    # 'get_configuration',
    # 'set_configuration',
    # 'get_interface',
    # 'set_interface',
    # 'ENDPOINT_HALT',
    # 'FUNCTION_SUSPEND',
    # 'DEVICE_REMOTE_WAKEUP',
    # 'U1_ENABLE',
    # 'U2_ENABLE',
    # 'LTM_ENABLE'import control

# ########################################################################
# Global DATA
# ########################################################################
# Board type. Set ONE of these board types to <1>, rest to <0>.
RDK = 0
RSK62N = 0
RSK62N_66597 = 0
RSK630 = 0 # Use also for RSK63T
RSK63N = 0
RSK111 = 1

# Add more verbose output. 0 or 1.
DEBUG = 0   # 0 or 1.

# Transmit block sizes MUST be same as for the RX code. Always check USB_SMP_XI_TRNSIZE! (USB_SMPL_TRNSIZE for RX111.)
USB_SMP_BI_TRNSIZE =    64   
USB_SMP_II_TRNSIZE =    64

EP1_ADDR_CMD_OUT =      0x01    # Endpoint 1. Used for user app level commands.  LibUSB-lab.
EP2_ADDR_ADC_IN  =      0x82    # Endpoint 2. With direction bit=IN this becomes bEndpointAddress...   LibUSB-lab
EP3_ADDR_INT_IN  =      0x83    # Endpoint 3. With direction bit=IN this becomes bEndpointAddress...  LibUSB-lab

USB_LIBUSB_CMD_LED =	'\x01'	# Command nr to toggle LED
USB_LIBUSB_CMD_ADC =	'\x02'	# Command nr to read ADC command
USB_LIBUSB_CMD_LCD =	'\x04'	# Command nr to set LCD
    
progname = 'Renesas LibUSB Demo'
if DEBUG: print progname
OUTPUT_DELAY = 0.05
EP_TIMEOUT = 3000   # ms.
DEFAULT_BG_CLR = "#000080"
btn_fontchoice =("Arial", 8, "normal") # See textbox_fontchoice for expl.
imagefile = './Renesas_logo.gif'

# === Choose font  ===
# Here are some families available on most Windows platforms:
#	Arial (corresponds to Helvetica), Courier New (Courier), Comic Sans MS, Fixedsys, MS Sans
#	Serif, MS Serif, Symbol, System, Times New Roman (Times), and Verdana.
# You can specify fongt as a tuple, e.g. : 
#	root.textbox_fontchoice =("Helvetica", 10, "bold italic")
textbox_fontchoice =("Courier New", 8, "normal")

# ########################################################################
# Global FUNCTIONS
# ########################################################################
def log_attributes(obj):
    """
    Print an object's attributes
    """
    rslt_str = ""
    print "Log attributes and values of", obj
    for attr, value in obj.__dict__.iteritems():
        print ' -', attr, '\t', value   #use this or next line
        time.sleep(OUTPUT_DELAY) # Give user chance to follow.
        #rslt_str = rslt_str + str(attr) + ": " +str(value) + ", "
    #print rslt_str, "\n"

# ####################################################
# USB global functions
# ####################################################
def findDeviceAndListConfigsAndEndpoints(vid, pid):
    # Search for device and return an instance object if it is found. 
    # If not, `None`` is returned.n
    # Find given PID & VID
    if DEBUG: print "findDeviceAndListConfigsAndEndpoints(): vid, pid:", vid, pid,  ". In hex:", hex(vid), hex(pid)
    device = usb.core.find(idVendor=vid, idProduct=pid) 
    if device is None:
        raise ValueError('Device with VID', hex(vid), 'and PID', hex(pid), 'NOT FOUND')
    else:
        print 'Device with VID', hex(vid), 'and PID', hex(pid), 'found:', device
        
    # ITERATE and PRINT A CONFIGURATION TO ACCESS THE INTERFACES, 
    # AND ITERATE OVER THE INTERFACES TO ACCESS THEIR ENDPOINTS. 
    # Each kind of object has as attributes the fields of the respective 
    # descriptor.
    print '+++ CONFIGURATION=>INTERFACES=>ENDPOINTS'
    for cfg in device:
        print 'cfg =', cfg
        print 'bConfigurationValue =', str(cfg.bConfigurationValue) + ':'
        for intf in cfg:
            print '\t bInterfaceNumber =', str(intf.bInterfaceNumber) + ', ' + 'bAlternateSetting =', str(intf.bAlternateSetting)
            for ep in intf:
                print '\t\t bEndpointAddress', str(hex(ep.bEndpointAddress))
                time.sleep(OUTPUT_DELAY) # Give user chance to follow.
    print ''
    return device
    
def listDescriptors(device):
    # Read descriptors test.
    # get_descriptor() syntax:
    #   Index of string descriptor is low-byte of wValue in the spec, but it is 
    #   actually a separate argument before wIndex. So as opposed to what is in control.py, 
    #   syntax should be:`
    # get_descriptor(my_usb_device, desc_size, desc_type, desc_wValue-hi(desc_type), desc_wValue-low(desc_index), wIndex = 0)
    # Should report to PyUSB.
    # Descriptor types (wValue of Standard Device Request) form table 9.5 in USB 2.0 spec: 
    #   DEVICE 1, CONFIGURATION 2, STRING 3, INTERFACE 4, ENDPOINT 5, 
    #   Hi-speed only; DEVICE_QUALIFIER 6 , OTHER_SPEED_CONFIGURATION 7, INTERFACE_POWER 8.
    #   See 2.0 spec.
    # See usb_gplibusb_SmplEpTbl1 in ..cdata.c
    result_array = usb.control.get_descriptor(device, 50, 1, 0, 0)
    print "DEVICE DESCRIPTOR:", result_array
    
    result_array = usb.control.get_descriptor(device, 50, 2, 0, 0)
    print "CONFIGURATION DESCRIPTOR:", result_array
    
    string_descriptors_to_read = [1, 2, 3, 4, 5]
    root.text_out_window.insert(AtEnd(), 'List string descriptors.\n')
    for i in string_descriptors_to_read:
        result_array = usb.control.get_descriptor(device, 64, 3, i, 0)
        print "STRING DESCRIPTOR", i, ':', result_array
        # Slice array to omit first two bytes; bLength, bDescriptorType.
        result_array = result_array[2:]
        result_string = result_array.tostring()
        # Remove characters you don't want to see.
        result_string = result_string.replace('\x00','') # Remove/replace NUL
        result_string = result_string.replace('\x03','') # Remove/replace ETX
        result_string = result_string.replace('\x04','') # Remove/replace EOT
        result_string = result_string.replace('\x10','') # Remove/replace DLE
        result_string = result_string.replace('\x12','') # Remove/replace DC2
        result_string = result_string.replace('\x16','') # Remove/replace SYN
        result_string = result_string.replace('\x1C','') # Remove/replace FS
        result_string = result_string.replace('\x0A','') # Remove/replace LF
        
        print '\t\t = ', result_string
        root.text_out_window.insert(AtEnd(), "string descriptor " + str(i) + ' = ')
        root.text_out_window.insert(AtEnd(), result_string + '\n')
        i +=1
        time.sleep(OUTPUT_DELAY)
        
# ####################################################
# USB GUI APPLICATION
# ####################################################
def dummyFunc(event):
    if DEBUG: print 'dummyFunc'

# ===== test only ============
def setKeyBindings():
    # See Python Documentation; Key Names
    root.bind("<Alt-b>", root.dummyFunc, add=1)
    root.bind("<KeyPress-H>", root.dummyFunc, add=1)		
    root.bind("<Key-Print>", root.dummyFunc, add=1)
    root.bind('<F10>', root.dummyFunc, add=1)
# ====================

# ###########################
#         M E N U	
# ###########################
# The Menu widget is used to implement toplevel, pulldown, and popup menus. This widget is used to display all kinds of menus used by an application. Since this widget uses native code where possible, you shouldn't try to fake menus using buttons and other Tkinter widgets. Toplevel menus are displayed just under the title bar of the root or any other toplevel windows. To create a toplevel menu, create a new Menu instance, and use add methods to add commands and other menu entries to it."
def openFileAction():
    if root.input != None:
        root.input.close()
    # New file handle
    try:
        if DEBUG: print 'root.input =', root.input
        root.input = open(tkFileDialog.askopenfilename())
    except:
        if DEBUG: print 'Exception in openFileAction()'
        tkMessageBox.showwarning('tkMessageBox.showwarning', root.filename + ' does not exist.')
        return
def menuSaveAsAction():
    try:
        savefilename = tkFileDialog.asksaveasfilename()
        if DEBUG: print 'savefilename =', savefilename
    except:
        tkMessageBox.showwarning('tkMessageBox.showwarning', savefilename + ' does not exist.')
        return
    root.output = open(savefilename, 'w')
    root.output.close()
def menuSaveAction():
    root.savePsalmToFile()
def menuCutAction():
    if DEBUG: print "cut"
def menuCopyAction():
    if DEBUG: print "copy"
def menuPasteAction():
    if DEBUG: print "paste"
def menuQuitAction():
    if DEBUG: print "quit"
    root.quit()
    
# #################################
# GUI USB button control functions 
# #################################
def LibUSBstartCommand(): 
    if DEBUG:
        root.text_out_window.insert(AtEnd(), "LibUSBstartCommand\n")

    # FIND ALL CONNECTED DEVICES ACCESSIBLE VIA LIBUSB BACKEND and list them.
    devices = usb.core.find(find_all=True)#, bDeviceClass=0xFF)
    print '\nList USB devices found:', devices, '\n In detail:'
    i = 0
    while i < len(devices):
        log_attributes(devices[i])
        print '\tVendor ID', hex(devices[i].idVendor), '& Product ID =', hex(devices[i].idProduct)
        i += 1

    # --- CONNECT; find device, read descriptors (enumerate) ------------------
    # Find a device by specifying VID and PID
    #
    # Renesas has PID range 0x2000-0x20FF reserved. The RSK USB function samples
    # codes use the following:
    # VID	PID	Name	Description
    # 0x45B	0x0512	RSK Generic LibUSB Application      LibUSB32 or LibUSB.
    # 0x45B	0x2013	RSK Generic HID Application         Human Interface Device demonstration.
    # 0x45B	0x2014	RSK Generic CDC Application         Communications Device Class demo.
    # 0x45B	0x2015	RSK Generic MSC Application         Mass Storage Class demo.

    # ##################
    #   Find the RX!
    # ##################
    vidstr = vid_text_entry.get()
    pidstr = pid_text_entry.get()
    if DEBUG: print "vidstr, pidstr", vidstr, pidstr
    vidint = int(vidstr, 16)
    pidint = int(pidstr, 16)
    if DEBUG: print "vidint, pidint", vidint, pidint
    #root.rx_usb_device = findDeviceAndListConfigsAndEndpoints(0x45B, 0x512)
    root.rx_usb_device = findDeviceAndListConfigsAndEndpoints(vidint, pidint)
    
    # Change color to green if connected.
    if root.rx_usb_device != None:
        root.libusb_startbutton.destroy()
        root.libusb_startbutton = Button(f1, text='OK! \n(Reconnect)', height=None, width=14, wraplength=None, font=btn_fontchoice, command=LibUSBstartCommand, borderwidth=2, background="#A0D0A0")
        #libusb_startbutton = Button(f1, text='OK! (Reenumerate)', height=None, width=14, wraplength=80, font=btn_fontchoice, command=LibUSBstartCommand, borderwidth=2, background="#A0D0A0")
        root.libusb_startbutton.grid(row=2, column=0, padx=8, pady=3, sticky=E)
          
    # Show descriptors to user
    listDescriptors(root.rx_usb_device)
    
    # Set the configuration to use. With no arguments, the first
    # configuration will be used. If you call this method without parameter, 
    # it will use the first configuration found.
    root.rx_usb_device.set_configuration(1)   # 0 means use NO config. 1 (default) for Renesas LibUSB 0492
    
# *** LED  ***
def ledControlCommand():
    if DEBUG: 
        print 'ledControlCommand'
        root.text_out_window.insert(AtEnd(), "ledControlCommand\n")
        
    # Write USB_LIBUSB_CMD_LED (1) to EP address EP1_ADDR_CMD_OUT to toggle LEDs.
    print 'Writing 1 to EP number', EP1_ADDR_CMD_OUT,  'to write LED.' 
    # Any ASCII character can be represented by \xHH, where HH is hex. For example,   the integer value 8 is equal to '\x08'. [Or: int('8') = '\x08']
    #   Adding a few extra bytes to see the transferred USB data in the debugger.
    nr_bytes_written = root.rx_usb_device.write(EP1_ADDR_CMD_OUT, USB_LIBUSB_CMD_LED)
    if DEBUG: print 'Number of bytes written:', nr_bytes_written
    
# *** LCD  ***
def lcdWriteCommand():
    if RSK62N_66597 == 1: 
        print 'No LCD available on RSK62N when using R8A66597 HS USB Evaluation Board.'
        root.text_out_window.insert(AtEnd(), 'No LCD available when using R8A66597 USB Evaluation Board.\n')
    else:
        # Count user calls of this function.
        root.i += 1
        if DEBUG:
            print 'lcdWriteCommand'
            root.text_out_window.insert(AtEnd(), "lcdWriteCommand\n")
        
        lcd_text = root.lcd_text_entry.get()

        # Append NULL to string.
        lcd_text = lcd_text + '\x00' # Always make sure strings have a trailing NULL character!

        if DEBUG: 
            print 'lcd_text = ', lcd_text

        # Write USB_LIBUSB_CMD_LCD (4) to EP address EP1_ADDR_CMD_OUT to write to display.
        print 'Writing 4 (USB_LIBUSB_CMD_LCD in target) to EP number', EP1_ADDR_CMD_OUT, 'to write to display.'
        # Second byte is LCD command. 9 clears the display. Board display shows 8/12 characters per line.
        nr_bytes_written = root.rx_usb_device.write(EP1_ADDR_CMD_OUT, USB_LIBUSB_CMD_LCD + '\x09\x00')
        if DEBUG: print 'Number of bytes written:', nr_bytes_written
        
        # Write to alternate line every other time user presses the LCD-button in the GUI.
        if root.i % 2:
            if RDK:
                nr_bytes_written = root.rx_usb_device.write(EP1_ADDR_CMD_OUT, USB_LIBUSB_CMD_LCD + '\x01' + lcd_text)
            else:
                nr_bytes_written = root.rx_usb_device.write(EP1_ADDR_CMD_OUT, USB_LIBUSB_CMD_LCD + '\x00' + lcd_text)
        else:
            if RDK:
                nr_bytes_written = root.rx_usb_device.write(EP1_ADDR_CMD_OUT, USB_LIBUSB_CMD_LCD + '\x02' + lcd_text)
            else:
                nr_bytes_written = root.rx_usb_device.write(EP1_ADDR_CMD_OUT, USB_LIBUSB_CMD_LCD + '\x01' + lcd_text)
        if DEBUG: print 'Number of bytes written:', nr_bytes_written
        
# *** ADC ****
def readAdcCommand():
    if DEBUG: 
        print 'readAdcCommand'
        root.text_out_window.insert(AtEnd(), "readAdcCommand\n")
    
    # Send demo command using the demo command out ep, then read the RSK demo bulk in ep.
    print 'Writing USB_LIBUSB_CMD_ADC (2) to EP', EP1_ADDR_CMD_OUT, 'to trigger ADC.'
    nr_bytes_written = root.rx_usb_device.write(EP1_ADDR_CMD_OUT, USB_LIBUSB_CMD_ADC)
    if DEBUG: print 'Number of bytes written:', nr_bytes_written
    
    # READ ADC endpoint. At this point the data has already been sent to the PC over USB by the
    #   target. The target sends it each time the ADC command 0x02 is written. It is available
    #   at the PC host at IN EP#2, so the target will need to do anything when this EP is read
    #   here. 
    # See usb.core.py for read. Syntax: 
    #   def read(self, endpoint, size, interface = None, timeout = None):
        
    print 'Read ADC data from EP', hex(EP2_ADDR_ADC_IN), '- AD-value is 2nd & 3rd byte.'
    # Set readsize to same as USB buffer size; 64-512 for Basic FW, except for R8A66597 where it should be 512"
    adc_array = root.rx_usb_device.read(EP2_ADDR_ADC_IN, USB_SMP_BI_TRNSIZE, 0, EP_TIMEOUT)

    print 'ADC array:', adc_array
    # Convert from array to hex integer and make pretty.
    adc_string = hex( int(adc_array[2])*0x100 + int(adc_array[1]) )
    # adc_string  is now "0xabc". 0 is character 0, c is character 4.
    adc_string = "0x" + string.upper(adc_string[2:5])
    print 'ADC =', adc_string
    root.text_out_window.insert(AtEnd(), adc_string + '\n')

# *** Interrupt transfer test. Board SW1. ****
def intTransCommandRSKswitch():
    if RSK62N_66597 == 1:
        text = 'User input switches not available when using R8A66597 USB Evaluation Board. Use of external bus occupies switch port.'
        print text
        root.text_out_window.insert(AtEnd(), text + '\n')
    elif RSK630 == 1:
        text = 'Interrupt test not impl. for RSK630/RSK63T since it uses different IRQs than other RSKs. It can easily be implemented for IRQ12/IRQ0. See IRQ8 for RSK63N as example.'
        print text
        root.text_out_window.insert(AtEnd(), text + '\n')
    else:
        # Read interrupt pipe in from RSK by pressing switch to trigger the interrupt, then read EP3 configured as a 
        #   INTERRUPT IN endpoint. The transfer does not occur until the host asks for the data in the Read EP command. 
        #   Press the switch with USB bus analyzer on and you will see - the host controls all!
        print '\nRead LAB interrupt pipe in data from EP', hex(EP3_ADDR_INT_IN)
        if 1: 
            USB_SMP_II_TRNSIZE = 64
            #print '\nRead LAB data from EP', hex(EP3_ADDR_EXTRA_IN)
            if (RDK | RSK62N):
                print 'Read interrupt pipe in from RSK by pressing SW1 - Do it!'
            elif RSK63N:
                print 'Read interrupt pipe in from RSK by pressing SW2 - Do it!'

        if RSK111 == 1:
            result_array = root.rx_usb_device.read(EP3_ADDR_INT_IN, USB_SMP_II_TRNSIZE, 0, EP_TIMEOUT*6)  #LibUSB-lab solution.
        else:
            result_array = root.rx_usb_device.read(EP3_ADDR_INT_IN, USB_SMP_II_TRNSIZE, 1, EP_TIMEOUT*6)  #LibUSB-lab solution.
        print 'EP3_ADDR_INT_IN result_array =\n', result_array
        root.text_out_window.insert(AtEnd(), result_array)
        root.text_out_window.insert(AtEnd(), '\n')

def clearTextCommand():
    root.text_out_window.delete(0.0, 66.66)


# ####################################################
#
#    M  A  I  N   
#
# ####################################################
if __name__ == '__main__':
    
    # Start Tkinter
    # """ Creates the window and layout of the program."""
    root = Tk()
    root.title(progname)    
    
    # Create widgets ###################################################
    
    # Create frames
    #	Relief must be FLAT, GROOVE, RAISED, RIDGE, SOLID, OR SUNKEN
    # FRAME 1
    f1 = Frame(bg=DEFAULT_BG_CLR, bd=1, relief=SOLID)
    f1.grid(row=0, column=0)
    if DEBUG: print 'f1.grid_size() =', f1.grid_size()
    # FRAME 2
    f2 = Frame(bg="#9090B0", bd=1, relief=RAISED)
    f2.grid(row=0, column=1, sticky = None, padx=5, pady=5)
    
    # === Frame 1 content ===============================
    prog_text = 'Communicate with\n RX LibUSB Demo\n using the LibUSB\n backend libusb0.dll'
    Label(f1, text=prog_text, bg="#9090C0", fg="#FFFFFF").grid(row=0, column=1, padx = 8, sticky=None)
    
    # LibUSB start button ===
    root.libusb_startbutton = Button(f1, text='LibUSB\nConnect', height=None, font=btn_fontchoice, width=14, command=LibUSBstartCommand, borderwidth=2, background="#F0F090")
    root.libusb_startbutton.grid(row=2, column=0, padx=8, pady=3, sticky=E)
    Label(f1, text=' VID:                 PID:', bg=DEFAULT_BG_CLR, fg="#FFFFFF").grid(row=1, column=1, padx = 4, sticky=W)
    vid_text_entry = Entry(f1, width=8)
    vid_text_entry.grid(row=2, column=1, padx=8, sticky=W)
    vid_text_entry.focus()
    pid_text_entry = Entry(f1, width=8)
    pid_text_entry.grid(row=2, column=1, padx=8, sticky=E)
    # Enter default VID and PID for LibUSB.
    vid_text_entry.insert(0, "0x45B")
    pid_text_entry.insert(0, "0x512")
    
    # L E D button  ====
    Button(f1, text='LED Control', height=1, font=btn_fontchoice, width=12, command=ledControlCommand, borderwidth=2, background=None).grid(row=3, column=0, padx=8, pady=3, sticky=E)
    
    # L C D button ====
    Button(f1, text='Write LCD', height=1, font=btn_fontchoice, width=12, command=lcdWriteCommand, borderwidth=2, background=None).grid(row=4, column=0, padx=8, pady=3, sticky=E)
    root.lcd_text_entry = Entry(f1, width=18)
    root.lcd_text_entry.grid(row=4, column=1, padx=8, sticky=W)
    # Counter to alternate LCD write line.
    root.i = 0
    
    # A D C button ====
    Button(f1, text='Read ADC', height=1, font=btn_fontchoice, width=12, wraplength=80, command=readAdcCommand, borderwidth=2, background=None).grid(row=5, column=0, padx=8, pady=3, sticky=E)

    # Interrupt transfer via board (user) switch ======
    if (RDK or RSK62N or RSK111):
        int_trans_text = 'Arm interrupt transfer\n Board SW1 is trigger'
    elif RSK630 == 1 or RSK62N_66597 == 1:
        int_trans_text = 'NA this device. Press for info.'
    elif RSK63N:
        int_trans_text = 'Arm interrupt transfer\n Board SW2 is trigger'
    Button(f1, text=int_trans_text, height=None, font=btn_fontchoice, width=12, wraplength=80, command=intTransCommandRSKswitch, borderwidth=2, background=None).grid(row=6, column=0, padx=8, pady=5, sticky=E)
    
    # Clear Text button ====
    Button(f1, text='Clear Text', height=None, font=btn_fontchoice, width=None, command=clearTextCommand, borderwidth=2, background=None).grid(row=6, column=1, padx=4, pady=4, sticky=SE)
    #if DEBUG: print 'f1.grid_size() =', f1.grid_size()
    
    # === Frame 2 content ===============================
    # Text out window ====
    root.text_out_window = Text(f2, height=20, width=54, wrap=WORD, foreground="#F0F0F0", background="#102028", font=textbox_fontchoice)
    root.text_out_window.grid(row=0, column=0)
    root.text_out_window.insert(AtEnd(), "Message output, ADC readings.\n\n")

    # === Other ===
    # Menu ===
    mbar = Menu()
    filemenu = Menu(mbar, tearoff=0)
    filemenu.add_command(label="Open", command=openFileAction)
    filemenu.add_command(label="Save", command=menuSaveAction)
    filemenu.add_command(label="Save", command=menuSaveAsAction)
    filemenu.add_command(label="Copy", command=menuCopyAction)
    filemenu.add_command(label="Paste", command=menuPasteAction)
    filemenu.add_separator() #----
    filemenu.add_command(label= "Spare!", command=menuCutAction)
    mbar.add_cascade(label="File", menu=filemenu)
    mbar.add_command(label="Quit", command=menuQuitAction)
    # Icon
    if DEBUG: print "Creating PhotoImage object from", imagefile
    # Create PhotoImage object to show images.cutAction
    tkimage_logo = PhotoImage(file=imagefile)
    if DEBUG: print "Creating Label from image", tkimage_logo
    Label(f1, image=tkimage_logo).grid(row=0, column=0, sticky = N, padx=2, pady=2)

    #setKeyBindings()

    # Container for our USB device with which we will communicate. For now we won't 'find' 
    #   anything. The user will select what to find later when Connect button is pushed.
    root.rx_usb_device = usb.core.find()

    root.config(menu=mbar)
    root.grid()
    if DEBUG: print "--- Running main loop ---"
    root.mainloop()

#file end
