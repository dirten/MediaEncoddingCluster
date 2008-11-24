/* 
 * File:   ContentBox.h
 * Author: jhoelscher
 *
 * Created on 20. November 2008, 09:49
 */

#include <Wt/WWebWidget>
#include "Div.h"
#ifndef _CONTENTBOX_H
#define	_CONTENTBOX_H

namespace org {
  namespace esb {
    namespace web {
      namespace wtk {
        class ContentBox : public Div {
        public:
          ContentBox(const char * name = "", WContainerWidget *parent = 0);
          void setHeaderText(std::string text);
          void setContent(Wt::WWidget * widget);
          void resize(const Wt::WLength &width, const Wt::WLength &height);
        private:
          Div * header;
          Div * m;
          Div * cont;
        };
      }
    }
  }
}


#endif	/* _CONTENTBOX_H */

