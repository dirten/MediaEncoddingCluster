#include "WebApp2.h"
#include "config.h"

#include "MainMenu.h"
#include <Wt/Ext/Container>
#include <Wt/WContainerWidget>
#include <Wt/WLayoutItem>
#include <Wt/WVBoxLayout>
#include <Wt/WFitLayout>
#include <Wt/WText>
#include <Wt/WLength>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/Ext/Panel>

#include <Files.cpp>
#include "Profiles.cpp"
#include "WatchFolder.cpp"

namespace org {
  namespace esb {
    namespace web {

      WebApp2::WebApp2(const Wt::WEnvironment & env) : WApplication(env) {
        setTitle("Hive Webadmin");
        useStyleSheet("filetree.css");
        useStyleSheet("main.css");

        setLoadingIndicator(new Wt::WOverlayLoadingIndicator());
        viewPort = new Wt::WContainerWidget(root());
        viewPort->resize(Wt::WLength::Auto, 600);

        Wt::WVBoxLayout *layout = new Wt::WVBoxLayout();
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        viewPort->setLayout(layout);

        /*begin Head Panel*/
        Wt::Ext::Panel *north = new Wt::Ext::Panel();

        north->setBorder(false);
        std::string h = "MediaEncodingCluster V-";
        h += MHIVE_VERSION;
        h += "($Rev: 893 $-"__DATE__ "-" __TIME__")";
        Wt::WText *head = new Wt::WText(h);
        head->setStyleClass("north");
        north->setLayout(new Wt::WFitLayout());
        north->layout()->addWidget(head);
        north->resize(Wt::WLength(), 35);
        layout->addWidget(north);
        /*end Head Panel*/

        /*begin Menu Panel*/
        MainMenu * menu = new MainMenu(this);
        layout->addWidget(menu);
        /*end Menu Panel*/


        /*begin Main Panel*/
        main_panel = new Wt::Ext::Panel();
        main_panel->setBorder(false);
        layout->addWidget(main_panel, 1);
        /*end Main Panel*/

        /*begin Footer Panel*/
        Wt::Ext::Panel *footer = new Wt::Ext::Panel();
        footer->setBorder(false);
        head = new Wt::WText("&copy; 2009 <a target=\"_blank\" href=\"http://codergrid.de/\">Jan Hölscher</a> - GPL License");
        head->setStyleClass("north");
        footer->setLayout(new Wt::WFitLayout());
        footer->layout()->addWidget(head);
        footer->resize(Wt::WLength(), 35);
        layout->addWidget(footer);
        /*end Footer Panel*/

        useStyleSheet("ext/resources/css/xtheme-gray.css");


      }

      void WebApp2::listAllFiles() {
        SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, size from files "));
        tab->resize(800, 500);
        tab->setColumnWidth(0, 10);
        tab->setColumnWidth(2, 10);
        tab->setColumnWidth(3, 20);
        setContent(tab);
      }

      void WebApp2::listImportedFiles() {
        SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, size from files where parent=0"));
        tab->resize(800, 500);
        tab->setColumnWidth(0, 10);
        tab->setColumnWidth(2, 10);
        tab->setColumnWidth(3, 20);
        setContent(tab);
      }

      void WebApp2::listEncodedFiles() {
        SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, size from files where parent>0"));
        tab->resize(800, 500);
        tab->setColumnWidth(0, 10);
        tab->setColumnWidth(2, 10);
        tab->setColumnWidth(3, 20);
        setContent(tab);
      }

      void WebApp2::listAllEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid order by 2,f.id DESC"));
        tab->resize(800, 500);
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listPendingEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) = 0 order by 2,f.id DESC"));
        tab->resize(800, 500);
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listActiveEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) > 0 and round(count(complete)/count(*)*100,2) < 100 order by 2,f.id DESC"));
        tab->resize(800, 500);
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listSuccessfullEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) = 100 order by 2,f.id DESC"));
        tab->resize(800, 500);
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        setContent(tab);
      }

      void WebApp2::listFailureEncodings() {
      }

      void WebApp2::listAllProfiles() {
        Profiles * profiles=new Profiles();
        setContent(profiles);
      }

      void WebApp2::createProfiles() {
      }

      void WebApp2::listAllWatchfolder() {
        WatchFolder * wf=new WatchFolder(0);
        setContent(wf);
      }

      void WebApp2::createWatchfolder() {
      }

      void WebApp2::editSystemConfiguration() {
      }

      void WebApp2::setContent(Wt::WWidget * w) {
        if (main_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = main_panel->layout()->itemAt(0);
          main_panel->layout()->removeItem(item);
        }
        main_panel->layout()->addWidget(w);
      }
    }
  }
}

