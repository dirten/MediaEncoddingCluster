/* 
 * File:   KeyValueModel.cpp
 * Author: HoelscJ
 * 
 * Created on 8. Oktober 2010, 13:10
 */

#include "KeyValueModel.h"
namespace org {
  namespace esb {
    namespace web {

      KeyValueModel::KeyValueModel() : Wt::WStandardItemModel() {
        insertColumns(0, 2);
      }

      KeyValueModel::~KeyValueModel() {
      }

      void KeyValueModel::addModelData(std::string key, std::string val) {
        insertRow(rowCount());
        setData(rowCount() - 1, 0, key);
        setData(rowCount() - 1, 1, val);
      }

    }
  }
}
