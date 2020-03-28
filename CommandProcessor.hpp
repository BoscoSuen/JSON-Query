//
//  CommandProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 2/15/20.
//

#ifndef CommandProcessor_hpp
#define CommandProcessor_hpp

#include "Model.hpp"
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

namespace ECE141 {
  
  class CommandProcessor {
  public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor &aCommand);
    CommandProcessor& operator=(const CommandProcessor &aCommand);

    vector<string> split_str(string str, string pattern);
    
    bool run(std::string &aCommand, Model &aModel, std::ostream &anOutput);
    
  };

}

#endif /* CommandProcessor_hpp */
