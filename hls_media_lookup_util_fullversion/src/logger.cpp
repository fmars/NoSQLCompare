#include "logger.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

int logger::loglevel = 0;
string logger::outFile = "";
   
logger::logger(const string &str,const string &file) {

        std::string logfileName;
        if(unsigned found = file.find_last_of("/\\")){
             logfileName = file.substr(found+1);
        }
	
	if(outFile.empty())
	{
                cout << getDateTime() << " " << logfileName << " : " << "<" << str << "> ";
	}
	else
	{
		if(!logFile.is_open())
		{
			logFile.open(outFile.c_str(), ios_base::out | ios_base::app);
		}
	
        logFile << getDateTime() << " " << logfileName << " : " << "<" << str << "> ";
   }
}

const std::string logger::getDateTime()
{
	std::ostringstream msg;
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_facet*const f = new boost::posix_time::time_facet("%x %X");
   msg.imbue(std::locale(msg.getloc(),f));
	msg << now;
	return msg.str();
}

logger::~logger() {
	if(outFile.empty())
		cout << std::endl;
	else
   	logFile <<  std::endl;
}

