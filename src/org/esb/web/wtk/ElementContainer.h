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
#include <Wt/WGridLayout>
#include "Wt/WLabel"
#include "Wt/WTable"
#include "Wt/WText"
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

          ElementContainer() {
            _table = new Wt::WTable();
            _nextRow = 0;
          }

          ~ElementContainer() {
            delete _table;
          }
          T * getElement(std::string name, std::string label = "", std::string value = "", Wt::WContainerWidget * parent = 0);
          T * addElement(std::string name, std::string label = "", std::string value = "", Wt::WGridLayout * l = 0);
          T * getRow(std::string name);
          void validate();
          std::map<std::string, T*> getElements();
        private:
          Wt::WTable * _table;
          std::map<std::string, T*> _cont;
          int _nextRow;
        };

        template <typename T >
        T * ElementContainer<T>::getElement(std::string name, std::string label, std::string value, Wt::WContainerWidget * parent) {
          if (_cont.find(name) == _cont.end()) {
            Wt::WContainerWidget * c = new Wt::WContainerWidget(parent);
            //                        c->resize(Wt::WLength::Auto, 25);
            Wt::WGridLayout * l = new Wt::WGridLayout();
            l->setVerticalSpacing(0);
            l->setHorizontalSpacing(0);
            Wt::WLabel * elementLabel = new Wt::WLabel(label);
//            elementLabel->resize(Wt::WLength(50, Wt::WLength::FontEx), 20);
            T * element = new T();
            l->addWidget(elementLabel, 0, 0);
            l->addWidget(element, 0, 1);
            element->setText(value);
            //                      element->resize(Wt::WLength(40, Wt::WLength::FontEx), 20);
            elementLabel->setBuddy(element);
            c->setLayout(l);
            _cont[name] = element;
          }
          return _cont[name];
        }

        template <typename T >
        T * ElementContainer<T>::addElement(std::string name, std::string label, std::string value, Wt::WGridLayout * l) {
          if (_cont.find(name) == _cont.end()) {
            Wt::WLabel * elementLabel = new Wt::WLabel(label);
//            elementLabel->resize(30, Wt::WLength::Auto);
            
            T * element = new T();
            int nextRow = l->rowCount();
            element->setText(value);
            element->resize(Wt::WLength(200, Wt::WLength::Pixel), Wt::WLength::Auto);
            //                        elementLabel->setBuddy(element);
            l->addWidget(elementLabel, nextRow, 0, Wt::AlignMiddle);
            l->addWidget(element, nextRow, 1, Wt::AlignMiddle);
            l->addWidget(new Wt::WText(), nextRow, 2, Wt::AlignMiddle);
            l->setColumnStretch(l->columnCount()-1,-1);

            _cont[name] = element;
          }
          return _cont[name];
        }

        template <typename T >
        T * ElementContainer<T>::getRow(std::string name) {
          return _cont[name];
        }
        template <typename T>
        std::map<std::string, T*> ElementContainer<T>::getElements() {
          return _cont;
        }

        template <typename T>
        void ElementContainer<T>::validate() {
          typename std::map<std::string, T*>::iterator it = _cont.begin();
          for (; it != _cont.end(); it++) {
            (*it).second->validate();
          }
        }
      }
    }
  }
}


#endif	/* _ELEMENTCONTAINER_H */

