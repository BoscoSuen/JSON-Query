//
//  Model.hpp
//  Assignment4
//
//  Created by rick gessner on 2/15/20.
//

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>
#include <string>
#include "JSONParser.hpp"
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <sstream>
#include <algorithm>

using namespace std;

namespace ECE141 {
    
  class ModelQuery; //forward declare

  //STUDENT: Your Model is built from a bunch of these...

  class ModelNode {
  public:
    //sometimes a node holds a simple value.
    //sometimes a node is named
    //sometimes a node holds a collection of elements (or even other nodes)
    //would subclasses help or hurt here?

    virtual Element getType()=0;

  protected:
      Element type;
  };

  class MapNode : public ModelNode {      // store the key-value pair Node
  public:
      Element getType() {
        return Element::object;
      }

      vector<ModelNode*> nodePairs;
  protected:
      Element type=Element::object;
  };

  class PairNode : public ModelNode {
  public:
      Element getType() {
        return Element::pair;
      }
      string key;
      ModelNode* val;

      PairNode(const string &key, ModelNode *val, Element type) : key(key), val(val), type(type) {}

  protected:
      Element type=Element::pair;
  };

  class ListNode : public ModelNode {
  public:
      Element getType() {
        return Element::array;
      }

      vector<ModelNode*> nodeList;
  protected:
      Element type=Element::array;
  };

  class QuotedNode : public ModelNode {
  public:
      Element getType() {
        return Element::quoted;
      }

      QuotedNode(const string &str) : str(str) {}

      const string &getStr() const {
        return str;
      }

      string str;
  private:
      Element type=Element::quoted;
  };

  template<typename T>
  class ValNode : public ModelNode {
  public:
      Element getType() {
        return Element::constant;
      }

      ValNode(T val) : val(val) {}

      T val;
  protected:
      Element type=Element::constant;
  };



  class Model : public JSONListener {
   public:

     Model();
     Model(const Model &aCommand);
     Model& operator=(const Model &aCommand);

     ModelQuery query();

     ModelNode* root;

  protected:

      virtual bool addKeyValuePair(const std::string &aKey, const std::string &aValue);
      virtual bool addItem(const std::string &aValue);
      virtual bool openContainer(const std::string &aKey, Element aType);
      virtual bool closeContainer(const std::string &aKey, Element aType);

      // STUDENT: Your model will contain a collection of ModelNode*'s
      //          Choose your container(s) wisely

      deque<ModelNode*> modelStack;
  };

    //this is what gets returned when you query a model for a list of elements
  class ModelCollection {
  public:
     //STUDENT: choose an STL container to hold ModelNode* elements from your Model after a query...
     vector<ModelNode*> modelList;
  };

  class ModelQuery {
  public:
    
    ModelQuery& all(const std::string &aTarget);
    ModelQuery& first(const std::string &aTarget);
    ModelQuery& last(const std::string &aTarget);
    ModelQuery& nth(const std::string &aTarget, size_t anIndex);    // split as ","
    ModelQuery& nth(size_t anIndex);    // split as ","

    ModelQuery& before(const std::string &aTarget);
    ModelQuery& after(const std::string &aTarget);
    ModelQuery& within(const std::string &aTarget);

    string valueOf();
    size_t            count();
    double            sum(const std::string &aField);
    double            avg(const std::string &aField);
    std::string       get();
    std::string       get(const std::string &aKey); //return value or empty string

    ModelQuery(Model &aModel);

    vector<string>    split_str(string str, string pattern);
    std::string       printNode(string& res,ModelNode*, int level);

    friend class Model;

    vector<ModelNode*> curCollection;
    Model &model;
  };
}

#endif /* Model_hpp */
