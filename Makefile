CC = g++
CFLAGS = -Wall
LDFLAGS =
TARGET = bofgame
SOURCEDIR = sources
HEADERSDIR = headers
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp) $(TARGET).cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean

all: $(TARGET) clean
	@echo "Binary ready !"

$(TARGET): $(OBJECTS)
	@echo "Compiling and linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERSDIR)/*.hpp
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -f $(OBJECTS)
