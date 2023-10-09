CODEDIRS=. ./algoritmos ./estructuras ./disk_manipulation_functions
INCDIRS=. ./algoritmos ./estructuras ./disk_manipulation_functions

SOURCE := $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.cpp))
EXCLUDE := ./leer.cpp ./main.cpp
LIBS := $(filter-out $(EXCLUDE), $(SOURCE))
OBJS	= $(patsubst %.cpp,%.o,$(LIBS))
DEPFILES = $(patsubst %.cpp,%.d,$(SOURCE))

OUT	= leer.exe main.exe
CC	 = g++
DEPFLAGS= $(foreach D, $(INCDIRS), -I$(D)) -MP -MD
FLAGS	 = -O3 -Wall -std=c++17
OBJFLAGS = -c -o

all: $(OUT)

main.exe: main.cpp $(OBJS)
	$(CC) -g $^ -o $@

leer.exe: leer.cpp $(OBJS)
	$(CC) -g $^ -o $@

%.o: %.cpp
	$(CC) -g $(FLAGS) $(DEPFLAGS) $(OBJFLAGS) $@ $<

clean:
	rm -f $(OBJS) $(DEPFILES) $(OUT)

cleanBin:
	rm -f *.bin

wclean:
	del /Q /S $(notdir $(OBJS)) $(notdir $(DEPFILES)) $(notdir $(OUT))

wcleanBin:
	del /Q *.bin