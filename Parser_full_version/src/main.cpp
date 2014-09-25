
// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <fstream>
#include "mediaDataParser.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "mediaInfoParser.h"
#include "dataStorage.h"
#include "logger.h"
#include <boost/timer.hpp> 
#include "mediaInfoRecord.h"
#include "mpuDataRecord.h"


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
   // Check command line and extract arguments.
   // Gets mpu.xml, media files and o/p cache file location from command line.
   if(argC < 5)
   {
      logError() <<"./bin path_to_mpu.xml path_to_media_files path_to_o/p_cache_file logfile_path";
      return -1;
   }
   
   logger::setLogLevel(logger::logINFO);
   logger::setOutFile(argV[4]);
   //
   //  Create varaibles and send to handler
   //
   std::string tableVar("mediaTable");
   std::string option1("MID");
   std::string option2("filename");
   std::string option3("midType");

   mediaDataParser handler;

   boost::unordered_map <std::string,mpuInfo> mpuData;
   mpuData.clear();
   
   // Print out the stats that we collected and time taken
   if (!handler.parseMediaData(argV[1],tableVar,option1,option2,option3,mpuData))
   {
      // No Error..
      if(mpuData.size() <= 0)
      {
         logError() << "No data in mpu..";
         return 0;
      }

      mediaInfoParser midparser;
      logInfo() << "mpu data size " << mpuData.size();
      // Path of media directory
      boost::unordered_map <std::string, midInfo> resultMap;
      midparser.parseMediaFile(argV[2], mpuData,resultMap);
      if(resultMap.size() == 0 )
      {
         logError() <<"Parsing error in media files.";
      }
      else
      {
         logInfo() << "Total database entry size " << resultMap.size();
         
         paxus_storage ps(argV[3]);
         bool ret = ps.create_database();
         if(!ret)
         {
            logError() << "Error in creating DB " << argV[3];
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
               logError() << "Error in inseting record into DB";
            }
            
            free(a);
         }

			ps.close_database();
			
         logInfo() << "Time taken to write to file: " << t.elapsed();
      }
   }
   else
   {
   	logError() << "Error during xml parsing";
      return 0;
	}

   return 0;
}
