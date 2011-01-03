/* 
 * File:   WebApp2.h
 * Author: jhoelscher
 *
 * Created on 26. August 2009, 13:13
 */
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WSignalMapper>
#include <Wt/WEvent>
#include <Wt/Ext/Panel>
#include <Wt/Ext/Container>
#include "Wt/WBorderLayout"
//#include "SqlTable.h"
#include "JobTable.h"
#include "ProjectTable.h"
#ifndef _WEBAPP2_H
#define	_WEBAPP2_H
namespace org {
  namespace esb {
    namespace web {
      class Login;
      class WebApp2 : public Wt::WApplication {
      public:
        WebApp2(const Wt::WEnvironment & env);
        //~WebApp2();

        //            private:
        void listProjects();
        void createProject();

        void listAllFiles();
        void listImportedFiles();
        void listEncodedFiles();

        void listAllEncodings();
        void listPendingEncodings();
        void listActiveEncodings();
        void listSuccessfullEncodings();
        void listFailureEncodings();

        void listAllProfiles();
        void createProfiles();
        void profilesCreated();
        void listAllWatchfolder();
        void createWatchfolder();
        void watchfolderCreated();
        void editSystemConfiguration();
        void viewNodes();

        void openConfiguration();
        void shutdown();

        void openPreview();
      private:
        void buildGui();
        void setContent(Wt::WWidget * w);
        Wt::Ext::Panel * main_panel;
        Wt::Ext::Panel * info_panel;
        Wt::Ext::Panel * object_panel;
        Wt::Ext::Container *viewPort;
        Wt::WBorderLayout *layout;
        //                Wt::Ext::Dialog * cwd;
        //                Wt::Ext::Dialog * cpd;
        //Wt::WSignalMapper<SqlTable*> *_fileSignalMap;
        Wt::WSignalMapper<JobTable*> *_jobSignalMap;
        //void fileSelected(SqlTable *);
        void jobSelected(JobTable *);
        Wt::WSignalMapper<ProjectTable*> *_projectSignalMap;
        //void fileSelected(SqlTable *);
        void projectSelected(ProjectTable *);
        //void fileSelected(SqlTable *);
        void presetSelected(int);
        void presetSelected2(std::string);
        Ptr<db::HiveDb> _db;
        void authenticated(db::User);
        bool _isAuthenticated;
        Login * login;
      };
    }
  }
}
#endif	/* _WEBAPP2_H */

