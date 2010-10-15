/* 
 * File:   PropertyTree.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Oktober 2010, 12:59
 */

#include "PropertyTree.h"
#include <boost/property_tree/xml_parser.hpp>
namespace org {
  namespace esb {
    namespace util {

      PropertyTree::PropertyTree(std::string filename) {
        boost::property_tree::read_xml(filename, pt);
        
      }

      PropertyTree::PropertyTree(const PropertyTree& orig) {
      }

      PropertyTree::~PropertyTree() {
      }
    }
  }
}
