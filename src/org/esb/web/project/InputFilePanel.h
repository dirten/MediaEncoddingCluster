/* 
 * File:   InputFilePanel.h
 * Author: HoelscJ
 *
 * Created on 13. Juli 2010, 17:09
 */

#ifndef INPUTFILEPANEL_H
#define	INPUTFILEPANEL_H
#include "org/esb/io/File.h"
#include "Wt/Ext/Panel"
namespace org {
  namespace esb {
    namespace web {

      class InputFilePanel : public Wt::Ext::Panel {
        classlogger("org.esb.web.project.InputFilePanel")
      public:
        InputFilePanel();
        InputFilePanel(const InputFilePanel& orig);
        virtual ~InputFilePanel();
        void setInputFile(org::esb::io::File path);
      private:
        void addVideoButtonClicked();
      private:

      };

    }
  }
}

#endif	/* INPUTFILEPANEL_H */

