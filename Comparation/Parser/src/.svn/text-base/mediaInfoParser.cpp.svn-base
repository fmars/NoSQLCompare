#include "mediaInfoParser.h"
#include "logger.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/timer.hpp> 

using namespace std;

mediaInfoParser::mediaInfoParser()
{

}

bool mediaInfoParser::parseMediaFile(const std::string &path, boost::unordered_map <string, mpuInfo> &mpuData, boost::unordered_map <std::string, midInfo> &resultMap)
{
   // mpuData has <mid->filename> 

   boost::timer t; // start timing
  
   string durationFinder("#EXTINF:");
	for ( boost::unordered_map <string, mpuInfo>::iterator ii = mpuData.begin(); ii != mpuData.end(); ii++ )
	{
		std::string filePath=path+"/"+ii->second.fileName+"/"+ii->second.fileName+".m3u8"; // m3u8 file path.
		if(boost::filesystem::exists(filePath))
		{
         //logInfo()<<"Processing file "<<filePath;
         // open the file and get .ts/mp3 file names and duration from it.
         std::ifstream mediaFile;
	    	std::string line;
 	    	mediaFile.open(filePath.c_str());
	    	float duration = 0;   
	    	if(mediaFile.is_open())
			{
				while(getline(mediaFile, line, '\n'))
				{
               std::size_t found = line.find(durationFinder); // duration
					// process duration
					if(found != std::string::npos)
					{
                  line.erase(found,durationFinder.size());
                  //logInfo() << "Now Line is " << line;

                  std::size_t sep = line.find(","); // duration
                  if(sep != std::string::npos)
                  {
                     // remove everything after 1st ',' including ','
                     line.erase(sep);
                     //logInfo() << "after removing , Line is " << line;
                  }
                  boost::algorithm::trim(line);
                  duration = boost::lexical_cast<float>(line);
                  // Then next line SHOULD BE media segment URI

                  if(!getline(mediaFile, line, '\n'))
                  {
                     logError() << "No line found after EXTINF: tag";
                     continue;
                  }
                  boost::algorithm::trim(line);
                  // get last segment name in case segment URI contains whole path
                  boost::trim_left_if(line,boost::is_any_of("/"));
                  boost::trim_right_if(line,boost::is_any_of("/"));

                  vector<string> token;
                  boost::split(token, line, boost::is_any_of("/"));

                  string segmentName = ii->second.fileName+"/"+token.at(token.size() -1);
                  //logInfo() << segmentName;

                  // Prepare structrure
                  midInfo md;
                  md.mid = boost::lexical_cast<int>(ii->first);
                  md.duration = duration;
                  strcpy(md.mediaType,ii->second.mediaType.c_str());
                  // Populate resultMap
                  resultMap[segmentName]=md;
                  //logInfo() << segmentName << " " << duration << " " << ii->second.mediaType;
                  duration = 0;
   	    		}
   			}
			}
         else
         {
        		logError() << "Could not open file " << mediaFile;
        		//return false;
        	}
      }
      else
      {
      	logError() << "File not found " << filePath;
      	//return false;
      }
	}
   ///....do something here...
   logInfo() << "Time taken: " << t.elapsed();
 
	return true;
}    		    
