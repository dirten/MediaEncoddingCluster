/* 
 * File:   WebApp2.h
 * Author: jhoelscher
 *
 * Created on 26. August 2009, 13:13
 */

#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WSignalMapper>
#include <Wt/WEvent>
#include <Wt/Ext/Panel>
#include <Wt/Ext/Container>
#include "Wt/WBorderLayout"
#include "SqlTable.h"
#include "JobTable.h"
#ifndef _WEBAPP2_H
#define	_WEBAPP2_H
namespace org {
    namespace esb {
        namespace web {

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

                void openConfiguration();


                void openPreview();
            private:
                void setContent(Wt::WWidget * w);
                Wt::Ext::Panel * main_panel;
                Wt::Ext::Panel * info_panel;
                Wt::Ext::Panel * object_panel;
                Wt::Ext::Container *viewPort;
                Wt::WBorderLayout *layout;
//                Wt::Ext::Dialog * cwd;
//                Wt::Ext::Dialog * cpd;
                Wt::WSignalMapper<SqlTable*> *_fileSignalMap;
                Wt::WSignalMapper<JobTable*> *_jobSignalMap;
                void fileSelected(SqlTable *);
                void jobSelected(JobTable *);


            };
        }
    }
}
#endif	/* _WEBAPP2_H */

