/* 
 * File:   NodeListPanel.h
 * Author: HoelscJ
 *
 * Created on 29. September 2010, 12:27
 */

#ifndef NODELISTPANEL_H
#define	NODELISTPANEL_H
#include "Wt/Ext/Container"
namespace org {
  namespace esb {
    namespace web {

      class NodeListPanel: public Wt::Ext::Container {
      public:
        NodeListPanel();
        NodeListPanel(const NodeListPanel& orig);
        virtual ~NodeListPanel();
      private:

      };
    }
  }
}

#endif	/* NODELISTPANEL_H */

