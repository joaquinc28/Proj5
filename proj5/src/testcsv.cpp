#include "CSVReader.h"
#include "CSVWriter.h"
#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include <sstream>

TEST(CSVTest, CSVReaderTest){
	std::stringstream in(" 1,2 , 3        ,4,5\x0d\x0a");
	CCSVReader Reader(in);
	std::vector<std::string> row;
	EXPECT_TRUE(Reader.ReadRow(row));
	EXPECT_EQ(row.size(), 5);
	if(row.size() >= 5){
		EXPECT_EQ(row[0],"1");
		EXPECT_EQ(row[1],"2");
		EXPECT_EQ(row[2],"3");
		EXPECT_EQ(row[3],"4");
		EXPECT_EQ(row[4],"5");
	}
	EXPECT_FALSE(Reader.ReadRow(row));

	std::stringstream in2(",,,,,\x0a");
	CCSVReader Reader2(in2);
	std::vector<std::string> row2;
	EXPECT_TRUE(Reader2.ReadRow(row2));
	EXPECT_EQ(row2.size(), 6);
	if(row2.size() >= 6){
		EXPECT_EQ(row2[0],"");
		EXPECT_EQ(row2[1],"");
		EXPECT_EQ(row2[2],"");
		EXPECT_EQ(row2[3],"");
		EXPECT_EQ(row2[4],"");
		EXPECT_EQ(row2[5],"");
	}
	EXPECT_FALSE(Reader2.ReadRow(row2));

	/*std::stringstream in3("\",\",\",\",\"\"");
	CCSVReader Reader3(in3);
	std::vector<std::string> row3;
	EXPECT_TRUE(Reader3.ReadRow(row3));
	EXPECT_EQ(row3.size(), 5);
	if(row3.size() >= 5){
		EXPECT_EQ(row3[0],"\"");
		EXPECT_EQ(row3[1],"\"");
		EXPECT_EQ(row3[2],"\"");
		EXPECT_EQ(row3[3],"\"");
		EXPECT_EQ(row3[4],"\"\"");
	}
	EXPECT_FALSE(Reader3.ReadRow(row3));

	std::stringstream in4("\"I call our world Flatland,\x0a"
                       "not because we call it so,\x0a"
                       "but to make its nature clearer\x0a"
                       "to you, my happy readers,\x0a"
                       "who are privileged to live in Space.\"");
	CCSVReader Reader4(in4);
	std::vector<std::string> row4;
	EXPECT_TRUE(Reader4.ReadRow(row4));
	EXPECT_EQ(row4.size(), 1);
	if(row4.size() >= 1){
		EXPECT_EQ(row4[0],"\"I call our world Flatland,\x0a"
	                       "not because we call it so,\x0a"
	                       "but to make its nature clearer\x0a"
	                       "to you, my happy readers,\x0a"
	                       "who are privileged to live in Space.\"");

	}
	EXPECT_FALSE(Reader4.ReadRow(row4));

	std::stringstream in5("\"\"\"a,b\"\"\",,\" \"\"\"\" \",\"\"\"\"\" \",\" \"\"\"\"\",\"\"\"\"\"\"");
	CCSVReader Reader5(in5);
	std::vector<std::string> row5;
	EXPECT_TRUE(Reader5.ReadRow(row5));
	EXPECT_EQ(row5.size(), 7);
	if(row5.size() >= 7){
		EXPECT_EQ(row5[0],"\"\"\"a");
		EXPECT_EQ(row5[1],"b\"\"\"");
		EXPECT_EQ(row5[2],"");
		EXPECT_EQ(row5[3],"\"\"\"\"\"\"");
		EXPECT_EQ(row5[4],"\"\"\"\"\"\"");
		EXPECT_EQ(row5[5],"\"\"\"\"\"\"");
		EXPECT_EQ(row5[6],"\"\"\"\"\"\"");
	}
	EXPECT_FALSE(Reader5.ReadRow(row5));
*/
	std::stringstream in6("1,2 ,3,4\x0a");
	CCSVReader Reader6(in6);
	std::vector<std::string> row6;
	EXPECT_TRUE(Reader6.ReadRow(row6));
	EXPECT_EQ(row6.size(), 4);
	if(row6.size() >= 4){
		EXPECT_EQ(row6[0],"1");
		EXPECT_EQ(row6[1],"2");
		EXPECT_EQ(row6[2],"3");
		EXPECT_EQ(row6[3],"4");
	}
	EXPECT_FALSE(Reader6.ReadRow(row6));

}

TEST(CSVWriter, BasicTest) {

	std::stringstream out;
	CCSVWriter Writer(out);
	std::vector<std::string> row{"1", "2", "3", "4", "5"};

	EXPECT_TRUE(Writer.WriteRow(row));
	EXPECT_EQ(out.str(),"\"1\",\"2\",\"3\",\"4\",\"5\"\n");

	std::stringstream out2;
	CCSVWriter Writer2(out2);
	std::vector<std::string> row2{"", "", "", "", "", ""};

	EXPECT_TRUE(Writer2.WriteRow(row2));
	EXPECT_EQ(out2.str(),"\"\",\"\",\"\",\"\",\"\",\"\"\n");

	std::stringstream out3;
	CCSVWriter Writer3(out3);
	std::vector<std::string> row3{"\"", "\"", "\"", "\"", "\"\""};

	EXPECT_TRUE(Writer3.WriteRow(row3));
	EXPECT_EQ(out3.str(),"\"\"\"\",\"\"\"\",\"\"\"\",\"\"\"\",\"\"\"\"\"\n");


}

TEST(XMLTest,XMLReader){
        std::stringstream str("<student SEAT=\"1\" SID= \"911\" SURNAME=\"Stein\">");
        CXMLReader x(str);
        SXMLEntity e;

        x.ReadEntity(e);

        EXPECT_TRUE(e.AttributeExists("SEAT"));
	EXPECT_EQ(e.AttributeValue("SEAT"),"1");
        EXPECT_EQ(e.AttributeValue("SID"),"911");
        EXPECT_EQ(e.AttributeValue("SURNAME"),"Stein");

	std::stringstream a;
	CXMLWriter write(a);
	SXMLEntity ent;

	ent.DType = SXMLEntity::EType::StartElement;
	ent.DNameData = "Harman";
	ent.SetAttribute("Seat", "1");
	write.WriteEntity(ent);
	EXPECT_EQ(a.str(), "<Harman Seat=\"1\">");

}
