#include "ElementContainer.h"
#include "Wt/WLabel"
#include "Wt/Ext/LineEdit"
#include "Wt/WContainerWidget"

namespace org {
  namespace esb {
    namespace web {
      namespace wtk {
/*
        template <typename T>
        T * ElementContainer<T>::getElement(std::string name, std::string label, std::string value, Wt::WWebWidget * parent) {
          Wt::WContainerWidget * c=new Wt::WContainerWidget(parent);
          Wt::WLabel * elementLabel = new Wt::WLabel(label);
          elementLabel->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          T * element = new T();
          c->addWidget(elementLabel);
          c->addWidget(element);
          element->setText(value);
          elementLabel->setBuddy(element);
          _cont[name] = element;
          return element;
        }
 */
      }
    }
  }
}

