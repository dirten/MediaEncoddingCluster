/* 
 * File:   GuiBuilder.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Oktober 2010, 12:44
 */

#include <map>

#include "GuiBuilder.h"
#include "boost/property_tree/detail/rapidxml.hpp"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Log.h"
#include "Wt/WFitLayout"
#include "Wt/WTable"
#include "Wt/WLabel"
#include "Wt/WText"
#include "Wt/WLabel"
#include "wtk/GroupBox.h"
#include "Wt/Ext/LineEdit"
//#include "Wt/Ext/CheckBox"
#include "Wt/WCheckBox"
#include "org/esb/util/StringUtil.h"
#include "wtk/KeyValueModel.h"
#include "wtk/ComboBox.h"
#include "introspec.h"
#include "Wt/WImage"
using namespace org::esb::io;
using namespace rapidxml;
namespace org {
  namespace esb {
    namespace web {

      class Reference {
      public:

        Reference(Wt::WWidget*from, Wt::WWidget*to) : _from(from), _to(to) {

        }

        Wt::WWidget * getFromReference() {
          return _from;
        }

        Wt::WWidget * getToReference() {
          return _to;
        }
      private:
        Wt::WWidget*_from;
        Wt::WWidget*_to;
      };

      GuiBuilder::GuiBuilder(std::string filename, std::map<std::string, std::string> & data_map) : _data_map(data_map), Wt::Ext::Panel() {
        setLayout(new Wt::WFitLayout());
        setAutoScrollBars(true);
        setBorder(false);

        _main = new Wt::WContainerWidget();
        layout()->addWidget(_main);
        _grid = new Wt::WGridLayout();
        _main->setLayout(_grid);



        _enablerSignalMap = new Wt::WSignalMapper<Wt::WObject*>(this);
        _enablerSignalMap->mapped().connect(SLOT(this, GuiBuilder::enabler));

        _sliderSignalMap = new Wt::WSignalMapper<Reference*>(this);
        _sliderSignalMap->mapped().connect(SLOT(this, GuiBuilder::sliderChanged));

        //        _comboSignalMap = new Wt::WSignalMapper<Wt::Ext::ComboBox*>(this);
        //        _comboSignalMap->mapped().connect(SLOT(this, GuiBuilder::comboChanged));

        _dataChangedSignalMap.mapped().connect(SLOT(this, GuiBuilder::internalDataChanged));

        FileInputStream fis(filename);
        std::string data;
        fis.read(data);

        char * testdata = const_cast<char*> (data.c_str());
        try {
          _doc.parse < 0 > (testdata);
        } catch (rapidxml::parse_error ex) {
          LOGERROR("Parser Exception :" << ex.what());
          return;
        }
        xml_node<>*node = _doc.first_node("guis");
        node = node->first_node("gui");

        xml_node<>*codecs = node->first_node("codecs");
        if (codecs) {
          xml_node<>*codec = codecs->first_node("codec");
          std::string codeclist;
          for (; codec; codec = codec->next_sibling("codec")) {
            codeclist += codec->first_attribute("id")->value();
            codeclist += ",";
          }
          _data_map["available_codecs"] = codeclist;
          LOGDEBUG("CodecList:" << codeclist);
        }
        node = node->first_node("options");
        if (node) {
          xml_node<>*grouptab = node->first_node("optiontab");
          for (; grouptab; grouptab = grouptab->next_sibling("optiontab")) {
            handleOptionGroupTab(grouptab);
          }
          xml_node<>*groupnode = node->first_node("optiongroup");
          for (; groupnode; groupnode = groupnode->next_sibling("optiongroup")) {
            handleOptionGroup(groupnode);
          }

          xml_node<>*optionnode = node->first_node("option");
          for (; optionnode; optionnode = optionnode->next_sibling("option")) {
            handleOption(optionnode);
          }
          _grid->addWidget(new Wt::WText(), _grid->rowCount(), 0);
          _grid->setRowStretch(_grid->rowCount() - 1, 1);
          init();
          /*after the complete gui is builded, fire an activated event on all elements*/
          std::map<std::string, Wt::WWidget *>::iterator elem = _elements.begin();
          for (; elem != _elements.end(); elem++) {
            if (instanceOf(*(*elem).second, ComboBox)) {
              static_cast<ComboBox*> ((*elem).second)->activated().emit(0);
            }
          }
        }
      }

      GuiBuilder::~GuiBuilder() {
        LOGDEBUG("GuiBuilder::~GuiBuilder()");
      }

      void GuiBuilder::handleOption(rapidxml::xml_node<> * ogn) {
        //LOGDEBUG("void GuiBuilder::handleOption(rapidxml::xml_node<> * on)");
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }
        if(data.count("required")){
          if(data["required"]=="true"){
            LOGDEBUG("setting "<<data["id"]<< " to true");
            _require_map[data["id"]]="true";
          }else{
            LOGDEBUG("setting "<<data["id"]<< " to false");
            _require_map[data["id"]]="false";
          }
        }
        xml_node<>*control = ogn->first_node("control");
        std::string type = control->first_attribute("type")->value();
        if (type == "TextBox") {
          handleOptionTextBox(ogn);
        } else
          if (type == "ComboBox") {
          handleOptionComboBox(ogn);
        } else
          if (type == "TrackBar") {
          handleOptionSlider(ogn);
        } else
          if (type == "CheckBox") {
          handleOptionCheckBox(ogn);
        } else
          if (type == "FileSelect") {
        } else
          if (type == "Label") {
          handleOptionLabel(ogn);
        } else
          LOGDEBUG("Unknown Control=" << type);

      }

      void GuiBuilder::handleOptionGroup(rapidxml::xml_node<> *ogn) {
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }

        if (data.count("tab") > 0) {
          /*handle OptionGroupTab*/

        } else {
          /*handle OptionGroupBox*/
          GroupBox * w = new GroupBox();
          w->setTitle(data["title"]);
          w->resize(500, Wt::WLength());
          _elements[data["id"]] = w;

          if (data.count("parentId") > 0) {
            if (_elements.count(data["parentId"]) > 0) {
              ((GroupBox*) _elements[data["parentId"]])->addWidget(w);
            } else {
              LOGERROR("no groupbox found with id " << data["parentId"]);
              LOGINFO("adding it to the normal panel");
              _grid->addWidget(w, _grid->rowCount(), 0);
            }
          } else {
            _grid->addWidget(w, _grid->rowCount(), 0);
          }
        }
      }

      void GuiBuilder::handleOptionGroupBox(rapidxml::xml_node<> *) {

      }

      void GuiBuilder::handleOptionGroupTab(rapidxml::xml_node<> * ogn) {
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }

      }

      void GuiBuilder::handleOptionLabel(rapidxml::xml_node<> *ogn) {
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }
        Wt::WLabel * text = new Wt::WLabel();
        _elements[data["id"]] = text;
        Wt::WTable * table = new Wt::WTable();
        table->elementAt(0, 0)->addWidget(new Wt::WText(""));
        table->elementAt(0, 0)->resize(200, Wt::WLength());
        table->elementAt(0, 1)->addWidget(text);
        if (data.count("optionGroupId") > 0) {
          if (_elements.count(data["optionGroupId"]) > 0) {
            ((GroupBox*) _elements[data["optionGroupId"]])->addWidget(table);
          } else {
            LOGERROR("no groupbox found with id " << data["optionGroupId"]);
          }
        } else {
          _grid->addWidget(table, _grid->rowCount(), 0);
        }
        text->setObjectName(data["id"]);
        text->resize(200, Wt::WLength());
        if (_data_map.count(data["id"]) > 0) {
          text->setText(_data_map[data["id"]] + " " + data["unit"]);
        } else {
          text->setText(data["default"] + " " + data["unit"]);
          _data_map[data["id"]] = data["default"];
        }
      }

      void GuiBuilder::handleOptionTextBox(rapidxml::xml_node<> *ogn) {
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }
        Wt::Ext::LineEdit * text = new Wt::Ext::LineEdit();
        _elements[data["id"]] = text;
        Wt::WTable * table = new Wt::WTable();
        Wt::WText * text_tmp=new Wt::WText(data["title"] + ":");
        //text_tmp->setToolTip("bla fasel");
        table->elementAt(0, 0)->addWidget(text_tmp);
        table->elementAt(0, 0)->resize(200, Wt::WLength());
        table->elementAt(0, 1)->addWidget(text);
        Wt::WLabel * label = new Wt::WLabel();

        label->resize(50, Wt::WLength());
        //label->setToolTip(std::string("tooltip now"));
        table->elementAt(0, 2)->addWidget(label);
        Wt::WImage * spacer = new Wt::WImage("icons/s.gif");
        spacer->resize(10, 10);
        label->setImage(spacer);
        label->setText("" + data["unit"]);
        if (data.count("optionGroupId") > 0) {
          if (_elements.count(data["optionGroupId"]) > 0) {
            ((GroupBox*) _elements[data["optionGroupId"]])->addWidget(table);
          } else {
            LOGERROR("no groupbox found with id " << data["optionGroupId"]);
          }
        } else {
          _grid->addWidget(table, _grid->rowCount(), 0);
        }
        text->setObjectName(data["id"]);
        table->setObjectName(data["id"]);
        text->resize(200, Wt::WLength());
        //_dataChangedSignalMap.mapConnect(text->keyWentUp(),text);
        _dataChangedSignalMap.mapConnect(text->changed(), text);

        if (_data_map.count(data["id"]) > 0) {
          text->setText(_data_map[data["id"]]);
        } else {
          text->setText(data["default"]);
          _data_map[data["id"]] = data["default"];
        }
      }

      void GuiBuilder::handleOptionCheckBox(rapidxml::xml_node<> *ogn) {
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }
        Wt::WCheckBox *box = new Wt::WCheckBox();
        box->resize(Wt::WLength(), 20);
        //box->checked().connect(SLOT(this,GuiBuilder::internalCheckboxDataChanged));
        //LOGDEBUG("CheckBox Signal connected:"<<data["id"]);
        _elements[data["id"]] = box;
        Wt::WTable * table = new Wt::WTable();
        table->elementAt(0, 0)->addWidget(new Wt::WText(data["title"] + ":"));
        table->elementAt(0, 0)->resize(200, Wt::WLength());
        table->elementAt(0, 1)->addWidget(box);
        if (data.count("optionGroupId") > 0) {
          if (_elements.count(data["optionGroupId"]) > 0) {
            ((GroupBox*) _elements[data["optionGroupId"]])->addWidget(table);
          } else {
            LOGERROR("no groupbox found with id " << data["optionGroupId"]);
          }
        } else {
          _grid->addWidget(table, _grid->rowCount(), 0);
        }

        xml_node<>*enables = ogn->first_node("enables");
        if (enables) {
          xml_node<>*option = enables->first_node("option");
          for (; option; option = option->next_sibling("option")) {
            _enablerMap[data["id"]]["1"].push_back(option->value());
          }
        }

        box->setObjectName(data["id"]);
        table->setObjectName(data["id"]);
        if (_data_map.count(data["id"]) > 0) {
          box->setChecked(atoi(_data_map[data["id"]].c_str()) != 0);
        } else {
          box->setChecked(atoi(data["default"].c_str())>0);
          _data_map[data["id"]] = data["default"];
        }
        _enablerSignalMap->mapConnect(box->checked(), box);
        _enablerSignalMap->mapConnect(box->unChecked(), box);
        _dataChangedSignalMap.mapConnect(box->checked(), box);
        _dataChangedSignalMap.mapConnect(box->unChecked(), box);
      }

      void GuiBuilder::handleOptionComboBox(rapidxml::xml_node<> *ogn) {
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }
        ComboBox * combo = new ComboBox();
        _elements[data["id"]] = combo;
        Wt::WTable * table = new Wt::WTable();
        table->elementAt(0, 0)->addWidget(new Wt::WText(data["title"] + ":"));
        table->elementAt(0, 0)->resize(200, Wt::WLength());
        table->elementAt(0, 1)->addWidget(combo);
         Wt::WLabel * label = new Wt::WLabel();
        label->resize(50, Wt::WLength());
        table->elementAt(0, 2)->addWidget(label);
        Wt::WImage * spacer = new Wt::WImage("icons/s.gif");
        spacer->resize(28, 10);
        label->setImage(spacer);
        label->setText("" + data["unit"]);
        if (data.count("optionGroupId") > 0) {
          if (_elements.count(data["optionGroupId"]) > 0) {
            ((GroupBox*) _elements[data["optionGroupId"]])->addWidget(table);
          } else {
            LOGERROR("no groupbox found with id " << data["optionGroupId"]);
          }
        } else {
          _grid->addWidget(table, _grid->rowCount(), 0);
        }
        xml_node<>*itemlist = ogn->first_node("list");
        xml_node<>*item = itemlist->first_node("item");
        unsigned int len = 10;
        KeyValueModel * model = new KeyValueModel();
        for (; item; item = item->next_sibling("item")) {
          len = strlen(item->first_attribute("title")->value()) > len ? strlen(item->first_attribute("title")->value()) : len;
          model->addModelData(item->first_attribute("title")->value(), item->first_attribute("value")->value());
          xml_node<>*enables = item->first_node("enables");
          if (enables) {
            xml_node<>*option = enables->first_node("option");
            for (; option; option = option->next_sibling("option")) {
              _enablerMap[data["id"]][item->first_attribute("value")->value()].push_back(option->value());
            }
          }
          xml_node<>*optiondata = item->first_node("optiondata");
          if (optiondata) {
            xml_node<>*odata = optiondata->first_node("data");
            for (; odata; odata = odata->next_sibling("data")) {
              std::map<std::string, std::string> optiondata;
              for (xml_attribute<> *oattr = odata->first_attribute(); oattr; oattr = oattr->next_attribute()) {
                optiondata[oattr->name()] = oattr->value();
              }
              std::map<std::string, std::string> tmp;
              tmp[optiondata["option"]] = optiondata["value"];
              _optionDataMap[data["id"]][item->first_attribute("value")->value()].push_back(tmp);
            }
          }

        }
        combo->setModel(model);
        combo->setObjectName(data["id"]);
        table->setObjectName(data["id"]);
        combo->resize(200, Wt::WLength());
        combo->setTextSize(len);
        if (_data_map.count(data["id"]) > 0) {
          combo->setSelectedEntry(_data_map[data["id"]], 1);
        } else {
          combo->setSelectedEntry(data["default"], 1);
          _data_map[data["id"]] = data["default"];
        }
        _enablerSignalMap->mapConnect(combo->activated(), combo);
        _dataChangedSignalMap.mapConnect(combo->activated(), combo);
        //        _dataChangedSignalMap.mapped().emit(combo);
        //combo->activated().emit(0);
      }

      void GuiBuilder::handleOptionSlider(rapidxml::xml_node<> *ogn) {
        std::map<std::string, std::string> data;
        for (xml_attribute<> *attr = ogn->first_attribute(); attr; attr = attr->next_attribute()) {
          data[attr->name()] = attr->value();
        }
        Wt::WSlider * slider = new Wt::WSlider(Wt::Horizontal);
        _elements[data["id"]] = slider;
        Wt::WTable * table = new Wt::WTable();
        table->elementAt(0, 0)->addWidget(new Wt::WText(data["title"] + ":"));
        table->elementAt(0, 0)->resize(200, Wt::WLength());
        table->elementAt(0, 1)->addWidget(slider);
        Wt::Ext::LineEdit * line = new Wt::Ext::LineEdit();
        //line->setEnabled(false);
        table->elementAt(0, 2)->addWidget(line);

        if (data.count("optionGroupId") > 0) {
          if (_elements.count(data["optionGroupId"]) > 0) {
            ((GroupBox*) _elements[data["optionGroupId"]])->addWidget(table);
          } else {
            LOGERROR("no groupbox found with id " << data["optionGroupId"]);
          }
        } else {
          _grid->addWidget(table, _grid->rowCount(), 0);
        }
        slider->setObjectName(data["id"]);
        table->setObjectName(data["id"]);
        slider->setMinimum(atoi(data["min"].c_str()));
        slider->setMaximum(atoi(data["max"].c_str()));
        if (_data_map.count(data["id"]) > 0) {
          slider->setValue(atoi(_data_map[data["id"]].c_str()));
          line->setText(_data_map[data["id"]] + " " + data["unit"]);
        } else {
          slider->setValue(atoi(data["default"].c_str()));
          line->setText(data["default"] + " " + data["unit"]);
          _data_map[data["id"]] = data["default"];
        }

        slider->setTickInterval(1);
        slider->setTickPosition(Wt::WSlider::TicksBelow);
        slider->resize(208, 25);

        _sliderSignalMap->mapConnect(slider->valueChanged(), new Reference(slider, line));
        _dataChangedSignalMap.mapConnect(slider->valueChanged(), slider);

      }

      void GuiBuilder::sliderChangeValue(int pos) {

      }

      void GuiBuilder::sliderChanged(Reference* ref) {
        Wt::WSlider * slider = static_cast<Wt::WSlider*> (ref->getFromReference());
        Wt::Ext::LineEdit * line = static_cast<Wt::Ext::LineEdit*> (ref->getToReference());
        line->setText(org::esb::util::StringUtil::toString(slider->value()));
      }

      void GuiBuilder::comboChanged(Wt::Ext::ComboBox*c) {

      }

      void GuiBuilder::enabler(Wt::WObject*obj) {
        //LOGDEBUG("Enabler Object Id=" << obj->objectName());
        if (instanceOf(*obj, ComboBox)) {
          ComboBox * box = static_cast<ComboBox*> (obj);
          std::string id = obj->objectName();
          std::string value = box->data(box->currentIndex(), 1);
          /*first disable all registered elements*/
          std::map<std::string, std::list<std::string> >::iterator values = _enablerMap[id].begin();
          for (; values != _enablerMap[id].end(); values++) {
            std::list<std::string>::iterator ids = (*values).second.begin();
            for (; ids != (*values).second.end(); ids++) {
              //              LOGDEBUG("Disable element "<<(*ids));
              elementEnable(_elements[(*ids)], false);
            }
          }
          /*now enable the congruent elements*/
          std::list<std::string>::iterator ids = _enablerMap[id][value].begin();
          for (; ids != _enablerMap[id][value].end(); ids++) {
            //              LOGDEBUG("Enable element "<<(*ids));
            elementEnable(_elements[(*ids)], true);
          }
          /*now setting values for other controls*/
          std::list<std::map<std::string, std::string> >::iterator ods = _optionDataMap[id][value].begin();
          for (; ods != _optionDataMap[id][value].end(); ods++) {
            std::map<std::string, std::string>::iterator mapit = (*ods).begin();
            for (; mapit != (*ods).end(); mapit++) {
              Wt::WObject * obj = _elements[(*mapit).first];
              if (obj == NULL) {
                LOGERROR("could not set the values for " << (*mapit).first << ", it is not in the element list");
                continue;
              }
              if (instanceOf(*obj, ComboBox)) {
                ComboBox * box = static_cast<ComboBox*> (obj);
                box->setSelectedEntry((*mapit).second, 1);
                box->activated().emit(0);
                LOGDEBUG("Setting combo data from " << box->objectName() << " to " << (*mapit).second);
              } else
                if (instanceOf(*obj, Wt::Ext::LineEdit)) {
                Wt::Ext::LineEdit * box = static_cast<Wt::Ext::LineEdit*> (obj);
                box->setText((*mapit).second);
                box->changed().emit();
              } else
                if (instanceOf(*obj, Wt::WSlider)) {
                Wt::WSlider * box = static_cast<Wt::WSlider*> (obj);
                box->setValue(atoi((*mapit).second.c_str()));

              } else
                if (instanceOf(*obj, Wt::WCheckBox)) {
                Wt::WCheckBox * box = static_cast<Wt::WCheckBox*> (obj);
                box->setChecked(atoi((*mapit).second.c_str()) > 0);
              }

            }
            //elementEnable(_elements[(*ids)], true);
          }


          //          LOGDEBUG("ComboBox " << box->text());
          //          LOGDEBUG("ComboBox value=" << box->data(box->currentIndex(), 1));
        }
      }

      void GuiBuilder::elementEnable(Wt::WWidget * element, bool enable) {
        if (!element)return;
        //element->setDisabled(!enable);
        if (instanceOf(*element, ComboBox)) {
          ComboBox * b = static_cast<ComboBox*> (element);
          b->setEnabled(enable);
        }
        if (instanceOf(*element, Wt::WCheckBox)) {
          Wt::WCheckBox * b = static_cast<Wt::WCheckBox*> (element);
          //if (!enable)
          //b->setChecked(false);
          b->setEnabled(enable);
        }
        if (instanceOf(*element, Wt::Ext::LineEdit)) {
          Wt::Ext::LineEdit * b = static_cast<Wt::Ext::LineEdit*> (element);
          b->setEnabled(enable);
        }
        if (instanceOf(*element, Wt::WSlider)) {
          Wt::WSlider * b = static_cast<Wt::WSlider*> (element);
          //          LOGDEBUG("Slider Parent Hidden:"<<b->parent()->parent()->parent()->isHidden());
          //b->setHidden(!enable || b->parent()->parent()->parent()->isHidden());
          b->setDisabled(!enable || b->parent()->parent()->parent()->isHidden());
          b->setHiddenKeepsGeometry(true);

        }
        if (instanceOf(*element, GroupBox)) {
          LOGDEBUG("GroupBox Instance");
          GroupBox * b = static_cast<GroupBox*> (element);
          b->setHidden(!enable);
          b->setHiddenKeepsGeometry(true);
          std::list<std::string> names=b->getChildrenObjectNames();
          /**iterate over all childs*/
          std::list<std::string>::iterator cit=names.begin();
          for(;cit!=names.end();cit++){
            std::string name = *cit;
            if (!enable) {
              if(_require_map[name]!="true"){
                //_full_data_map[name]=_data_map[name];
                int elements_removed = _data_map.erase(name);
                LOGDEBUG("disabled elements removed count = " << elements_removed);
              }
            } else {
              internalDataChanged(_elements[name]);
            }
          }
        }
        /**
         * removing disabled or adding enabled data from/to element map
         */
        if (!enable) {
          std::string name = element->objectName();
          if(_require_map[name]!="true"){
            _full_data_map[name]=_data_map[name];
            int elements_removed = _data_map.erase(name);
            LOGDEBUG("disabled elements removed count = " << elements_removed);
          }
        } else {
          internalDataChanged(element);
        }

      }

      void GuiBuilder::init() {
        /*setting initial enable/disable for the elements*/
        std::map<std::string, std::map<std::string, std::list<std::string> > >::iterator tmp = _enablerMap.begin();
        for (; tmp != _enablerMap.end(); tmp++) {
          std::string id = (*tmp).first;
          /*first disable all registered elements*/
          std::map<std::string, std::list<std::string> >::iterator values = _enablerMap[id].begin();
          for (; values != _enablerMap[id].end(); values++) {
            std::list<std::string>::iterator ids = (*values).second.begin();
            for (; ids != (*values).second.end(); ids++) {
              elementEnable(_elements[(*ids)], false);
            }
          }
          std::string value;
          if (instanceOf(*_elements[id], ComboBox)) {
            ComboBox * box = static_cast<ComboBox*> (_elements[id]);
            value = box->data(box->currentIndex(), 1);
          }
          if (_enablerMap[id].count(value) > 0) {
            std::list<std::string>::iterator ids = _enablerMap[id][value].begin();
            for (; ids != _enablerMap[id][value].end(); ids++) {
              elementEnable(_elements[(*ids)], true);
            }
          }
        }
      }

      void GuiBuilder::internalDataChanged(Wt::WObject*obj) {
        LOGDEBUG("Object" << obj);
        if (instanceOf(*obj, ComboBox)) {
          ComboBox * box = static_cast<ComboBox*> (obj);
          //_full_data_map[obj->objectName()]=box->data(box->currentIndex(), 1);
          if (box->isEnabled())
            _data_map[box->objectName()] = box->data(box->currentIndex(), 1);
          LOGDEBUG("Combo key:" << box->objectName() << " val:" << box->data(box->currentIndex(), 1));
        } else
          if (instanceOf(*obj, Wt::Ext::LineEdit)) {
          Wt::Ext::LineEdit * box = static_cast<Wt::Ext::LineEdit*> (obj);
          std::string data = box->text().narrow();
          //_full_data_map[obj->objectName()]=data.c_str();
          if (box->isEnabled())
            _data_map[box->objectName()] = data.c_str();
        } else
          if (instanceOf(*obj, Wt::WSlider)) {
          Wt::WSlider * box = static_cast<Wt::WSlider*> (obj);
          //_full_data_map[obj->objectName()]=org::esb::util::StringUtil::toString(box->value());
          if (box->isEnabled())
            _data_map[box->objectName()] = org::esb::util::StringUtil::toString(box->value());

        } else
          if (instanceOf(*obj, Wt::WCheckBox)) {
          Wt::WCheckBox * box = static_cast<Wt::WCheckBox*> (obj);
          //_full_data_map[obj->objectName()]=box->isChecked() ? "1" : "0";
          if (box->isEnabled())
            _data_map[box->objectName()] = box->isChecked() ? "1" : "0";
          }

        dataChanged.emit();
      }
    }
  }
}
