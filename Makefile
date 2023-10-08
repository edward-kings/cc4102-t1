CODEDIRS=. ./algoritmos ./estructuras ./disk_manipulation_functions
INCDIRS=. ./algoritmos ./estructuras ./disk_manipulation_functions

SOURCE := $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.cpp))
EXCLUDE := ./leer.cpp ./main.cpp ./crear.cpp
LIBS := $(filter-out $(EXCLUDE), $(SOURCE))
OBJS	= $(patsubst %.cpp,%.o,$(LIBS))
DEPFILES = $(patsubst %.cpp,%.d,$(SOURCE))

OUT	= crear.exe leer.exe main.exe
CC	 = g++
DEPFLAGS= $(foreach D, $(INCDIRS), -I$(D)) -MP -MD
FLAGS	 = -Wall -std=c++17
OBJFLAGS = -c -o

all: $(OUT)

crear.exe: crear.cpp $(OBJS)
	$(CC) -g $^ -o $@

main.exe: main.cpp $(OBJS)
	$(CC) -g $^ -o $@

leer.exe: leer.cpp $(OBJS)
	$(CC) -g $^ -o $@

%.o: %.cpp
	$(CC) $(FLAGS) $(DEPFLAGS) $(OBJFLAGS) $@ $<

clean:
	rm -f $(OBJS) $(DEPFILES) $(OUT)
	rm -f querys.bin rectangulos.bin

wclean:
	del /Q /S $(notdir $(OBJS)) $(notdir $(DEPFILES)) $(notdir $(OUT))
	del /Q querys.bin rectangulos.bin