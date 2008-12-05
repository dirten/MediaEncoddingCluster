/* 
 * File:   Setup.h
 * Author: jhoelscher
 *
 * Created on 18. November 2008, 13:08
 */

#include <Wt/WApplication>

#include <Wt/WEnvironment>
#include <Wt/WStackedWidget>
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/Button>
#include <Wt/WText>

#include <map>
#include <string>
#include "wtk/Div.h"

#include "org/esb/util/Properties.h"
#include "wtk/ElementContainer.h"

#ifndef _SETUP_H
#define	_SETUP_H
namespace org{
  namespace esb{
    namespace web{
            class ButtonLeft : public wtk::Div{
      public:
        ButtonLeft(const char * name="") : Div("") {
          setStyleClass("button-right");
          resize(100,26);
          inner=new Div("");
          inner->setStyleClass("prev");
          inner->resize(70,26);
          addWidget(inner);
          Wt::WText * text=new Wt::WText(name);
          text->setStyleClass("button");
          inner->addWidget(text);
//          clicked.connect(SLOT(this,ButtonLeft::testClicked));
        }

        Div * inner;
      };
      class ButtonRight : public wtk::Div{
      public:
        ButtonRight(const char * name="") : Div("") {
          setStyleClass("button-left");
//          resize(100,26);
          inner=new Div("");
          inner->setStyleClass("next");
//          inner->resize(70,26);
          addWidget(inner);
          Wt::WText * text=new Wt::WText(name);
          text->setStyleClass("button");
          inner->addWidget(text);
//          clicked.connect(SLOT(this,ButtonRight::testClicked));
        }
        void testClicked(){
//          std::cout << "ButtonClicked"<<std::endl;
        }
        Div * inner;
      };

      class Setup: public Wt::WApplication{
      public:
        Setup(const Wt::WEnvironment & env);
        Wt::WWebWidget * createDbPage();
        Wt::WWebWidget * createDbEmbeddedPage();
        Wt::WWebWidget * createHivePage();
        Wt::WWebWidget * createAdminPage();
        Wt::WWebWidget * createSavePage();

      private:
        void nextStep();
        void prevStep();
        void saveConfig();
        void loadConfig();
        void copyDbParams();
        void copyHiveParams();
        void copyAdminParams();
//        void copySaveParams();
        void checkConnection();
        Wt::WLabel * lbl_host;
        Wt::WLabel * lbl_db;
        Wt::WLabel * lbl_user;
        Wt::WLabel * lbl_pass;
        Wt::WLabel * lbl_hport;
        Wt::WLabel * lbl_wport;
        Wt::WLabel * lbl_scan_base;
        Wt::WLabel * lbl_scan_int;
        Wt::WLabel * lbl_adm_name;
        Wt::WLabel * lbl_adm_login;
        Wt::WLabel * lbl_adm_passwd;
        Wt::WLabel * lbl_adm_email;

        Wt::Ext::LineEdit * line_host;
        Wt::Ext::LineEdit * line_db;
        Wt::Ext::LineEdit * line_user;
        Wt::Ext::LineEdit * line_pass;
        Wt::Ext::LineEdit * line_hport;
        Wt::Ext::LineEdit * line_wport;
        Wt::Ext::LineEdit * line_scan_base;
        Wt::Ext::LineEdit * line_scan_int;
        Wt::Ext::LineEdit * line_adm_name;
        Wt::Ext::LineEdit * line_adm_login;
        Wt::Ext::LineEdit * line_adm_passwd;
        Wt::Ext::LineEdit * line_adm_email;

        Wt::Ext::Button * dbCheckButton;
        Wt::Ext::Panel * center;
        ButtonLeft*butPrev;
        ButtonRight*butNext;
        int stepper;

        Wt::WText * error;
        Wt::WWebWidget * dbPage;
        Wt::WWebWidget * hivePage;
        Wt::WWebWidget * adminPage;
        Wt::WWebWidget * savePage;
//        Wt::Ext::Button * dbSaveButton;
        std::map<std::string,std::string> _parameters;
        org::esb::util::Properties _props;
        wtk::ElementContainer<Wt::Ext::LineEdit> _el;
        Wt::WStackedWidget * stack;
      };
    }
  }
}


#endif	/* _SETUP_H */

