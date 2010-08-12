/* 
 * File:   TreeMainMenu.h
 * Author: HoelscJ
 *
 * Created on 11. August 2010, 17:52
 */

#ifndef TREEMAINMENU_H
#define	TREEMAINMENU_H

#include <Wt/Ext/Panel>
#include <Wt/WTreeNode>
#include "WebApp2.h"
namespace org {
  namespace esb {
    namespace web {

      class TreeMainMenu: public Wt::Ext::Panel {
      public:
        TreeMainMenu(org::esb::web::WebApp2 * parent);
        TreeMainMenu(const TreeMainMenu& orig);
        virtual ~TreeMainMenu();
      private:
        typedef void (org::esb::web::WebApp2::*MenuFunc)();

        Wt::WTreeNode * createMenuNode(const Wt::WString& label, Wt::WTreeNode *parentNode, MenuFunc f, const char * icon);

        void dummy();
        org::esb::web::WebApp2 * _parent;
      };
    }
  }
}

#endif	/* TREEMAINMENU_H */

