/* 
 * File:   ElementContainer.h
 * Author: jhoelscher
 *
 * Created on 25. November 2008, 10:22
 */
#include <map>
#include <string>

#include <Wt/Ext/Component>
#include <Wt/WWebWidget>
#include "Wt/WLabel"
#include "Wt/Ext/LineEdit"
#include "Wt/WContainerWidget"

#ifndef _ELEMENTCONTAINER_H
#define	_ELEMENTCONTAINER_H
namespace org {
  namespace esb {
    namespace web {
      namespace wtk {

        template <typename T>
        class ElementContainer {
        public:
          T * getElement(std::string name, std::string label="", std::string value="", Wt::WContainerWidget * parent=0);
          void validate();
        private:
          std::map<std::string, T*> _cont;
        };

        template <typename T>
        T * ElementContainer<T>::getElement(std::string name, std::string label, std::string value, Wt::WContainerWidget * parent) {
          if (_cont.find(name) == _cont.end()) {
            Wt::WContainerWidget * c = new Wt::WContainerWidget(parent);
            Wt::WLabel * elementLabel = new Wt::WLabel(label);
            elementLabel->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
            T * element = new T();
            c->addWidget(elementLabel);
            c->addWidget(element);
            element->setText(value);
            element->resize(Wt::WLength(40, Wt::WLength::FontEx), Wt::WLength());
            elementLabel->setBuddy(element);
            _cont[name] = element;
          }
          return _cont[name];
        }

        template <typename T>
        void ElementContainer<T>::validate(){
          typename std::map<std::string, T*>::iterator it=_cont.begin();
          for(;it!=_cont.end();it++){
            (*it).second->validate();
          }
        }
      }
    }
  }
}


#endif	/* _ELEMENTCONTAINER_H */

