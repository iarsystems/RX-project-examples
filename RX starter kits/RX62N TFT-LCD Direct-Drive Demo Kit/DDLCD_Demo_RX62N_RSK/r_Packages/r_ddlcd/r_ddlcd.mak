VPATH+= $(PACKAGE_DIR)/src
INC_PATH+= $(PACKAGE_DIR)/src
INC_PATH+= $(ROOT_DIR)/include
CFLAGS+= -nostuff

# check which components to use
DD_USE_EXMEMORY_MANAGER:=$(call h-parse,config_r_ddlcd,DD_USE_EXMEMORY_MANAGER)

C_FILES:=r_ddlcd  r_ddlcd_frames
PUBLIC_API_FILES:=r_ddlcd_public.h config_r_ddlcd.h config_r_ddlcd_panel.h

ifeq ($(DD_USE_EXMEMORY_MANAGER), 1)
  C_FILES+= r_ddlcd_exmemory
  PUBLIC_API_FILES+= r_ddlcd_exmemory.h
endif

