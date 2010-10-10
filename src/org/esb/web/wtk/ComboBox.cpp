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
      }

      ComboBox::~ComboBox() {
      }

      void ComboBox::setModel(Wt::WAbstractItemModel* model) {
        _model = model;
        Wt::Ext::ComboBox::setModel(model);
      }

      void ComboBox::setSelectedEntry(std::string key, int index) { 
        if (_model) {
          for (int a = 0; a < _model->rowCount(); a++) {
            std::string k = boost::any_cast<std::string > (_model->data(a, index));
            if (k == key) {
              setCurrentIndex(a);
            }
          }
        }
      }

      std::string ComboBox::currentSelected(int col) {
        std::string result=data(currentIndex(), col);
        return result;
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
