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
namespace org {
  namespace esb {
    namespace web {

      class PresetList : public Wt::Ext::TableView {
      public:
        PresetList();
        virtual ~PresetList();
        Wt::Signal<std::string> presetSelected;
      private:
        void selectionChanged();
      };
    }
  }
}

#endif	/* PRESETLIST_H */

