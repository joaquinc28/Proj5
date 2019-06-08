#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <ostream>
#include <string>
#include <vector>


class CCSVWriter{
    protected:
    std::ostream &output;

    public:
        CCSVWriter(std::ostream &ou);
        ~CCSVWriter();

        bool WriteRow(const std::vector< std::string > &row);
};

#endif
