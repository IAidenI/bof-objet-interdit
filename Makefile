# =========================
# = Makefile Windows/Unix =
# =========================

# ================== Config commune ==================
CC         = g++
TARGET     = bofgame
SOURCEDIR  = sources
HEADERSDIR = headers

# Toutes les sources du dossier + le fichier principal à la racine
SOURCES := $(wildcard $(SOURCEDIR)/*.cpp) $(TARGET).cpp

SOURCES := $(filter-out $(MAIN_EXCLUDES), $(SOURCES))

OBJECTS := $(SOURCES:.cpp=.o)

CFLAGS  = -Wall -g -I$(HEADERSDIR)
LDFLAGS =
LDLIBS  =

# ================== Détection plateforme ==================
ifeq ($(OS),Windows_NT)
  # ---- Windows (MinGW / w64devkit fourni par raylib) ----
  EXE      := .exe
  RAYLIB_DIR ?= C:/raylib

  CFLAGS  += -I$(RAYLIB_DIR)/include
  LDFLAGS += -L$(RAYLIB_DIR)/lib/win64
  LDLIBS  += -lraylib -lopengl32 -lgdi32 -lwinmm

  # rm portable sous Windows
  RM      := rm -f
else
  # ---- Linux ----
  EXE      :=
  
  LDLIBS  += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

  RM      := rm -f
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
	./$(TARGET)$(EXE)
