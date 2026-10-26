#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#ifdef PLATFORM_BUILD
#define CTK_CONF_MOUSE_SUPPORT    1
#define CTK_CONF_WINDOWS          1
#define CTK_CONF_WINDOWMOVE       1
#define CTK_CONF_WINDOWCLOSE      1
#define CTK_CONF_ICONS            1
#define CTK_CONF_ICON_BITMAPS     0
#define CTK_CONF_ICON_TEXTMAPS    1
#define CTK_CONF_MENUS            1
#define CTK_CONF_MENUWIDTH        16
#define CTK_CONF_MAXMENUITEMS     10
#else /* PLATFORM_BUILD */
#define CTK_CONF_MOUSE_SUPPORT    1
#define CTK_CONF_WINDOWS          0
#define CTK_CONF_WINDOWMOVE       0
#define CTK_CONF_WINDOWCLOSE      0
#define CTK_CONF_ICONS            0
#define CTK_CONF_MENUS            0
#endif /* PLATFORM_BUILD */

#ifdef PLATFORM_BUILD
#define LOADER_CONF_ARCH "loader/dll-loader.h"
#endif /* PLATFORM_BUILD */

#define PROGRAM_HANDLER_CONF_MAX_NUMDSCS 10
#define PROGRAM_HANDLER_CONF_QUIT_MENU   1


#define EMAIL_CONF_WIDTH  76
#define EMAIL_CONF_HEIGHT 30
#ifndef PLATFORM_BUILD
#define EMAIL_CONF_ERASE   0
#endif

#define IRC_CONF_WIDTH         78
#define IRC_CONF_HEIGHT        30
#define IRC_CONF_SYSTEM_STRING "Win32"


#define SHELL_CONF_WITH_PROGRAM_HANDLER 1


#define SHELL_GUI_CONF_XSIZE 78
#define SHELL_GUI_CONF_YSIZE 30


#define TELNETD_CONF_GUI 1


#ifdef PLATFORM_BUILD
#define WWW_CONF_WEBPAGE_WIDTH  76
#define WWW_CONF_WEBPAGE_HEIGHT 30
#endif /* PLATFORM_BUILD */

#endif /* __CONTIKI_CONF_H__ */
