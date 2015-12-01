TARGET = VitaJelly
OBJS   = JellyPhysics/AABB.o JellyPhysics/Body.o JellyPhysics/ClosedShape.o JellyPhysics/InternalSpring.o \
		JellyPhysics/PointMass.o JellyPhysics/PressureBody.o JellyPhysics/SpringBody.o JellyPhysics/Vector2.o \
		JellyPhysics/VectorTools.o JellyPhysics/World.o font.o Main.o

LIBS = -lvita2d -lSceDisplay_stub -lSceGxm_stub -lSceCommonDialog_stub \
	-lSceCtrl_stub -lSceTouch_stub -lfreetype -lpng -lz -lm -lstdc++

PREFIX  	= arm-vita-eabi
CC      	= $(PREFIX)-gcc
CXX      	= $(PREFIX)-g++
CFLAGS  	= -Wl,-q -Wall -O3
CXXFLAGS  	= -Wl,-q -Wall -O3 -std=c++11
ASFLAGS 	= $(CFLAGS)

all: $(TARGET).velf

%.velf: %.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS)
