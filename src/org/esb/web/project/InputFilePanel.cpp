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
#include <Wt/Ext/MessageBox>
#include <Wt/WMessageBox>
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
          layout()->setContentsMargins(0, 0, 0, 0);
          setModel(new InputFileTableModel());
          setColumnWidth(0,50);
          setColumnWidth(1,300);
          setColumnWidth(2,300);
        }
        void setMediaFiles(std::vector<db::MediaFile> files){
          InputFileTableModel*oldptr=static_cast<InputFileTableModel*>(model());
          setModel(new InputFileTableModel());
          delete oldptr;
   
          static_cast<InputFileTableModel*>(model())->setMediaFileData(files);          
        }
      };

      InputFilePanel::InputFilePanel() : Wt::Ext::Panel() {
        setTitle("Input Videos");
        setLayout(new Wt::WFitLayout());
        _filetable=boost::shared_ptr<InputFileTable>(new InputFileTable());

        _filetable->setAlternatingRowColors(true);
        _filetable->setHighlightMouseOver(true);
        _filetable->setSelectionBehavior(Wt::SelectRows);
        _filetable->setSelectionMode(Wt::SingleSelection);

        layout()->addWidget(_filetable.get());
        layout()->setContentsMargins(0, 0, 0, 0);


        setTopToolBar(new Wt::Ext::ToolBar());

        Wt::Ext::Button * addVideoButton = topToolBar()->addButton("Add Input Video");
        Wt::Ext::Button * removeVideoButton = topToolBar()->addButton("Remove Input Video");

        addVideoButton->clicked.connect(SLOT(this, InputFilePanel::addVideoButtonClicked));
        removeVideoButton->clicked.connect(SLOT(this, InputFilePanel::removeVideo));

      }

      void InputFilePanel::setProject(boost::shared_ptr<db::Project> p){
        _project=p;
        _filetable->setModel(new InputFileTableModel());
        _filetable->setMediaFiles(p->mediafiles().get().all());
      }

      void InputFilePanel::setInputFile(org::esb::io::File file) {
        LOGDEBUG("try import file"<<file.getPath());
        int result=import(file);
        Wt::Ext::Dialog * dialog=new Wt::Ext::Dialog();
        std::string message=file.getFileName();
          if(result>0){
          db::MediaFile mfile=litesql::select<db::MediaFile>(_project->getDatabase(), db::MediaFile::Id==result).one();
          _project->mediafiles().link(mfile);
          _filetable->setMediaFiles(_project->mediafiles().get().all());
          message+=" successfull imported!";
          dialog->setTitle("Media File imported!" );

          LOGDEBUG("File imported:"<<result);
        }else{
          message+=" is not a valid media file!";
          dialog->setTitle("Failed to import Media File!" );
          LOGINFO("no valid file")
        }
          dialog->setLayout(new Wt::WFitLayout());
          dialog->resize(400,150);
          dialog->layout()->addWidget(new Wt::WText(message));
          dialog->addButton(new Wt::Ext::Button("Continue"));
          dialog->buttons().back()->clicked.connect(SLOT(dialog, Wt::Ext::Dialog::accept));
          dialog->show();
          dialog->exec();

      }
      void InputFilePanel::removeVideo(){
        
        int pid=atoi(boost::any_cast<string > (_filetable->model()->data(_filetable->selectedRows()[0], 0)).c_str());
        LOGDEBUG("remove Video File id:"<<pid);
        vector<db::MediaFile> files=_project->mediafiles().get().all();
        vector<db::MediaFile>::iterator it=files.begin();

        for(;it!=files.end();it++){
          if((*it).id==pid){
            LOGDEBUG("Video File found id:"<<pid);
            _project->mediafiles().unlink((*it));
          }
        }
        _filetable->setMediaFiles(_project->mediafiles().get().all());
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

