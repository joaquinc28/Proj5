CXX = g++
TESTLDFLAGS = -lgtest -lgtest_main  -lpthread
TESTLDFLAGS += -L '/opt/local/lib'
CURDIR = $(shell pwd)
LIBCSVDIR = libcsv-3.0.3
LIBDIR = ./lib
LIBCSV_NAME = libcsv.a
INCLUDEDIR = ./include
SRCDIR = ./src
OBJDIR = ./obj
CSV_READER_OBJ = $(OBJDIR)/CSVReader.o
CSV_WRITER_OBJ = $(OBJDIR)/CSVWriter.o
CSV_TEST_OBJ= $(OBJDIR)/testcsv.o

MAP_ROUTER_OBJ = $(OBJDIR)/MapRouter.o
TEST_ROUTER_OBJ = $(OBJDIR)/testrouter.o

STRCPP = $(SRCDIR)/StringUtils.cpp
STRH = $(INCLUDEDIR)/StringUtils.h
STRO = $(OBJDIR)/str.o

XML_READER_OBJ = $(OBJDIR)/XMLReader.o
XML_WRITER_OBJ = $(OBJDIR)/XMLWriter.o
XML_TEST_OBJ = $(OBJDIR)/testxml.o
INCLUDE = -I src/
INCLUDE += -I include/
CXXFLAGS = -std=c++14 $(INCLUDE)
EXPAT = -lexpat

all:directories $(LIBDIR)/$(LIBCSV_NAME) test bin/yee

test: $(OBJDIR)/ROUTERTEST 
	$(OBJDIR)/ROUTERTEST

bin/yee:$(OBJDIR)/main.o $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(EXPAT) $(STRO) $(MAP_ROUTER_OBJ)
	$(CXX) $(OBJDIR)/main.o $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(EXPAT) $(STRO) $(MAP_ROUTER_OBJ) $(CXXFLAGS) -o bin/yee

#$(OBJDIR)/CSVTEST: $(CSV_TEST_OBJ) $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(STRO) $(XML_TEST_OBJ) $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(EXPAT)
#	$(CXX) $(CSV_TEST_OBJ) $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(STRO) $(XML_TEST_OBJ) $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(EXPAT) $(CXXFLAGS) -o $(OBJDIR)/CSVTEST $(TESTLDFLAGS)

$(OBJDIR)/ROUTERTEST: $(TEST_ROUTER_OBJ) $(MAP_ROUTER_OBJ) $(XML_READER_OBJ) $(CSV_READER_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(EXPAT)
	$(CXX) $(TEST_ROUTER_OBJ) $(MAP_ROUTER_OBJ) $(XML_READER_OBJ) $(CSV_READER_OBJ) $(LIBDIR)/$(LIBCSV_NAME) $(CXXFLAGS) $(EXPAT) -o $(OBJDIR)/ROUTERTEST $(TESTLDFLAGS)

#$(OBJDIR)/XMLTEST: $(XML_TEST_OBJ) $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(EXPAT) 
#	$(CXX) -std=c++14 $(XML_TEST_OBJ) $(XML_READER_OBJ) $(XML_WRITER_OBJ) -lexpat -o $(OBJDIR)/XMLTEST $(TESTLDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCLUDEDIR)/CSVReader.h $(INCLUDEDIR)/CSVWriter.h $(INCLUDEDIR)/XMLReader.h $(INCLUDEDIR)/XMLWriter.h $(INCLUDEDIR)/MapRouter.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/main.cpp -c -o $(OBJDIR)/main.o  

$(STRO): $(STRCPP) $(STRH) 
	$(CXX) $(STRCPP) $(CXXFLAGS) -c  -o $(STRO)


$(MAP_ROUTER_OBJ): $(SRCDIR)/MapRouter.cpp $(INCLUDEDIR)/MapRouter.h $(INCLUDEDIR)/XMLReader.h 
	$(CXX) $(SRCDIR)/MapRouter.cpp $(CXXFLAGS) -c -o $(MAP_ROUTER_OBJ)

$(TEST_ROUTER_OBJ): $(SRCDIR)/testrouter.cpp $(INCLUDEDIR)/MapRouter.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/testrouter.cpp -c -o $(TEST_ROUTER_OBJ) $(TESTLDFLAGS)

$(CSV_READER_OBJ): $(SRCDIR)/CSVReader.cpp $(INCLUDEDIR)/CSVReader.h
	$(CXX) $(SRCDIR)/CSVReader.cpp $(CXXFLAGS) -c  -o $(CSV_READER_OBJ)

$(CSV_WRITER_OBJ): $(SRCDIR)/CSVWriter.cpp $(INCLUDEDIR)/CSVWriter.h $(STRH)
	$(CXX) $(SRCDIR)/CSVWriter.cpp $(CXXFLAGS) -c  -o $(CSV_WRITER_OBJ)

$(CSV_TEST_OBJ):$(SRCDIR)/testcsv.cpp $(INCLUDEDIR)/CSVReader.h $(INCLUDEDIR)/CSVWriter.h 
	$(CXX) $(CXXFLAGS) $(SRCDIR)/testcsv.cpp -c -o $(CSV_TEST_OBJ) $(TESTLDFLAGS)

$(XML_READER_OBJ): $(SRCDIR)/XMLReader.cpp $(INCLUDEDIR)/XMLReader.h
	 $(CXX) $(SRCDIR)/XMLReader.cpp $(CXXFLAGS) -c -o $(XML_READER_OBJ)

$(XML_WRITER_OBJ): $(SRCDIR)/XMLWriter.cpp $(INCLUDEDIR)/XMLWriter.h
	$(CXX) $(SRCDIR)/XMLWriter.cpp $(CXXFLAGS) -c  -o $(XML_WRITER_OBJ)

$(XML_TEST_OBJ): $(SRCDIR)/testxml.cpp $(INCLUDEDIR)/XMLReader.h $(INCLUDEDIR)/XMLWriter.h
	$(CXX) $(CXXFLAGS) $(SRCDIR)/testxml.cpp -c -lexpat  -o $(XML_TEST_OBJ) $(TESTLDFLAGS)

$(LIBCSVDIR)/makefile: 
	cd $(LIBCSVDIR); chmod 755 configure; ./configure --prefix=$(CURDIR); cd ..

$(LIBCSVDIR)/libcsv.la:$(LIBCSVDIR)/makefile
	cd $(LIBCSVDIR); make; cd ..
$(LIBDIR)/$(LIBCSV_NAME): $(LIBCSVDIR)/libcsv.la
	cd $(LIBCSVDIR); make install; cd ..

directories: obj/ bin/

bin/:
	mkdir bin

obj/:
	mkdir obj
clean:
	rm -f lib/*
	rm -f obj/*
	rm -f include/csv.h
	cd $(LIBCSVDIR); make clean; cd ..
	rm -f $(LIBCSVDIR)/makefile
	rm -f *.o core

