PORT_DIR=$(PACKAGE_DIR)/source/portable/Renesas/$(call h-parse,config_r_freertos,RTOS_PORT)


INC_PATH+= $(PACKAGE_DIR)/source/include $(PORT_DIR)

VPATH+= $(PACKAGE_DIR)/source $(PACKAGE_DIR)/source/portable/MemMang $(INC_PATH);

C_FILES= croutine list queue tasks port heap_3

CONFIG_FILES+= freertosconfig.h
PUBLIC_API_FILES:=$(CONFIG_FILES) FreeRTOS.h task.h queue.h semphr.h croutine.h

