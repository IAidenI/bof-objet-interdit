# ================== Config commune ==================
CC         = g++
TARGET     = bofgame
SOURCEDIR  = sources
HEADERSDIR = headers

SOURCES := $(wildcard $(SOURCEDIR)/*.cpp) $(TARGET).cpp
OBJECTS := $(SOURCES:.cpp=.o)

CFLAGS  = -Wall -g -std=c++20 -I$(HEADERSDIR)
LDFLAGS =
LDLIBS  =

# ================== Détection plateforme ==================
ifeq ($(OS),Windows_NT)
  EXE := .exe
  RAYLIB_DIR ?= C:/raylib

  CFLAGS  += -I$(RAYLIB_DIR)/include -I$(RAYLIB_DIR)/raylib/src

  LDFLAGS += -L$(RAYLIB_DIR)/lib/win64
  LDLIBS  += -lraylib -lopengl32 -lgdi32 -lwinmm

  RM := rm -f

  RUN_PREFIX :=
else
  EXE :=

  LDLIBS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

  RM := rm -f

  RUN_PREFIX := ./
endif

# ================== Règles ==================
.PHONY: all clean run

all: $(TARGET)$(EXE)
	@echo "Binary ready ! -> $(TARGET)$(EXE)"
	@$(MAKE) --no-print-directory clean

$(TARGET)$(EXE): $(OBJECTS)
	@echo "Compiling and linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp $(HEADERSDIR)/*.hpp
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	-$(RM) $(OBJECTS)

run: $(TARGET)$(EXE)
	$(RUN_PREFIX)$(TARGET)$(EXE)
