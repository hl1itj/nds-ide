#
# This makefile should be included into ARM subarchitecture
#

TEMP_SOURCE_DIRS += $(MACHINE_PATH)/.. \
		 $(MACHINE_PATH)/nds \
		 $(MACHINE_PATH)/nds/drivers		 

SOURCES += nds_arm9_bootstrap_stage1.S \
	nds_tpl_sleep.S \
	nds_tpl_stubs.c \
	tpl_stacks.S \
	tpl_switch_context.S

SOURCES += sevencore_io.c \
	down.s \
	up.s