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

#include "org/esb/hive/FileImporter.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Thread.h"
#include "boost/bind.hpp"
#include "Wt/WApplication"
namespace org {
  namespace esb {
    namespace web {

      class InputFileTableModel : public Wt::WStandardItemModel {
      public:

        InputFileTableModel() : Wt::WStandardItemModel() {

          insertColumns(0, 3);
          setHeaderData(0, std::string("Id"));
          setHeaderData(1, std::string("Filename"));
          setHeaderData(2, std::string("Path"));
        }

        void setMediaFileData(std::vector<db::MediaFile> files) {
          std::vector<db::MediaFile>::iterator it = files.begin();
          //          if(files.size()==0)
          //            this->removeRows(0,rowCount());
          if(rowCount()>files.size()){
            if(files.size()>0)
              removeRow(files.size()-1);
            else{
              removeRows(0,rowCount());
            }
          }

          for (int a = 0; it != files.end(); it++, a++) {
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

      class InputFileTable : public Wt::Ext::TableView {
      public:

        InputFileTable() {
          layout()->setContentsMargins(0, 0, 0, 0);
          setModel(new InputFileTableModel());
          setColumnWidth(0, 50);
          setColumnWidth(1, 300);
          setColumnWidth(2, 300);
          setToolTip("Click on Add Input Video to import an Input Media File!");
        }
        void refresh(){
          Wt::Ext::TableView::refresh();
        }
        void setMediaFiles(std::vector<db::MediaFile> files) {
//          InputFileTableModel*oldptr = static_cast<InputFileTableModel*> (model());
//          setModel(new InputFileTableModel());
//          delete oldptr;

          static_cast<InputFileTableModel*> (model())->setMediaFileData(files);
        }
      };

      InputFilePanel::InputFilePanel() : Wt::Ext::Panel() {
        setTitle("Input Videos");
        setLayout(new Wt::WFitLayout());
        _filetable = boost::shared_ptr<InputFileTable > (new InputFileTable());

        _filetable->setAlternatingRowColors(true);
        _filetable->setHighlightMouseOver(true);
        _filetable->setSelectionBehavior(Wt::SelectRows);
        _filetable->setSelectionMode(Wt::SingleSelection);
        _filetable->itemSelectionChanged.connect(SLOT(this, InputFilePanel::enableButtons));
        layout()->addWidget(_filetable.get());
        layout()->setContentsMargins(0, 0, 0, 0);


        setTopToolBar(new Wt::Ext::ToolBar());

        Wt::Ext::Button * addVideoButton = topToolBar()->addButton("Add Input Video");
        addVideoButton->setIcon("icons/add-icon.png");
        topToolBar()->addSeparator();
        removeVideoButton = topToolBar()->addButton("Remove Input Video");
        removeVideoButton->setIcon("icons/remove-icon.png");
        removeVideoButton->setEnabled(false);
        addVideoButton->clicked.connect(SLOT(this, InputFilePanel::addVideoButtonClicked));
        removeVideoButton->clicked.connect(SLOT(this, InputFilePanel::removeVideo));

      }

      void InputFilePanel::setProject(boost::shared_ptr<db::Project> p) {
        _filetable->setMediaFiles(p->mediafiles().get().all());
        _project = p;
      }

      void InputFilePanel::setInputFile(std::list<Ptr<org::esb::io::File> > files) {
        /*
        LOGDEBUG("try import file count" << files.size());
        std::list<Ptr<org::esb::io::File> >::iterator fit = files.begin();
        _chooser->accept();
        _chooser.reset();
        for (; fit != files.end(); fit++) {
          importFile(*fit);
        }*/


        /*
          dialog->setLayout(new Wt::WFitLayout());
          dialog->resize(400,150);
          dialog->layout()->addWidget(new Wt::WText(message));
          dialog->addButton(new Wt::Ext::Button("Continue"));
          dialog->buttons().back()->clicked.connect(SLOT(dialog, Wt::Ext::Dialog::accept));
          dialog->show();
          dialog->exec();*/
        removeVideoButton->setEnabled(false);
      }

      void InputFilePanel::countFiles(Ptr<org::esb::io::File> path) {
        if(_cycle%100==0){
          std::string message="<br/>Calculating the Files to import:<br/>";
          progressText->setText(message+org::esb::util::StringUtil::toString(_fileCount)+std::string(" Files found and keeps counting."));
          Wt::WApplication::instance()->processEvents();
        }
        _cycle++;
        if (!path->exists())return;
        if (path->isDirectory()) {
          org::esb::io::FileList flist = path->listFiles();
          org::esb::io::FileList::iterator fit = flist.begin();
          for (; fit != flist.end(); fit++)
            countFiles(*fit);
        } else {
          _fileCount++;
        }
      }

      void InputFilePanel::importFiles(std::list<Ptr<org::esb::io::File> > files) {
        std::list<Ptr<org::esb::io::File> >::iterator cit = files.begin();
        _cycle=0;
        _fileCount=0;
        _importCount=0;
        for (; cit != files.end(); cit++) {
          countFiles(*cit);
        }
        std::list<Ptr<org::esb::io::File> >::iterator fit = files.begin();
        for (; fit != files.end(); fit++) {
          importFile(*fit);
        }
        _filetable->setMediaFiles(_project->mediafiles().get().all());
      }

      void InputFilePanel::importFile(Ptr<org::esb::io::File> file) {
          std::string message="<br/>Importing the Files :<br/>";
          progressText->setText(message+org::esb::util::StringUtil::toString(_importCount)+"/"+org::esb::util::StringUtil::toString(_allCount)+std::string(" Files from")+org::esb::util::StringUtil::toString(_fileCount)+" imported.");
          Wt::WApplication::instance()->processEvents();
        //importFile(*fit);
        if (file->exists()) {
          if (file->isFile()) {
            _allCount++;
            progressText->setText(file->getFileName());
            int result = import(*file.get());
            if (result > 0) {
              db::MediaFile mfile = litesql::select<db::MediaFile > (_project->getDatabase(), db::MediaFile::Id == result).one();
              _project->mediafiles().link(mfile);
              _importCount++;
              LOGDEBUG("File imported:" << result);
            } else {
              LOGINFO("no valid file")
            }
          } else
            if (file->isDirectory()) {
            LOGDEBUG(file->getPath() << " isDirectory");
            org::esb::io::FileList files = file->listFiles();
            org::esb::io::FileList::iterator files_it = files.begin();
            for (; files_it != files.end(); files_it++) {
              importFile(*files_it);
            }
          }
        }
      }

      void InputFilePanel::removeVideo() {

        int pid = atoi(boost::any_cast<string > (_filetable->model()->data(_filetable->selectedRows()[0], 0)).c_str());
        LOGDEBUG("remove Video File id:" << pid);
        vector<db::MediaFile> files = _project->mediafiles().get().all();
        vector<db::MediaFile>::iterator it = files.begin();

        for (; it != files.end(); it++) {
          if ((*it).id == pid) {
            LOGDEBUG("Video File found id:" << pid);
            _project->mediafiles().unlink((*it));
          }
        }
        _filetable->setMediaFiles(_project->mediafiles().get().all());
        removeVideoButton->setEnabled(false);
      }

      InputFilePanel::InputFilePanel(const InputFilePanel& orig) {
      }

      InputFilePanel::~InputFilePanel() {
        LOGDEBUG("InputFilePanel::~InputFilePanel() ");
      }

      void InputFilePanel::enableButtons() {
        if (_filetable->selectedRows().size() > 0) {
          removeVideoButton->setEnabled(true);
        } else {
          removeVideoButton->setEnabled(false);
        }
      }

      void InputFilePanel::addVideoButtonClicked() {
        _chooser = Ptr<FileChooser > (new FileChooser("Add Video", "/"));
        //        _chooser->selected.connect(SLOT(this, InputFilePanel::setInputFile));
        _chooser->show();
        if (_chooser->exec() == Wt::Ext::Dialog::Accepted) {
          std::list<Ptr<org::esb::io::File> > files = _chooser->getSelectedFiles();
          //          boost::thread(boost::bind(&InputFilePanel::importFiles, this, files));
          _chooser->setHidden(true);
          dial = new Wt::Ext::Dialog("bla");
          dial->resize(300, 200);
          dial->setLayout(new Wt::WFitLayout());
          progressMessageText = new Wt::WText();
          progressText = new Wt::WText();
          dial->layout()->addWidget(progressText);
          dial->show();

          //dial->exec();
          //Wt::WMessageBox::show("really", "Wtf", Wt::Ok|Wt::Cancel);
          //org::esb::lang::Thread::sleep2(10000);
          LOGDEBUG("importing");
          importFiles(files);
          dial->accept();
          delete dial;


        }
        removeVideoButton->setEnabled(false);
      }
    }
  }
}

