/* 
 * File:   ComboBox.cpp
 * Author: HoelscJ
 * 
 * Created on 8. Oktober 2010, 13:19
 */

#include "ComboBox.h"
namespace org {
  namespace esb {
    namespace web {

      ComboBox::ComboBox() : Wt::Ext::ComboBox() {
        _model = NULL;
        _isEditable=false;
      }

      ComboBox::~ComboBox() {
      }

      void ComboBox::setModel(Wt::WAbstractItemModel* model) {
        _model = model;
        Wt::Ext::ComboBox::setModel(model);
      }

      bool ComboBox::setSelectedEntry(std::string key, int index) {
        bool result=false;
        if (_model) {          
          for (int a = 0; a < _model->rowCount(); a++) {
            std::string k = boost::any_cast<std::string > (_model->data(a, index));
            if (k == key) {
              setCurrentIndex(a);
              result=true;
            }
          }
        }
      }

      std::string ComboBox::currentSelected(int col) {
        std::string result=data(currentIndex(), col);
        return result;
      }
      void ComboBox::setEditable(bool how) {
        _isEditable=how;
        Wt::Ext::ComboBox::setEditable(how);
      }
      bool ComboBox::isEditable() {
        return _isEditable;
      }

      std::string ComboBox::data(int row,int col) {
        std::string result;
        if (_model) {
          boost::any data = _model->data(row, col);
          if (!data.empty())
            result = boost::any_cast<std::string > (data);
        }else{
          result=currentText().narrow();
        }
        return result;
      }
    }
  }
}
