/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <CellularAutomata.hpp>
CellularAutomata mCellularAutomata;
int main(int argc, char *argv[])
{
   
   
   if (strcmp(argv[1], "help") == 0){
      mCellularAutomata.help();
      return EXIT_SUCCESS;
   } 

   mCellularAutomata.init(argv[1], (argc > 2)) ; 
        
   while (mCellularAutomata.isRunning()){
      mCellularAutomata.update();    
   }
   
   mCellularAutomata.finalize();
   
   return EXIT_SUCCESS;
}

