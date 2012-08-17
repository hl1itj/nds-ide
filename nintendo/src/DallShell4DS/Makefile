#ver0.9
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error Please set DEVKITPRO in your environment)
endif


ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
SOURCES		:=	source FreeRTOS FreeRTOS/NDS gfx/bin  
INCLUDES	:=	include build gfx FreeRTOS/include FreeRTOS/NDS
GRAPHICS	:=  image
AUDIO       :=	audio

PAPATH  = $(DEVKITPRO)/PAlib

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-g -Wall -O2\
 			-march=armv5te -mtune=arm946e-s -fomit-frame-pointer\
			-ffast-math \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -mno-fpu -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lpa9 -lfilesystem -lfat -lnds9 -ldswifi9
 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBNDS)

LIBDIRS += $(PAPATH)
 
#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))
					
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

# Determine the name of the tools

PREFIX := arm-eabi-

export CC      := $(PREFIX)gcc
export CXX     := $(PREFIX)g++
export AR      := $(PREFIX)ar
export AS      := $(PREFIX)as
export OBJCOPY := $(PREFIX)objcopy

# Collect source code files
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

# Use CXX to link C++ projects or CC for standard C
ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif

# Collect resource files
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))
PNGFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
PCXFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pcx)))
PALFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pal)))
RAWFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.raw)))
MAPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.map)))
JPGFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.jpg)))
GIFFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.gif)))
BMPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bmp)))
MP3FILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.mp3)))
 
#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(BINFILES:.bin=.o) $(PCXFILES:.pcx=.o) $(PALFILES:.pal=.o) \
					$(RAWFILES:.raw=.o) $(MAPFILES:.map=.o) $(JPGFILES:.jpg=.o) \
					$(GIFFILES:.gif=.o) $(BMPFILES:.bmp=.o) $(MP3FILES:.mp3=.o) \
					$(PNGFILES:.png=.o) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
 
export INCLUDE	:= -I$(CURDIR)/$(BUILD) \
                  $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                  $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                  $(foreach dir,$(LIBDIRS),-I$(dir)/include/nds) \
                  -I$(PAPATH)/include/nds
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
 
 

# Header dependencies path (.d files)
export DEPSDIR := $(CURDIR)/$(BUILD)

# Filesystem
ifneq ($(strip $(wildcard $(CURDIR)/filesystem)),)
	export FILESYSTEM := -d $(CURDIR)/filesystem
else
	export FILESYSTEM :=
endif
 
.PHONY: $(BUILD) clean
 
#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
 
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba 
 
 
#---------------------------------------------------------------------------------
else
 
DEPENDS	:=	$(OFILES:.o=.d)
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).nds	: 	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)
 
define bin2o
	cp $< $*
	bin2s $* | $(AS) -o $@
	rm $*

	echo "extern const u8" $*"[];" > $*.h
	echo "extern const u32" $*_size";" >> $*.h
endef


#---------------------------------------------------------------------------------
%.o: %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.pcx
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.pal
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.raw
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.map
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.jpg
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.gif
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.bmp
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o: %.mp3
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.s %.h	: %.png %.grit
#---------------------------------------------------------------------------------
	grit $< -fts -o$*
 
 
#---------------------------------------------------------------------------------
# Header file dependencies
#---------------------------------------------------------------------------------
-include $(DEPSDIR)/*.d

-include $(DEPENDS)
 
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
