#ifndef XMLREADER_H
#define XMLREADER_H

#include "XMLEntity.h"
#include <istream>
#include <expat.h>
#include <queue>
class CXMLReader{
    private:
       std::istream &in;
       SXMLEntity XML;
       XML_Parser parser;
       std::queue<SXMLEntity>xmlqueue;
       static void startelement(void *userdata, const XML_Char *name, const XML_Char**atts);
       static void endelement(void *userdata, const XML_Char *name);
       static void characterhandler(void *userdata, const XML_Char *s, int len);
              
    public:
        CXMLReader(std::istream &is);
        ~CXMLReader();
        
        bool End();
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};

#endif
