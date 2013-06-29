###############################################################################
#                             Undead Animus Makefile [Pandora|Linux] Ver. 1.0 #
###############################################################################

##### Make sure that the Angstrom ARM toolchain root was defined ##############
ifeq ($(strip $(PNDSDK)),)
$(error Please set PNDSDK to point to your Pandora toolchain root directory  )
endif

ifeq ($(strip $(ZEDSDK)),)
$(error Please set ZEDSDK to point to your ZED SDK root directory )
endif

ifndef TOPDIR
export TOPDIR		:= $(shell cd ../../ && pwd)
export TOPSRC		= $(TOPDIR)/Source
export TARGETDIR	= $(TOPDIR)/Bin/Pandora
export OBJSDIR		= $(TOPDIR)/Obj/Pandora/$(GCCVER)/$(BUILD)
endif
SOURCEDIR	= Source ../Common/Source ../Common/Source/OGLES2

TARGET := UndeadAnimus
OUTFILE	= $(TARGETDIR)/$(TARGET)

TOOL_PREFIX = pandora-

BUILD_PLATFORM	= PANDORA
PLATFORM		= Pandora
ARCH			= ARM
BITTYPE			= 32

GCCVER	= $(shell $(CPP) -dumpversion)

BUILD_TYPE := debug

CPP = $(TOOL_PREFIX)g++

CPPFLAGS	=	-c -DBUILD_$(BUILD_DEF) -DPLATFORM_$(BUILD_PLATFORM) \
				-DPLATFORM_$(BUILD_PLATFORM)_$(ARCH) \
				-DPLATFORM_$(BUILD_PLATFORM)_$(ARCH)_$(BITTYPE) \
				-DBITSIZE_$(BITTYPE) -DARCH_$(ARCH) -DSUPPORT_X11 \
				-I$(TOPSRC)/Pandora/Headers -I$(TOPSRC)/Common/Headers
CPPFLAGS_EXT=	-ffriend-injection -std=c++0x -mcpu=cortex-a8 \
				-Wa,-mcpu=cortex-a8 -mfpu=neon -Wa,-mfpu=neon \
				-mfloat-abi=softfp
SYSIPATH	=	-idirafter $(PNDSDK)/usr/include
INCPATH		=	-I$(ZEDSDK)/include/Pandora_Linux
SYSLPATH	=	-L$(PNDSDK)/usr/lib
LIBPATH		=	-L$(ZEDSDK)/lib/Pandora_Linux/arm/32
SYSLIBS		=	-lX11 -lGLESv2 -lEGL -lrt -lc -lIMGegl -lXau -lXdmcp -lsrv_um
LINKFLAGS	=

##### Debug #######
debug:		BUILD		= Debug
debug:		BUILD_TYPE	= debug
debug:		BUILD_DEF	= DEBUG
debug:		TARGET := $(TARGET)D
debug:		CPPFLAGS += -g -ggdb -Wall -D_DEBUG $(CPPFLAGS_EXT)
debug:		LIBS = -lZEDRendererD -lZEDArithmeticD -lZEDSystemD
debug:		$(TARGET)

##### Release #####
release:	BUILD		= Release
release:	BUILD_TYPE	= release
release:	BUILD_DEF	= RELEASE
release:	TARGET := $(TARGET)
release:	CPPFLAGS += -O3 $(CPPFLAGS_EXT)
release:	LINKFLAGS += -Wl,-S
release:	$(TARGET)

##### Profile #####
profile:	BUILD		= Profile
profile:	BUILD_TYPE	= profile
profile:	BUILD_DEF	= PROFILE
profile:	TARGET := $(TARGET)P
profile:	CPPFLAGS += -O3 -g -ggdb -Wall -D_DEBUG $(CPPFLAGS_EXT)
profile:	$(TARGET)

##### Build the object files while not in the Obj directory ###################
ifneq ($(OBJSDIR), $(CURDIR))

VERSIONINFO:
	@printf "%s" "------------------------- Generating Version Information ---"
	@printf "%s\n" "-------------------"
	@mkdir -p Headers
	@sh ./GitVersion.sh ./Headers/GitVersion.hpp
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"

TARGETDIR:
	@mkdir -p $(TARGETDIR)

OBJSDIR:
	@mkdir -p $(OBJSDIR)

CPPFILES	:= $(foreach dir,$(SOURCEDIR),$(notdir $(wildcard $(dir)/*.cpp)))
export VPATH	:= $(foreach dir,$(SOURCEDIR),$(CURDIR)/$(dir))
export OBJS		:= $(CPPFILES:.cpp=.o)

$(TARGET): OBJSDIR TARGETDIR VERSIONINFO
	@/usr/bin/time -f "%E" -o $(OBJSDIR)/compiletime $(MAKE) \
	--no-print-directory -C $(OBJSDIR) -f $(TOPSRC)/Pandora/Makefile \
	$(BUILD_TYPE)
	@printf "\n%s" "----------------------------------------------------------"
	@printf "%s\n" "---------------------"
	@printf "Compile time: "
	@cat $(OBJSDIR)/compiletime
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"

else

##### In the intermediate build directory #####################################

$(TARGET): $(OBJS)
	@printf "\n\n%s" "------------------------- MESSAGES FROM THE COMPILER ---"
	@printf "%s\n" "-----------------------"
	@cat *.cmsgs
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n\n" "-------------------"
	@printf "Creating: $(OUTFILE)... "
	@$(CPP) -o $(OUTFILE) $(LINKFLAGS) $(OBJS) $(SYSLPATH) $(LIBPATH) \
	$(SYSLIBS) $(LIBS) 2> lmsgs;\
	RETVAL=$$?;\
	if [[ $$RETVAL == 0 ]]; then\
		printf "[OK]\n";\
	else\
		printf "\n\n%s" "------------------------------------ ERROR --------";\
		printf "%s\n" "----------------------------";\
		cat lmsgs;\
		printf "%s" "-------------------------------------------------------";\
		printf "%s\n" "------------------------";\
		exit 1;\
	fi

%.o: %.cpp
	@printf "Compiling: $<... "
	@$(CPP) -MMD -MP -MF $*.d $(CPPFLAGS) $(SYSIPATH) $(INCPATH) \
	$< -o $@ 2> $*.cmsgs;\
	RETVAL=$$?;\
	if [[ $$RETVAL == 0 ]]; then\
		printf "[OK]\n";\
	else\
		printf "\n\n%s" "--------------------------------- ERROR -----------";\
		printf "%s\n" "----------------------------";\
		cat $*.cmsgs;\
		printf "%s" "-------------------------------------------------------";\
		printf "%s\n\n" "------------------------";\
		exit 1;\
	fi

-include $(OBJSDIR)/*.d

endif

.PHONY: clean
clean:
	@printf "Removing files from: $(TARGETDIR)... "
	@cd $(TARGETDIR) && rm -f ./*
	@printf "[OK]\n"
	@printf "Removing files from: $(OBJSDIR)... "
	@cd $(OBJSDIR) && rm -rf ./*
	@printf "[OK]\n"

