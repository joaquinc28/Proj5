#include <istream>
#include "XMLReader.h"


CXMLReader::CXMLReader(std::istream &is): in(is){
    parser = XML_ParserCreate(nullptr);
    XML_SetElementHandler(parser,startelement, endelement);
    XML_SetUserData(parser,this);
    XML_SetCharacterDataHandler(parser,characterhandler);        


}
CXMLReader::~CXMLReader(){


}

bool CXMLReader::End(){
	if(in.eof() && xmlqueue.empty()){
		return true;
	}
	else{
		return false;
	}


}
        
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    while (skipcdata && !xmlqueue.empty() && xmlqueue.front().DType == SXMLEntity::EType::CharData){
        xmlqueue.pop();
    }

    while(!in.eof() && xmlqueue.empty() ) {
        char buffer[8192];
        in.read(buffer,sizeof(buffer));
        XML_Parse(parser,buffer,in.gcount(),in.eof());
        while (skipcdata && !xmlqueue.empty() && xmlqueue.front().DType == SXMLEntity::EType::CharData){
            xmlqueue.pop();
        }
    }

    if(!xmlqueue.empty()){
	    entity = xmlqueue.front();
	    xmlqueue.pop();
    }
    if(xmlqueue.empty()){
	    return false;
    }
    return true;
    
}

void CXMLReader::startelement(void *userdata, const XML_Char *name, const XML_Char **atts){
    CXMLReader *ptr = static_cast<CXMLReader*>(userdata);
    SXMLEntity entity;
    entity.DNameData = name;
    entity.DType = SXMLEntity::EType::StartElement;
    int index = 0;
    while(atts[index]){
        entity.SetAttribute(atts[index],atts[index + 1]);
        index += 2;
    }
    ptr->xmlqueue.push(entity);
}

void CXMLReader::characterhandler(void *userdata, const XML_Char *s, int len) {
    CXMLReader *ptr = static_cast<CXMLReader*>(userdata);
    SXMLEntity entity;
    entity.DNameData = std::string(s,len);
    entity.DType = SXMLEntity::EType::CharData;
    ptr->xmlqueue.push(entity);
}

void CXMLReader::endelement(void *userdata, const XML_Char *name) {
    CXMLReader *ptr = static_cast<CXMLReader*>(userdata);
    SXMLEntity entity;
    entity.DNameData = name;
    entity.DType = SXMLEntity::EType::EndElement;
    ptr->xmlqueue.push(entity);
}



