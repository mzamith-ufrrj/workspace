   /*
 *  CellularAutomata.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <CellularAutomata.hpp>
#include <T_UFRRJ.hpp>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <ctime>
#include <climits>
#include <thread>
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
   //char date_prefix[STRINGSIZE];
   //time_t rawtime;
   //struct tm *timeinfo;
   //time(&rawtime);
   //timeinfo = localtime(&rawtime);

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

void CellularAutomata::help(void){
   cout << endl;
   cout << endl;
   cout << "HELP" << endl;
   cout << "\tApplication: " << _APPLICATION << endl;
   cout << "\t    Version: " << _VERSION << endl;
   cout << "\t  Execution: ./<" << _APPLICATION << "> <config file>" << endl;
   cout << "\t     Author: Marcelo Zamith" << endl;
   cout << "\t     e-mail: zamith.marcelo@gmail.com " << endl;
   cout << "\t    Website: http://www.dcc.ufrrj.br/~marcelo" << endl;
   
   cout << "=================================================================" << endl;
   cout << "Structure sizes:" << endl;
   cout << "\ttpVehiclesType: " << setw(3) << sizeof(tpVehiclesType) << " bytes" << endl;
   cout << "\t       Vehicle: " << setw(3) << sizeof(Vehicle) << " bytes" << endl;
   cout << "\t       Blocked: " << setw(3) << sizeof(Blocked) << " bytes" <<  endl;
   cout << "=================================================================" << endl;
  // cout << "Number of physical cores:" << thread::hardware_concurrency() << endl;
   cout << endl;
   
   cout << endl;
   
}


/**
 *  inicialize 
 *  @param fileName: config file name
 */
void CellularAutomata::init(const string &fileName, bool onlyPrintLog)
{  
   
  
   
    
    loadConfigFile(fileName);
    
    //Printing information about parameters

    std::cout << " Cellular automata parameters: \n";
    std::cout << " Model = " << mRules->getParam()->modelName << std::endl;
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
    std::cout << " iDensity     = " << fixed << setprecision(5) << mRules->getParam()->iDensity << std::endl;
    std::cout << " fDensity     = " << mRules->getParam()->fDensity << std::endl;
    std::cout << " dDensity     = " << mRules->getParam()->dDensity << std::endl;
    std::cout << " vMax         = " << mRules->getParam()->vMax << std::endl;
    std::cout << " inFlow       = " << fixed << setprecision(8) << setw(11) << mRules->getParam()->inFlow << std::endl;
    std::cout << " outFlow      = " << mRules->getParam()->outFlow << std::endl;
    std::cout << "=====================================================" << std::endl << std::endl;
    std::cout << fixed << setprecision(0);
    std::cout << "Groups of vehicles: " << mRules->getGrid()->getVehiclesTypeSize() << std::endl;
    for (int i = 0; i < mRules->getGrid()->getVehiclesTypeSize(); i++){
        tpVehiclesType *type =  mRules->getGrid()->getVehicleType(i);
        std::cout << "Profile: " << type->description << " " \
        << "Percent: " << fixed << setprecision(2) << type->percent \
        << " Size: " << type->size \
        << " vMax: " << type->vMax \
        << " inc: "  << type->inc  \
        << " aheadInt: " << type->aheadInt \
        << " SafeDist: " << type->safeDist \
        << " Beta Velocity (" << setprecision(0) << type->param[0] << ", " << type->param[1] << ") " \
        << " Beta Distance (" << type->param[5] << ", " << type->param[6] << ") " \
        << " Left prob.:" <<   setprecision(2) << type->left_p << " Right prob." << type->right_p \
        << " Log vehicles: " << type->vehicleLog \
        << std::endl;
        /*
        if (type->vehicleLog){
                std::cout << "\t Vehicle range: " << type->lo_ID << "," << type->hi_ID << " file sufix: " << type->sufix << std::endl;
        }
        */
    }
    std::cout.flush();
    
    if (onlyPrintLog)
        exit(EXIT_SUCCESS);
    

    
    //Saving maximum gap possible, considering the maxiumum
    //speed limit (Global paramater, no behabior parameter)
    //2017 January, 30th
    
    //openLog();
  
    
        
};

/**
 * Finalize and dealloc all variables.
 * It also saves log file
 */
void CellularAutomata::finalize(void){
    
    mRules->getGrid()->clearMemories();
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
                
                std::cout << std::endl << "Density: " << std::setiosflags(std::ios::fixed) << \
                    std::setprecision(4) << mdGlobal << "/" << mRules->getParam()->fDensity << \
                    std::endl;
                    
                mRules->update(0, false, true);// update(0, dGlobal);
                //openLog(mRules->getParam()->sTime, mdGlobal);
                
                
                START_STOPWATCH(mStopwatch);
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
                    
                    mRules->applyRules(mStep);
                    //mRules->apply(step, dGlobal, step >= mRules->getParam()->dTime);
                    //saveLog();
                
                    mStep++;

                    return;
//-------------------------------------------------------------------                    
                }else{ 
                   mState = 1;
                   mdGlobal += mRules->getParam()->dDensity;
                   
                   STOP_STOPWATCH(mStopwatch);
                   std::cout << "\t    Elapsedtime: [" << std::fixed << std::setw(18) << std::setprecision(5) << mStopwatch.mElapsedTime << "]";
                   return;
                }//end-if (mStep <= mRules->getParam()->sTime){
            }//end- if (mState == 2){
            
            
        }else{
            mState = 3;
            return;
        }//end-if (mdGlobal <  mRules->getParam()->fDensity){
    }//end-if ((mState >= 1) && (mState <= 2)){

    if (mState == 3){
        //closeLog();
        mRunning = false;
        cout << endl << "[End the game!]" << endl;
        return;
    }//end-if (mState == 3){
    
    
   
    
    
};

void CellularAutomata::debugAllVehicles(void){
    Entity *pEntity = mRules->getGrid()->getEntities();
    while (pEntity != NULL){
        std::cerr << "V1(ID= " << pEntity->ID << ",X= "
            <<  pEntity->x  << ",Y= "
            <<  pEntity->y  << std::endl;
        pEntity = pEntity->next;
    }
}

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void CellularAutomata::debug(int time){
    
    int lane[2] = {0,0}; 
    
    for (int j = 0; j < mRules->getParam()->cellY; j++){


    	for (int i = 0; i < mRules->getParam()->cellX;  i++){
            	    Entity *e = mRules->getGrid()->getGrid(i, j);
                    if (e != EMPTY){
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
/*
void CellularAutomata::openLog(void){
    if (!mSaveMode)
        return;
    
    
    if (strcmp(mRules->getParam()->LGFile,"NULL") == 0){
        
      string fileName = "timeSteps." + string(mRules->getParam()->LGFile) + ".log"  ;
      
      mOutputState.open(fileName, std::fstream::out | std::fstream::binary | std::fstream::trunc);
      std::cout << "Log file: [" << fileName << "] opened" << std::endl;

      mMemoryUsed = 0; 
      
      mOutputState.write(reinterpret_cast <const char*> (&mMemoryUsed), sizeof(mMemoryUsed)); mMemoryUsed += sizeof(mMemoryUsed);
      
      mOutputState.write(reinterpret_cast <const char*> (&mRules->getParam()->cellX), sizeof(mRules->getParam()->cellX)); mMemoryUsed += sizeof(mRules->getParam()->cellX);
      
      mOutputState.write(reinterpret_cast <const char*> (&mRules->getParam()->cellY), sizeof(mRules->getParam()->cellY)); mMemoryUsed += sizeof(mRules->getParam()->cellY);
      
      mOutputState.write(reinterpret_cast <const char*> (&mRules->getParam()->deltaH), sizeof(mRules->getParam()->deltaH)); mMemoryUsed += sizeof(mRules->getParam()->deltaH);


      
   }

}
*/
/**
 *  Save informations about the number of vehicles as well as times instante simulation
 *
 *  @param times Number of time step simulation
 *  @param density is the density adopted
 */
/*
void CellularAutomata::openLog(int times, float density){
    if (!mSaveMode)
        return;
    
    tpTableDensityMem mem;
    if (strcmp(mRules->getParam()->LGFile,"NULL") == 0){
      mem.density = density;
      mem.time = times;
      mem.vehicles = mRules->getGrid()->getEntitySize();
      mem.memory = mMemoryUsed;
      mLogTable.push_back(mem);
   }   

}
*/
/**
 *  Close log
 *
 */
/*
void CellularAutomata::closeLog(void){
    if (!mSaveMode)
        return;
   if (strcmp(mRules->getParam()->LGFile,"NULL") == 0){
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

   
   if (strcmp(mRules->getParam()->LGFile,"NULL") == 0){
    
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
*/
/*
 * String manipulation functions - change the file to another
 * one
 */ 

string trim (string txt){
    string out;
    for (unsigned int i = 0; i < txt.length(); i++){
        if (txt[i] != ' ')
            out += txt[i];
    }
    return out;
}

void parse(string line, string *lParam, string *rParam){
   string delimiter = "=", l, r;
   int pos  = line.find(delimiter);
   l = line.substr(0, pos);
   r = line.substr(pos + 1, line.length() - pos);
   
   *lParam = trim (l);
   *rParam = trim (r);
}

void getProfileParameters(string *temp,
                          string *lParam,
                          string *rParam){
    string lrParam;
    int pos = temp->find(";");
    lrParam = temp->substr(0, pos);
    *temp = temp->substr(pos + 1, temp->length() - pos);
    
    parse(lrParam, lParam, rParam);
                              
    
}

void CellularAutomata::fillProfile(int *nType,
                                   const string &params){
    string temp = params,
           lParam,
           rParam;
    
    temp[0] = ' ';
    temp[temp.length()-1] = ' ';

    memset( mRules->getGrid()->getVehicleType(*nType)->param, 0x00, sizeof(double) * 10 ); //It indicates the number of parameters of beta function. Considering two policy (acceleration and safe distance)/
    
    while (temp.find(";") !=  string::npos){
        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("name") == 0){
            strcpy(mRules->getGrid()->getVehicleType(*nType)->description, rParam.c_str());
        }
        
        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("betaA-acc") == 0){
            mRules->getGrid()->getVehicleType(*nType)->param[0] = stof(rParam);
        }

        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("betaB-acc") == 0){
            mRules->getGrid()->getVehicleType(*nType)->param[1] = stof(rParam);
        }

        if (lParam.compare("betaA-dist") == 0){
            mRules->getGrid()->getVehicleType(*nType)->param[5] = stof(rParam);
        }

        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("betaB-adist") == 0){
            mRules->getGrid()->getVehicleType(*nType)->param[6] = stof(rParam);
        }

        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("headway") == 0){
            mRules->getGrid()->getVehicleType(*nType)->aheadInt = stoi(rParam);
        }


        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("headway") == 0){
            mRules->getGrid()->getVehicleType(*nType)->aheadInt = stoi(rParam);
        }
        
        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("safe-distance") == 0){
            mRules->getGrid()->getVehicleType(*nType)->safeDist = stoi(rParam);
        }

        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("vehicle-size") == 0){
            mRules->getGrid()->getVehicleType(*nType)->size = stoi(rParam);
        }

        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("max-speed") == 0){
            mRules->getGrid()->getVehicleType(*nType)->vMax = stoi(rParam);
        }
        
        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("acceleration") == 0){
            mRules->getGrid()->getVehicleType(*nType)->inc = stoi(rParam);
        }

        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("left-probability") == 0){
            mRules->getGrid()->getVehicleType(*nType)->left_p = stof(rParam);
        }
        
        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("right-probability") == 0){
            mRules->getGrid()->getVehicleType(*nType)->right_p = stof(rParam);
        }
        
        getProfileParameters(&temp, &lParam, &rParam);
        if (lParam.compare("log") == 0){
            /*
             * Change to setup which vehicles will record theyselves profiles
             * Not used for while!
             */ 
            strcpy(mRules->getGrid()->getVehicleType(*nType)->vehicleLog, rParam.c_str());
        }
    }

    mRules->getGrid()->getVehicleType(*nType)->param[2] = gamaFunction(mRules->getGrid()->getVehicleType(*nType)->param[0]);
    mRules->getGrid()->getVehicleType(*nType)->param[3] = gamaFunction(mRules->getGrid()->getVehicleType(*nType)->param[1]);
    mRules->getGrid()->getVehicleType(*nType)->param[4] = gamaFunction(mRules->getGrid()->getVehicleType(*nType)->param[0]+mRules->getGrid()->getVehicleType(*nType)->param[1]);

    //anticipation
    mRules->getGrid()->getVehicleType(*nType)->param[7] = gamaFunction(mRules->getGrid()->getVehicleType(*nType)->param[5]);
    mRules->getGrid()->getVehicleType(*nType)->param[8] = gamaFunction(mRules->getGrid()->getVehicleType(*nType)->param[6]);
    mRules->getGrid()->getVehicleType(*nType)->param[9] = gamaFunction(mRules->getGrid()->getVehicleType(*nType)->param[5]+mRules->getGrid()->getVehicleType(*nType)->param[6]);

    
    
}

void CellularAutomata::fillParam(const string &lParam, 
                                 const string &rParam,
                                 int *nType,
                                 tpParam *param){
    if (lParam.compare("model") == 0){
        strcpy(param->modelName, rParam.c_str());   
       
        if (strcmp(param->modelName, "T-UFRRJ") == 0){
            mRules = new T_UFRRJ(param->modelName);
        }else if ( strcmp(param->modelName, "T-UFRRJ-ML-SYM") == 0){
            mRules = new T_UFRRJ_ML_SYM(param->modelName);
        }else if (strcmp(param->modelName, "T-UFRRJ-ML-ASYM") == 0){
            mRules = new T_UFRRJ_ML_ASYM(param->modelName);
        } 

       
       return;
    }
    
    if (lParam.compare("max-speed") == 0){
       param->vMax = stoi(rParam);
       return;
    }
    
    if (lParam.compare("cellX") == 0){
       param->cellX = stoi(rParam);
       return;
    }
    
    if (lParam.compare("cellY") == 0){
       param->cellY = stoi(rParam);
       return;
    }

    
    if (lParam.compare("size") == 0){
       param->deltaH = stof(rParam);
       return;
    }
    
    if (lParam.compare("default-size") == 0){
       param->defaultSize = stof(rParam);
       return;
    }
    
    if (lParam.compare("statistic-sufix") == 0){
       strcpy(param->STFile, rParam.c_str());
       return;
    }

    if (lParam.compare("log-sufix") == 0){
       strcpy(param->LGFile, rParam.c_str());
       return;
    }
    
    if (lParam.compare("time-steps") == 0){
       param->sTime = stoi(rParam);
       return;
    }
    
    if (lParam.compare("transiente-steps") == 0){
       param->dTime = stoi(rParam);
       return;
    }
    
    if (lParam.compare("statistic-steps") == 0){
       param->stTime = stoi(rParam);
       return;
    }

    
    if (lParam.compare("initial-density") == 0){
       param->iDensity = stof(rParam);
       return;
    }

    
    if (lParam.compare("delta-density") == 0){
       param->dDensity = stof(rParam);
       return;
    }


    if (lParam.compare("final-density") == 0){
       param->fDensity = stof(rParam);
       return;
    }
    
    if (lParam == "inFlow"){
       param->inFlow = stof(rParam);
       return;
    }

    
    if (lParam == "outFlow"){
      param->outFlow = stof(rParam);
      return;
    }

    if (lParam.compare("profiles-occupation") == 0){
       strcpy(param->profilesOcc, rParam.c_str());
       int count = 0;
       for (unsigned int i = 0; i < strlen(param->profilesOcc); i++){
           if (param->profilesOcc[i] == ','){
               count++;
           }
       }//end-for (int i = 0; i < param->profilesOcc.length(); i++){
       
       param->nProfiles = count + 1;
       
       mRules->getGrid()->allocVehicleType(param->nProfiles);
       
      
       
       string temp = param->profilesOcc;
       temp[0] = ' ';
       temp[temp.length()-1] = ' ';
       
       int pos  = temp.find(",");
       int index = 0;
       float acc = 0.0f;
       while (pos > 0){
           string aux = temp.substr(0, pos);
           mRules->getGrid()->getVehicleType(index++)->percent = stof(aux);
           acc += stof(aux);
           temp = temp.substr(pos + 1, temp.length() - pos);
           pos  = temp.find(",");
       }
       
       mRules->getGrid()->getVehicleType(index)->percent = stof(temp);
       acc += stof(temp);
       assert(acc <= 1.0f);
       
       return;
    }//end-if (lParam.compare("profiles-occupation") == 0){

    if (lParam.compare("profile") == 0){
        
       fillProfile(nType, rParam);
       *nType = *nType + 1;
       return;
    }
}


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
   string  line, lParam, rParam;
   
   bool    loading = true;

   input.open(configFile, fstream::in);
   assert(input.is_open());
 
   int nType = 0;
   while (loading){
      getline(input, line, '\n');
      if ((line[0] != '#')&& (line != "EOF")){
         parse(line, &lParam, &rParam);
         fillParam(lParam, rParam, &nType, &param);
              
      }//end- if ((line[0] != "#")&& (line == "EOF"))
      
      if (line == "EOF")
        loading = false;
        
   }//end-while (loading){
   input.close();
   strcpy(mRules->getParam()->STFile, param.STFile);
   strcpy(mRules->getParam()->LGFile, param.LGFile);
   strcpy(mRules->getParam()->modelName, param.modelName);
   strcpy(mRules->getParam()->profilesOcc, param.profilesOcc);
   mRules->getParam()->nProfiles = param.nProfiles;
   mRules->getParam()->cellX = param.cellX;
   mRules->getParam()->cellY = param.cellY;
   mRules->getParam()->deltaH = param.deltaH;
   mRules->getParam()->defaultSize = param.defaultSize;
   mRules->getParam()->sTime = param.sTime;
   mRules->getParam()->dTime = param.dTime;
   mRules->getParam()->stTime = param.stTime;
   mRules->getParam()->vMax = param.vMax;
   mRules->getParam()->iDensity = param.iDensity;
   mRules->getParam()->fDensity = param.fDensity;
   mRules->getParam()->dDensity = param.dDensity;
   mRules->getParam()->inFlow = param.inFlow;
   mRules->getParam()->outFlow = param.outFlow;
   mRules->getGrid()->allocGrid(param.cellX, param.cellY);
   

   //memcpy(mRules->getParam(), &param, sizeof(param));
/*
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
   } else if (strcmp(param.modelName, "T-UFRRJ-MR") == 0){
      mRules = new T_UFRRJ_MR(param.modelName);
   } else if (strcmp(param.modelName, "NASCH") == 0){
      mRules = new NaSch(param.modelName);
   }else if (strcmp(param.modelName, "QILANG") == 0){
      mRules = new QiLang(param.modelName);
   }else if (strcmp(param.modelName, "T-UFRRJ-ML-SYM") == 0){
      mRules = new T_UFRRJ_ML_SYM(param.modelName);
   }else if (strcmp(param.modelName, "T-UFRRJ-ML-ASYM") == 0){
      mRules = new T_UFRRJ_ML_ASYM(param.modelName);
   }
    
    }else if (strcmp(param.modelName, "TACUFF2") == 0){
       mRules = new T_UFF(param.modelName);
    }else if (strcmp(param.modelName, "TACUFF_MR2") == 0){
        mRules = new T_UFF_MR(param.modelName);
    } 

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
        mRules->getGrid()->(typeVet[i], i);
    }

    
    */
    
}
 
