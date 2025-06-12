# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Include path
INCLUDES = -Iinclude -Iinclude/roles

# SFML libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRC = src/Game.cpp \
      src/Player.cpp \
      src/roles/Governor.cpp \
      src/roles/Spy.cpp \
      src/roles/Baron.cpp \
      src/roles/General.cpp \
      src/roles/Judge.cpp \
      src/roles/Merchant.cpp \
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
