/* 
 * File:   KeyValueModel.h
 * Author: HoelscJ
 *
 * Created on 8. Oktober 2010, 13:10
 */

#ifndef KEYVALUEMODEL_H
#define	KEYVALUEMODEL_H
#include "Wt/WStandardItemModel"
namespace org {
  namespace esb {
    namespace web {

      class KeyValueModel:public Wt::WStandardItemModel {
      public:
        KeyValueModel();
        virtual ~KeyValueModel();
        void addModelData(std::string key, std::string val);
      private:

      };
    }
  }
}

#endif	/* KEYVALUEMODEL_H */

