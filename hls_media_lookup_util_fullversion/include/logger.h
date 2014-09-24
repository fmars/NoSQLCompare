#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <map>
#include <string>
#include <fstream>

//using namespace std;

#define logInfo() \
if (logger::logINFO > logger::getLogLevel()) ; \
else logger("Info",__FILE__)

#define logDebug() \
if (logger::logDEBUG > logger::getLogLevel()) ; \
else logger("Debug",__FILE__)

#define logError() \
if (logger::logERROR > logger::getLogLevel()) ; \
else logger("Err",__FILE__)

#define logWarning() \
if (logger::logWARNING > logger::getLogLevel()) ; \
else logger("Warn",__FILE__)

//TODO: 
class logger {
   
   public:
   
      enum logLevels {logERROR = 0, logWARNING,logINFO,logDEBUG};
      
      logger(const std::string &str,const std::string &file);

      ~logger();

      template<class T> logger & operator<<(const T &x) {
      	if(outFile.empty())
      		std::cout << x;
      	else
      	   logFile << x;
         return *this;
      }
      
      static void setLogLevel(logLevels l)
      {
      	loglevel = l;
      }
      
      static int getLogLevel()
      {
      	return loglevel;
      }
      
      static void setOutFile(const std::string &file)
      {
      	outFile = file;
      }
      
   private:
      //std::ostringstream oss;
      static int loglevel;
      std::ofstream logFile;
      static std::string outFile;
      const std::string getDateTime();
};

#endif // LOGGER_H

