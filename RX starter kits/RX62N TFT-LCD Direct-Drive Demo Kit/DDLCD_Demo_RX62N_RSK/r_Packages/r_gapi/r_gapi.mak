INC_PATH+= $(ROOT_DIR)/include
VPATH+= $(PACKAGE_DIR)/src;

# check which components to use
GAPI_MEMORY_POOLS:=$(call h-parse,config_r_GAPI,GAPI_MEMORY_POOLS)

C_FILES=r_gapic_copy r_gapic_font r_gapic_gradient r_gapic_xlate r_gapiu_bmp r_gapiw_button r_gapiw_scroll 

PUBLIC_API_FILES:=r_gapiu_utility.h r_gapic_core.h r_gapiw_wrapper.h

ifneq ($(GAPI_MEMORY_POOLS), 0)
  C_FILES+= r_gapiu_memory
endif

