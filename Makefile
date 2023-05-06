# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# SFML options
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SFML_INC = -I/opt/homebrew/include

SFML_LDFLAGS = -L/opt/homebrew/lib

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable file
EXEC = game

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(SFML_LIBS) $(SFML_LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
