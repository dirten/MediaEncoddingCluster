/* 
 * File:   PropertyTree.h
 * Author: HoelscJ
 *
 * Created on 13. Oktober 2010, 12:59
 */

#ifndef PROPERTYTREE_H
#define	PROPERTYTREE_H
#include <string>
#include <list>
#include "boost/property_tree/ptree.hpp"
#include "boost/foreach.hpp"
#include "exports.h"

namespace org {
  namespace esb {
    namespace util {

      class UTIL_EXPORT PropertyTree {
      public:
        PropertyTree(std::string filename);
        PropertyTree(const PropertyTree& orig);
        virtual ~PropertyTree();

        template<typename T> std::list<T> getList(std::string name){
          using boost::property_tree::ptree;
          std::list<T> result;
          BOOST_FOREACH(ptree::value_type & v,pt.get_child(name))
            result.push_back(v.second.data());
          return result;
        }

        template<typename T> T get(std::string t){
          return pt.get<T>(t);
        }

      private:
        boost::property_tree::ptree pt;
      };
    }
  }
}

#endif	/* PROPERTYTREE_H */

