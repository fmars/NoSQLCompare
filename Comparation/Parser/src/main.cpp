
// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <fstream>
#include "mediaDataParser.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
//#include "mediaInfoParser.h"
//#include "dataStorage.h"
//#include "logger.h"
#include <boost/timer.hpp> 
//#include "mediaInfoRecord.h"
#include "mpuDataRecord.h"


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
   // Check command line and extract arguments.
   // Gets mpu.xml, media files and o/p cache file location from command line.
   char xml_file_name[128]={};
   if(argC < 2)
   {
	   strcpy(xml_file_name,"mpu.xml");
   }else
   		strcpy(xml_file_name,argV[1]);
   
   //logger::setLogLevel(logger::logINFO);
   //:logger::setOutFile(argV[4]);
   //
   //  Create varaibles and send to handler
   //
   std::string tableVar("videoDescriptionTable");
   //std::string option1("MID");
   //std::string option2("title");
   //std::string option3("director");
   int n_arr=15; 
   char arr[][128]={"MID","LID","title","shortTitle","director","cast","year","genre","description","shortDescription","trailerMID","review","countryOrigin","peopleScore","criticScore"}; 
   std::vector<std::string>parseFieldOptionVector;
   for(int i=0;i<n_arr;i++)
	   parseFieldOptionVector.push_back(arr[i]);

   mediaDataParser handler;

   boost::unordered_map <std::string,mpuInfo> mpuData;
   mpuData.clear();
   
   // Print out the stats that we collected and time taken
   if (!handler.parseMediaData(xml_file_name,tableVar,parseFieldOptionVector,mpuData))
   {
	   
      // No Error..
      if(mpuData.size() <= 0)
      {
         std::cout << "No data in mpu..";
         return 0;
      }else{
      	//std::cout<< "mpu data size " << mpuData.size()<<std::endl;
		for(boost::unordered_map<std::string,mpuInfo>::iterator it=mpuData.begin();it!=mpuData.end();++it){
			//std::cout<<"------------------------------------"<<std::endl;
			std::cout<<"NEW RECORD"<<std::endl;
			std::cout<<it->first<<std::endl;
			for(std::map<std::string,std::string>::iterator itt=it->second.info.begin();itt!=it->second.info.end();itt++){
				if(!itt->second.empty() && itt->second!="NULL")
					std::cout<<itt->first<<std::endl<<itt->second<<std::endl;
			}
			std::cout<<"END RECORD"<<std::endl;
			//std::cout<<"------------------------------------"<<std::endl;
		}
	  }


/*
      mediaInfoParser midparser;
      std::cout<< << "mpu data size " << mpuData.size();
      // Path of media directory
      boost::unordered_map <std::string, midInfo> resultMap;
      midparser.parseMediaFile(argV[2], mpuData,resultMap);
      if(resultMap.size() == 0 )
      {
         std::cout <<"Parsing error in media files.";
      }
      else
      {
         std::cout << "Total database entry size " << resultMap.size();
         
         paxus_storage ps(argV[3]);
         bool ret = ps.create_database();
         if(!ret)
         {
            std::cout << "Error in creating DB " << argV[3];
            return 0;
         }

         boost::unordered_map <std::string, midInfo>::iterator ir;

         boost::timer t; // start timing

         for(ir=resultMap.begin(); ir!=resultMap.end();++ir)
         {
            //logInfo() << ir->first << " duration " << ir->second.duration << " MID " << ir->second.mid << " media type " << ir->second.mediaType;
            // add records to data storage.
            void *a = NULL;
            a =  (struct midInfo *)malloc(sizeof(midInfo)); 
            ((midInfo *)a)->duration = ir->second.duration;
            ((midInfo *)a)->mid = ir->second.mid;
            strcpy(((midInfo *)a)->mediaType,ir->second.mediaType);
            bool res = ps.insert_record((void *)ir->first.c_str(),ir->first.size(),a,sizeof(midInfo));
            if(!res)
            {
               std::cout << "Error in inseting record into DB";
            }
            
            free(a);
         }

			ps.close_database();
			
         std::cout << "Time taken to write to file: " << t.elapsed();
      }
	   */
   }
   else
   {
   	std::cout << "Error during xml parsing";
      return 0;
	}

   return 0;
}
