// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include "mediaDataParser.h"
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
//#include "logger.h"
#include <boost/timer.hpp> 


using namespace std;

// ---------------------------------------------------------------------------
//  mediaDataParser: Constructors and Destructor
// ---------------------------------------------------------------------------
mediaDataParser::mediaDataParser() :
     fSawErrors(false)
    , itIsmediaTable(false),
    parser(NULL)
{
   SAXParser::ValSchemes valScheme = SAXParser::Val_Auto;

   try
   {
      XMLPlatformUtils::Initialize();
   }
   catch(const XMLException& toCatch)
   {
      cout << "Error during initialization! Message:\n"<< toCatch.getMessage();
      return;
   }

   //  Create a SAX parser object. Then, according to what we were told on
   //  the command line, set it to validate or not.
   //
   parser = new SAXParser;
   parser->setValidationScheme(valScheme);
}

mediaDataParser::~mediaDataParser()
{
   if(NULL != parser)
      delete parser;

   // And call the termination method
   XMLPlatformUtils::Terminate();
}

bool mediaDataParser:: parseMediaData(const char* xmlFile,const std::string &mediaTag,const std::vector<std::string>&parseFieldOptionVector,boost::unordered_map <string,mpuInfo> &xmlData)
{
	cout << "Parsing xml file " << xmlFile;
   if(NULL == parser)
   {
      cout << "Parser object not created.";
      return false;
   }
   
   this->mediatag = mediaTag;
   this->parseFieldOptionVector=parseFieldOptionVector;
   for(int i=0;i<parseFieldOptionVector.size();i++){
	   putInVector.push_back(false);
	   parseFieldResult.push_back("");
   }
   //parseFieldOption1 = opt1;
   //parseFieldOption2 = opt2;
   //parseFieldOption3 = opt3;

   parser->setDocumentHandler(this);
   parser->setErrorHandler(this);

   resetErrors();
   boost::timer t; // start timing

   try
   {
      parser->parse(xmlFile);
   }
   catch (const OutOfMemoryException&)
   {
      cout << "OutOfMemoryException";
      return false;
   }
   catch (...)
   {
      cout << "\nUnexpected exception during parsing: '" << xmlFile;
      return false;
   }
   cout << "Time taken to parse mpu.xml: " << t.elapsed()<<endl;
   xmlData = resultMap;
   resultMap.clear();
   return getSawErrors();
}


// ---------------------------------------------------------------------------
//  mediaDataParser: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void mediaDataParser::startElement(const   XMLCh* const     name 
                                    ,       AttributeList&  attributes)
{
   char *strLocalname = XMLString::transcode(name);
   if((NULL !=strLocalname) && !(std::strcmp("table",strLocalname)))
   {
      if(!strcmp(mediatag.c_str(),XMLString::transcode(attributes.getValue(XMLString::transcode("name")))))
      {
         itIsmediaTable = true;
      }
   }
   else if((NULL !=strLocalname) && !(std::strcmp("field",strLocalname)) && itIsmediaTable)
   {
      const XMLCh* varType = XMLString::transcode("name");
      char *temp = XMLString::transcode(attributes.getValue(varType));
      capturedData.clear();
	  for(int i=0;i<parseFieldOptionVector.size();i++)
		  if((NULL !=temp) && (std::strcmp(temp,parseFieldOptionVector[i].c_str())==0)){
			  putInVector[i]=true;
			  break;
		  }

	  /*
      if((NULL !=temp) && (std::strcmp(temp,parseFieldOption1.c_str())==0))
      {
         putInMap = true;
      }
      else if((NULL !=temp) && (!(std::strcmp(temp,parseFieldOption2.c_str()))))
      {
         putInMapfilename = true;
      }
      else if((NULL !=temp) && (!(std::strcmp(temp,parseFieldOption3.c_str()))))
      {
         putInMapMidType = true;
      }
	  */
   }
   XMLString::release(&strLocalname); //Must release memory to avoid memory leak.
}

void mediaDataParser::endElement(const XMLCh* const name)
{
   char *lname =XMLString::transcode(name);
   int i;
   if((!strcmp(lname,"field")) && itIsmediaTable){
	   for(i=0;i<putInVector.size();i++)
		   if( putInVector[i]){
			   putInVector[i]=false;
			   if(i==0)mid=capturedData;
			   if(resultMap[mid].info[parseFieldOptionVector[i]]=="" ||
			   		resultMap[mid].info[parseFieldOptionVector[i]]=="NULL" ){
			   if(capturedData=="NULL" || capturedData.empty() )
				   resultMap[mid].info[parseFieldOptionVector[i]]="NULL";
				else {
			   		resultMap[mid].info[parseFieldOptionVector[i]]=capturedData;
					//cout<<mid<<" "<<parseFieldOptionVector[i]<<" "<<capturedData<<endl;
				}
			   }
			   break;
		   }
   }else if( !strcmp(lname,"table"))
   		itIsmediaTable = false;
	capturedData.clear();
   /*
   if((!strcmp(lname,"field")) && itIsmediaTable && putInMap)
   {
      mid = capturedData;
      putInMap = false;
   }
   else if((!strcmp(lname,"field")) && itIsmediaTable && putInMapMidType)
   {
      midType = capturedData;
      if(midType.compare("audioAggregate") == 0 || midType.compare("audio") == 0)
      {
         midType = "AOD";
      }
      else if(midType.compare("videoAggregate") == 0 || midType.compare("video") == 0)
      {
         midType  = "VOD";
      }
      
      putInMapMidType = false;
      if(capturedData.empty() || capturedData=="NULL")
      {
         mid.clear();
         filename.clear();
         midType.clear();
      }
      else if(!filename.empty())
      {
         mpuInfo info;
         info.fileName = filename;
         info.mediaType = "exw_"+midType;
         resultMap[mid] = info;
         mid.clear();
         filename.clear();
         midType.clear();
      }
   }
   else if((!strcmp(lname,"field")) && itIsmediaTable && putInMapfilename)
   {
      putInMapfilename = false;
      filename = capturedData;
      if(capturedData.empty() || capturedData=="NULL")
      {
         mid.clear();
         filename.clear();
         midType.clear();
      }
      else if(!midType.empty())
      {
         mpuInfo info;
         info.fileName = filename;
         info.mediaType = "exw_"+midType;
         resultMap[mid] = info;
         mid.clear();
         filename.clear();
         midType.clear();
      }
   }
   else if(!strcmp(lname,"table"))
      itIsmediaTable = false;
   else if(!std::strcmp(lname,"table") && !itIsmediaTable)
      return;
	  */

   capturedData.clear();
}	

void mediaDataParser::characters(  const   XMLCh* const    chars
								    , const XMLSize_t       length)
{
   char *tempCapturedData= XMLString::transcode(chars);
   std::string tempString(tempCapturedData);
   capturedData = tempString;
}


// ---------------------------------------------------------------------------
//  SAXCountHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void mediaDataParser::error(const SAXParseException& e)
{
   fSawErrors = true;
   cout <<" Error " << e.getMessage();
}

void mediaDataParser::fatalError(const SAXParseException& e)
{
   fSawErrors = true;
   cout <<"FatalError " << e.getMessage();
}

void mediaDataParser::warning(const SAXParseException& e)
{
   cout <<" Warning " << e.getMessage();
}

void mediaDataParser::resetErrors()
{
   fSawErrors = false;
}

