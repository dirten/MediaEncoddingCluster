/* 
 * File:   GroupBox.h
 * Author: HoelscJ
 *
 * Created on 13. Oktober 2010, 16:43
 */

#ifndef GROUPBOX_H
#define	GROUPBOX_H
#include "Wt/WGroupBox"
namespace org {
  namespace esb {
    namespace web {

      class GroupBox: public Wt::WGroupBox {
      public:
        GroupBox();
        virtual ~GroupBox();
        void setHidden(bool);
        std::list<std::string> getChildrenObjectNames();
      private:

      };
    }
  }
}

#endif	/* GROUPBOX_H */

