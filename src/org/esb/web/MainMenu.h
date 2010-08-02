/* 
 * File:   MainMenu.h
 * Author: jhoelscher
 *
 * Created on 31. August 2009, 12:56
 */
#include "org/esb/db/hivedb.hpp"
#include "boost/shared_ptr.hpp"
#include <Wt/WContainerWidget>
#include "WebApp2.h"

#ifndef _MAINMENU_H
#define	_MAINMENU_H

class MainMenu: public Wt::WContainerWidget{
public:
    MainMenu(org::esb::web::WebApp2 * parent=NULL);
private:
  boost::shared_ptr<db::HiveDb> _db;
};

#endif	/* _MAINMENU_H */

