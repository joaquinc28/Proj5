#include <istream>
#include <string>
#include <vector>
#include "CSVReader.h"
#include <queue>

CCSVReader::CCSVReader(std::istream &in) : CInput(in){
	csv_init(&parse, 0);
}

CCSVReader::~CCSVReader()
{
	csv_free(&parse);
}
bool CCSVReader::ReadRow(std::vector<std::string> &row){
	while(!CInput.eof() && bufferedrows.empty()){
	     char buffer[8924];	
             CInput.read(buffer,sizeof(buffer));
	     csv_parse(&parse,buffer,CInput.gcount(),callbackfield,callbackrow,this);
             if(CInput.eof()){
                  csv_fini(&parse, callbackfield, callbackrow, this);
             }
         }

         if(bufferedrows.empty()){
            return false;
         }
         if(!bufferedrows.empty()){

             row = bufferedrows.front();
	     bufferedrows.pop();
         }
         return true;       
         
}
bool CCSVReader::End() const{
	if(CInput.eof() && bufferedrows.empty()){
		return true;
	}
	else{
		return false;
	}
}

void CCSVReader::callbackfield(void *str, size_t len, void *data){
	CCSVReader *ptr = static_cast<CCSVReader*>(data);
	
	ptr->rowbuffer.push_back(std::string(static_cast<const char*>(str),len));
	
}
void CCSVReader::callbackrow(int ch, void *data){
       CCSVReader *ptr2 = static_cast<CCSVReader*>(data);
       if (ch == '\n' or ch == '\r' or ch == EOF){
           ptr2->bufferedrows.push(ptr2->rowbuffer);
           ptr2-> rowbuffer.clear();
	   
        }	
}


