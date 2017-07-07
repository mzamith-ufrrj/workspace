################################################################################
#
#
#  Universidade Federal Rural do Rio de Janeiro
#  Instituto Multidisciplinar - Departamento de Ciencia da Computacao
#  por: Marcelo Zamith - zamith.marcelo@gmail.com
#  para compilar com target release use o comando: make TARGET=release
#  versão: 1.10MC  --> MC Multicelula --> CPU --> means CPU one core!!!
#  Estamos migrando para arquivo XML de configuração
#  Utilização da biblioteca   
#
#  #-DONE #-DHALF #-DZERO used to define of alpha (0.0f, 0.5f, 1.0f)
#  #-DDOUBLEACC  defines acceleration equal 2xinc if there is space enough
#  #-DALPHAVELOCITY defines eff = min(alpha * v, d)
#
################################################################################
EXEFILE      = t-ufrrj
VERSION      = -D_VERSION=\"3.1\"
APPLICATION  = -D_APPLICATION=\"$(EXEFILE)\"
CPUCC     = g++
GPUCC     = nvcc
CPPFLAGS  = --std=c++11 -m64 -fpermissive -Wno-reorder -fopenmp 
DEFS      = $(APPLICATION)  $(VERSION)
lib_dir   = /Users/mzamith/Developer/CPP

INCLUDES  =	-I.

LIBDIR   = -L/usr/lib\
			
         
LIBS     =  -lm -lGLU -lglut -lGL 

LINK     =  $(LIBDIR) $(LIBS)

CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)
GPU_COMPILE = $(GPUCC) $(DEFS) $(INCLUDES) $(GPUFLAGS)


ifeq ($(TARGET), release)
  DEFS      += 
  CPPFLAGS  +=  -O3
else
  DEFS     += -D_DEBUG
  CPPFLAGS += -g -Wall -O0
endif

all:	Grid 			\
	T_UFRRJ 		\
        NaSch 			\
	Rules 			\
	QiLang			\
	CellularAutomata 	\
        MovementSensor		\
        MainOpenGLWindowEvent   \
	Vehicle 		\
	main

	$(CPU_COMPILE)	Grid.o 			\
			T_UFRRJ.o 		\
			NaSch.o 		\
			QiLang.o 		\
			Rules.o      		\
			CellularAutomata.o 	\
			MovementSensor.o	\
			MainOpenGLWindowEvent.o \
			Vehicle.o		\
			main.o 			\
	$(LINK) -o $(EXEFILE)

main:
	$(CPU_COMPILE) -c main.cpp

Vehicle:
	$(CPU_COMPILE) -c Vehicle.cpp

MainOpenGLWindowEvent:
	$(CPU_COMPILE) -c MainOpenGLWindowEvent.cpp

Rules:
	$(CPU_COMPILE) -c Rules.cpp

NaSch:
	$(CPU_COMPILE) -c NaSch.cpp

QiLang:
	$(CPU_COMPILE) -c QiLang.cpp

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

