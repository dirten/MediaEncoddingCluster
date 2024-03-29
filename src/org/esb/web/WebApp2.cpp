#include "Wt/Ext/MessageBox"
#include "DbTable.h"

#include "ColumnConfig.h"
#include "WebApp2.h"
#include "config.h"
#include <signal.h>
#include "MainMenu.h"
#include <Wt/Ext/Container>
#include <Wt/WContainerWidget>
#include <Wt/WLayoutItem>
#include <Wt/WVBoxLayout>
#include <Wt/WBorderLayout>
#include <Wt/WFitLayout>
#include <Wt/WAccordionLayout>
#include <Wt/WText>
#include <Wt/WLength>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/Ext/Panel>

#include <Wt/Ext/Splitter>
//#include <Files.cpp>
#include "Profiles.cpp"
#include "Projects.h"

#include "ProfilesForm.h"
//#include "WatchFolder.cpp"
//#include "WatchfolderForm.h"
#include "Configuration.cpp"

//#include "FileInfo.cpp"
//#include "StreamInfo.h"
#include "ProfileCreator.h"

#include "introspec.h"
#include "project/PreviewPanel.h"
#include "JobTable.h"
#include "ProjectTable.h"
#include "project_v2/ProjectWizard.h"
#include "MediaFileTable.h"

#include "TreeMainMenu.h"
#include "job/JobInfoPanel.h"
#include "system/NodeListPanel.h"
#include "presets/PresetsEditorWindow.h"
#include "presets/PresetsEditor.h"
#include "org/esb/hive/DatabaseService.h"
#include "project/ProjectWizard.h"
#include "presets/PresetList.h"
#include "Login.cpp"
/*on windows there is a macro defined with name MessageBox*/
#ifdef MessageBox
#undef MessageBox
#endif
namespace org {
  namespace esb {
    namespace web {

      WebApp2::WebApp2(const Wt::WEnvironment & env) :
      WApplication(env) {

        if (string(org::esb::config::Config::getProperty("hive.mode", "no")) == "setup") {
          WApplication::instance()->redirect("/setup");
          WApplication::instance()->quit();
        }
        _isAuthenticated = false;
        /*
         std::string syslog_file = org::esb::config::Config::getProperty("hive.base_path");
         syslog_file.append("/sys.log");
         Wt::WApplication::readConfigurationProperty("log-file", const_cast<char*> (syslog_file.c_str()));
         */
        std::string h = "MediaEncodingCluster ";
        h.append("( ").append(MHIVE_VERSION).append(" " __DATE__ "-" __TIME__ ")");
        setLoadingIndicator(new Wt::WOverlayLoadingIndicator());
        setTitle(h);
        useStyleSheet("filetree.css");
        useStyleSheet("main.css");
        _db = new db::HiveDb(org::esb::hive::DatabaseService::getDatabase());

        viewPort = new Wt::Ext::Container(root());
        layout = new Wt::WBorderLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        viewPort->setLayout(layout);



        //useStyleSheet("ext/resources/css/xtheme-slate.css");
        useStyleSheet("ext/resources/css/xtheme-gray.css");
        useStyleSheet("main.css");

        /**
         * Signal Map for the SqlTables and the Detail view
         */
        _jobSignalMap = new Wt::WSignalMapper<JobTable *>(this);
        _jobSignalMap->mapped().connect(SLOT(this, WebApp2::jobSelected));
        _projectSignalMap = new Wt::WSignalMapper<ProjectTable *>(this);
        _projectSignalMap->mapped().connect(SLOT(this, WebApp2::projectSelected));
        /*begin Menu Panel*/
        menu_panel = new Wt::Ext::Panel();
        menu_panel->setLayout(new Wt::WFitLayout());
        menu_panel->resize(Wt::WLength::Auto, 30);
        menu_panel->hiddenKeepsGeometry();
        layout->addWidget(menu_panel, Wt::WBorderLayout::North);
        /*end Menu Panel*/

        /*begin Main Panel*/
        main_panel = new Wt::Ext::Panel();
        Wt::WFitLayout * fit = new Wt::WFitLayout();
        main_panel->setLayout(fit);
        main_panel->resize(600, Wt::WLength());
        main_panel->setResizable(true);
        layout->addWidget(main_panel, Wt::WBorderLayout::Center);
        /*end Main Panel*/

        object_panel = new Wt::Ext::Panel();
        object_panel->setResizable(true);
        object_panel->setLayout(new Wt::WFitLayout());
        object_panel->setBorder(false);
        object_panel->resize(Wt::WLength(), 200);
        layout->addWidget(object_panel, Wt::WBorderLayout::South);
        object_panel->collapse();
        login = NULL;
        if (org::esb::config::Config::get("authentication") == "true") {
          showLogin();
        } else {
          _user = new db::User(*_db.get());
          _user->authname="none";
          buildGui();
        }
      }

      void WebApp2::showLogin() {
        delete login;
        login = new Login();
        login->authenticated.connect(SLOT(this, WebApp2::authenticated));
        login->show();
      }

      void WebApp2::logout() {
        WApplication::instance()->redirect("/");
        WApplication::instance()->quit();
        /*
        _isAuthenticated = false;
        menu_panel->layout()->removeItem(menu_panel->layout()->itemAt(0));
        showLogin();*/
      }

      void WebApp2::buildGui() {

        //Wt::WLayoutItem * item = layout->itemAt(Wt::WBorderLayout::North);
        /*begin Menu Panel*/
        MainMenu * menu = new MainMenu(this);
        menu->resize(Wt::WLength::Auto, 30);
        menu->hiddenKeepsGeometry();
        menu->setUser(_user);
        menu_panel->layout()->addWidget(menu);
        //layout->addWidget(menu, Wt::WBorderLayout::North);
        /*end Menu Panel*/

        object_panel->expand();
      }

      void WebApp2::authenticated(db::User user) {
        _isAuthenticated=true;
        _user = new db::User(user);
        login->accept();
        buildGui();
      }
        Ptr<db::User> WebApp2::getUser(){
          return _user;
        }
      void WebApp2::openPreview() {
        Wt::Ext::Dialog *dil = new Wt::Ext::Dialog();
        dil->contents()->addWidget(new PreviewPanel());

        dil->addButton(new Wt::Ext::Button("Cancel"));
        dil->buttons().back()->clicked().connect(SLOT(dil, Wt::Ext::Dialog::reject));

        dil->show();

        dil->exec();
        delete dil;

      }

      void WebApp2::listProjects() {
        //        Projects * p = new Projects();
        ProjectTable * p = new ProjectTable();
        //_projectSignalMap->mapConnect(p->itemSelectionChanged(), p);
        setContent(p);
      }

      void WebApp2::createProject() {
        ProjectTable * p = new ProjectTable();
        setContent(p);
        p->createProject();
      }

      void WebApp2::listAllFiles() {
        MediaFileTable * t = new MediaFileTable();
        setContent(t);

        /*
                SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, concat(round(size/1024/1024,2),' MB') as size, concat(round(duration/1000000),' sec.') as duration from files "));
                tab->setColumnWidth(0, 10);
                tab->setColumnWidth(2, 10);
                tab->setColumnWidth(3, 20);
                tab->setColumnWidth(4, 20);
                std::string t = "double click to edit Meta information";
                tab->setToolTip(t);
                //_fileSignalMap->mapConnect(tab->doubleClicked, tab);
                _fileSignalMap->mapConnect(tab->itemSelectionChanged, tab);
                info_panel->expand();*/

      }

      void WebApp2::listImportedFiles() {
        list<ColumnConfig> columnConfigs;
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Id, "Id", 20));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Path, "Path", 200));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Filename, "Filename", 300));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Containertype, "Type", 40));
        DbTable * table = new DbTable(columnConfigs, db::MediaFile::Parent == 0);
        setContent(table);
        /*
                SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, concat(round(size/1024/1024,2),' MB') as size , concat(round(duration/1000000),' sec.') as duration from files where parent=0"));
                tab->setColumnWidth(0, 10);
                tab->setColumnWidth(2, 10);
                tab->setColumnWidth(3, 20);
                tab->setColumnWidth(4, 20);
                _fileSignalMap->mapConnect(tab->itemSelectionChanged, tab);
                setContent(tab);*/
      }

      void WebApp2::listEncodedFiles() {
        list<ColumnConfig> columnConfigs;
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Id, "Id", 20));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Path, "Path", 200));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Filename, "Filename", 300));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Containertype, "Type", 40));
        DbTable* table = new DbTable(columnConfigs, db::MediaFile::Parent > 0);
        setContent(table);

        /*        SqlTable * tab = new SqlTable(std::string("select id, filename, container_type type, concat(round(size/1024/1024,2),' MB') as size, concat(round(duration/1000000),' sec.') as duration from files where parent>0"));
                tab->setColumnWidth(0, 10);
                tab->setColumnWidth(2, 10);
                tab->setColumnWidth(3, 20);
                tab->setColumnWidth(4, 20);
                _fileSignalMap->mapConnect(tab->itemSelectionChanged, tab);
                setContent(tab);*/
      }

      void WebApp2::listAllEncodings() {
        //        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid order by 2,f.id DESC"));
        //        SqlTable * tab = new SqlTable(std::string("select outfiles.filename,ifnull(round(max((end_ts-instreams.start_time)*instreams.time_base_num/instreams.time_base_den)/(infiles.duration/1000000),3)*100,0) as progress,ifnull(min(send),0) as start_time,ifnull(sum(timestampdiff(SECOND,send,process_units.complete)),0)\"cpu-time\" from jobs, files infiles, files outfiles, job_details, streams instreams, streams outstreams left join process_units on(process_units.target_stream=outstreams.id) where inputfile=infiles.id and outputfile=outfiles.id and jobs.id=job_details.job_id and instream=instreams.id and outstream=outstreams.id group by outfiles.id"));
        //        SqlTable * tab = new SqlTable(std::string("select outfiles.filename,ifnull(round(min(((select max(end_ts) from process_units pinner where pinner.target_stream=outstreams.id)-instreams.start_time)*instreams.time_base_num/instreams.time_base_den)/(infiles.duration/1000000),3)*100,0) as progress,ifnull(min(send),0) as start_time,ifnull(sum(timestampdiff(SECOND,send,process_units.complete)),0)\"cpu-time\" from jobs, files infiles, files outfiles, job_details, streams instreams, streams outstreams left join process_units on(process_units.target_stream=outstreams.id) where inputfile=infiles.id and outputfile=outfiles.id and jobs.id=job_details.job_id and instream=instreams.id and outstream=outstreams.id group by outfiles.id"));
        //SELECT filename ,min(round(((last_pts-(((start_time/time_base_den)*time_base_num)*1000000))/files.duration)*100)), begin FROM files, jobs, job_details, streams where files.id=jobs.outputfile and jobs.id=job_details.job_id and job_details.instream=streams.id group by files.id
        /*
                SqlTable
         * tab =
                        new SqlTable(
                            std::string(
                                "SELECT filename ,if(min(round(((last_dts-(((start_time/time_base_den)*time_base_num)*1000000))/files.duration)*100))<0,0,min(round(((last_dts-(((start_time/time_base_den)*time_base_num)*1000000))/files.duration)*100))) as progress, begin FROM files, jobs, job_details, streams where files.id=jobs.outputfile and jobs.id=job_details.job_id and job_details.instream=streams.id group by files.id"));
                tab->setColumnWidth(1, 10);
                tab->setColumnWidth(2, 20);
                tab->setColumnWidth(3, 20);
                tab->setColumnWidth(4, 10);
                _jobSignalMap->mapConnect(tab->itemSelectionChanged, tab);
                setContent(tab);*/
        /*
                list<ColumnConfig> columnConfigs;
                columnConfigs.push_back(ColumnConfig(db::Job::Id,"Id" ,200));
                columnConfigs.push_back(ColumnConfig(db::Job::Begintime,"BeginTime" ,200));
                columnConfigs.push_back(ColumnConfig(db::Job::Endtime,"EndTime" ,200));
                columnConfigs.push_back(ColumnConfig(db::Job::Progress,"Progress" ,200));
                DbTable * table= new DbTable(columnConfigs,litesql::Expr());
         */
        /*
       Wt::Ext::TableView *view = new Wt::Ext::TableView();

       db::HiveDb dbCon("mysql", org::esb::config::Config::getProperty("db.url"));
       std::vector<db::Job> jobs = litesql::select<db::Job > (dbCon).all();
       view->setModel(new JobTableModel(jobs));
       view->setAlternatingRowColors(true);
       view->resizeColumnsToContents(true);
       view->setHighlightMouseOver(true);
       view->setSelectionBehavior(Wt::SelectRows);
       view->setSelectionMode(Wt::SingleSelection);
       view->setColumnWidth(2,35);
       view->setColumnWidth(3,35);
       view->setColumnWidth(4,35);
       view->setColumnWidth(5,25);
       view->setColumnWidth(6,15);
        
       std::string renderer= "function change(val) {"
               "if (val > 0){"
               "return '<span style=\"color:green;\">' + val + '</span>';"
               "} else if(val < 0) {"
               "return '<span style=\"color:red;\">' + val + '</span>';"
               "}"
               "return val;"
               "}";
       view->setRenderer(5, renderer);
       renderer= "function change(val) {"
               "if (val == \"running\"){"
               "return '<img src=\"/icons/encoding-in-progress.gif\"/>';"
               "} else if(val == \"failed\") {"
               "return '<img src=\"/icons/remove-icon.png\"/>';"
               "} else if(val == \"exported\") {"
               "return '<img src=\"/icons/accept-icon.png\"/>';"
               "} else if(val == \"queued\") {"
               "return '<img src=\"/icons/queued-icon.png\"/>';"
               "}"
               "return val;"
               "}";
       view->setRenderer(6, renderer);
         */
        JobTable *table = new JobTable();
        _jobSignalMap->mapConnect(table->itemSelectionChanged(), table);
        setContent(table);

      }

      void WebApp2::listPendingEncodings() {
      }

      void WebApp2::listActiveEncodings() {
      }

      void WebApp2::listSuccessfullEncodings() {
      }

      void WebApp2::listFailureEncodings() {
      }

      void WebApp2::listAllProfiles() {
        PresetList * list = new PresetList();
        setContent(list);
        //        list->presetSelected.connect(SLOT(this, WebApp2::presetSelected2));
        //Profiles * profiles = new Profiles();
        //profiles->profileSelected.connect(SLOT(this, WebApp2::presetSelected));
        //setContent(profiles);
      }

      void WebApp2::createProfiles() {
        PresetList * list = new PresetList();
        setContent(list);

        PresetsEditorWindow * edit = new PresetsEditorWindow("");
        edit->show();
        if (edit->exec() == Wt::Ext::Dialog::Accepted) {
          list->refresh();
        }
        delete edit;
      }

      void WebApp2::openConfiguration() {

      }

      void WebApp2::profilesCreated() {
        listAllProfiles();
        //        delete cpd;
      }

      void WebApp2::listAllWatchfolder() {
        //        WatchFolder * wf = new WatchFolder(0);
        //        setContent(wf);
      }

      void WebApp2::createWatchfolder() {
        //        WatchFolder * wf = new WatchFolder(0);
        //        setContent(wf);
        //        wf->createWatchFolder();
      }

      void WebApp2::watchfolderCreated() {
        listAllWatchfolder();
        //        delete cwd;
      }

      void WebApp2::editSystemConfiguration() {
        Configuration * conf = new Configuration();
        setContent(conf);
      }

      void WebApp2::viewNodes() {
        org::esb::web::NodeListPanel * panel = new org::esb::web::NodeListPanel();
        setContent(panel);
      }

      void WebApp2::jobSelected(JobTable * tab) {
        if (tab == NULL)return;
        JobInfoPanel * panel = new JobInfoPanel();
        if (object_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = object_panel->layout()->itemAt(0);
          object_panel->layout()->removeItem(item);
          delete item->widget();
        }
        int id = atoi(boost::any_cast<string > (tab->model()->data(tab->selectedRows()[0], 0)).c_str());
        object_panel->layout()->addWidget(panel);
        panel->setJob(id);
      }

      void WebApp2::projectSelected(ProjectTable * tab) {
        LOGDEBUG("void WebApp2::projectSelected(ProjectTable * tab)");
        if (tab == NULL)return;
        v2::ProjectWizard * panel = NULL;
        if (object_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = object_panel->layout()->itemAt(0);
          LOGDEBUG("Item name" << typeid (*item->widget()).name());
          if (!instanceOf(*item->widget(), org::esb::web::v2::ProjectWizard)) {
            object_panel->layout()->removeItem(item);
            delete item->widget();
            panel = new v2::ProjectWizard();
            object_panel->layout()->addWidget(panel);
          } else {
            panel = static_cast<org::esb::web::v2::ProjectWizard*> (item->widget());
            if (!panel)
              LOGERROR("could not cast item to org::esb::web::v2::ProjectWizard*");
          }
        } else {
          panel = new v2::ProjectWizard();
          object_panel->layout()->addWidget(panel);
        }
        int id = atoi(boost::any_cast<string > (tab->model()->data(tab->selectedRows()[0], 0)).c_str());
        panel->open(id);

      }

      void WebApp2::presetSelected(int presetid) {
        Ptr<db::Profile> profile = new db::Profile(litesql::select<db::Profile > (*_db.get(), db::Profile::Id == presetid).one());
        PresetsEditor * editor = new PresetsEditor(profile);
        if (object_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = object_panel->layout()->itemAt(0);
          object_panel->layout()->removeItem(item);
          delete item->widget();
        }
        object_panel->layout()->addWidget(editor);
      }

      void WebApp2::presetSelected2(std::string filename) {
        LOGDEBUG("preset selected");
        PresetsEditor * editor = new PresetsEditor(filename);
        if (object_panel->layout()->count() > 0) {
          Wt::WLayoutItem * item = object_panel->layout()->itemAt(0);
          object_panel->layout()->removeItem(item);
          delete item->widget();
        }
        object_panel->layout()->addWidget(editor);
      }

      void WebApp2::setContent(Wt::WWidget * w) {
        if (org::esb::config::Config::get("authentication") == "true" && !_isAuthenticated) {
          showLogin();
        } else {
          if (main_panel->layout()->count() > 0) {
            Wt::WLayoutItem * item = main_panel->layout()->itemAt(0);
            main_panel->layout()->removeItem(item);
            delete item->widget();
          }
          main_panel->layout()->addWidget(w);
          main_panel->refresh();
        }
        //        main_panel->layout()->addWidget(w);
      }

      void WebApp2::shutdown() {
        Wt::Ext::MessageBox *box = new Wt::Ext::MessageBox("Shutdown Media Encoding Cluster", "do you really want to shutdown the Media Encoding Cluster<br/>this will abort all currently running operations!!!", Wt::Warning, Wt::Ok | Wt::Cancel);
        if (!box->exec() == Wt::Ext::Dialog::Accepted)
          return;
        LOGDEBUG("shutdown from webinterface!");
#ifdef __WIN32__
        HANDLE hProcess;
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId());
        TerminateProcess(hProcess, (DWORD) - 1);
        CloseHandle(hProcess);
#else
        ::kill(getpid(), 9);
#endif
      }

    }
  }
}

