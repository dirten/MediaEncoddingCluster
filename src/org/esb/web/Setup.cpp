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
#include <Wt/WButtonGroup>
#include <Wt/WGroupBox>
//#include <web/DomElement.h>
#include <ostream>
#include <iostream>
#include "introspec.h"
#include "wtk/ContentBox.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Properties.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/signal/Message.h"
#include "org/esb/hive/Setup.h"
#include "config.h"
//#include "wtk/Div.h"
using namespace org::esb;
namespace org {
  namespace esb {
    namespace web {

      Setup::Setup(const Wt::WEnvironment & env) : WApplication(env) {
        using namespace wtk;
        setTitle("Hive WebSetup");
        Wt::Ext::Container *viewPort = new Wt::Ext::Container(root());
        Wt::WBorderLayout * border = new Wt::WBorderLayout();
        viewPort->setLayout(border);
        //        std::string res = org::esb::config::Config::getProperty("hive.path");

        std::string res = std::string(org::esb::config::Config::getProperty("hive.base_path"));
        res.append("/res/setup");
        messageResourceBundle().use(res.c_str(), false);
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


        butPrev->clicked().connect(SLOT(this, Setup::prevStep));
        butNext->clicked().connect(SLOT(this, Setup::nextStep));

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
        stack->addWidget(createModePage());
        stack->addWidget(createClientPage());

        stack->addWidget(createDbEmbeddedPage());

        stack->addWidget(createHivePage());
        stack->addWidget(createAdminPage());
        stack->addWidget(createServerSuccessPage());
        stack->addWidget(createClientSuccessPage());

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
        LOGDEBUG("Step=" << stack->currentIndex());
        _el.validate();
        int idx = stack->currentIndex();
        //        if (idx == 0 && (!_elchk.getElement("mode.client")->isChecked()))
        if (idx == 0 && (!mode->selectedButtonIndex() == 1))
          idx++;
        //        if (idx == 1 && (!_elchk.getElement("mode.server")->isChecked()))
        if (idx == 1 && (!mode->selectedButtonIndex() == 0))
          idx += 3;
        if (idx == 4)
          stack->insertWidget(5, createSavePage());
        if (idx < stack->count()) {
          stack->setCurrentIndex(++idx);
        } else {
          //            butNext->setHidden(true);
        }
        if (idx > 0)
          butPrev->setHidden(false);
        else
          butPrev->setHidden(true);

        if (idx < 5)
          butNext->setHidden(false);
        else
          butNext->setHidden(true);

      }

      void Setup::prevStep() {
        int idx = stack->currentIndex();
        //        if (idx == 5 && (!_elchk.getElement("mode.server")->isChecked()))
        if (idx == 5 && (!mode->selectedButtonIndex() == 0))
          idx -= 3;
        //        if (idx == 2 && (!_elchk.getElement("mode.client")->isChecked()))
        if (idx == 2 && (!mode->selectedButtonIndex() == 1))
          idx--;
        if (idx == 5) {
          Wt::WWidget * ref = stack->widget(5);
          stack->removeWidget(ref);
          delete ref;
        }

        if (idx > 0) {
          stack->setCurrentIndex(--idx);
        }
        if (idx > 0)
          butPrev->setHidden(false);
        else
          butPrev->setHidden(true);

        if (idx < 5)
          butNext->setHidden(false);
        else
          butNext->setHidden(true);

      }

      Wt::WWebWidget * Setup::createModePage() {
        Wt::WGroupBox *container = new Wt::WGroupBox("Select Mode");

        //        Wt::WTable * db_table = new Wt::WTable();
        mode = new Wt::WButtonGroup(container);
        Wt::WGridLayout * l = new Wt::WGridLayout();
        //        l->setVerticalSpacing(3);
        //        l->setHorizontalSpacing(0);
        //        l->setMinimumSize(Wt::WLength::Auto, 30);
        container->setLayout(l);


        _elradio.addElement("mode.server", "Server Mode", "", l); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _elradio.addElement("mode.client", "Client Mode", "", l); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));

        mode->addButton(_elradio.getElement("mode.server"));
        mode->addButton(_elradio.getElement("mode.client"));
        mode->setSelectedButtonIndex(0);
        /*
                _elradio.getElement("mode.server", "Server Mode", "", db_table->elementAt(0, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
                _elradio.getElement("mode.client", "Client Mode", "", db_table->elementAt(1, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
                group->addButton(_elradio.getElement("mode.server"));
                group->addButton(_elradio.getElement("mode.client"));
                group->setSelectedButtonIndex(0);
         */

        wtk::Div * div_db = new wtk::Div("");
        div_db->addWidget(new Wt::WText(Wt::WString::tr("mode-setup")));
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(container);
        //        Wt::Ext::Button * checkDb = new Wt::Ext::Button("check Connection", db_table->elementAt(4, 0));
        //        checkDb->clicked.connect(SLOT(this, Setup::checkConnection));
        butNext->setHidden(false);
        wtk::ContentBox * c_db = new wtk::ContentBox("stepbox");
        c_db->setContent(div_db);
        return c_db;

      }

      Wt::WWebWidget * Setup::createClientPage() {
        Wt::WTable * db_table = new Wt::WTable();
        _el.getElement("client.host", "Server Hostname", "", db_table->elementAt(0, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _el.getElement("client.port", "Server Port", "", db_table->elementAt(1, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        //        _el.getElement("db.user", "Database User", "", db_table->elementAt(2, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        //        _el.getElement("db.pass", "Database Password", "", db_table->elementAt(3, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(3, 1));

        wtk::Div * div_db = new wtk::Div("");
        div_db->addWidget(new Wt::WText(Wt::WString::tr("client-setup")));
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(db_table);
        //        Wt::Ext::Button * checkDb = new Wt::Ext::Button("check Connection", db_table->elementAt(4, 0));
        //        checkDb->clicked.connect(SLOT(this, Setup::checkConnection));
        butNext->setHidden(false);
        wtk::ContentBox * c_db = new wtk::ContentBox("stepbox");
        c_db->setContent(div_db);
        return c_db;

      }

      Wt::WWebWidget * Setup::createDbEmbeddedPage() {
        //        _el.getElement("db.db")->setText("hive");
        butNext->setHidden(false);
        wtk::Div * div_db = new wtk::Div("");
        div_db->addWidget(new Wt::WText(Wt::WString::tr("database-embedded-setup")));
        div_db->addWidget(new Wt::WBreak());
        div_db->addWidget(new Wt::WBreak());
        _el.getElement("db.host", "Database Host", ""); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _el.getElement("db.db", "Database Name", "hive"); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        _el.getElement("db.user", "Database User", ""); //new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        _el.getElement("db.pass", "Database Password", ""); //new Wt::Ext::LineEdit(db_table->elementAt(3, 1));
        wtk::ContentBox * c_db = new wtk::ContentBox("stepbox");
        c_db->resize(400, 200);
        c_db->setContent(div_db);
        return c_db;
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
        Wt::Ext::Button * checkDb = new Wt::Ext::Button("check Connection", db_table->elementAt(4, 0));
        checkDb->clicked().connect(SLOT(this, Setup::checkConnection));

        wtk::ContentBox * c_db = new wtk::ContentBox("stepbox");
        c_db->setContent(div_db);
        return c_db;
      }

      Wt::WWebWidget * Setup::createHivePage() {
        //        Wt::WTable * hive_table = new Wt::WTable();
        Wt::WGridLayout * l = new Wt::WGridLayout();
        l->setVerticalSpacing(3);
        l->setHorizontalSpacing(3);
        //        l->setMinimumSize(Wt::WLength::Auto, 30);
        wtk::Div * div_hive = new wtk::Div("");
        div_hive->setLayout(l);

        l->addWidget(new Wt::WText(Wt::WString::tr("hive-setup")), 0, 0);
        l->addWidget(new Wt::WBreak(), 1, 0);
        _el.addElement("hive.hport", "Hive Listener Port", "", l); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _el.addElement("hive.wport", "Web Listener Port", "", l); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        _el.addElement("hive.tmp_data_dir", "Temporary Data Directory", "", l); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        _el.getElement("hive.hport")->setText("20200");
        _el.getElement("hive.wport")->setText("8080");
        std::string tmppath=config::Config::getProperty("hive.base_path");
        tmppath+="/tmp";
        _el.getElement("hive.tmp_data_dir")->setText(tmppath.c_str());
        //        _el.getElement("hive.scan_base", "Scan Base Dir", "", hive_table->elementAt(2, 0)); //new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        //        _el.getElement("hive.scan_int", "Scan Interval", "", hive_table->elementAt(3, 0))->setValidator(new Wt::WIntValidator(1, 300)); //new Wt::Ext::LineEdit(db_table->elementAt(3, 1));
        //        div_hive->addWidget(hive_table);

        wtk::ContentBox * c_hive = new wtk::ContentBox("stepbox");
        c_hive->setContent(div_hive);
        return c_hive;
      }

      Wt::WWebWidget * Setup::createAdminPage() {
//        Wt::WTable * admin_table = new Wt::WTable();
        Wt::WGridLayout * l = new Wt::WGridLayout();
        l->setVerticalSpacing(3);
        l->setHorizontalSpacing(3);
        //        l->setMinimumSize(Wt::WLength::Auto, 30);
        wtk::Div * div_admin = new wtk::Div("");
        div_admin->setLayout(l);
        l->addWidget(new Wt::WText(Wt::WString::tr("admin-setup")),0,0);
        l->addWidget(new Wt::WBreak(),1,0);


        _el.addElement("adm.name", "Admin Name", "not used", l); //new Wt::Ext::LineEdit(db_table->elementAt(0, 1));
        _el.addElement("adm.login", "Admin Login", "not used", l); //new Wt::Ext::LineEdit(db_table->elementAt(1, 1));
        _el.addElement("adm.passwd", "Admin Password", "not used", l); //new Wt::Ext::LineEdit(db_table->elementAt(2, 1));
        _el.addElement("adm.email", "Admin eMail", "not used", l); //new Wt::Ext::LineEdit(db_table->elementAt(3, 1));

        
        wtk::ContentBox * c_admin = new wtk::ContentBox("stepbox");
        c_admin->setContent(div_admin);
        return c_admin;
      }

      Wt::WWebWidget * Setup::createSavePage() {


        wtk::Div * div_admin = new wtk::Div("");

        const char * server_mode = mode->selectedButtonIndex() == 0 ? "On" : "Off";
        const char * client_mode = mode->selectedButtonIndex() == 1 ? "On" : "Off";


#ifdef USE_EMBEDDED_MYSQL
        div_admin->addWidget(new Wt::WText(Wt::WString::tr("save-setup-embedded").
#else
        div_admin->addWidget(new Wt::WText(Wt::WString::tr("save-setup").
#endif
            arg(Wt::WString(server_mode)).
            arg(Wt::WString(client_mode)).
            arg(_el.getElement("db.host")->text().narrow()).
            arg(_el.getElement("db.db")->text().narrow()).
            arg(_el.getElement("db.user")->text().narrow()).
            arg(_el.getElement("db.pass")->text().narrow()).
            arg(_el.getElement("hive.hport")->text().narrow()).
            arg(_el.getElement("hive.wport")->text().narrow()).
            arg(_el.getElement("adm.name")->text().narrow()).
            arg(_el.getElement("adm.login")->text().narrow()).
            arg(_el.getElement("adm.passwd")->text().narrow()).
            arg(_el.getElement("adm.email")->text().narrow())
            ));
        Wt::Ext::Button * save = new Wt::Ext::Button("Save Config");
        div_admin->addWidget(save);
        save->clicked().connect(SLOT(this, Setup::saveConfig));


        wtk::ContentBox * c_admin = new wtk::ContentBox("stepbox");
        c_admin->setContent(div_admin);

        return c_admin;
      }

      Wt::WWebWidget * Setup::createServerSuccessPage() {
        wtk::Div * div_admin = new wtk::Div("");
        div_admin->addWidget(new Wt::WText(Wt::WString::tr("success-server-setup")));

        Wt::Ext::Button * bt = new Wt::Ext::Button(Wt::WString::tr("go-to-login-screen"));
        div_admin->addWidget(bt);
        bt->clicked().connect(SLOT(this, Setup::setLoginScreen));

        wtk::ContentBox * c_admin = new wtk::ContentBox("stepbox");
        c_admin->setContent(div_admin);

        return c_admin;
      }

      Wt::WWebWidget * Setup::createClientSuccessPage() {
        wtk::Div * div_admin = new wtk::Div("");
        div_admin->addWidget(new Wt::WText(Wt::WString::tr("success-client-setup")));
        /*
                Wt::Ext::Button * bt = new Wt::Ext::Button(Wt::WString::tr("go-to-login-screen"));
                div_admin->addWidget(bt);
                bt->clicked.connect(SLOT(this, Setup::setLoginScreen));
         */

        wtk::ContentBox * c_admin = new wtk::ContentBox("stepbox");
        c_admin->setContent(div_admin);

        return c_admin;
      }

      void Setup::loadConfig() {

      }

      void Setup::checkConnection() {
        std::string constr = std::string("mysql:host=").append(_el.getElement("db.host")->text().narrow()).
            append(";db=").append(_el.getElement("db.db")->text().narrow()).
            append(";user=").append(_el.getElement("db.user")->text().narrow()).
            append(";passwd=").append(_el.getElement("db.pass")->text().narrow());
            /*
        Connection con(constr, false);
        try {
          if (_el.getElement("db.db")->text().narrow().length() == 0) {
            error->setText("Database Name cannot be empty!");
            butNext->setHidden(true);
            return;
          }
          con.connect();
          error->setText("Database Connection Success");
          butNext->setHidden(false);
        } catch (SqlException & ex) {
          LOGERROR(std::string(ex.what()));
          error->setText(ex.what());
          butNext->setHidden(true);
        }
        */
      }

      void Setup::setLoginScreen() {
        WApplication::instance()->redirect("/");
        WApplication::instance()->quit();
      }

      void Setup::saveConfig() {
        org::esb::util::Properties props;
        const char * server_mode = mode->selectedButtonIndex() == 0 ? "On" : "Off";
        const char * client_mode = mode->selectedButtonIndex() == 1 ? "On" : "Off";
        props.setProperty("mode.server", server_mode);
        props.setProperty("mode.client", client_mode);
        config::Config::setProperty("mode.server", server_mode);
        config::Config::setProperty("mode.client", client_mode);

        if (mode->selectedButtonIndex() == 1) {
          props.setProperty("client.port", _el.getElement("client.port")->text().narrow().c_str());
          props.setProperty("client.host", _el.getElement("client.host")->text().narrow().c_str());
        }
        if (mode->selectedButtonIndex() == 0) {
          props.setProperty("db.connection",
              std::string("mysql:host=").append(_el.getElement("db.host")->text().narrow()).
              append(";db=").append(_el.getElement("db.db")->text().narrow()).
              append(";user=").append(_el.getElement("db.user")->text().narrow()).
              append(";passwd=").append(_el.getElement("db.pass")->text().narrow())
              );
          //        error->setText(Wt::WString::tr("setup-saved"));
          //        using namespace org::esb;
          config::Config::setProperty("db.connection", props.getProperty("db.connection"));
          config::Config::setProperty("host", _el.getElement("db.host")->text().narrow().c_str());
          config::Config::setProperty("user", _el.getElement("db.user")->text().narrow().c_str());
          config::Config::setProperty("passwd", _el.getElement("db.pass")->text().narrow().c_str());
          config::Config::setProperty("database", _el.getElement("db.db")->text().narrow().c_str());
          /*
                  con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('host','") + _el.getElement("db.host")->text().narrow() + "')");
                  con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('database','") + _el.getElement("db.db")->text().narrow() + "')");
                  con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('user','") + _el.getElement("db.user")->text().narrow() + "')");
                  con.executeNonQuery(std::string("insert into config (config_key, config_val) values ('passwd','") + _el.getElement("db.pass")->text().narrow() + "')");
           **/

          error->setText("Database Model created!");
          config::Config::setProperty("hive.mode", "server");
          config::Config::setProperty("hive.port", _el.getElement("hive.hport")->text().narrow().c_str());
          config::Config::setProperty("hive.start", "true");
          config::Config::setProperty("hive.scandir", "/");
          config::Config::setProperty("hive.outdir", "/");
          config::Config::setProperty("hive.autoscan", "true");
          config::Config::setProperty("hive.scaninterval", "30");
          config::Config::setProperty("web.start", "true");
          config::Config::save2db();
        }
        std::string configfile = org::esb::config::Config::getProperty("hive.base_path");
        configfile.append("/.hive.cfg");
        org::esb::io::File file(configfile);
        org::esb::io::FileOutputStream fos(&file);
        props.save(&fos);
        fos.close();
        /*Restarting the Services after configuration*/
        using namespace org::esb::signal;
        signal::Messenger::getInstance().sendRequest(Message().setProperty("directoryscan", org::esb::hive::STOP));
        signal::Messenger::getInstance().sendRequest(Message().setProperty("jobscanner", org::esb::hive::STOP));
        signal::Messenger::getInstance().sendRequest(Message().setProperty("jobwatcher", org::esb::hive::STOP));
        signal::Messenger::getInstance().sendRequest(Message().setProperty("processunitwatcher", org::esb::hive::STOP));
        //        signal::Messenger::getInstance().sendRequest(Message().setProperty("hivelistener", org::esb::hive::STOP));
        //        signal::Messenger::getInstance().sendRequest(Message().setProperty("webserver", org::esb::hive::STOP));
        signal::Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));

        if (string(org::esb::config::Config::getProperty("hive.start")) == "true") {
          Messenger::getInstance().sendMessage(Message().setProperty("processunitwatcher", org::esb::hive::START));
          //    Messenger::getInstance().sendMessage(Message().setProperty("jobwatcher", org::esb::hive::START));
          signal::Messenger::getInstance().sendRequest(Message().setProperty("hivelistener", org::esb::hive::START));
        }

        if (string(org::esb::config::Config::getProperty("hive.autoscan")) == "true") {
          signal::Messenger::getInstance().sendRequest(Message().
              setProperty("directoryscan", org::esb::hive::START).
              setProperty("directory", org::esb::config::Config::getProperty("hive.scandir")).
              setProperty("interval", org::esb::config::Config::getProperty("hive.scaninterval")));
          signal::Messenger::getInstance().sendRequest(Message().setProperty("exportscanner", org::esb::hive::START));
        signal::Messenger::getInstance().sendRequest(Message().setProperty("jobscanner", org::esb::hive::START));
        }
        if (string(org::esb::config::Config::getProperty("mode.client")) == "On") {
          signal::Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::START));
        }

        //config::Config::init(configfile);

        int idx = stack->currentIndex();
        stack->setCurrentIndex(++idx);

      }
    }
  }
}
