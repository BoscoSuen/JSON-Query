//
//  Model.cpp
//  Assignment4
//
//  Created by rick gessner on 2/15/20.
//

#include "Model.hpp"

namespace ECE141 {

  ModelQuery::ModelQuery(Model &aModel) : model(aModel) {
    // the root must be the whole collection of the child level
    PairNode* beginNode = new PairNode("",this->model.root,Element::pair);
    curCollection.push_back(dynamic_cast<ModelNode*>(beginNode));
  }

  ModelQuery& ModelQuery::all(const std::string &aTarget) {
    vector<ModelNode*> childCollection;
    for (int i = 0; i < curCollection.size();++i) {
      if (curCollection[i]->getType() == Element::pair) {
          childCollection.push_back(dynamic_cast<PairNode*>(curCollection[i])->val);
      }
    }
    // the childCollection is a set of mapNode
    int curSize = childCollection.size();
    for (int i = 0; i < curSize; ++i) {
      if (childCollection[i]->getType() == Element::object) {
        MapNode* cur = dynamic_cast<MapNode*>(childCollection[i]);
        for (int j = 0; j < cur->nodePairs.size();++j) {
          PairNode* pair = dynamic_cast<PairNode*>(cur->nodePairs[j]);
          if (aTarget == "*" || pair->key == aTarget) {
            curCollection.push_back(dynamic_cast<ModelNode*>(pair));
          }
        }
      } else if (childCollection[i]->getType() == Element::array) {
        ListNode* cur = dynamic_cast<ListNode*>(childCollection[i]);
        for (int j = 0; j < cur->nodeList.size(); ++j) {
          if (cur->nodeList[j]->getType()==Element::object) {
            MapNode* child = dynamic_cast<MapNode*>(cur->nodeList[j]);
            for (int k = 0; k < child->nodePairs.size(); ++k) {
              if (aTarget == "*" || dynamic_cast<PairNode*>(child->nodePairs[k])->key == aTarget) {
                curCollection.push_back(dynamic_cast<ModelNode*>(child->nodePairs[k]));
              }
            }
          } else if (cur->nodeList[j]->getType() == Element::quoted) {
            curCollection.push_back(cur->nodeList[j]);
          } else if (cur->nodeList[j]->getType() == Element::constant) {
            curCollection.push_back(cur->nodeList[j]);
          }
        }
      }
    }
    // remove the previous query result
    curCollection.erase(curCollection.begin(),curCollection.begin()+curSize <= curCollection.end() ? curCollection.begin()+curSize : curCollection.end());
    return *this;
  }

  ModelQuery& ModelQuery::first(const std::string &aTarget) {
    // keep the first node in the previous result and remove the nodes after that.
    int queryResIdx;
    for (int i = 0; i < curCollection.size();++i) {
      if (aTarget == "*" || dynamic_cast<PairNode*>(curCollection[i])->key == aTarget) {
        queryResIdx = i;
        break;
      }
    }
    curCollection.erase(curCollection.begin(),curCollection.begin()+queryResIdx <= curCollection.end() ? curCollection.begin()+queryResIdx : curCollection.end());
    if (curCollection.begin() + 1 <= curCollection.end()) {
      curCollection.erase(curCollection.begin() + 1,curCollection.end());
    }
    return *this;
  }

  ModelQuery& ModelQuery::last(const std::string &aTarget) {
    int queryResIdx;
    for (int i = curCollection.size() - 1; i >= 0; --i) {
      if (aTarget == "*" || dynamic_cast<PairNode*>(curCollection[i])->key == aTarget) {
        queryResIdx = i;
        break;
      }
    }
    curCollection.erase(curCollection.begin(),curCollection.begin()+queryResIdx <= curCollection.end() ? curCollection.begin()+queryResIdx : curCollection.end());
    if (curCollection.begin() + 1 <= curCollection.end()) {
      curCollection.erase(curCollection.begin() + 1,curCollection.end());
    }
    return *this;
  }

  ModelQuery& ModelQuery::nth(const std::string &aTarget, size_t anIndex) {
    // the first element position is one
    int queryResIdx;
    int count = 0;
    for (int i = 0; i < curCollection.size(); ++i) {
      if (aTarget == "*" || dynamic_cast<PairNode*>(curCollection[i])->key == aTarget) {
        count++;
        if (count == anIndex) {
          queryResIdx = i;
          break;
        }
      }
    }
    curCollection.erase(curCollection.begin(),curCollection.begin()+queryResIdx <= curCollection.end() ? curCollection.begin()+queryResIdx : curCollection.end());
    if (curCollection.begin() + queryResIdx + 1 <= curCollection.end()) {
      curCollection.erase(curCollection.begin() + queryResIdx + 1,curCollection.end());
    }
    return *this;
  }

    ModelQuery& ModelQuery::nth(size_t anIndex) {
      // the first element position is one
      int queryResIdx;
      int count = 0;
      for (int i = 0; i < curCollection.size(); ++i) {
          count++;
          if (count == anIndex) {
            queryResIdx = i;
            break;
          }
      }
      curCollection.erase(curCollection.begin(),curCollection.begin()+queryResIdx <= curCollection.end() ? curCollection.begin()+queryResIdx : curCollection.end());
      if (curCollection.begin() + queryResIdx + 1 <= curCollection.end()) {
        curCollection.erase(curCollection.begin() + queryResIdx,curCollection.end());
      }
      return *this;
    }



  ModelQuery& ModelQuery::before(const std::string &aTarget) {
    int queryResIdx;
    for (int i = 0; i < curCollection.size();++i) {
      if (aTarget == "*" || dynamic_cast<PairNode*>(curCollection[i])->key == aTarget) {
        queryResIdx = i;
        break;
      }
    }
    // remove all the node that are not "before", including the current matched index
    if (curCollection.begin() + queryResIdx <= curCollection.end()) {
      curCollection.erase(curCollection.begin() + queryResIdx,curCollection.end());
    }
    reverse(curCollection.begin(),curCollection.end());
    return *this;
  }

  ModelQuery& ModelQuery::after(const std::string &aTarget) {
    int queryResIdx;
    for (int i = 0; i < curCollection.size();++i) {
      if (aTarget == "*" || dynamic_cast<PairNode*>(curCollection[i])->key == aTarget) {
        queryResIdx = i;
        break;
      }
    }
    // remove all the node that are not "after", including the current matched index
    curCollection.erase(curCollection.begin(),curCollection.begin() + queryResIdx + 1 <= curCollection.end() ? curCollection.begin() + queryResIdx + 1 : curCollection.end());
    return *this;
  }

  ModelQuery& ModelQuery::within(const std::string &aTarget) {
    vector<string> strs = split_str(aTarget,".");
    for (int i = 0; i < strs.size(); ++i) {
      this->all(strs[i]);
    }
    this->all("*");
    return *this;
  }

  vector<string> ModelQuery::split_str(string str, string pattern) {
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

  std::string ModelQuery::valueOf() {
    return std::string("hi");
  }

  std::string ModelQuery::get() {
    vector<ModelNode*> resList =  curCollection;
    int level = 0;
    string res = "";
    for (int i = 0; i < resList.size(); ++i) {
      printNode(res,resList[i],level);
    }
    return res;
  }

  std::string ModelQuery::printNode(string& res, ModelNode* cur, int level) {
    if (!cur) return "";
    if (cur->getType() == Element::object) {
      res += "{\n";
      for (int i = 0; i < dynamic_cast<MapNode*>(cur)->nodePairs.size();++i) {
        PairNode* curPair = dynamic_cast<PairNode*>(dynamic_cast<MapNode*>(cur)->nodePairs[i]);
        res += curPair->key + " : ";
        printNode(res,curPair->val,level+1);
        if (i < dynamic_cast<MapNode*>(cur)->nodePairs.size() - 1) res += ",";
        res += "\n";
      }
      res += "}";
    } else if (cur->getType() == Element::array) {
      res += "[ ";
      for (int i = 0; i < dynamic_cast<ListNode*>(cur)->nodeList.size();++i) {
        printNode(res, dynamic_cast<ListNode*>(cur)->nodeList[i],level);
        res += ", ";
      }
      res += " ]";
    } else if (cur->getType() == Element::quoted) {
      res += dynamic_cast<QuotedNode*>(cur)->str;
    } else if (cur->getType() == Element::constant) {
      res += dynamic_cast<ValNode<string>*>(cur)->val;
    } else if (cur->getType() == Element::pair) {
      PairNode* curPair = dynamic_cast<PairNode*>(cur);
      res += curPair->key + " : ";
      printNode(res,curPair->val,level+1);
      res += "\n";
    }
    return res;
  }

  
  std::string ModelQuery::get(const std::string &aKey) {
    for (int i = 0; i < curCollection.size(); ++i) {
      PairNode* curNode = dynamic_cast<PairNode*>(curCollection[i]);
      if (curNode->key == aKey) {
        if (curNode->val->getType() == Element::quoted) {
          stringstream ss;
          ss << (dynamic_cast<QuotedNode*>(curNode->val))->str;
          return ss.str();
        } else if (curNode->val->getType() == Element::constant) {
          stringstream ss;
          ss << dynamic_cast<ValNode<string>*>(curNode->val)->val;
          return ss.str();
        } else {
          return "Invalid Query Input!";
        }
      }
    }
    return std::string(""); //retrieve the key from a value if query matches, otherwise return ""
  }

  size_t ModelQuery::count() {
    return this->curCollection.size();
  }

  double ModelQuery::avg(const std::string &aField) {
    double sum = 0;
    for (int i = 0; i < curCollection.size(); ++i) {
      string item = dynamic_cast<ValNode<string>*>(curCollection[i])->val;
      sum += stof(item);
    }
    return sum / curCollection.size();
  }

  double ModelQuery::sum(const std::string &aField) {
    double sum = 0;
    for (int i = 0; i < curCollection.size(); ++i) {
      string item = dynamic_cast<ValNode<string>*>(curCollection[i])->val;
      sum += stof(item);
    }
    return sum;
  }

  //---------------------- implement the model class... --------------------


  Model::Model() {
  }

  Model::Model(const Model &aCommand) {
    this->modelStack = aCommand.modelStack;
  }
  
  Model& Model::operator=(const Model &aCommand) {
    this->modelStack = aCommand.modelStack;
    return *this;
  }
    
  ModelQuery Model::query() {
    ModelQuery theQuery(*this);
    return theQuery;
  }

  bool Model::addKeyValuePair(const std::string &aKey, const std::string &aValue) {
    std::cout <<  "\t|" << aKey <<":" << aValue << "| \n";
    MapNode* cur = dynamic_cast<MapNode *>(modelStack.back());
    ModelNode* newNode;
    if (aValue[0] >= '0' && aValue[0] <= '9') {
      if (aValue.find('.') == -1) {     // int
        newNode = new ValNode<string>(aValue);
      } else {    // float
        newNode = new ValNode<string>(aValue);
      }
    } else if (aValue[0] == 'T' || aValue[0] == 't') {
      newNode = new ValNode<string>("true");
    } else if (aValue[0] == 'F' || aValue[0] == 'f') {
      newNode = new ValNode<string>("false");
    } else {
      newNode = new QuotedNode(aValue);
    }
    ModelNode* pairNode = new PairNode(aKey,newNode,Element::pair);
    cur->nodePairs.push_back(pairNode);
    return true;
  }
    
  bool Model::addItem(const std::string &aValue) {
    std::cout <<  "\t" << aValue << " \n";
    ListNode* cur = dynamic_cast<ListNode *>(modelStack.back());
    ModelNode* newNode;
    if (aValue[0] >= '0' && aValue[0] <= '9') {
      if (aValue.find('.') == -1) {     // int
        newNode = new ValNode<string>(aValue);
      } else {    // float
        newNode = new ValNode<string>(aValue);
      }
    } else if (aValue[0] == 'T' || aValue[0] == 't') {
      newNode = new ValNode<string>("true");
    } else if (aValue[0] == 'F' || aValue[0] == 'f') {
      newNode = new ValNode<string>("false");
    } else {
      newNode = new QuotedNode(aValue);
    }
    cur->nodeList.push_back(newNode);
    return true;
  }

  bool Model::openContainer(const std::string &aContainerName, ECE141::Element aType){
    //open a new unnamed container in our model...
    if (aType == Element::array) {
      std::cout << aContainerName << " [ \n";
    } else {
      std::cout << aContainerName << " { \n";
    }
    if (aType == Element::object) {
      ModelNode* newNode = new MapNode;
      if (modelStack.size() == 0) {
        root = newNode;
      }
      modelStack.push_back(newNode);
    } else if (aType == Element::array) {     // [
      ModelNode* newNode = new ListNode;
      modelStack.push_back(newNode);
    }
    return true;
  }

  bool Model::closeContainer(const std::string &aContainerName, ECE141::Element aType){
    if (aType == Element::array) {
      std::cout  << " ] \n";
    } else {
      std::cout  << " } \n";
    }
    ModelNode* nested = dynamic_cast<ModelNode*>(modelStack.back());
    modelStack.pop_back();
    if (modelStack.size() == 0) {
      return true;
    }
    ModelNode* parent = modelStack.back();
    if (parent->getType() == Element::object) {
      ModelNode* newNode = new PairNode(aContainerName, nested, Element::pair);
      dynamic_cast<MapNode*>(parent)->nodePairs.push_back(newNode);
    } else if (parent->getType() == Element::array) {
      dynamic_cast<ListNode *>(parent)->nodeList.push_back(nested);
    }
    return true;
  }
}