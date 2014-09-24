// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include "mediaDataParser.h"
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "logger.h"
#include <boost/timer.hpp> 


using namespace std;

// ---------------------------------------------------------------------------
//  mediaDataParser: Constructors and Destructor
// ---------------------------------------------------------------------------
mediaDataParser::mediaDataParser() :
     fSawErrors(false)
    , itIsmediaTable(false)
    , putInMap(false)
    , putInMapfilename(false)
    , putInMapMidType(false),
    parser(NULL)
{
   SAXParser::ValSchemes valScheme = SAXParser::Val_Auto;

   try
   {
      XMLPlatformUtils::Initialize();
   }
   catch(const XMLException& toCatch)
   {
      logError() << "Error during initialization! Message:\n"<< toCatch.getMessage();
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

bool mediaDataParser:: parseMediaData(const char* xmlFile,const std::string &mediaTag, const std::string &opt1, const std::string &opt2,const std::string &opt3,boost::unordered_map <string,mpuInfo> &xmlData)
{
logInfo() << "Parsing xml file " << xmlFile;
   if(NULL == parser)
   {
      logError() << "Parser object not created.";
      return false;
   }
   
   mediatag = mediaTag;
   parseFieldOption1 = opt1;
   parseFieldOption2 = opt2;
   parseFieldOption3 = opt3;

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
      logError() << "OutOfMemoryException";
      return false;
   }
   catch (...)
   {
      logError() << "\nUnexpected exception during parsing: '" << xmlFile;
      return false;
   }
   logInfo() << "Time taken to parse mpu.xml: " << t.elapsed();
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
   }
   XMLString::release(&strLocalname); //Must release memory to avoid memory leak.
}

void mediaDataParser::endElement(const XMLCh* const name)
{
   char *lname =XMLString::transcode(name);
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
   logError() <<" Error " << e.getMessage();
}

void mediaDataParser::fatalError(const SAXParseException& e)
{
   fSawErrors = true;
   logError() <<"FatalError " << e.getMessage();
}

void mediaDataParser::warning(const SAXParseException& e)
{
   logWarning() <<" Warning " << e.getMessage();
}

void mediaDataParser::resetErrors()
{
   fSawErrors = false;
}

