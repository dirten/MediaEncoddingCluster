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


#ifndef _PROFILECREATOR_H
#define	_PROFILECREATOR_H

namespace org {
  namespace esb {
    namespace web {

      class ProfileCreator : public Wt::Ext::Dialog {
      public:
        ProfileCreator();
        virtual ~ProfileCreator();

        void show();
        void setProfile(int id);
        Wt::Signal<void> saved;
        Wt::Signal<void> canceled;

      private:
        void cancel();
        void save();

        Wt::Ext::TabWidget * tab;
        //        wtk::ElementContainer<Wt::Ext::LineEdit> _el;
        //        wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;

        /**
         * private class GeneralPanel
         */
        class BasePanel : public Wt::Ext::Panel {
        public:
          BasePanel();
          virtual ~BasePanel();
          void addWidget(Wt::WWidget *widget, int row, int col);
          virtual std::map<std::string, std::string> getKeyValue();
          virtual void setKeyValue(std::map<std::string, std::string>);

        protected:
          Wt::WContainerWidget * _cont;
          wtk::ElementContainer<Wt::Ext::LineEdit> _el;
          wtk::ElementContainer<Wt::Ext::ComboBox> _elcb;

        };

        /**
         * private class GeneralPanel
         */
        class GeneralPanel : public BasePanel {
        public:
          GeneralPanel();
          virtual ~GeneralPanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:

        };

        /**
         * private class FilePanel
         */
        class FilePanel : public BasePanel {
        public:
          FilePanel();
          virtual ~FilePanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:
          void setAvailableExtensions();

        };

        /**
         * private class FilePanel
         */
        class VideoPanel : public BasePanel {
        public:
          VideoPanel();
          virtual ~VideoPanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:

        };

        /**
         * private class FilePanel
         */
        class AudioPanel : public BasePanel {
        public:
          AudioPanel();
          virtual ~AudioPanel();
          std::map<std::string, std::string> getKeyValue();
          void setKeyValue(std::map<std::string, std::string>);
        private:

        };

      };

    }
  }
}


#endif	/* _PROFILECREATOR_H */

