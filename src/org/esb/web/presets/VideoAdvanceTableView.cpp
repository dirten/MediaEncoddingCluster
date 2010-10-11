/* 
 * File:   VideoAdvanceTableView.cpp
 * Author: HoelscJ
 * 
 * Created on 6. Oktober 2010, 13:02
 */

#include "VideoAdvanceTableView.h"
#include "Wt/WStandardItemModel"
#include "Wt/Ext/ToolBar"
#include "org/esb/av/AV.h"
#include "org/esb/util/Log.h"
#include "Wt/WStandardItem"
#include "../wtk/ComboBox.h"
#include "../wtk/KeyValueModel.h"

namespace org {
  namespace esb {
    namespace web {

      class VideoAdvanceTableModel : public Wt::WStandardItemModel {
      public:

        VideoAdvanceTableModel() : Wt::WStandardItemModel() {
          insertColumns(0, 2);
          setHeaderData(0, std::string("Name"));
          setHeaderData(1, std::string("Value"));
        }

        void setTableData(std::map<std::string, std::string> & data) {
          std::map<std::string, std::string>::iterator it = data.begin();
          if (rowCount() > data.size()) {
            if (data.size() > 0)
              removeRow(data.size() - 1);
            else {
              removeRows(0, rowCount());
            }
          }

          for (int a = 0; it != data.end(); it++, a++) {
            if (rowCount() <= a)
              insertRow(rowCount());
            setData(a, 0, (*it).first);
            setData(a, 1, (*it).second);
          }
          //          insertRow(rowCount());
          //          setData(rowCount() - 1, 0, std::string("add new entry"));
        }

        void refresh() {

        }
      };

      VideoAdvanceTableView::VideoAdvanceTableView(std::map<std::string, std::string>&dat, int flag) : _dat(dat), Wt::Ext::TableView() {
        setModel(new VideoAdvanceTableModel());
        resize(400, 300);
        setColumnWidth(0, 200);
        setAutoExpandColumn(1);
        /*
         * setting up the toolbar
         */
        setTopToolBar(new Wt::Ext::ToolBar());
        Wt::Ext::Button * addOptionButton = topToolBar()->addButton("Add Option");
        addOptionButton->setIcon("icons/add-icon.png");
        addOptionButton->clicked().connect(SLOT(this, VideoAdvanceTableView::addOption));
        topToolBar()->addSeparator();
        removeOptionButton = topToolBar()->addButton("Remove Option");
        removeOptionButton->setIcon("icons/remove-icon.png");
        removeOptionButton->setEnabled(false);
        removeOptionButton->clicked().connect(SLOT(this, VideoAdvanceTableView::removeOption));
        /*
         * here now the Column Editor for the Options Table
         */
        options = new ComboBox();
        //options->activated().connect(SLOT(this, VideoAdvanceTableView::dataChanged));
        //model()->dataChanged().connect(SLOT(this, VideoAdvanceTableView::dataChanged));
        (static_cast<Wt::WStandardItemModel*> (model()))->itemChanged().connect(SLOT(this, VideoAdvanceTableView::itemChanged));
        cellClicked().connect(SLOT(this, VideoAdvanceTableView::itemSelectionChangedMethod));
        line = new Wt::Ext::LineEdit();
        //line->changed().connect(SLOT(this, VideoAdvanceTableView::dataChanged));

        /*
         * fill up options -> help map
         */
        KeyValueModel * options_model = new KeyValueModel();
        const AVOption * option = NULL;
        AVCodecContext * codec = avcodec_alloc_context2(AVMEDIA_TYPE_VIDEO);
        while (option = av_next_option(codec, option)) {
          if (option->offset > 0 &&
                  option->flags & AV_OPT_FLAG_ENCODING_PARAM &&
                  option->flags & flag
                  ) {

            std::string name = option->name;
            std::string help = name;
            help += "(";
            if (option->help) {
              help += option->help;
            }
            help += ")";
            options_model->addModelData(name, help);
            //            _options[help] = name;
          }
        }
        options_model->sort(1, Wt::AscendingOrder);
        options->setModel(options_model);
        options->setModelColumn(1);

        setEditor(0, options);
        setEditor(1, line);
        refresh();
      }

      VideoAdvanceTableView::~VideoAdvanceTableView() {
      }

      void VideoAdvanceTableView::refresh() {
        LOGDEBUG("refresh");
        _dontChangeModel = true;
        static_cast<VideoAdvanceTableModel*> (model())->setTableData(_dat);
        _dontChangeModel = false;
      }

      void VideoAdvanceTableView::itemSelectionChangedMethod() {
        LOGDEBUG("void VideoAdvanceTableView::itemSelectionChanged()");
        removeOptionButton->setEnabled(true);
      }

      void VideoAdvanceTableView::itemChanged(Wt::WStandardItem * item) {
        /*_dontChangeModel will be used in the resulting event of ModelChanged
         * when the model will be refeshed, this shuffles the _dat variable
         */
        if (_dontChangeModel)return;
        if (item->row() >= 0 && item->column() >= 0 && !model()->data(item->row(), 0).empty()) {
          std::string name = boost::any_cast<std::string > (model()->data(item->row(), 0));
          std::string value;

          if (!model()->data(item->row(), 1).empty())
            value = boost::any_cast<std::string > (model()->data(item->row(), 1));
          int count=options->count();
          for(int a=0;a<count;a++){
            if(options->data(a,1)==name)
              name=options->data(a,0);
          }
          _dat[name] = value;
          LOGDEBUG(" item changed name=" << name << " value=" << value);
          changed.emit();
        }
      }

      void VideoAdvanceTableView::dataChanged(int data) {
        std::string key = options->data(data, 0);
        model()->setData(currentRow(), 0, key);
      }

      void VideoAdvanceTableView::addOption() {
        model()->insertRow(model()->rowCount());
        model()->setData(model()->rowCount()-1,0,std::string("double click to edit"));
        //model()->setData(model()->rowCount()-1,1,std::string("to edit"));
      }

      void VideoAdvanceTableView::removeOption() {
        std::string key=boost::any_cast<std::string>(model()->data(this->currentRow(),0));
        model()->removeRow(this->currentRow());
        _dat.erase(key);
      }
    }
  }
}
