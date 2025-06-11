# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Include path
INCLUDES = -Iinclude

# SFML libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRC = src/Game.cpp \
      src/Player.cpp \
      src/Governor.cpp \
      src/Spy.cpp \
      src/Baron.cpp \
      src/General.cpp \
      src/Judge.cpp \
      src/Merchant.cpp \
      src/Button.cpp \
      src/TextBox.cpp \
      src/main_gui.cpp \
      GameGUI.cpp

# Executable name
TARGET = GUI

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC) $(LIBS)

# Clean rule
clean:
	rm -f $(TARGET)
