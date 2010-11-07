/* 
 * File:   PresetsEditor.h
 * Author: HoelscJ
 *
 * Created on 4. Oktober 2010, 11:52
 */

#ifndef PRESETSEDITOR_H
#define	PRESETSEDITOR_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/lang/Ptr.h"
#include "Wt/Ext/Panel"
#include "Wt/Ext/LineEdit"
#include "org/esb/hive/PresetReader.h"

#include <map>
namespace org {
  namespace esb {
    namespace web {

      class PresetsEditor:public Wt::Ext::Panel {
      public:
        PresetsEditor(Ptr<db::Profile>);
        PresetsEditor(std::string filename);
        virtual ~PresetsEditor();
        void save();
      private:
        void buildGui();
        Wt::WWidget * createTop();
        Wt::WWidget * createContent();
        Ptr<db::Profile>_profile;
        Wt::Ext::LineEdit * name;
        std::map<std::string, std::string> _presetparameter;
        org::esb::hive::PresetReader::FilterList _filterparameter;
        std::map<std::string, std::string> _audioparameter;
        std::map<std::string, std::string> _videoparameter;

        std::string _filename;
      };
    }
  }
}

#endif	/* PRESETSEDITOR_H */

