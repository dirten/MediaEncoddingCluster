#include "Setup.h"

#include <Wt/Ext/Container>
#include <Wt/Ext/Panel>
#include <Wt/WFitLayout>

#include <Wt/WGridLayout>
#include <Wt/WStackedWidget>
#include <Wt/WBorderLayout>
#include <Wt/WText>
#include <Wt/WString>
#include <Wt/WAnchor>
#include <Wt/WResource>
#include <Wt/WBoxLayout>
#include <Wt/WDefaultLayout>

#include <Wt/WInteractWidget>
#include <Wt/WIntValidator>
#include <Wt/WWebWidget>
#include <Wt/WBreak>
#include <Wt/WTable>
#include <Wt/WLabel>
//#include <web/DomElement.h>
#include <ostream>
#include <iostream>
#include "introspec.h"
#include "wtk/ContentBox.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Properties.h"
#include "org/esb/sql/Connection.h"

#include "org/esb/hive/Setup.h"

//#include "wtk/Div.h"

namespace org {
  namespace esb {
    namespace web {

      Setup::Setup(const Wt::WEnvironment & env) : WApplication(env) {
        using namespace org::esb::web::wtk;
        if (!(string(org::esb::config::Config::getProperty("hive.mode")) == "setup")) {
          WApplication::instance()->redirect("/");
          WApplication::instance()->quit();
        }
        stepper = 0;
        setTitle("Hive Websetup");
        Wt::Ext::Container *viewPort = new Wt::Ext::Container(root());
        Wt::WBorderLayout * border = new Wt::WBorderLayout();
        viewPort->setLayout(border);
        messageResourceBundle().use("../res/setup", false);
        useStyleSheet("main.css");

        //        useStyleSheet("ext/resources/css/xtheme-gray.css");
        Wt::Ext::Panel * west = new Wt::Ext::Panel();
        center = new Wt::Ext::Panel();
        Wt::Ext::Panel * north = new Wt::Ext::Panel();

        Wt::WBorderLayout * northLayout = new Wt::WBorderLayout();
        north->setLayout(northLayout);

        west->setBorder(false);
        center->setBorder(false);
        north->setBorder(false);

        west->resize(250, Wt::WLength());
        north->resize(Wt::WLength(), 30);

        border->addWidget(west, Wt::WBorderLayout::West);
        border->addWidget(center, Wt::WBorderLayout::Center);
        border->addWidget(north, Wt::WBorderLayout::North);

        Wt::Ext::Panel * northRight = new Wt::Ext::Panel();
        Wt::Ext::Panel * northLeft = new Wt::Ext::Panel();

        northRight->resize(180, Wt::WLength());
        northLeft->setBorder(false);
        northRight->setBorder(false);

        northLayout->addWidget(northRight, Wt::WBorderLayout::East);
        northLayout->addWidget(northLeft, Wt::WBorderLayout::Center);
        butPrev = new ButtonLeft("Previus");
        butNext = new ButtonRight("Next");
        butPrev->setHidden(true);
        butNext->setHidden(true);


        butPrev->clicked.connect(SLOT(this, Setup::prevStep));
        butNext->clicked.connect(SLOT(this, Setup::nextStep));

        northRight->layout()->addWidget(butPrev);
        northRight->layout()->addWidget(butNext);

        ContentBox * head = new ContentBox("stepbox");
        head->setContent(new Wt::WText(Wt::WString::tr("setup-head")));
        head->resize(Wt::WLength(), 15);

        northLeft->layout()->addWidget(head);
        //        west->layout()->addWidget(bla);

        ContentBox * bla = new ContentBox("stepbox");
        bla->setContent(new Wt::WText(Wt::WString::tr("stepbox")));
        bla->resize(200, 110);
        west->layout()->addWidget(bla);
        ContentBox * errorBox = new ContentBox("stepbox");
        error = new Wt::WText();
        errorBox->setContent(error);
        errorBox->resize(200, 100);
        west->layout()->addWidget(errorBox);

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
        stack = new Wt::WStackedWidget();
        stack->addWidget(createDbPage());
        stack->addWidget(createHivePage());
        stack->addWidget(createAdminPage());

        (center->layout())->addWidget(stack);
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
        _el.validate();
        if(stepper==2)
            stack->addWidget(createSavePage());
        if (stepper < 3) {
//          butNext->setHidden(false);
          stack->setCurrentIndex(++stepper);
        }else{
//            butNext->setHidden(true);
        }
        if (stepper > 0)
          butPrev->setHidden(false);
        else
          butPrev->setHidden(true);

        if (stepper < 3)
          butNext->setHidden(false);
        else
            butNext->setHidden(true);

      }

      void Setup::prevStep() {
        if(stepper==3){
          Wt::WWidget * ref=stack->widget(3);
            stack->removeWidget(ref);
            delete ref;
        }

        if (stepper > 0) {
          stack->setCurrentIndex(--stepper);
        }
        if (stepper > 0)
          butPrev->setHidden(false);
        else
          butPrev->setHidden(true);

        if (stepper < 3)
          butNext->setHidden(false);
        else
            butNext->setHidden(true);

      }


      Wt::WWebWidget * Setup::createDbPage() {

        Wt::WTable * db_table = new Wt::WTable();
        _el.getElement("db.host", "Database Host", "", db_table->elementAt(0, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _el.getElement("db.db", "Database Name", "", db_table->elementAt(1, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        _el.getElement("db.user", "Database User", "", db_table->elementAt(2, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        _el.getElement("db.pass", "Database Password", "", db_table->elementAt(3, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(3, 1));

        wtk::Div * div_db = new wtk::Div("");
        div_db->addWidget(new Wt::WText(Wt::WString::tr("database-setup")));
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(db_table);
        Wt::Ext::Button * checkDb=new Wt::Ext::Button("check Connection",db_table->elementAt(4,0));
        checkDb->clicked.connect(SLOT(this, Setup::checkConnection));

        wtk::ContentBox * c_db = new wtk::ContentBox("stepbox");
        c_db->setContent(div_db);
        return c_db;
      }

      Wt::WWebWidget * Setup::createHivePage() {
        Wt::WTable * hive_table = new Wt::WTable();
        _el.getElement("hive.hport", "Hive Listener Port", "", hive_table->elementAt(0, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _el.getElement("hive.wport", "Web Listener Port", "", hive_table->elementAt(1, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        _el.getElement("hive.scan_base", "Scan Base Dir", "", hive_table->elementAt(2, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        _el.getElement("hive.scan_int", "Scan Interval", "", hive_table->elementAt(3, 0))->setValidator(new Wt::WIntValidator(1, 300)); //new Wt::Ext::LineEdit(db_table->elementAt(3, 1));
        wtk::Div * div_hive = new wtk::Div("");
        div_hive->addWidget(new Wt::WText(Wt::WString::tr("hive-setup")));
        div_hive->addWidget(new Wt::WBreak());
        div_hive->addWidget(new Wt::WBreak());
        div_hive->addWidget(hive_table);

        wtk::ContentBox * c_hive = new wtk::ContentBox("stepbox");
        c_hive->setContent(div_hive);
        return c_hive;
      }

      Wt::WWebWidget * Setup::createAdminPage() {
        Wt::WTable * admin_table = new Wt::WTable();
        _el.getElement("adm.name", "Admin Name", "", admin_table->elementAt(0, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _el.getElement("adm.login", "Admin Login", "", admin_table->elementAt(1, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        _el.getElement("adm.passwd", "Admin Password", "", admin_table->elementAt(2, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        _el.getElement("adm.email", "Admin eMail", "", admin_table->elementAt(3, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(3, 1));

        wtk::Div * div_admin = new wtk::Div("");
        div_admin->addWidget(new Wt::WText(Wt::WString::tr("admin-setup")));
        div_admin->addWidget(new Wt::WBreak());
        div_admin->addWidget(new Wt::WBreak());
        div_admin->addWidget(admin_table);

        wtk::ContentBox * c_admin = new wtk::ContentBox("stepbox");
        c_admin->setContent(div_admin);
        return c_admin;
      }

      Wt::WWebWidget * Setup::createSavePage() {


        wtk::Div * div_admin = new wtk::Div("");
        div_admin->addWidget(new Wt::WText(Wt::WString::tr("save-setup").
            arg(_el.getElement("db.host")->text().narrow()).
            arg(_el.getElement("db.db")->text().narrow()).
            arg(_el.getElement("db.user")->text().narrow()).
            arg(_el.getElement("db.pass")->text().narrow()).
            arg(_el.getElement("hive.hport")->text().narrow()).
            arg(_el.getElement("hive.wport")->text().narrow()).
            arg(_el.getElement("hive.scan_base")->text().narrow()).
            arg(_el.getElement("hive.scan_int")->text().narrow()).
            arg(_el.getElement("adm.name")->text().narrow()).
            arg(_el.getElement("adm.login")->text().narrow()).
            arg(_el.getElement("adm.passwd")->text().narrow()).
            arg(_el.getElement("adm.email")->text().narrow())
            ));

        Wt::Ext::Button * save = new Wt::Ext::Button("Save Config");
        div_admin->addWidget(save);
        save->clicked.connect(SLOT(this, Setup::saveConfig));


        wtk::ContentBox * c_admin = new wtk::ContentBox("stepbox");
        c_admin->setContent(div_admin);

        return c_admin;
      }

      void Setup::loadConfig() {

      }

      void Setup::checkConnection() {
        using namespace org::esb::sql;
        std::string constr=std::string("mysql:host=").append(_el.getElement("db.host")->text().narrow()).
            append(";db=").append(_el.getElement("db.db")->text().narrow()).
            append(";user=").append(_el.getElement("db.user")->text().narrow()).
            append(";passwd=").append(_el.getElement("db.pass")->text().narrow());
        Connection con(constr, false);
        try{
          if(_el.getElement("db.db")->text().narrow().length()==0){
            error->setText("Database Name cannot be empty!");
            butNext->setHidden(true);
            return;
          }
          con.connect();
          error->setText("Database Connection Success");
          butNext->setHidden(false);
        }catch(SqlException & ex){
			sql::Connection con("");
			con.executeNonQuery(string("CREATE DATABASE ").append(_el.getElement("db.db")->text().narrow()));

          logerror(std::string(ex.what()));
          error->setText(ex.what());
          butNext->setHidden(true);
        }
      }

      void Setup::saveConfig() {
        org::esb::io::File file(org::esb::config::Config::getProperty("config.file"));
        org::esb::io::FileOutputStream fos(&file);
        org::esb::util::Properties props;
        props.setProperty("db.connection",
            std::string("mysql:host=").append(_el.getElement("db.host")->text().narrow()).
            append(";db=").append(_el.getElement("db.db")->text().narrow()).
            append(";user=").append(_el.getElement("db.user")->text().narrow()).
            append(";passwd=").append(_el.getElement("db.pass")->text().narrow())
            );
        props.save(&fos);
        fos.close();
        error->setText(Wt::WString::tr("setup-saved"));
        using namespace org::esb;
        config::Config::setProperty("db.connection",props.getProperty("db.connection"));
        hive::Setup::buildDatabaseModel("../sql/hive-0.0.1.sql");
        sql::Connection con(std::string(props.getProperty("db.connection")));
        con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('host','")+_el.getElement("db.host")->text().narrow()+"')");
        con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('database','")+_el.getElement("db.db")->text().narrow()+"')");
        con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('user','")+_el.getElement("db.user")->text().narrow()+"')");
        con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('passwd','")+_el.getElement("db.pass")->text().narrow()+"')");
        con.executeNonQuery(std::string("INSERT INTO `user` (`id`, `auth_name`, `auth_passwd`, `first_name`, `last_name`, `email`, `user_type`, `created`, `updated`) VALUES (1, '").append(_el.getElement("adm.login")->text().narrow()).append("', '").append(_el.getElement("adm.passwd")->text().narrow()).append("', 'Admin', 'User', 'hiveadmin@localhost', 4, '0000-00-00 00:00:00', '0000-00-00 00:00:00')"));

        error->setText("Database Model created!");
        config::Config::setProperty("hive.mode","server");
      }
    }
  }
}
