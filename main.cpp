//
//  main.cpp
//  Assignment4
//
//  Created by rick gessner on 2/12/20.
//

#include <iostream>
#include <fstream>
#include "Testing.hpp"

std::ostream& getOutput() {return std::cout;}

std::string getJSONPath() {return std::string("/Users/suen/Desktop/ECE141/Assignments/wi20-assignment4-Yukinichi/test.json");}  //e.g. test.json
std::string getCmdsPath() {return std::string("/Users/suen/Desktop/ECE141/Assignments/wi20-assignment4-Yukinichi/test.txt");} //e.g. text.txt

int main(int argc, const char * argv[]) {

  std::fstream theJSONFile(getJSONPath());
  std::fstream theCommands(getCmdsPath());

// the Test class will construct a JSONParser and a Model class for you...

  ECE141::Testing::runTests(theJSONFile, theCommands, getOutput());

  return 0;
}