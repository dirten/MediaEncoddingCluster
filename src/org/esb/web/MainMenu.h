/* 
 * File:   MainMenu.h
 * Author: jhoelscher
 *
 * Created on 31. August 2009, 12:56
 */
#include <Wt/WContainerWidget>
#include "WebApp2.h"

#ifndef _MAINMENU_H
#define	_MAINMENU_H

class MainMenu: public Wt::WContainerWidget{
public:
    MainMenu(org::esb::web::WebApp2 * parent=NULL);
    
};

#endif	/* _MAINMENU_H */

