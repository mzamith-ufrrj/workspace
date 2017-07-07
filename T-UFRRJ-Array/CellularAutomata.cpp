   /*
 *  CellularAutomata.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <CellularAutomata.hpp>
#include <QiLang.hpp>
#include <NaSch.hpp>
#include <T_UFRRJ.hpp>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <ctime>
#include <climits>
using namespace std;

/*
 *
 */
CellularAutomata::CellularAutomata():
mRules(NULL),
mMemoryUsed(0),
mSaveMode(true),
mState(0),
mRunning(true),
mpStep(0),
mdGlobal(0.0f)
{ 
   char date_prefix[STRINGSIZE];
   time_t rawtime;
   struct tm *timeinfo;
   time(&rawtime);
   timeinfo = localtime(&rawtime);

    cout << "\nStarting application" << endl;

    /*
    sprintf(date_prefix, "%.4d_%.2d_%.2d", 1900+timeinfo->tm_year, timeinfo->tm_mon+1, timeinfo->tm_mday);

    mEnvironmentVariables.clear();
    mEnvironmentVariables["$NULL$"]    = "NULL";
    mEnvironmentVariables["$null$"]    = "NULL";
    mEnvironmentVariables["$default$"] = "TCA_default";
    mEnvironmentVariables["$DEFAULT$"] = "TCA_default";
    mEnvironmentVariables["$debug$"]   = "TCA_debug";
    mEnvironmentVariables["$DEBUG$"]   = "TCA_debug";
    mEnvironmentVariables["$date$"]    = date_prefix;
    mEnvironmentVariables["$DATE$"]    = date_prefix;
    mEnvironmentVariables["$simulation_version$"] = "Current_Version";
*/
}

/*
 *
 */
CellularAutomata::~CellularAutomata() 
{ 
   cout << "\nFinalizing application" << endl; 

#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif

   
   
};




/*
 * Clear memories
 */
//void CellularAutomata::clear(void) { mRules->getGrid()->allocGrid(mParam.cellX, mRules->getParam()->cellY); };


/**
 *  inicialize 
 *  @param fileName: config file name
 */
void CellularAutomata::init(const string &fileName)
{  
    
    loadConfigFile(fileName);
    
    //Printing information about parameters

    std::cout << " Cellular automata parameters: \n";
    std::cout << " cellX  = " << mRules->getParam()->cellX << std::endl;
    std::cout << " cellY  = " << mRules->getParam()->cellY << std::endl;
    std::cout << " DeltaH = " << mRules->getParam()->deltaH << std::endl;
    std::cout << " STFile = " << mRules->getParam()->STFile << std::endl;
    std::cout << " LGFile = " << mRules->getParam()->LGFile << std::endl;
    //std::cout << " LNFile = " << mRules->getParam()->LNFile << std::endl;

    
    std::cout << " sTime        = " << mRules->getParam()->sTime << std::endl;
    std::cout << " dTime        = " << mRules->getParam()->dTime << std::endl;
    std::cout << " stTime       = " << mRules->getParam()->stTime << std::endl;
    //std::cout << " deltaTime    = " << mRules->getParam()->deltaTime << std::endl;
    std::cout << " iDensity     = " << mRules->getParam()->iDensity << std::endl;
    std::cout << " fDensity     = " << mRules->getParam()->fDensity << std::endl;
    std::cout << " dDensity     = " << mRules->getParam()->dDensity << std::endl;
    std::cout << " vMax         = " << mRules->getParam()->vMax << std::endl;
    std::cout << " Model        = " << mRules->getParam()->modelName << std::endl;
    std::cout << "=====================================================" << std::endl << std::endl;
    
    std::cout << "Groups of vehicles: " << mRules->getGrid()->getVehiclesTypeSize() << std::endl;
    for (int i = 0; i < mRules->getGrid()->getVehiclesTypeSize(); i++){
        tpVehiclesType *type =  mRules->getGrid()->getVehicleType(i);
        std::cout << "Behavior: " << type->description << " " << "Percent: " << type->percent << " Size: " << (int)type->size << " vMax: " << (int) type->vMax \
        << " inc: " << type->inc << " aheadInt: " << type->aheadInt << " Beta Velocity (" \
        << type->param[0] << ", " << type->param[1] << ") " << " Beta Distance (" \
        << type->param[5] << ", " << type->param[6] << ") " \
        << " Left prob.:" <<  type->left_p << " Right prob." << type->right_p << std::endl;
        if (type->vehicleLog){
                std::cout << "\t Vehicle range: " << type->lo_ID << "," << type->hi_ID << " file sufix: " << type->sufix << std::endl;
        }
        
    }
    std::cout.flush();
    
    

    
    //Saving maximum gap possible, considering the maxiumum
    //speed limit (Global paramater, no behabior parameter)
    //2017 January, 30th
    
    openLog();
  
    
        
};

/**
 * Finalize and dealloc all variables.
 * It also saves log file
 */
void CellularAutomata::finalize(void){
    

	mRules->finalizer();
    if (mRules != NULL)
        delete mRules;

};

/*
 * Running the simulation
 */

void CellularAutomata::update(void){
    
    if (mState == 0){
       

        mdGlobal = mRules->getParam()->iDensity;
        mpStep   = mRules->getParam()->sTime / 20; 
        
        if (mpStep == 0)
            mpStep = 1;
        
        mState++;
        return;
    }

    if ((mState >= 1) && (mState <= 2)){
        if (mdGlobal <  mRules->getParam()->fDensity){
            if (mState == 1){
                mRules->initialCondition(mdGlobal);
                //mRules->getSensor()->clear();
                mRules->initialCondition(mdGlobal);
                
                std::cout << std::endl << "Density: " << std::setiosflags(std::ios::fixed) << \
                    std::setprecision(4) << mdGlobal << "/" << mRules->getParam()->fDensity << \
                    " " << mRules->getGrid()->getVehiclesSize() << std::endl;
                    
                mRules->update(0, mdGlobal, false, true);// update(0, dGlobal);
                openLog(mRules->getParam()->sTime, mdGlobal);
                
                mStep = 1;
                mState++;
                return;
            }//end- if (mState == 1){
            
            if (mState == 2){
                if (mStep <= mRules->getParam()->sTime){
//-------------------------------------------------------------------                    
                    if ((mStep % mpStep) == 0){
                        std::cout << ".";
                        std::cout.flush();
                    }
                    
                    if (mStep >= mRules->getParam()->dTime){
                        if (mStep == mRules->getParam()->dTime){
                            std::cout << "|";
                            //mRules->setStatistic(true);
                        }
                        
                        
                        
                    }
                    
                    //debug(mStep);
                    
                    mRules->apply(mStep, mdGlobal);
                    //mRules->apply(step, dGlobal, step >= mRules->getParam()->dTime);
                    saveLog();
                
                    mStep++;

                    return;
//-------------------------------------------------------------------                    
                }else{ 
                   mState = 1;
                   mdGlobal += mRules->getParam()->dDensity;
                   return;
                }//end-if (mStep <= mRules->getParam()->sTime){
            }//end- if (mState == 2){
            
            
        }else{
            mState = 3;
            return;
        }//end-if (mdGlobal <  mRules->getParam()->fDensity){
    }//end-if ((mState >= 1) && (mState <= 2)){

    if (mState == 3){
        closeLog();
        mRunning = false;
        return;
    }//end-if (mState == 3){
    
    
   
    
    
};

void CellularAutomata::debugAllVehicles(void){
    /*
    for (int i = 0; i < mRules->getGrid()->getVehiclesSize(); i++){
        Vehicle vehicles = mRules->getGrid()->getVehicles(i);
        cerr << vehicles.getAttribute()->ID << " " << vehicles.getAttribute()->x << " " << vehicles.getAttribute()->y << " " << vehicles.getAttribute()->lg << endl;
        
     
    }

     */
}

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void CellularAutomata::debug(int time){
    
    int lane[2] = {0,0};
    
    for (int j = 0; j < mRules->getParam()->cellY; j++){


    	for (int i = 0; i < mRules->getParam()->cellX;  i++){
            	    int idx = mRules->getGrid()->getGrid(i, j);
                    if (idx != -1){
                        lane[j]++;
                    }
    	            /*
                    if (idx != -1){
    	                cerr << (int) mRules->getGrid()->getVehicles(idx).ID + 1;

    	            }
    	            else
    	                cerr << ".";
                    */
    	}
        
    	//cerr << endl;

    }
    
    
    if (lane[0] != lane[1]){
        cerr << "Time = " << time;
        cerr << "  -> " << lane[0] << " \t\t " << lane[1] << " ";
        cerr << endl;
    }

    

};



//------------------------------------------------------------------------------
/**
 *  Open LOG file
 */
void CellularAutomata::openLog(void){
    if (!mSaveMode)
        return;
    
    
    if (strcmp(mRules->getParam()->LGFile, "NULL") != 0){
      mOutputState.open(mRules->getParam()->LGFile, std::fstream::out | std::fstream::binary | std::fstream::trunc);
      std::cout << "Log file: [" << mRules->getParam()->LGFile << "] opened" << std::endl;

      mMemoryUsed = 0;
      
      mOutputState.write(reinterpret_cast <const char*> (&mMemoryUsed), sizeof(mMemoryUsed)); mMemoryUsed += sizeof(mMemoryUsed);
      
      mOutputState.write(reinterpret_cast <const char*> (&mRules->getParam()->cellX), sizeof(mRules->getParam()->cellX)); mMemoryUsed += sizeof(mRules->getParam()->cellX);
      
      mOutputState.write(reinterpret_cast <const char*> (&mRules->getParam()->cellY), sizeof(mRules->getParam()->cellY)); mMemoryUsed += sizeof(mRules->getParam()->cellY);
      
      mOutputState.write(reinterpret_cast <const char*> (&mRules->getParam()->deltaH), sizeof(mRules->getParam()->deltaH)); mMemoryUsed += sizeof(mRules->getParam()->deltaH);


      
   }

}

/**
 *  Save informations about the number of vehicles as well as times instante simulation
 *
 *  @param times Number of time step simulation
 *  @param density is the density adopted
 */
void CellularAutomata::openLog(int times, float density){
    if (!mSaveMode)
        return;
    
    tpTableDensityMem mem;
   if (strcmp(mRules->getParam()->LGFile, "NULL") != 0)
   {
      mem.density = density;
      mem.time = times;
      mem.vehicles = mRules->getGrid()->getVehiclesSize();
      mem.memory = mMemoryUsed;
      mLogTable.push_back(mem);
   }   

}

/**
 *  Close log
 *
 */
void CellularAutomata::closeLog(void){
    if (!mSaveMode)
        return;
   if (strcmp(mRules->getParam()->LGFile, "NULL") != 0){
      std::cout << std::endl << std::endl;
      std::cout << "Closing log file" << std::endl;
      std::cout.flush();

      size_t size = mLogTable.size();
      mOutputState.write(reinterpret_cast <const char*> (&size), sizeof(size)); 
      mOutputState.write(reinterpret_cast <const char*> (&(mLogTable[0])), sizeof(tpTableDensityMem)*size   ); 
      

      mOutputState.seekg(0);
      mOutputState.write(reinterpret_cast <const char*> (&mMemoryUsed), sizeof(mMemoryUsed));
      std::cout << "Log file: [" << mRules->getParam()->LGFile << "] closed" << std::endl;

      mOutputState.close();
      
   }

}

void CellularAutomata::saveLog(void)
{

   if (!mSaveMode)
       return;

   
   if (strcmp(mRules->getParam()->LGFile, "NULL") != 0)
   {
      for (int i = 0; i < mRules->getGrid()->getVehiclesSize(); i++)
      {
         mOutputState.write(reinterpret_cast <const char*> (&mRules->getGrid()->getVehicles(i).ID), sizeof(mRules->getGrid()->getVehicles(i).ID)); mMemoryUsed += sizeof(mRules->getGrid()->getVehicles(i).ID);
        
         mOutputState.write(reinterpret_cast <const char*> (&mRules->getGrid()->getVehicles(i).x), sizeof(mRules->getGrid()->getVehicles(i).x)); mMemoryUsed += sizeof(mRules->getGrid()->getVehicles(i).x);
         
         mOutputState.write(reinterpret_cast <const char*> (&mRules->getGrid()->getVehicles(i).y), sizeof(mRules->getGrid()->getVehicles(i).y)); mMemoryUsed += sizeof(mRules->getGrid()->getVehicles(i).y);

          mOutputState.write(reinterpret_cast <const char*> (&mRules->getGrid()->getVehicles(i).lg), sizeof(mRules->getGrid()->getVehicles(i).lg)); mMemoryUsed += sizeof(mRules->getGrid()->getVehicles(i).lg);

          mOutputState.write(reinterpret_cast <const char*> (&mRules->getGrid()->getVehicles(i).mType), sizeof(mRules->getGrid()->getVehicles(i).mType)); mMemoryUsed += sizeof(mRules->getGrid()->getVehicles(i).mType);

          mOutputState.write(reinterpret_cast <const char*> (&mRules->getGrid()->getVehicles(i).type->inc), sizeof(mRules->getGrid()->getVehicles(i).type->inc)); mMemoryUsed += sizeof(mRules->getGrid()->getVehicles(i).type->inc);
          
      }
      
   }
   
};




/**
 *  This function:
 *
 *  @param in configFile The text string name of the HLA configuration file.
 */
void CellularAutomata::loadConfigFile(const string& configFile){
   fstream input;
   tpParam param;
   vector <tpVehiclesType> typeVet;
   tpVehiclesType mytype;
   string  line;
   int behaviors = 0;

   input.open(configFile, fstream::in);
   assert(input.is_open());

   getline(input, line, '\n'); strcpy(param.modelName, line.c_str());

   getline(input, line, '\n'); param.vMax = stoi(line);
   getline(input, line, '\n'); param.cellX = stoi(line);
   getline(input, line, '\n'); param.cellY = stoi(line);

   getline(input, line, '\n'); param.deltaH = stof(line);
   getline(input, line, '\n'); param.defaultSize = stof(line);
   getline(input, line, '\n'); strcpy(param.STFile, line.c_str()); 
   getline(input, line, '\n'); strcpy(param.LGFile, line.c_str()); 

   getline(input, line, '\n'); param.sTime = stoi(line);
   getline(input, line, '\n'); param.dTime = stoi(line);
   getline(input, line, '\n'); param.stTime = stoi(line);

   getline(input, line, '\n'); param.iDensity = stof(line);
   getline(input, line, '\n'); param.dDensity = stof(line);
   getline(input, line, '\n'); param.fDensity = stof(line);

   getline(input, line, '\n'); behaviors = stoi(line);
   
   typeVet.clear();
   behaviors--;
   assert(behaviors > 0);
   float occ = 0.0f;
   for (int i = 0 ; i < behaviors; i++){
      assert(posix_memalign((void**) &mytype.param, ALIGN, sizeof(double) * PDFPARAM[BETA] * 2) == 0); // Acceleration and distance perception !!!
   
      
      getline(input, line, '\n'); strcpy(mytype.description, line.c_str());
      
      getline(input, line, '\n'); mytype.param[0] = stof(line);
      getline(input, line, '\n'); mytype.param[1] = stof(line);
      getline(input, line, '\n'); mytype.param[5] = stof(line);
      getline(input, line, '\n'); mytype.param[6] = stof(line);
      
      getline(input, line, '\n'); mytype.aheadInt = stoi(line);
      getline(input, line, '\n'); mytype.safeDist = stoi(line);
      getline(input, line, '\n'); mytype.size = stoi(line);
      getline(input, line, '\n'); mytype.percent = stof(line);
      getline(input, line, '\n'); mytype.vMax = stoi(line);
      getline(input, line, '\n'); mytype.inc = stoi(line);
      getline(input, line, '\n'); mytype.left_p = stof(line);
      getline(input, line, '\n'); mytype.right_p = stof(line);
      
      getline(input, line, '\n');
      //Verifying ID range to follow through file
      line.find("-");
      string lo = line.substr(0, line.find("-"));
      string hi = line.substr(line.find("-")+1, line.length() - line.find("-"));
      mytype.lo_ID = stoi(lo);
      if ((hi == "N") || (hi == "n"))
        mytype.hi_ID = INT_MAX;
      else
        mytype.hi_ID = stoi(hi);
      
      getline(input, line, '\n');
    
      if ((line == "false") || (line == "FALSE"))
        mytype.vehicleLog = false;
      else
       mytype.vehicleLog = true;
      
      strcpy(mytype.sufix, param.modelName);
          
      
      occ += mytype.percent;
      assert(occ <= 1.0f);

      //Aceleration
      mytype.param[2] = gamaFunction(mytype.param[0]);
      mytype.param[3] = gamaFunction(mytype.param[1]);
      mytype.param[4] = gamaFunction(mytype.param[0]+mytype.param[1]);

      //anticipation
      mytype.param[7] = gamaFunction(mytype.param[5]);
      mytype.param[8] = gamaFunction(mytype.param[6]);
      mytype.param[9] = gamaFunction(mytype.param[5]+mytype.param[6]);



      typeVet.push_back(mytype);
   }
   input.close();

   //mRules = new NaSch(param.modelName);
   
   if (strcmp(param.modelName, "T-UFRRJ") == 0){
      mRules = new T_UFRRJ(param.modelName);
   }/*else if (strcmp(param.modelName, "T-UFRRJ-MR") == 0){
      mRules = new T_UFRRJ_MR(param.modelName);
   }*/else if (strcmp(param.modelName, "NASCH") == 0){
      mRules = new NaSch(param.modelName);
   }else if (strcmp(param.modelName, "QILANG") == 0){
      mRules = new QiLang(param.modelName);
   }else if (strcmp(param.modelName, "T-UFRRJ-ML-SYM") == 0){
      mRules = new T_UFRRJ_ML_SYM(param.modelName);
   }else if (strcmp(param.modelName, "T-UFRRJ-ML-ASYM") == 0){
      mRules = new T_UFRRJ_ML_ASYM(param.modelName);
   }
   /*
    }else if (strcmp(param.modelName, "TACUFF2") == 0){
       mRules = new T_UFF(param.modelName);
    }else if (strcmp(param.modelName, "TACUFF_MR2") == 0){
        mRules = new T_UFF_MR(param.modelName);
    }*/

    assert(mRules != NULL);
   //mEnvironmentVariables["$simulation_version$"] = param.Version;
    
    
    memcpy(mRules->getParam(), &param, sizeof(param));
    
   // defineVar(mRules->getParam()->STFile, param.STFile);
   // defineVar(mRules->getParam()->LGFile, param.LGFile);
   // mRules->getGrid()->clearGAPSvet();
    mRules->getGrid()->allocMaximumGaps(param.vMax);
    mRules->getGrid()->allocGrid(param.cellX, param.cellY);
    int size = typeVet.size();
    int ptr = 0;
    for (int i = 0; i < size; i++){
        if ((strcmp(typeVet[i].description, "blocked") == 0) || (strcmp(typeVet[i].description, "BLOCKED") == 0)){
            tpVehiclesType swap = typeVet[i];
            typeVet[i] = typeVet[ptr];
            typeVet[ptr] = swap;
            ptr++;
        }
    }
    
    
    
    mRules->getGrid()->allocVehicleType(size);
    for (int i = 0; i < size; i++){
        mRules->getGrid()->addVehicleType(typeVet[i], i);
    }

    
    
    
}
 
