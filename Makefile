# Author: noapatito123@gmail.com

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# SFML location (adjust if needed)
SFML_LIB_DIR = /usr/lib
SFML_INC_DIR = /usr/include

# Include directories
INCLUDES = -Iinclude -Iinclude/gui -Iinclude/roles -Isrc -Isrc/gui -Isrc/roles -I$(SFML_INC_DIR)

# SFML libraries
LIBS = -L$(SFML_LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system

# Source files excluding main and GUI for testing
SRC_TESTABLE = src/Game.cpp \
               src/Player.cpp \
               src/Button.cpp \
               src/TextBox.cpp \
               src/exceptions.cpp \
               src/roles/Governor.cpp \
               src/roles/Spy.cpp \
               src/roles/Baron.cpp \
               src/roles/General.cpp \
               src/roles/Judge.cpp \
               src/roles/Merchant.cpp

# GUI source files
SRC_GUI = src/main.cpp \
          src/gui/GameGUI.cpp \
          src/gui/InGame_GameGUI.cpp \
          src/gui/TargetSelection_GameGUI.cpp \
          src/gui/Draw_GameGUI.cpp

# All sources
SRC = $(SRC_TESTABLE) $(SRC_GUI)

# Test source files
TEST_SRC = tests/TestGame.cpp tests/TestPlayer.cpp tests/TestRoles.cpp

# Executable names
TARGET = Main
TEST_TARGET = Test

# Build and run GUI
Main: $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC) $(LIBS)
	./$(TARGET)

# Build and run tests (excluding GUI and main)
test: $(SRC_TESTABLE) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TEST_TARGET) $(SRC_TESTABLE) $(TEST_SRC) $(LIBS)
	./$(TEST_TARGET)

# Run Valgrind on unit tests only
valgrind: test
	valgrind --leak-check=full --track-origins=yes ./$(TEST_TARGET)

# Clean build files
clean:
	rm -f $(TARGET) $(TEST_TARGET)