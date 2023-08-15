# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Include paths
INCLUDES = -I../common -Itinyrpc

# Linker flags
LDFLAGS = -lpthread -ltinyrpc # Add the tinyrpc library here

# Source files (include all necessary source files here)
SOURCES = logTest.cc ../common/log.cc  # Add other source files as needed

# Object files
OBJECTS = $(SOURCES:.cc=.o)

# Target executable name
TARGET = my_program

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
