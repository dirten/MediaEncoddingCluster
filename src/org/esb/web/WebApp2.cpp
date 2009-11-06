#include "WebApp2.h"
#include "config.h"

#include "MainMenu.h"
#include <Wt/Ext/Container>
#include <Wt/WContainerWidget>
#include <Wt/WLayoutItem>
#include <Wt/WVBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WFitLayout>
#include <Wt/WText>
#include <Wt/WLength>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/Ext/Panel>

#include <Files.cpp>
#include "Profiles.cpp"
#include "ProfilesForm.h"
#include "WatchFolder.cpp"
#include "WatchfolderForm.h"
#include "Configuration.cpp"
namespace org {
  namespace esb {
    namespace web {

      WebApp2::WebApp2(const Wt::WEnvironment & env) : WApplication(env) {
        if (string(org::esb::config::Config::getProperty("hive.mode")) == "setup") {
          WApplication::instance()->redirect("/setup");
          WApplication::instance()->quit();
        }
        std::string h = "MediaEncodingCluster V-";
        h += MHIVE_VERSION;
        h += "($Rev: 893 $-"__DATE__ "-" __TIME__")";
        setTitle(h);
        useStyleSheet("filetree.css");
        useStyleSheet("main.css");

        setLoadingIndicator(new Wt::WOverlayLoadingIndicator());
        viewPort = new Wt::Ext::Container(root());
        //        viewPort->resize(Wt::WLength::Auto, 600);

        //        Wt::WVBoxLayout *layout = new Wt::WVBoxLayout();
        layout = new Wt::WBorderLayout();
        //        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        viewPort->setLayout(layout);

        /*begin Head Panel*/
        Wt::Ext::Panel *north = new Wt::Ext::Panel();

        north->setBorder(false);

        Wt::WText *head = new Wt::WText(h);
        head->setStyleClass("north");
        north->setLayout(new Wt::WFitLayout());
        north->layout()->addWidget(head);
        north->resize(Wt::WLength(), 35);
        //        layout->addWidget(north, Wt::WBorderLayout::North);
        /*end Head Panel*/

        /*begin Menu Panel*/
        MainMenu * menu = new MainMenu(this);
        menu->resize(Wt::WLength::Auto, 30);
        layout->addWidget(menu, Wt::WBorderLayout::North);
        /*end Menu Panel*/


        /*begin Main Panel*/
        main_panel = new Wt::Ext::Panel();
        Wt::WFitLayout * fit = new Wt::WFitLayout();
        main_panel->setLayout(fit);
        //        main_panel->setBorder(true);
        layout->addWidget(main_panel, Wt::WBorderLayout::Center);
        /*end Main Panel*/

        /*begin Footer Panel*/
        Wt::Ext::Panel *footer = new Wt::Ext::Panel();
        footer->setBorder(false);
        head = new Wt::WText("&copy; 2009 <a target=\"_blank\" href=\"http://codergrid.de/\">Jan H&ouml;lscher</a> - GPL License");
        head->setStyleClass("north");
        footer->setLayout(new Wt::WFitLayout());
        footer->layout()->addWidget(head);
        footer->resize(Wt::WLength(), 35);
        layout->addWidget(footer, Wt::WBorderLayout::South);
        /*end Footer Panel*/

        useStyleSheet("ext/resources/css/xtheme-slate.css");


      }

      void WebApp2::listAllFiles() {
        SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, concat(round(size/1024/1024,2),' MB') as size, concat(round(duration/1000000),' sec.') as duration from files "));
        tab->setColumnWidth(0, 10);
        tab->setColumnWidth(2, 10);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 20);
        setContent(tab);
      }

      void WebApp2::listImportedFiles() {
        SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, concat(round(size/1024/1024,2),' MB') as size , concat(round(duration/1000000),' sec.') as duration from files where parent=0"));
        tab->setColumnWidth(0, 10);
        tab->setColumnWidth(2, 10);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 20);
        setContent(tab);
      }

      void WebApp2::listEncodedFiles() {
        SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, concat(round(size/1024/1024,2),' MB') as size, concat(round(duration/1000000),' sec.') as duration from files where parent>0"));
        tab->setColumnWidth(0, 10);
        tab->setColumnWidth(2, 10);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 20);
        setContent(tab);
      }

      void WebApp2::listAllEncodings() {
        //        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid order by 2,f.id DESC"));
        SqlTable * tab = new SqlTable(std::string("select filename,round(max((end_ts-s2.start_time)*s2.time_base_num/s2.time_base_den)/(f.duration/1000000),3)*100 as progress,min(send) as start_time,sum(timestampdiff(SECOND,send,complete))\"cpu-time\" from process_units pu, streams s, streams s2, files f where pu.target_stream =s.id and pu.source_stream=s2.id and s.fileid=f.id group by f.id"));
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listPendingEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) = 0 order by 2,f.id DESC"));
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listActiveEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) > 0 and round(count(complete)/count(*)*100,2) < 100 order by 2,f.id DESC"));
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listSuccessfullEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) = 100 order by 2,f.id DESC"));
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listFailureEncodings() {
      }

      void WebApp2::listAllProfiles() {
        Profiles * profiles = new Profiles();
       
        setContent(profiles);
      }

      void WebApp2::createProfiles() {
        listAllProfiles();
        cpd = new Wt::Ext::Dialog("Profile");
        cpd->resize(500, 400);
        ProfilesForm * pf = new ProfilesForm(cpd->contents());
        pf->profileSaved.connect(SLOT(this, WebApp2::profilesCreated));
        pf->profileCanceled.connect(SLOT(cpd, Wt::Ext::Dialog::accept));
        cpd->show();
      }
      void WebApp2::openConfiguration() {

      }

      void WebApp2::profilesCreated() {
        listAllProfiles();
        delete cpd;
      }

      void WebApp2::listAllWatchfolder() {
        WatchFolder * wf = new WatchFolder(0);
        setContent(wf);
      }

      void WebApp2::createWatchfolder() {
        listAllWatchfolder();
        cwd = new Wt::Ext::Dialog("Watchfolder");
        cwd->resize(450, 200);
        WatchfolderForm * pf = new WatchfolderForm(cwd->contents());
        pf->saved.connect(SLOT(this, WebApp2::watchfolderCreated));
        pf->canceled.connect(SLOT(cwd, Wt::Ext::Dialog::accept));
        cwd->show();
      }

      void WebApp2::watchfolderCreated() {
        listAllWatchfolder();
        delete cwd;
      }

      void WebApp2::editSystemConfiguration() {
         Configuration * conf=new Configuration();
        setContent(conf);
      }

      void WebApp2::setContent(Wt::WWidget * w) {
        if (main_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = main_panel->layout()->itemAt(0);
          main_panel->layout()->removeItem(item);
        }
        main_panel->layout()->addWidget(w);

        //        main_panel->layout()->addWidget(w);
      }
    }
  }
}

