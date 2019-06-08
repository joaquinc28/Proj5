#include "StringUtils.h"
#include <algorithm> 
#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <cstddef>
#include <vector>
#include <numeric>
#include<cmath>
#include <algorithm>


namespace StringUtils{
    
std::string Slice(const std::string &str, ssize_t start, ssize_t end){
    std::string str2;
    int count = 0;
    for (ssize_t i = 0;i < str.length();i++)
        count++;

    if(start > count){
        printf("damn");
        str2 = "";
    }
    else if (start > 0 and end == 0){
        str2 = str.substr(start);
        std::cout << str2 << std::endl;
    }

    else if (start >= 0 and end >= 0) {
        str2 = str.substr(start, end);
        std::cout << str2 << std::endl;
    }
    else if (start >= 0 and end < 0){
        std::cout << end << std::endl;
        end = end - start + str.size();
        std::cout << end << std::endl;
        str2 = str.substr(start,end);
        std::cout << str2 << std::endl;

    }
    else if (start < 0 and end >= 0){
        start = str.size() + start;
        str2 = str.substr(start, end);
        std::cout << str2 << std::endl;


    }
    else if (start < 0 and end < 0){
        printf("hey");
        end = end - start;
        start = str.size() + start;
        std::cout << end << std::endl;

        str2 = str.substr(start, end);
        std::cout << str2 << std::endl;


    }
    return str2;
}
    // Your code goes here


std::string Capitalize(const std::string &str){//Code from profesor
    std::string Ret;
    for(auto Ch : str){
        Ret += tolower(Ch);
    }
    Ret[0] = toupper(Ret[0]);
    std::cout<<Ret<<std::endl;
    return Ret;
    // Your code goes here
}

std::string Title(const std::string &str){
    std::string Ret;
    for(auto Ch : str) {
        Ret += tolower(Ch);
    }
    Ret[0] = toupper(Ret[0]);
    int i = 0;
    while(Ret[i]){
        if(isalpha(Ret[i])){
        }
        else{
            Ret[i+1] = toupper(Ret[i+1]);
        }
        i++;
    }
    std::cout<<Ret<<std::endl;
    return Ret;
    // Your code goes here
}

std::string LStrip(const std::string &str){
    std::size_t found = str.find_first_not_of(" ");
    std::cout<<found<<std::endl;
    std::string str2;
    str2 = str.substr(found,str.size());
    std::cout<<str2<<std::endl;
    return str2;
    // Your code goes here
}

std::string RStrip(const std::string &str){
    std::size_t found = str.find_last_not_of(" ");
    std::string str2;
    str2 = str.substr(0,found + 1);
    std::cout<<str2<<std::endl;
    return str2;
    // Your code goes here
}

std::string Strip(const std::string &str){
    std::string str2;
    str2 = RStrip(LStrip(str));
    std::cout<<str2<<std::endl;
    return str2;
    // Your code goes here
}

std::string Center(const std::string &str, int width, char fill){
    std::string str2;
    for(auto Ch : str) {
        str2 += Ch;
    }
    int size;
    size = width - str2.size();
    int left = size / 2;
    int right = size - left;
    int i = 0;
    while(i < left){
        str2.insert(0,1,fill);
        i++;
    }
    int j = 0;
    while(j < right){
        str2.push_back(fill);
        j++;
    }
    std::cout<<str2<<std::endl;
    return str2;    // Your code goes here
}

std::string LJust(const std::string &str, int width, char fill){
    std::string str2;
    for(auto Ch : str) {
        str2 += Ch;
    }
    int i;
    i = str2.size();
    while(i < width){
        str2.push_back(fill);
        i++;
    }

    std::cout<<str2<<std::endl;
    return str2;
}    // Your code goes here


std::string RJust(const std::string &str, int width, char fill){
    std::string str2;
    for(auto Ch : str) {
        str2 += Ch;
    }
    int i;
    i = str2.size();
    while(i < width){
        str2.insert(0,1,fill);
        i++;

    }
    std::cout<<str2<<std::endl;
    return str2;
    // Your code goes here
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep){
    std::string str2;
    for (auto Ch : str) {
        str2 += Ch;
    }


    size_t i = 0;
    while ((i = str.find(old, i)) != std::string::npos) {//https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
        str2.replace(i, old.length(), rep);
        i += rep.length();
        std::cout << str2 << std::endl;


    }
    return str2;
    // Your code goes here
}

std::vector< std::string > Split(const std::string &str, const std::string &splt){//https://stackoverflow.com/questions/20825417/splitting-string-into-vector-c
    std:: string strTempString;
    std::vector<int> splitIndices;
    std::vector<std::string> splitLine;
    std::string str2;
    for (auto Ch : str) {
        str2 += Ch;
    }

    int found = str2.find('\t');
    int found2 = str2.find('\n');
    if(found2 >= 0){
        str2.replace(found2,1," ");
    }
    if (found >= 0){
        str2.replace(found,1," ");

    }
    char split[0];
    strcpy(split,splt.c_str());
    int nLineSize = str2.size();



    // find indices
    splitIndices.push_back(-1);
    if(nLineSize!=0 && str2[0]==split[0])
    {
        splitLine.push_back("");
        splitIndices[0]=0;
        for(int i = 1; i < nLineSize; i++)
        {
            if(str2[i] == split[0])
                splitIndices.push_back(i);
        }

    }
    else{
        for(int j = 1;j< nLineSize;j++){
            if(!isalpha(str2[j]) and j != nLineSize - 1){
                splitIndices.push_back(j);
            }
        }

    }

    int j = splitIndices.size() - 1;
    int i = splitIndices[j];
    for(int i = 1; i <int(splitIndices.size()); i++)

    {
        strTempString = str2.substr(splitIndices[i-1]+1, (splitIndices[i] - (splitIndices[i-1]+1) ));
        if(!isalpha(strTempString[0])){

        }
        else{
            splitLine.push_back(strTempString);
        }
    }

    strTempString = str2.substr(i + 1);
    splitLine.push_back(strTempString);
    for(int j = 0;j<splitIndices.size();j++){
        std::cout << splitIndices[j] <<std::endl;

    }


    std::cout << splitLine[4] <<std::endl;


    return splitLine;
    // Your code goes here
}

std::string Join(const std::string &str, const std::vector< std::string > &vect){//https://stackoverflow.com/questions/5689003/how-to-implode-a-vector-of-strings-into-a-string-the-elegant-way/6334153
    std::string s;
    for (std::vector<std::string>::const_iterator ii = vect.begin(); ii != vect.end(); ++ii)
    {
        s += (*ii);
        if ( ii + 1 != vect.end() ) {
            s += str;
        }
    }
    std::cout << s<<std::endl;
    return s;
    // Your code goes here
}

std::string ExpandTabs(const std::string &str, int tabsize){
    std::string str2;
    for (auto Ch : str) {
        str2 += Ch;
    }
    std::vector<std::string>vec1;
    int i = 0;
    while(i < str.length()){
        if(str[i] == '\t'){
            long val = str2.find_first_of('\t');
            vec1.push_back(str2.substr(0,val));
            str2 = str2.substr(val+1);
            std::cout<<str2<<std::endl;
        }
        i++;
    }
    vec1.push_back(str2.substr(0));
    std::vector<long>size(4);
    long len = vec1.size() - 1;
    int j = 0;
    while(j < len) {
        if (tabsize == 0){
            size.at(j) = 0;
        }
        else if (vec1[j].length() < tabsize) {
            int val = (tabsize - vec1[j].size());
            size.at(j) = val;
        }
        else{
            int val2 = (tabsize - (vec1[j].size() - tabsize));
            size.at(j) = val2;
        }
        j = j + 1;
    }
    std::string s;
    int num = 0;
    for (std::vector<std::string>::const_iterator ii = vec1.begin(); ii != vec1.end(); ++ii)
    {
        s += (*ii);
        if ( ii + 1 != vec1.end() ) {
            int i = 0;
            if(size[num] == 0){
                while(i < tabsize){
                    s += ' ';
                    i++;
                }
            }
            else{
                while(i < size[num]){
                    s += ' ';
                    i++;
                }

            }
            num++;
        }
    }
    std::cout<<size[0]<<std::endl;
    std::cout<<s<<std::endl;
    return s;
    // Your code goes here
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase){https://rosettacode.org/wiki/Levenshtein_distance#C.2B.2B
    std::string s1;
    for (auto Ch : left) {
        if (ignorecase == true) {
            s1 += tolower(Ch);

        } else {
            s1 += Ch;
        }
    }
    std::string s2;
    for (auto Ch : right) {
        if (ignorecase == true) {
            s2 += tolower(Ch);

        } else {
            s2 += Ch;
        }
    }
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0) return n;
    if (n == 0) return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++) costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i) {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j) {
            size_t upper = costs[j + 1];
            if (*it1 == *it2) {
                costs[j + 1] = corner;
            } else {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;
    std::cout << result << std::endl;


    return result;
    // Your code goes here
}


}
