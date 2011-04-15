/* 
 * File:   MediaFileTable.cpp
 * Author: HoelscJ
 * 
 * Created on 26. August 2010, 12:58
 */
#include "org/esb/db/hivedb.hpp"
#include "org/esb/web/MediaFileTable.h"
#include "Wt/WStandardItemModel"
#include "org/esb/util/StringUtil.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include <Wt/WApplication>
#include "WebApp2.h"
namespace org {
  namespace esb {
    namespace web {

      class MediaFileTableModel : public Wt::WStandardItemModel {
      public:

        MediaFileTableModel(std::vector<db::MediaFile> files) : Wt::WStandardItemModel(0, 0, NULL) {
          insertColumns(0, 6);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Path"));
          setHeaderData(2, std::string("Name"));
          setHeaderData(3, std::string("Size"));
          setHeaderData(4, std::string("Duration"));
          setHeaderData(5, std::string("Format"));
          setModelData(files);

        }

        void refresh(std::vector<db::MediaFile> files) {
          setModelData(files);
        }

        void setModelData(std::vector<db::MediaFile> files) {
          if (rowCount() < files.size())
            insertRows(rowCount(), files.size() - rowCount());
          if (rowCount() > files.size()) {
            removeRows(0, rowCount() - files.size());
          }
          for (unsigned int a = 0; a < files.size(); a++) {
            db::MediaFile file = files[a];
            setData(a, 0, org::esb::util::StringUtil::toString(file.id.value()));
            setData(a, 1, file.path.value());
            setData(a, 2, file.filename.value());
            setData(a, 3, org::esb::util::StringUtil::toString(round((file.filesize.value()/1024/1024)*100)/100)+" MB");
            setData(a, 4, org::esb::util::StringUtil::toString(file.duration.value()/1000000)+" sec.");

            setData(a, 5, file.containertype.value());
          }
        }



      };

      MediaFileTable::MediaFileTable() : Wt::Ext::TableView() {
        db::HiveDb dbCon=org::esb::hive::DatabaseService::getDatabase();
        LOGDEBUG("WebAppInstance"<<static_cast<WebApp2*>(Wt::WApplication::instance())->getUser()->authname);
        std::vector<db::MediaFile> files = litesql::select<db::MediaFile > (dbCon).all();
        _model = Ptr<MediaFileTableModel > (new MediaFileTableModel(files));
        setModel(_model.get());
        setAlternatingRowColors(true);
        resizeColumnsToContents(true);
        setHighlightMouseOver(true);
        setSelectionBehavior(Wt::SelectRows);
        setSelectionMode(Wt::SingleSelection);
        setColumnWidth(0, 3);
        setAutoExpandColumn(1);
        setAutoExpandColumn(2);
        setColumnWidth(3, 30);
        setColumnWidth(4, 30);
        setColumnWidth(5, 30);
      }

      void MediaFileTable::refresh() {
        LOGDEBUG("MediaFileTable::refresh()");
        db::HiveDb dbCon=org::esb::hive::DatabaseService::getDatabase();
        std::vector<db::MediaFile> files = litesql::select<db::MediaFile > (dbCon).all();
        _model->refresh(files);
      }

      MediaFileTable::MediaFileTable(const MediaFileTable& orig) {
      }

      MediaFileTable::~MediaFileTable() {
      }
    }
  }
}

