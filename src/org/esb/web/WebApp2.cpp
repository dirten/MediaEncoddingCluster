#include "DbTable.h"
#include "ColumnConfig.h"
#include "WebApp2.h"
#include "config.h"

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

//#include <Files.cpp>
#include "Profiles.cpp"
#include "Projects.h"
#include "ProfilesForm.h"
#include "WatchFolder.cpp"
#include "WatchfolderForm.h"
#include "Configuration.cpp"

#include "FileInfo.cpp"
#include "StreamInfo.h"
#include "ProfileCreator.h"


#include "project/PreviewPanel.h"
namespace org {
  namespace esb {
    namespace web {

      WebApp2::WebApp2(const Wt::WEnvironment & env) :
        WApplication(env) {
        if (string(org::esb::config::Config::getProperty("hive.mode")) == "setup") {
          WApplication::instance()->redirect("/setup");
          WApplication::instance()->quit();
        }/*
         std::string syslog_file = org::esb::config::Config::getProperty("hive.base_path");
         syslog_file.append("/sys.log");
         Wt::WApplication::readConfigurationProperty("log-file", const_cast<char*> (syslog_file.c_str()));
         */
        std::string h = "MediaEncodingCluster V-";
        h += MHIVE_VERSION;
        h += "($Revision: 0 $-"__DATE__ "-" __TIME__")";
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
        /*
        Wt::Ext::Panel *north = new Wt::Ext::Panel();
        north->setBorder(false);
       
        head->setStyleClass("north");
        north->setLayout(new Wt::WFitLayout());
        north->layout()->addWidget(head);
        north->resize(Wt::WLength(), 35);*/
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

        /*begin Info Panel*/
        info_panel = new Wt::Ext::Panel();

        Wt::WAccordionLayout * info_layout = new Wt::WAccordionLayout();
        //Wt::WFitLayout * info_fit = new Wt::WFitLayout();

        info_panel->setLayout(info_layout);
        info_panel->setCollapsible(true);
        info_panel->setAnimate(true);

        //        Wt::Ext::Panel *p=new Wt::Ext::Panel();
        //        p->setTitle("File Details");
        //        info_panel->layout()->addWidget(p);
        /*
         p=new Wt::Ext::Panel();
         p->setTitle("Video Details");
         info_panel->layout()->addWidget(p);
         p=new Wt::Ext::Panel();
         p->setTitle("Audio Details");
         info_panel->layout()->addWidget(p);*/
        info_panel->expand();
        /*
         info_layout->addWidget(new Wt::Ext::Panel());
         info_layout->addWidget(new Wt::Ext::Panel());
         */
        info_panel->resize(250, Wt::WLength());
        info_panel->setResizable(true);
        layout->addWidget(info_panel, Wt::WBorderLayout::East);
        /*end Info Panel*/

        /*begin Footer Panel*/
        Wt::Ext::Panel *footer = new Wt::Ext::Panel();
        footer->setBorder(false);
        Wt::WText *head = new Wt::WText("&copy; 2000 - 2010 <a target=\"_blank\" href=\"http://codergrid.de/\">CoderGrid.de</a> - GPL License");
        head->setStyleClass("north");
        footer->setLayout(new Wt::WFitLayout());
        footer->layout()->addWidget(head);
        footer->resize(Wt::WLength(), 35);
        layout->addWidget(footer, Wt::WBorderLayout::South);
        /*end Footer Panel*/

        //useStyleSheet("ext/resources/css/xtheme-slate.css");
        useStyleSheet("ext/resources/css/xtheme-gray.css");

        /**
         * Signal Map for the SqlTables and the Detail view
         */
        /*
        _fileSignalMap = new Wt::WSignalMapper<SqlTable *>(this);
        _fileSignalMap->mapped.connect(SLOT(this, WebApp2::fileSelected));
        _jobSignalMap = new Wt::WSignalMapper<SqlTable *>(this);
        _jobSignalMap->mapped.connect(SLOT(this, WebApp2::jobSelected));
        */
      }

      void WebApp2::openPreview(){
        Wt::Ext::Dialog *dil=new Wt::Ext::Dialog();
        dil->contents()->addWidget(new PreviewPanel());

        dil->addButton(new Wt::Ext::Button("Cancel"));
        dil->buttons().back()->clicked.connect(SLOT(dil, Wt::Ext::Dialog::reject));

        dil->show();

        dil->exec();
        delete dil;
        
      }

      void WebApp2::listProjects() {
        Projects * p = new Projects();
        setContent(p);
      }
      void WebApp2::createProject() {
        Projects * p = new Projects();
        setContent(p);
        p->createProject();

      }
      void WebApp2::listAllFiles() {
        list<ColumnConfig> columnConfigs;
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Id,"Id" ,20));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Path,"Path" ,200));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Filename,"Filename" ,300));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Containertype,"Type" ,40));
        DbTable * table= new DbTable(columnConfigs,litesql::Expr());
        setContent(table);

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
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Id,"Id" ,20));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Path,"Path" ,200));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Filename,"Filename" ,300));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Containertype,"Type" ,40));
        DbTable * table= new DbTable(columnConfigs, db::MediaFile::Parent==0);
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
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Id,"Id" ,20));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Path,"Path" ,200));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Filename,"Filename" ,300));
        columnConfigs.push_back(ColumnConfig(db::MediaFile::Containertype,"Type" ,40));
        DbTable* table= new DbTable(columnConfigs, db::MediaFile::Parent>0);
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
        info_panel->collapse();
	
	list<ColumnConfig> columnConfigs;
        columnConfigs.push_back(ColumnConfig(db::Job::Id,"Id" ,200));
        columnConfigs.push_back(ColumnConfig(db::Job::Begintime,"BeginTime" ,200));
        columnConfigs.push_back(ColumnConfig(db::Job::Endtime,"EndTime" ,200));
        columnConfigs.push_back(ColumnConfig(db::Job::Progress,"Progress" ,200));
        DbTable * table= new DbTable(columnConfigs,litesql::Expr());
        setContent(table);
	
      }

      void WebApp2::listPendingEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) = 0 order by 2,f.id DESC"));
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        _jobSignalMap->mapConnect(tab->itemSelectionChanged, tab);
        setContent(tab);
      }

      void WebApp2::listActiveEncodings() {
        SqlTable * tab = new SqlTable(std::string(
            "SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) > 0 and round(count(complete)/count(*)*100,2) < 100 order by 2,f.id DESC"));
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        _jobSignalMap->mapConnect(tab->itemSelectionChanged, tab);
        setContent(tab);
      }

      void WebApp2::listSuccessfullEncodings() {
        SqlTable * tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid having round(count(complete)/count(*)*100,2) = 100 order by 2,f.id DESC"));
        tab->setColumnWidth(1, 10);
        tab->setColumnWidth(2, 20);
        tab->setColumnWidth(3, 20);
        tab->setColumnWidth(4, 10);
        _jobSignalMap->mapConnect(tab->itemSelectionChanged, tab);
        setContent(tab);
      }

      void WebApp2::listFailureEncodings() {
      }

      void WebApp2::listAllProfiles() {
        Profiles * profiles = new Profiles();
        //       _sqlTableSignalMap->mapConnect(profiles->itemSelectionChanged, profiles);
        setContent(profiles);
      }

      void WebApp2::createProfiles() {
        Profiles * profiles = new Profiles();
        //       _sqlTableSignalMap->mapConnect(profiles->itemSelectionChanged, profiles);
        setContent(profiles);
        profiles->createProfile();
      }

      void WebApp2::openConfiguration() {

      }

      void WebApp2::profilesCreated() {
        listAllProfiles();
        //        delete cpd;
      }

      void WebApp2::listAllWatchfolder() {
        WatchFolder * wf = new WatchFolder(0);
        setContent(wf);
      }

      void WebApp2::createWatchfolder() {
        WatchFolder * wf = new WatchFolder(0);
        setContent(wf);
        wf->createWatchFolder();
      }

      void WebApp2::watchfolderCreated() {
        listAllWatchfolder();
//        delete cwd;
      }

      void WebApp2::editSystemConfiguration() {
        Configuration * conf = new Configuration();
        setContent(conf);
      }

      void WebApp2::fileSelected(SqlTable * tab) {
        if (tab->selectedRows().size() > 0) {
          /**
           * retriving selected file from grid
           */
          std::string idstr = boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0], 0));
          /**
           * remove old widgets from the info_panel
           */
          int c = info_panel->layout()->count();
          LOGINFO("InfoPanel have "<<c<<" widgets");
          for (int a = 0; a < c; a++) {
            Wt::WLayoutItem * item = info_panel->layout()->itemAt(0);
            info_panel->layout()->removeItem(item);
          }
          /**
           * adding new items to the info panel
           */
          info_panel->layout()->addWidget(new FileInfo(atoi(idstr.c_str())));

          /**adding the streaminfo for the streams from the selected file*/
          Connection con(Config::getProperty("db.connection"));
          std::string sql = "SELECT * FROM streams WHERE fileid = ";
          sql += StringUtil::toString(atoi(idstr.c_str()));
          Statement st = con.createStatement(sql.c_str());
          ResultSet rs = st.executeQuery();
          while(rs.next()){
            info_panel->layout()->addWidget(new StreamInfo(&rs));
          }
        }
      }

      void WebApp2::jobSelected(SqlTable * tab) {
        //        logdebug("Job Table clicked:");
        if (tab->selectedRows().size() > 0) {
          std::string idstr = boost::any_cast<string>(tab->model()->data(tab->selectedRows()[0], 0));
          //          info->setData(atoi(idstr.c_str()));
          //          pSelector->setFileId(atoi(idstr.c_str()));
          //          logdebug("jobSelected" << idstr);
        }
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

