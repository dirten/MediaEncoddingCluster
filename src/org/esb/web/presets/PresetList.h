/* 
 * File:   PresetList.h
 * Author: jholscher
 *
 * Created on 31. Oktober 2010, 22:42
 */

#ifndef PRESETLIST_H
#define	PRESETLIST_H
#include "Wt/WSignal"
#include "Wt/Ext/TableView"
#include "Wt/Ext/Button"
namespace org {
  namespace esb {
    namespace web {

      class PresetList : public Wt::Ext::TableView {
      public:
        PresetList();
        virtual ~PresetList();
        Wt::Signal<std::list<std::string> > presetSelected;
        void createPreset();
        void refresh();

      private:
        void selectionChanged();
        Wt::Ext::Button * create_button;
        Wt::Ext::Button * edit_button;
        Wt::Ext::Button * delete_button;

        void editPreset();
        void deletePreset();
        void enableButtons();
        void presetSaved();

      };
    }
  }
}

#endif	/* PRESETLIST_H */

