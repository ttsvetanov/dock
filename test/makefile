CC 			= g++
CCFLAGS 	= -static -std=c++1z -Wall -Wextra -Werror -pedantic
INCLUDE		= -I"../src" -I"../lib"
TARGET		= test.exe
OPTIMIZE	= -O0

$(TARGET):	test.cpp
	$(CC) $(CCFLAGS) $(INCLUDE) $(OPTIMIZE) test.cpp -o $(TARGET)

all: $(TARGET)

clean:
	rm test.exe