#include "CSVReader.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>


int main(int argc, char* argv[]){
	for(int i = 0; i < argc;i++){
        std::ifstream fp(argv[i]);
        std::string str(argv[i]);

        size_t found = str.find_last_of('.');
        std::string ext = str.substr(found + 1);


        if(ext == "csv"){
            size_t find = str.find_first_of('_');
            std::string subject = str.substr(0,find);
            std::string str1 = str.substr(find + 1);
            size_t find1 = str1.find_first_of('_');
            std::string course = str1.substr(0,find1);
            std::string str2 = str1.substr(find1 + 1);
            size_t find2 = str2.find_last_of('.');
            std::string section = str2.substr(0,find2);

            std::string filena = subject + "_" + course + "_" + section + ".xml";


            CCSVReader read(fp);
            std::ofstream file;
            file.open(filena);
            
            std::vector<std::string>header;
            read.ReadRow(header);
            for(int i = 0;i<header.size();i++){
                std::cout<<header[i]<<std::endl;
            }
            SXMLEntity head;
            head.DNameData = "course";
            head.DType = SXMLEntity::EType::StartElement;
            head.SetAttribute("SUBJ", subject);
            head.SetAttribute("CRSE", course);
            head.SetAttribute("SEC", section);

            CXMLWriter write(file);
            write.WriteEntity(head);
            file<<"\n";
	    while(!read.End()){
		std::cout<<"im in"<<std::endl;
                std::vector<std::string>row;
                SXMLEntity e;
                e.DNameData = "Student";
                e.DType = SXMLEntity::EType::CompleteElement;
		read.ReadRow(row);
		for(int i = 0;i<header.size();i++){
                       std::cout<<row[i]<<std::endl;
            }

                for (int i = 0; i < header.size(); i++) {
                      e.SetAttribute(header[i], row[i]);
		}
               file<<"   ";
               write.WriteEntity(e);
	       file<<"\n";
            }
            file<<"</course>"<<std::endl;

        }
	
	if(ext == "xml"){
            CXMLReader read(fp);
            SXMLEntity courseinfo;
            read.ReadEntity(courseinfo,true);

            std::string subj = courseinfo.AttributeValue("SUBJ");
            std::string course = courseinfo.AttributeValue("CRSE");
            std::string sec = courseinfo.AttributeValue("SEC");

            std::string filename;
            filename = subj + "_" + course + "_" + sec + ".csv";

            std::ofstream myfile;

            myfile.open(filename);

            std::cout<<courseinfo.DNameData<<std::endl;

            SXMLEntity student;

            read.ReadEntity(student,true);

	    std::cout<<student.DNameData<<std::endl;

            std::vector<std::string>h;
	    std::vector<std::string>d;
            for(auto c:student.DAttributes){
		    printf(" %d \n",__LINE__);
                h.push_back(std::get<0>(c));
		d.push_back(std::get<1>(c));
		std::cout<<std::get<0>(c)<<std::endl;
                std::cout<<std::get<1>(c)<<std::endl;

            }
            CCSVWriter writer(myfile);
	    writer.WriteRow(h);
	    writer.WriteRow(d);
	    
	    
            while(!read.End()){
                SXMLEntity data;
                std::vector<std::string>info;
                read.ReadEntity(data,true);
		
                for(auto d: data.DAttributes){
                    info.push_back(std::get<1>(d));
                }
		writer.WriteRow(info);

            }
	}
	}

return 0;
}

