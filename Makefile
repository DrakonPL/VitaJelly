TITLE_ID = VITAJELLY
TARGET = VitaJelly
OBJS   = JellyPhysics/AABB.o JellyPhysics/Body.o JellyPhysics/ClosedShape.o JellyPhysics/InternalSpring.o \
		JellyPhysics/PointMass.o JellyPhysics/PressureBody.o JellyPhysics/SpringBody.o JellyPhysics/Vector2.o \
		JellyPhysics/VectorTools.o JellyPhysics/World.o font.o Main.o

LIBS = -lvita2d -lSceDisplay_stub -lSceGxm_stub -lSceCommonDialog_stub \
	-lSceCtrl_stub -lSceTouch_stub -lSceSysmodule_stub -lfreetype -lpng -lz -lm -lstdc++

PREFIX  	= arm-vita-eabi
CC      	= $(PREFIX)-gcc
CXX      	= $(PREFIX)-g++
CFLAGS  	= -Wl,-q -Wall -O3
CXXFLAGS  	= -std=c++11 -Wall -O3 -ftree-vectorize -mfloat-abi=hard -ffast-math -fsingle-precision-constant -ftree-vectorizer-verbose=2 -fopt-info-vec-optimized
ASFLAGS 	= $(CFLAGS)

all: $(TARGET).vpk

%.vpk: eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "$(TARGET)" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin $@

eboot.bin: $(TARGET).velf
	vita-make-fself $< eboot.bin

%.velf: %.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).velf $(TARGET).elf $(TARGET).vpk $(OBJS) param.sfo eboot.bin
