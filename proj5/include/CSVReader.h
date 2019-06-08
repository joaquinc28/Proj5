#ifndef CSVREADER_H
#define CSVREADER_H

#include <istream>
#include <string>
#include <vector>
#include <csv.h>
#include <queue>

class CCSVReader{
    protected:
      std::istream &CInput;
      struct csv_parser parse;
      std::vector<std::string>rowbuffer;
      std::queue<std::vector<std::string>>bufferedrows;
      static void callbackfield(void *str, size_t len, void *data);
      static void callbackrow(int, void*);	      
    public:
        CCSVReader(std::istream &in);
        ~CCSVReader();

        bool End() const;
        bool ReadRow(std::vector< std::string > &row);
};

#endif
