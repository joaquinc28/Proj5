#include <ostream>
#include <string>
#include <vector>
#include <utility>
#include "XMLWriter.h"
CXMLWriter::CXMLWriter(std::ostream &os): output(os){

}

CXMLWriter::~CXMLWriter() {

}

bool CXMLWriter::Flush() {
	if(!stack.empty()){
		while(!stack.empty()){
			output << "</" + stack.top() + ">";

        }
    }
	return true;
}
std::string Replace(std::string str, std::string old, std::string rep) {
    std::string str2;
    for (auto Ch : str) {
        str2 += Ch;
    }
    size_t i = 0;
    while ((i = str.find(old, i)) != std::string::npos) {
        str2.replace(i, old.length(), rep);
        i += rep.length();
    }
    return str2;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if(entity.DType == SXMLEntity::EType::StartElement){
        std::string str;
        str += "<" + entity.DNameData;
        for(auto c: entity.DAttributes){
            std::get<1>(c) = Replace(std::get<1>(c), "&", "&amp");
            std::get<1>(c) = Replace(std::get<1>(c), "<", "&lt");
            std::get<1>(c) = Replace(std::get<1>(c), ">", "&gt");
            std::get<1>(c) = Replace(std::get<1>(c), "\"", "&quot");
            std::get<1>(c) = Replace(std::get<1>(c), "\'", "&apos");
            str += " " + std::get<0>(c) + "=" + "\"" + std::get<1>(c) + "\"";
        }
        str += ">";
        output << str;
        stack.push(entity.DNameData);
	return true;
    }
    if(entity.DType == SXMLEntity::EType::CharData){
        output<< entity.DNameData;
	return true;

    }
    if(entity.DType == SXMLEntity::EType::EndElement){
        output<< "</" + entity.DNameData + ">";
        if(entity.DNameData == stack.top()){
            stack.pop();
        }
	return true;

    }
    if(entity.DType == SXMLEntity::EType::CompleteElement){
        std::string str;
        str += "<" + entity.DNameData;
        for(auto c: entity.DAttributes){
            str += " " + std::get<0>(c) + "=" + "\"" + std::get<1>(c) + "\"";
        }
        str += "/>";
        output << str;
	return true;
    }
return false;
}

