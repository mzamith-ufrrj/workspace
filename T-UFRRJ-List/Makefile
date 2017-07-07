################################################################################
#
#
#  Universidade Federal Rural do Rio de Janeiro
#  Instituto Multidisciplinar - Departamento de Ciencia da Computacao
#  por: Marcelo Zamith - zamith.marcelo@gmail.com
#  para compilar com target release use o comando: make target=release + rápido
#  compilador padrão da intel make compiler=gnu para g++
#  versão de on-ramp
#
################################################################################
EXEFILE      = t-ufrrj
VERSION      = -D_VERSION=\"3.3\"
APPLICATION  = -D_APPLICATION=\"$(EXEFILE)\"
CPUCC     = /opt/intel/bin/icpc
CPPFLAGS  = --std=c++11 -m64 -fpermissive -Wno-reorder 
DEFS      = $(APPLICATION)  $(VERSION)


INCLUDES  =	-I.

LIBDIR   = -L/usr/lib\
			
         
LIBS     =  -lm 

LINK     =  $(LIBDIR) $(LIBS)

CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

ifeq ($(compiler), gnu)
	CPUCC     = g++
endif

ifeq ($(target), release)
  DEFS      += 
  CPPFLAGS  +=  -Ofast
else
  DEFS     += -D_DEBUG
  CPPFLAGS += -g -Wall -O0
endif

all:	Grid 			\
	T_UFRRJ 		\
        Rules 			\
	CellularAutomata 	\
        MovementSensor		\
	Vehicle 		\
	main

	$(CPU_COMPILE)  Grid.o             \
			T_UFRRJ.o          \
			Rules.o            \
			CellularAutomata.o \
			MovementSensor.o   \
			Vehicle.o          \
			main.o             \
	$(LINK) -o $(EXEFILE)

main:
	$(CPU_COMPILE) -c main.cpp

Vehicle:
	$(CPU_COMPILE) -c Vehicle.cpp

Rules:
	$(CPU_COMPILE) -c Rules.cpp

T_UFRRJ:
	$(CPU_COMPILE) -c T_UFRRJ.cpp

Grid:
	$(CPU_COMPILE) -c Grid.cpp


MovementSensor:
	$(CPU_COMPILE) -c MovementSensor.cpp
   

CellularAutomata:
	$(CPU_COMPILE) -c CellularAutomata.cpp

clean:
	rm *.o; rm $(EXEFILE)

files:
	rm *.txt; rm *.dat; rm *.o

