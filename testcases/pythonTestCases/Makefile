CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11 -lpython3.10  -I/usr/include/python3.10
LDFLAGS = -lpython3.10  -I/usr/include/python3.10 

.PHONY: all clean

all: myprogram

# myprogram: myprogram.o
# 	$(CXX) $(LDFLAGS) -o $@ $< -lcurl

# myprogram.o: hello.cpp
# 	$(CXX) $(CXXFLAGS) -c -o $@ $< -lcurl
myprogram: hello.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) -lcurl
clean:
	rm -f myprogram myprogram.o
