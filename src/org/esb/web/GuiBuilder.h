/* 
 * File:   GuiBuilder.h
 * Author: HoelscJ
 *
 * Created on 13. Oktober 2010, 12:44
 */

#ifndef GUIBUILDER_H
#define	GUIBUILDER_H
#include "Wt/Ext/Panel"
#include "Wt/WWebWidget"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "Wt/WSignalMapper"
#include "Wt/WSlider"
#include "Wt/WText"
#include "Wt/Ext/ComboBox"

#include "boost/property_tree/detail/rapidxml.hpp"
#include <map>
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace web {
      class Reference;
      class GuiBuilder :public Wt::Ext::Panel{
        classlogger("org.esb.web.GuiBuilder");
      public:
        GuiBuilder(std::string filename,std::map<std::string, std::string> & data_map);
        virtual ~GuiBuilder();
        Wt::Signal<void> dataChanged;
      private:
        void handleOption(rapidxml::xml_node<> *);
        void handleOptionGroup(rapidxml::xml_node<> *);
        void handleOptionGroupBox(rapidxml::xml_node<> *);
        void handleOptionGroupTab(rapidxml::xml_node<> *);
        void handleOptionTextBox(rapidxml::xml_node<> *);
        void handleOptionCheckBox(rapidxml::xml_node<> *);
        void handleOptionComboBox(rapidxml::xml_node<> *);
        void handleOptionSlider(rapidxml::xml_node<> *);
        void handleOptionLabel(rapidxml::xml_node<> *);

        rapidxml::xml_document<> _doc;
        std::map<std::string, Wt::WWidget *> _elements;
        Wt::WContainerWidget * _main;
        Wt::WGridLayout * _grid;
        std::map<std::string, std::string> & _data_map;
        Wt::WSignalMapper<Wt::WObject*> *_enablerSignalMap;
        Wt::WSignalMapper<Wt::WObject*> _dataChangedSignalMap;
        Wt::WSignalMapper<Reference*> *_sliderSignalMap;
        //Wt::WSignalMapper<Wt::Ext::ComboBox*> *_comboSignalMap;
        void internalDataChanged(Wt::WObject*);
        void sliderChanged(Reference*);
        void comboChanged(Wt::Ext::ComboBox*);
        void sliderChangeValue(int pos);
        void enabler(Wt::WObject*);
        void init();
        void elementEnable(Wt::WWidget * element,bool enable);
        std::map<std::string,std::map<std::string,std::list<std::string> > > _enablerMap;
        std::map<std::string,std::map<std::string,std::list<std::map<std::string, std::string> > > > _optionDataMap;
      };
    }
  }
}

#endif	/* GUIBUILDER_H */

