#include <ostream>
#include <string>
#include <vector>
#include "CSVWriter.h"
#include "StringUtils.h"

CCSVWriter::CCSVWriter(std::ostream &ou): output(ou){
}
CCSVWriter::~CCSVWriter(){
}

bool CCSVWriter::WriteRow(const std::vector< std::string > &row){
    std::vector<std::string> OutputRow;
    for(auto &Field : row) {
        OutputRow.push_back(std::string("\"") + StringUtils::Replace(Field,"\"","\"\"") + "\"");
    }
    output<<StringUtils::Join(",",OutputRow)<<std::endl;
    return true;

}

