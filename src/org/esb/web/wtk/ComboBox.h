/* 
 * File:   ComboBox.h
 * Author: HoelscJ
 *
 * Created on 8. Oktober 2010, 13:19
 */

#ifndef COMBOBOX_H
#define	COMBOBOX_H
#include "Wt/Ext/ComboBox"
#include "Wt/WAbstractItemModel"
namespace org {
  namespace esb {
    namespace web {

      class ComboBox: public Wt::Ext::ComboBox {
      public:
        ComboBox();
        virtual ~ComboBox();
        void setModel(Wt::WAbstractItemModel *model);
        Wt::WAbstractItemModel * getModel();
        bool setSelectedEntry(std::string key, int index=0);
        std::string currentSelected(int col=0);
        std::string data(int row,int col=0);
        void setEditable(bool how);
        bool isEditable();
      private:
        Wt::WAbstractItemModel * _model;
        bool _isEditable;
      };
    }
  }
}

#endif	/* COMBOBOX_H */

