// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <iostream>
#include <string>
#include <boost/unordered_map.hpp>
#include "mpuDataRecord.h"


XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class AttributeList;
XERCES_CPP_NAMESPACE_END

class mediaDataParser : public HandlerBase
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    mediaDataParser();
    ~mediaDataParser();

    bool parseMediaData(const char* xmlFile,const std::string &mediaTag, const std::vector<std::string> &parseFieldOptionVector,boost::unordered_map <std::string, mpuInfo> &);
    
private:
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool            fSawErrors;
    bool 	    itIsmediaTable;
    std::string     capturedData;
    //bool	    putInMap;
    //bool	    putInMapfilename;
    //bool putInMapMidType;
    std::string	    mid;
    //std::string     filename;
    //std::string     midType;
    std::string     mediatag;
    //std::string     parseFieldOption1;
    //std::string     parseFieldOption2;
    //std::string     parseFieldOption3;

	//fmars add
	std::vector<bool>			putInVector;	
	std::vector<std::string>	parseFieldResult;
	std::vector<std::string>	parseFieldOptionVector;

    SAXParser* parser;
    boost::unordered_map<std::string, mpuInfo> resultMap;
    
    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const
    {
        return fSawErrors;
    }

    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void startElement(const XMLCh* const name, AttributeList& attributes);
    void characters(const XMLCh* const chars, const XMLSize_t length);
    void endElement(const XMLCh* const name);

    // -----------------------------------------------------------------------
    //  Handlers for the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& exc);
    void error(const SAXParseException& exc);
    void fatalError(const SAXParseException& exc);
    void resetErrors();
};
