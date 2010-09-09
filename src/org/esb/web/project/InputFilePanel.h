/* 
 * File:   InputFilePanel.h
 * Author: HoelscJ
 *
 * Created on 13. Juli 2010, 17:09
 */

#ifndef INPUTFILEPANEL_H
#define	INPUTFILEPANEL_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/io/File.h"
#include "Wt/Ext/Panel"
#include "org/esb/lang/Ptr.h"
#include "org/esb/web/FileChooser.h"
namespace org {
  namespace esb {
    namespace io{
      class File;
    }
    namespace web {
      class InputFileTable;
      
      class InputFilePanel : public Wt::Ext::Panel {
        classlogger("org.esb.web.project.InputFilePanel")
      public:
        InputFilePanel();
        InputFilePanel(const InputFilePanel& orig);
        virtual ~InputFilePanel();
        void setProject(boost::shared_ptr<db::Project>);
        void setInputFile(std::list<Ptr<org::esb::io::File> >path);
      private:
        void addVideoButtonClicked();
        void removeVideo();
        boost::shared_ptr<db::Project> _project;
        boost::shared_ptr<InputFileTable> _filetable;
        Wt::Ext::Button * removeVideoButton ;
        void enableButtons();
        Ptr<FileChooser> _chooser;
        void importFile(Ptr<org::esb::io::File> file);
        
      };

    }
  }
}

#endif	/* INPUTFILEPANEL_H */

