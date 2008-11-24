/* 
 * File:   Div.h
 * Author: jhoelscher
 *
 * Created on 20. November 2008, 09:53
 */
#include <Wt/WContainerWidget>
#ifndef _DIV_H
#define	_DIV_H
namespace org {
  namespace esb {
    namespace web {
      namespace wtk {
      class Div : public Wt::WContainerWidget {
      public:
        Div(const std::string& style,WContainerWidget *parent=0);
      };
      }
    }
  }
}


#endif	/* _DIV_H */

