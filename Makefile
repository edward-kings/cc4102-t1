OBJS	= crear.o binaryFileFunctions.o generateRectangles.o
SOURCE	= crear.cpp ./disk_manipulation_functions/binaryFileFunctions.cpp ./disk_manipulation_functions/generateRectangles.cpp
HEADER	= Rect.h
OUT	= crear.exe
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

crear.o: crear.cpp
	$(CC) $(FLAGS) crear.cpp -std=c++17

binaryFileFunctions.o: ./disk_manipulation_functions/binaryFileFunctions.cpp
	$(CC) $(FLAGS) ./disk_manipulation_functions/binaryFileFunctions.cpp -std=c++17

generateRectangles.o: ./disk_manipulation_functions/generateRectangles.cpp
	$(CC) $(FLAGS) ./disk_manipulation_functions/generateRectangles.cpp -std=c++17


clean:
	rm -f $(OBJS) $(OUT)

wclean:
	erase $(OBJS) $(OUT)