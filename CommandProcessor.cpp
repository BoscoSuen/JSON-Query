//
//  CommandProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/15/20.
//

#include "CommandProcessor.hpp"


namespace ECE141 {

  CommandProcessor::CommandProcessor() {}
  CommandProcessor::CommandProcessor(const CommandProcessor &aCommand) {}
  CommandProcessor& CommandProcessor::operator=(const CommandProcessor &aCommand) {
  return *this;
}

  bool CommandProcessor::run(std::string &aCommand, Model &aModel, std::ostream &anOutput) {
    //STUDENT -- Run your query and send your output to the output stream ...
    anOutput << "current query is: " << aCommand << "\n";
    anOutput << "The query result is: " << endl;
    vector<string> strs = split_str(aCommand, " ");
    ModelQuery query = aModel.query();
    int idx = 0;
    while (idx < strs.size()) {
      if (strs[idx].find("all") != -1) {
        int left = strs[idx].find("(");
        int right = strs[idx].find(")");
        query.all(strs[idx].substr(left + 1, right - left - 1));
        idx++;
      } else if (strs[idx].find("within") != -1) {
        query.within(strs[idx+1].substr(1,strs[idx+1].length()-2));
        idx += 2;
      } else if (strs[idx].find("first") != -1) {
        query.first(strs[idx+1].substr(1,strs[idx+1].length()-2));
        idx += 2;
      } else if (strs[idx].find("before") != -1) {
        query.before(strs[idx+1].substr(1,strs[idx+1].length()-2));
        idx += 2;
      } else if (strs[idx].find("after") != -1) {
        query.after(strs[idx+1].substr(1,strs[idx+1].length()-2));
        idx += 2;
      } else if (strs[idx].find("last") != -1) {
        query.last(strs[idx + 1].substr(1, strs[idx + 1].length() - 2));
        idx += 2;
      } else if (strs[idx].find("nth") != -1) {
        int left = strs[idx].find("(");
        int right = strs[idx].find(")");
        string str = strs[idx].substr(left + 1,right - left - 1);
        if (str.find(",") == -1) {
          stringstream ss(str);
          size_t anIndex;
          ss >> anIndex;
          query.nth(anIndex);
        } else {
          vector<string> paras = split_str(str,",");
          stringstream ss(paras[1]);
          size_t anIndex;
          ss >> anIndex;
          query.nth(paras[0],anIndex);
        }
        idx++;
      } else if (strs[idx].find("count") != -1) {
        anOutput << query.count() << endl;
        idx++;
      } else if (strs[idx].find("sum") != -1) {
        int left = strs[idx].find("(");
        int right = strs[idx].find(")");
        anOutput << query.sum(strs[idx].substr(left + 1, right - left - 1)) << endl;
        idx++;
      } else if (strs[idx].find("avg") != -1) {
        int left = strs[idx].find("(");
        int right = strs[idx].find(")");
        anOutput << query.avg(strs[idx].substr(left + 1, right - left - 1)) << endl;
        idx++;
      } else if (strs[idx].find("get") != -1) {
        int left = strs[idx].find("(");
        int right = strs[idx].find(")");
        string target = strs[idx].substr(left + 1, right - left - 1);
        if (target.length() == 0) anOutput << query.get() << endl;
        else anOutput << query.get(target.substr(1,target.length()-2)) << endl;
        idx++;
      } else {
        anOutput << "Invalid Query Input!";
        idx++;
      }
    }
    return true;
  }
  vector<string> CommandProcessor::split_str(string str, string pattern) {
    string::size_type pos;
    vector<string> res;
    str += pattern;
    size_t size = str.size();
    for(size_t i = 0; i < size; i++) {
      pos = str.find(pattern, i);
      if (pos < size) {
        string s = str.substr(i, pos - i);
        res.push_back(s);
        i = pos + pattern.size() - 1;
      }
    }
    return res;
  }
}
