#include "Setup.h"

#include <Wt/Ext/Container>
#include <Wt/Ext/Panel>
#include <Wt/WFitLayout>

#include <Wt/WGridLayout>
#include <Wt/WBorderLayout>
#include <Wt/WText>
#include <Wt/WString>
#include <Wt/WAnchor>
#include <Wt/WResource>
#include <Wt/WBoxLayout>
#include <Wt/WDefaultLayout>

#include <Wt/WInteractWidget>
#include <Wt/WWebWidget>
#include <Wt/WBreak>
#include <Wt/WTable>
#include <Wt/WLabel>
//#include <web/DomElement.h>
#include <ostream>
#include <iostream>
#include "introspec.h"
#include "wtk/ContentBox.h"
//#include "wtk/Div.h"

namespace org {
  namespace esb {
    namespace web {
/*
      class TestResource : public Wt::WResource {
      public:

        TestResource() {

        }

        const std::string resourceMimeType() const {
          return "text/css";
        }

        bool streamResourceData(std::ostream &stream, const ArgumentMap &arguments) {
          stream << "bla hier der content";
          return true;
        }

        void setArguments(const ArgumentMap &arguments) {

        }
      };
 */
/*
      class Div : public Wt::WContainerWidget {
      public:
        Div(const std::string& style,WContainerWidget *parent=0)
        : Wt::WContainerWidget(parent) {
          setStyleClass(style);
        }
      };

 */


  /*      
      class ContentBox: public Div {
      public:
        ContentBox(const char * name="",WContainerWidget *parent=0): Div(name, parent){
//          setId(name);
          Div * t1=new Div("t");
          Div * t2=new Div("t");
          Div * t3=new Div("t");

          Div * b1=new Div("b");
          Div * b2=new Div("b");
          Div * b3=new Div("b");

 * t1->addWidget(t2);
          t2->addWidget(t3);

          b1->addWidget(b2);
          b2->addWidget(b3);

          addWidget(t1);
//          cont=new Div("");
//          cont->setStyleClass("m");
          
          m=new Div("m");
//          m->setStyleClass("m");
          
//          header=new Div("t");
//          header->setStyleClass("header");

//          cont->addWidget(header);
//          cont->addWidget(m);
          
          addWidget(m);
          
          addWidget(b1);
        }
        
        void setHeaderText(std::string  text){
//          header->addWidget(new Wt::WText(text));          
        }
        void setContent(Wt::WWidget * widget){
          m->addWidget(widget);
        }
        
        void resize(const Wt::WLength &width, const Wt::WLength &height){
//          Div::resize(width,height);
          m->resize(width,height);
//          cont->resize(width,height);
//          header->resize(width,Wt::WLength());
        }
      private:
        Div * header;
        Div * m;
        Div * cont;
      };
*/      
      Setup::Setup(const Wt::WEnvironment & env) : WApplication(env) {
        using namespace org::esb::web::wtk;
        stepper=0;
        setTitle("Hive Websetup");
        Wt::Ext::Container *viewPort = new Wt::Ext::Container(root());
        Wt::WBorderLayout * border=new Wt::WBorderLayout();
        viewPort->setLayout(border);
        messageResourceBundle().use("../setup", false);
        useStyleSheet("main.css");

//        useStyleSheet("ext/resources/css/xtheme-gray.css");
        Wt::Ext::Panel * west=new Wt::Ext::Panel();
        center=new Wt::Ext::Panel();
        Wt::Ext::Panel * north=new Wt::Ext::Panel();

        Wt::WBorderLayout * northLayout=new Wt::WBorderLayout();
        north->setLayout(northLayout);

        west->setBorder(false);
        center->setBorder(false);
        north->setBorder(false);
        
        west->resize(250,Wt::WLength());
        north->resize(Wt::WLength(),30);

        border->addWidget(west,Wt::WBorderLayout::West);
        border->addWidget(center,Wt::WBorderLayout::Center);
        border->addWidget(north,Wt::WBorderLayout::North);
        
        Wt::Ext::Panel * northRight=new Wt::Ext::Panel();
        Wt::Ext::Panel * northLeft=new Wt::Ext::Panel();
        
        northRight->resize(180,Wt::WLength());
        northLeft->setBorder(false);
        northRight->setBorder(false);

        northLayout->addWidget(northRight,Wt::WBorderLayout::East);
        northLayout->addWidget(northLeft,Wt::WBorderLayout::Center);
        ButtonLeft*butPrev=new ButtonLeft("Previus");
        ButtonRight*butNext=new ButtonRight("Next");

        butPrev->clicked.connect(SLOT(this,Setup::prevStep));
        butNext->clicked.connect(SLOT(this,Setup::nextStep));

        northRight->layout()->addWidget(butPrev);
        northRight->layout()->addWidget(butNext);

        ContentBox * head=new ContentBox("stepbox");
        head->setContent(new Wt::WText(Wt::WString::tr("setup-head")));
        head->resize(Wt::WLength(),15);

        northLeft->layout()->addWidget(head);
//        west->layout()->addWidget(bla);

        ContentBox * bla=new ContentBox("stepbox");
        bla->setContent(new Wt::WText(Wt::WString::tr("stepbox")));
        bla->resize(200,110);
        west->layout()->addWidget(bla);
//        viewPort->layout()->addWidget(bla);
//        border->addWidget(bla,Wt::WBorderLayout::West);
//        border->addWidget(new Wt::WText("links"),Wt::WBorderLayout::West);
/*
        ContentBox * bla2=new ContentBox("stepbox");
        bla2->setContent(new Wt::WText(Wt::WString::tr("about1")));
        bla2->resize(400,200);
        center->layout()->addWidget(bla2);
 */
//        border->addWidget(new Wt::WText("center"),Wt::WBorderLayout::Center);

//        border->addWidget(bla2,Wt::WBorderLayout::Center);
//        return;
        
        
//        Wt::WContainerWidget * cont=new Wt::WContainerWidget();
//        cont->setLayout(new Wt::WBoxLayout(Wt::WBoxLayout::LeftToRight));
//        cont->setLayout(new Wt::WGridLayout());
/*
        Wt::Ext::Panel *p = new Wt::Ext::Panel();
        p->setLayout(new Wt::WFitLayout());
        p->resize(Wt::WLength(),800);
//        p->setCollapsible(true);
        p->setTitle("Hive Setup");
        p->setBorder(true);
        viewPort->layout()->addWidget(p);
        p->layout()->addWidget(cont);

        Wt::WText * text = new Wt::WText(Wt::WString::tr("setup"));
        TestResource * t = new TestResource();
        Wt::WAnchor * a = new Wt::WAnchor();
        a->setResource(t);
        a->setText("hier click");
//        p->layout()->addWidget(text);
//        p->layout()->addWidget(a);
  */
        /*
        ContentBox * head=new ContentBox();
//        head->setContent(new Wt::WText("Im Falle eine Einstellung nicht unterstützt wird (gekennzeichnet mit Nein) entspricht Ihr System nicht den Mindestvoraussetzungen. Bitte nehmen Sie die notwendigen Schritte vor um diese Fehler zu korrigieren. Joomla! wird nicht korrekt funktionieren, wenn Sie dies unterlassen!"));
        head->setContent(new Wt::WText(""));
        ((Wt::WBorderLayout*)viewPort->layout())->addWidget(head,Wt::WBorderLayout::North);
*/
//        ContentBox * content=new ContentBox();
        /*
        ContentBox * c_intro=new ContentBox();
        c_intro->setContent(new Wt::WText(Wt::WString::tr("about")));
        c_intro->resize(Wt::WLength(),200);
        ((Wt::WGridLayout*)cont->layout())->addWidget(c_intro,0,0,1,3);
         */

/*
        ((Wt::WGridLayout*)cont->layout())->addWidget(createDbPage(),1,0);
        ((Wt::WGridLayout*)cont->layout())->addWidget(createHivePage(),1,1);
        ((Wt::WGridLayout*)cont->layout())->addWidget(createAdminPage(),1,2);
 */


        (center->layout())->addWidget(createDbPage());
//        (center->layout())->addWidget(createAdminPage());

//        ContentBox * c_general=new ContentBox();
//        c_general->setContent(new Wt::WText("Im Falle eine Einstellung nicht unterstützt wird (gekennzeichnet mit Nein) entspricht Ihr System nicht den Mindestvoraussetzungen. Bitte nehmen Sie die notwendigen Schritte vor um diese Fehler zu korrigieren. Joomla! wird nicht korrekt funktionieren, wenn Sie dies unterlassen!"));
//        c_admin->resize(400,200);
//        ((Wt::WGridLayout*)cont->layout())->addWidget(c_general,2,0,1,3);
        

        /*
        c->setHidden(true);
        ((Wt::WGridLayout*)cont->layout())->addWidget(c5,0,1);
        ((Wt::WGridLayout*)cont->layout())->addWidget(c3,0,2);
        ((Wt::WGridLayout*)cont->layout())->addWidget(c4,1,1);
        ((Wt::WGridLayout*)cont->layout())->addWidget(c2,0,0);
        ((Wt::WGridLayout*)cont->layout())->addWidget(new Div(""),2,0);
*/
/*
        Wt::Ext::Panel *p2 = new Wt::Ext::Panel();
        p2->setTitle("Webserver Setup");
        p2->setBorder(true);
        Wt::WText * textWeb = new Wt::WText(Wt::WString::tr("setup-web"));
        p2->layout()->addWidget(textWeb);
*/
//        viewPort->layout()->addWidget(p2);

      }

      void Setup::nextStep() {
        std::cout << "nextStep in"<<stepper<<std::endl;
        if (stepper == 0) {
          (center->layout())->removeItem((center->layout())->itemAt(0));
          (center->layout())->addWidget(createHivePage());
          stepper++;
        } else
          if (stepper == 1) {
          (center->layout())->removeItem((center->layout())->itemAt(0));
          (center->layout())->addWidget(createAdminPage());
          stepper++;
        }
        std::cout << "nextStep out"<<stepper<<std::endl;
      }

      void Setup::prevStep(){
        std::cout << "prevStep in"<<stepper<<std::endl;
        if (stepper == 1) {
          (center->layout())->removeItem((center->layout())->itemAt(0));
          (center->layout())->addWidget(createDbPage());
          stepper--;
        } else
          if (stepper == 2) {
          (center->layout())->removeItem((center->layout())->itemAt(0));
          (center->layout())->addWidget(createHivePage());
          stepper--;
        }
        std::cout << "prevStep out"<<stepper<<std::endl;

      }

      Wt::WWebWidget * Setup::createDbPage(){
        Wt::WTable * db_table=new Wt::WTable();

        lbl_host = new Wt::WLabel("Database Host ",db_table->elementAt(0, 0));
        lbl_db = new Wt::WLabel("Database Name ",db_table->elementAt(1, 0));
        lbl_user = new Wt::WLabel("Database User ",db_table->elementAt(2, 0));
        lbl_pass = new Wt::WLabel("Database Passwd ",db_table->elementAt(3, 0));

        line_host = new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        line_db = new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        line_user = new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        line_pass = new Wt::Ext::LineEdit(db_table->elementAt(3, 1));

        line_host->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_db->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_user->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_pass->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());

        lbl_host->setBuddy(line_host);
        lbl_db->setBuddy(line_db);
        lbl_user->setBuddy(line_user);
        lbl_pass->setBuddy(line_pass);

        dbCheckButton=new Wt::Ext::Button("Check Connection",db_table->elementAt(4, 1));

        wtk::Div * div_db=new wtk::Div("");
        div_db->addWidget(new Wt::WText(Wt::WString::tr("database-setup")));
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(db_table);

        wtk::ContentBox * c_db=new wtk::ContentBox("stepbox");
        c_db->setContent(div_db);
        return c_db;
      }

      Wt::WWebWidget * Setup::createHivePage(){
        Wt::WTable * hive_table=new Wt::WTable();
        lbl_hport = new Wt::WLabel("Hive Listener Port ",hive_table->elementAt(0, 0));
        lbl_wport = new Wt::WLabel("Web Listener Port ",hive_table->elementAt(1, 0));
        lbl_scan_base = new Wt::WLabel("Scan Base Dir ",hive_table->elementAt(2, 0));
        lbl_scan_int = new Wt::WLabel("Scan Intervall ",hive_table->elementAt(3, 0));
        lbl_hport->setToolTip("hier kommt ein guter tooltip");

        line_hport = new Wt::Ext::LineEdit(hive_table->elementAt(0, 1));
        line_wport = new Wt::Ext::LineEdit(hive_table->elementAt(1, 1));
        line_scan_base = new Wt::Ext::LineEdit(hive_table->elementAt(2, 1));
        line_scan_int = new Wt::Ext::LineEdit(hive_table->elementAt(3, 1));

        line_hport->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_wport->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_scan_base->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_scan_int->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());

        wtk::Div * div_hive=new wtk::Div("");
        div_hive->addWidget(new Wt::WText(Wt::WString::tr("hive-setup")));
        div_hive->addWidget(new Wt::WBreak());
        div_hive->addWidget(new Wt::WBreak());
        div_hive->addWidget(hive_table);

        wtk::ContentBox * c_hive=new wtk::ContentBox("stepbox");
        c_hive->setContent(div_hive);
        return c_hive;
      }

      Wt::WWebWidget * Setup::createAdminPage(){
        Wt::WTable * admin_table=new Wt::WTable();
        lbl_adm_name = new Wt::WLabel("Admin Name ",admin_table->elementAt(0, 0));
        lbl_adm_login = new Wt::WLabel("Admin Login  ",admin_table->elementAt(1, 0));
        lbl_adm_passwd = new Wt::WLabel("Admin Password ",admin_table->elementAt(2, 0));
        lbl_adm_email = new Wt::WLabel("Admin eMail ",admin_table->elementAt(3, 0));

        line_adm_name = new Wt::Ext::LineEdit(admin_table->elementAt(0, 1));
        line_adm_login = new Wt::Ext::LineEdit(admin_table->elementAt(1, 1));
        line_adm_passwd = new Wt::Ext::LineEdit(admin_table->elementAt(2, 1));
        line_adm_email = new Wt::Ext::LineEdit(admin_table->elementAt(3, 1));

        line_adm_name->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_adm_login->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_adm_passwd->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
        line_adm_email->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());


        wtk::Div * div_admin=new wtk::Div("");
        div_admin->addWidget(new Wt::WText(Wt::WString::tr("admin-setup")));
        div_admin->addWidget(new Wt::WBreak());
        div_admin->addWidget(new Wt::WBreak());
        div_admin->addWidget(admin_table);

        wtk::ContentBox * c_admin=new wtk::ContentBox("stepbox");
        c_admin->setContent(div_admin);
        return c_admin;
      }
    }
  }
}
