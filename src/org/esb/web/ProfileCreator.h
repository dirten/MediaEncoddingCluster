/*----------------------------------------------------------------------
*  File    : ProfileCreator.h
*  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
*  Purpose :
*  Created : 6. Januar 2010, 13:59 by Jan Hölscher <jan.hoelscher@esblab.com>
*
*
* MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
*
* This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of the
*  License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
*  02111-1307 USA
*
* ----------------------------------------------------------------------
*/
#include "org/esb/db/hivedb.hpp"
#include <map>
#include "Wt/Ext/Dialog"
#include "Wt/Ext/Panel"
#include "Wt/Ext/LineEdit"
#include "Wt/Ext/ComboBox"
#include "Wt/WContainerWidget"
#include "wtk/ElementContainer.h"
#include "Wt/WWidget"
#include "Wt/WSignal"

#include "org/esb/av/AV.h"
#include "org/esb/util/Log.h"

#ifndef _PROFILECREATOR_H
#define	_PROFILECREATOR_H

namespace org {
  namespace esb {
    namespace web {

      class ProfileCreator : public Wt::Ext::Dialog {
        classlogger("org.esb.web.ProfileCreator")
      public:
        ProfileCreator();
        ProfileCreator(db::Profile&profile);
        virtual ~ProfileCreator();

        void show();
        void setProfile(int id);
        Wt::Signal<void> saved;
        Wt::Signal<void> canceled;

      private:
        void cancel();
        void save();

        Wt::Ext::TabWidget * tab;
        void setContextText(int tab_index);
        //        wtk::ElementContainer<Wt::Ext::LineEdit> _el;
        //        wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;

        /**
        * private class GeneralPanel
        */
        class BasePanel : public Wt::Ext::Panel {
          classlogger("org.esb.web.ProfileCreator.BasePanel")
        public:
          //          BasePanel();
          BasePanel(db::Profile&profile);
          virtual ~BasePanel();
          void addWidget(Wt::WWidget *widget, int row, int col);
          virtual std::map<std::string, std::string> getKeyValue();
          virtual void setKeyValue(std::map<std::string, std::string>);
          virtual void changed()=0;
          void setChangedListener();

        protected:
          Wt::WContainerWidget * _cont;
          wtk::ElementContainer<Wt::Ext::LineEdit> _el;
          wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;
          db::Profile & _profile;
        };

        /**
        * private class GeneralPanel
        */
        class GeneralPanel : public BasePanel {
          classlogger("org.esb.web.ProfileCreator.GeneralPanel")
        public:
          //          GeneralPanel();
          GeneralPanel(db::Profile&profile);
          virtual ~GeneralPanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:
          void changed();
        };

        /**
        * private class FilePanel
        */
        class FilePanel : public BasePanel {
          classlogger("org.esb.web.ProfileCreator.FilePanel")
        public:
          //          FilePanel();
          FilePanel(db::Profile&profile);
          virtual ~FilePanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:
          void setAvailableExtensions();
          void changed();

        };

        /**
        * private class FilePanel
        */
        class VideoPanel : public BasePanel {
          classlogger("org.esb.web.ProfileCreator.VideoPanel")
        public:
          //          VideoPanel();
          VideoPanel(db::Profile&profile);
          virtual ~VideoPanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:
          void setPredefinedCodecFlags();
          void setSelectedPredifinedCodecFlags();
          void changed();
        };

        /**
        * private class FilePanel
        */
        class AudioPanel : public BasePanel {
          classlogger("org.esb.web.ProfileCreator.AudioPanel")
        public:
          //          AudioPanel();
          AudioPanel(db::Profile&profile);
          virtual ~AudioPanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:
          void changed();

        };

      };

    }
  }
}


#endif	/* _PROFILECREATOR_H */

