# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Include directories
INCLUDES = -Iinclude -Iinclude/roles -Isrc -Isrc/gui -Isrc/roles

# SFML libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRC = src/Game.cpp \
      src/Player.cpp \
      src/Button.cpp \
      src/TextBox.cpp \
      src/main.cpp \
      src/roles/Governor.cpp \
      src/roles/Spy.cpp \
      src/roles/Baron.cpp \
      src/roles/General.cpp \
      src/roles/Judge.cpp \
      src/roles/Merchant.cpp \
      src/gui/GameGUI.cpp \
      src/gui/InGame_GameGUI.cpp \
      src/gui/TargetSelection_GameGUI.cpp \
      src/gui/Draw_GameGUI.cpp

# Executable names
TARGET = Main
TEST_TARGET = Test

# Build and run GUI
Main: $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o Main $(SRC) $(LIBS)
	./Main

# Build and run tests
test: $(SRC) test/test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TEST_TARGET) $(SRC) test/test.cpp
	./$(TEST_TARGET)

# Valgrind memory check
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET) $(TEST_TARGET)
