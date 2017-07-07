/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <CellularAutomata.hpp>
#include <MainOpenGLWindowEvent.hpp>
CellularAutomata mCellularAutomata;
MainOpenGLWindowEvent *mOpenGLWindow;


int main(int argc, char *argv[])
{
   
  


   mCellularAutomata.init(argv[1]) ; 
        
   while (mCellularAutomata.isRunning()){
      mCellularAutomata.update();    
   }
   
   mCellularAutomata.finalize();
   
   return EXIT_SUCCESS;
}

