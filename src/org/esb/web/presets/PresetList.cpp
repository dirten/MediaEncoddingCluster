/* 
 * File:   PresetList.cpp
 * Author: jholscher
 * 
 * Created on 31. Oktober 2010, 22:42
 */

#include "PresetList.h"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/ToolBar"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/PresetReader.h"
#include "org/esb/web/presets/PresetsEditorWindow.h"
namespace org {
  namespace esb {
    namespace web {
      using namespace org::esb::io;

      class PresetListTableModel : public Wt::WStandardItemModel {
      public:

        PresetListTableModel(std::list<std::list<std::string> > data) : Wt::WStandardItemModel() {
          insertColumns(0, 2);
          setHeaderData(0, std::string("File"));
          setHeaderData(1, std::string("Name"));
          setModelData(data);
        }

        void setModelData(std::list<std::list<std::string> > data) {
          if (rowCount() < data.size())
            insertRows(rowCount(), data.size() - rowCount());
          if (rowCount() > data.size()) {
            removeRows(0, rowCount() - data.size());
          }
          std::list<std::list<std::string> >::iterator lines = data.begin();
          for (int a = 0; lines != data.end(); lines++, a++) {
            std::list<std::string>::iterator cols = (*lines).begin();
            for (int b = 0; cols != (*lines).end(); cols++, b++) {
              setData(a, b, (*cols));
            }
          }
        }
      };

      PresetList::PresetList() : Wt::Ext::TableView() {
        File presetdir(org::esb::config::Config::get("preset.path"));
        if (presetdir.exists()) {
          FileList files = presetdir.listFiles();
          std::list<std::list<std::string> > data;
          FileList::iterator file_it = files.begin();
          for (; file_it != files.end(); file_it++) {
            org::esb::hive::PresetReader reader((*file_it)->getPath());
            LOGDEBUG((*file_it)->getPath());
            std::list<std::string> line;
            line.push_back((*file_it)->getFileName());
            line.push_back(reader.getPreset()["name"]);
            data.push_back(line);
          }
          setModel(new PresetListTableModel(data));
          setAlternatingRowColors(true);
          resizeColumnsToContents(true);
          setColumnHidden(0, true);
          setHighlightMouseOver(true);
          setSelectionBehavior(Wt::SelectRows);
          setSelectionMode(Wt::SingleSelection);
          setColumnWidth(0, 50);
          setAutoExpandColumn(1);
          itemSelectionChanged().connect(SLOT(this, PresetList::selectionChanged));


          setTopToolBar(new Wt::Ext::ToolBar());
          create_button = topToolBar()->addButton("Create a new Preset");
          create_button->setIcon("icons/add-icon.png");
          topToolBar()->addSeparator();
          edit_button = topToolBar()->addButton("Edit selected Preset");
          edit_button->setIcon("icons/edit-icon.png");
          //topToolBar()->addSeparator();
          //delete_button = topToolBar()->addButton("Delete selected Preset");
          //delete_button->setIcon("icons/encoding-project-remove-icon.png");

          create_button->setEnabled(true);
          edit_button->setEnabled(false);
          //delete_button->setEnabled(false);

          create_button->clicked().connect(SLOT(this, PresetList::createPreset));
          edit_button->clicked().connect(SLOT(this, PresetList::editPreset));
          //delete_button->clicked().connect(SLOT(this, PresetList::deletePreset));

        } else {
          LOGERROR("Preset Directory does not exist! " << presetdir.getPath());
        }
      }

      void PresetList::refresh() {
        File presetdir(org::esb::config::Config::get("preset.path"));
        if (presetdir.exists()) {
          FileList files = presetdir.listFiles();
          std::list<std::list<std::string> > data;
          FileList::iterator file_it = files.begin();
          for (; file_it != files.end(); file_it++) {
            org::esb::hive::PresetReader reader((*file_it)->getPath());
            LOGDEBUG((*file_it)->getPath());
            std::list<std::string> line;
            line.push_back((*file_it)->getFileName());
            line.push_back(reader.getPreset()["name"]);
            data.push_back(line);
          }
         static_cast<PresetListTableModel*>(model())->setModelData(data);
        }
      }

      void PresetList::createPreset() {
        PresetsEditorWindow * editor = new PresetsEditorWindow("");
        editor->show();
        editor->exec();
        refresh();
        delete editor;
        
      }

      void PresetList::editPreset() {
        std::string file = boost::any_cast<string > (model()->data(selectedRows()[0], 0));
        file = org::esb::config::Config::get("preset.path") + "/" + file;
        PresetsEditorWindow * editor = new PresetsEditorWindow(file);
        editor->show();
        editor->exec();
        refresh();
        delete editor;
      }

      void PresetList::deletePreset() {

      }

      PresetList::~PresetList() {
      }

      void PresetList::selectionChanged() {
        if (selectedRows().size() > 0) {
          std::string file = boost::any_cast<string > (model()->data(selectedRows()[0], 0));
          presetSelected.emit(org::esb::config::Config::get("preset.path") + "/" + file);
          edit_button->setEnabled(true);
          //delete_button->setEnabled(true);
        }
      }
    }
  }
}
