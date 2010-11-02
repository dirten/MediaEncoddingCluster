/* 
 * File:   PresetList.cpp
 * Author: jholscher
 * 
 * Created on 31. Oktober 2010, 22:42
 */

#include "PresetList.h"
#include "Wt/WStandardItemModel"
#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/PresetReader.h"
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
        } else {
          LOGERROR("Preset Directory does not exist! " << presetdir.getPath());
        }
      }

      PresetList::~PresetList() {
      }

      void PresetList::selectionChanged() {
        if (selectedRows().size() > 0) {
          std::string file = boost::any_cast<string > (model()->data(selectedRows()[0], 0));          
          presetSelected.emit(org::esb::config::Config::get("preset.path") + "/"+file);
        }
      }
    }
  }
}
