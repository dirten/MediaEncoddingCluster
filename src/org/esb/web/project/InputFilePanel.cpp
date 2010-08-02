/* 
 * File:   InputFilePanel.cpp
 * Author: HoelscJ
 * 
 * Created on 13. Juli 2010, 17:08
 */

#include "InputFilePanel.h"
#include "Wt/WFitLayout"
#include "Wt/WText"
#include <Wt/WStandardItemModel>

#include "Wt/Ext/ToolBar"
#include "Wt/Ext/Button"
#include <Wt/Ext/TableView>
#include "org/esb/web/FileChooser.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace web {

      class InputFileTableModel:public Wt::WStandardItemModel{
      public:
        InputFileTableModel(){
          
          insertColumns(0, 3);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Filename"));
          setHeaderData(2, std::string("Path"));
        }

        void setMediaFileData(std::vector<db::MediaFile> files){
          std::vector<db::MediaFile>::iterator it=files.begin();
//          if(files.size()==0)
//            this->removeRows(0,rowCount());
          for(int a = 0;it!=files.end();it++, a++){
            if (rowCount() <= a)
              insertRow(rowCount());
//            setData(a, 0, std::string("1"));
//            setData(a, 1, std::string("2"));
//            setData(a, 2, std::string("3"));

            setData(a, 0, org::esb::util::StringUtil::toString((*it).id));
            setData(a, 1, (std::string)(*it).filename);
            setData(a, 2, (std::string)(*it).path);
          }
        }
      };
      class InputFileTable:public Wt::Ext::TableView{
      public:
        InputFileTable(){
          setModel(new InputFileTableModel());
          setColumnWidth(0,50);
          setColumnWidth(1,300);
          setColumnWidth(2,300);
        }
        void setMediaFiles(std::vector<db::MediaFile> files){
          static_cast<InputFileTableModel*>(model())->setMediaFileData(files);          
        }
      };

      InputFilePanel::InputFilePanel() : Wt::Ext::Panel() {
        setTitle("Input Videos");
        setLayout(new Wt::WFitLayout());
        _filetable=boost::shared_ptr<InputFileTable>(new InputFileTable());

        layout()->addWidget(_filetable.get());
        layout()->setContentsMargins(0, 0, 0, 0);


        setTopToolBar(new Wt::Ext::ToolBar());

        Wt::Ext::Button * addVideoButton = topToolBar()->addButton("Add Input Video");
        Wt::Ext::Button * removeVideoButton = topToolBar()->addButton("Remove Input Video");

        addVideoButton->clicked.connect(SLOT(this, InputFilePanel::addVideoButtonClicked));


      }

      void InputFilePanel::setProject(boost::shared_ptr<db::Project> p){
        _project=p;
        _filetable->setModel(new InputFileTableModel());
        _filetable->setMediaFiles(p->mediafiles().get().all());
      }

      void InputFilePanel::setInputFile(org::esb::io::File file) {
        LOGDEBUG("try import file"<<file.getPath());
        int result=import(file);
        db::HiveDb db("mysql",org::esb::config::Config::getProperty("db.url"));
        db::MediaFile mfile=litesql::select<db::MediaFile>(db, db::MediaFile::Id==result).one();
        _project->mediafiles().link(mfile);
        _filetable->setMediaFiles(_project->mediafiles().get().all());
        LOGDEBUG("File imported:"<<result);
      }

      InputFilePanel::InputFilePanel(const InputFilePanel& orig) {
      }

      InputFilePanel::~InputFilePanel() {
        
      }

      void InputFilePanel::addVideoButtonClicked() {
        FileChooser * chooser=new FileChooser("Add Video", "/");
        chooser->selected.connect(SLOT(this, InputFilePanel::setInputFile));
        chooser->show();
      }
    }
  }
}

