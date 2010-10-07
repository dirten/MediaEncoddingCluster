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

      VideoAdvanceTableView::VideoAdvanceTableView(std::map<std::string, std::string>&dat) : _dat(dat), Wt::Ext::TableView() {
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
        options = new Wt::Ext::ComboBox();
        //        options->activated().connect(SLOT(this, VideoAdvanceTableView::dataChanged));
        //model()->dataChanged().connect(SLOT(this, VideoAdvanceTableView::dataChanged));
        (static_cast<Wt::WStandardItemModel*> (model()))->itemChanged().connect(SLOT(this, VideoAdvanceTableView::itemChanged));
        //itemSelectionChanged().connect(SLOT(this, VideoAdvanceTableView::itemSelectionChangedMethod));
        line = new Wt::Ext::LineEdit();

        /*
         * fill up options -> help map
         */
        const AVOption * option = NULL;
        AVCodecContext * codec = avcodec_alloc_context2(AVMEDIA_TYPE_VIDEO);
        while (option = av_next_option(codec, option)) {
          if (option->offset > 0 &&
                  option->flags & AV_OPT_FLAG_ENCODING_PARAM &&
                  option->flags & AV_OPT_FLAG_VIDEO_PARAM
                  ) {

            std::string name = option->name;
            std::string help=name;
            help+="(";
            if (option->help) {
              help += option->help;
            }
            help+=")";
            _options[help] = name;
          }
        }
        std::map<std::string, std::string>::iterator op_it = _options.begin();
        for (; op_it != _options.end(); op_it++) {
          std::string op = (*op_it).first;
          options->addItem(op);
        }
        setEditor(0, options);
        setEditor(1, line);
        static_cast<VideoAdvanceTableModel*> (model())->setTableData(_dat);
        _dontChangeModel=false;
      }

      VideoAdvanceTableView::~VideoAdvanceTableView() {
      }

      void VideoAdvanceTableView::refresh() {
        _dontChangeModel=true;
        static_cast<VideoAdvanceTableModel*> (model())->setTableData(_dat);
        _dontChangeModel=false;
      }
      
      void VideoAdvanceTableView::itemSelectionChangedMethod(){
        LOGDEBUG("void VideoAdvanceTableView::itemSelectionChanged()");
      }

      void VideoAdvanceTableView::itemChanged(Wt::WStandardItem * item) {
        /*_dontChangeModel will be used in the resulting event of ModelChanged
         * when the model will be refeshed, this shuffles the _dat variable
         */
        if(_dontChangeModel)return;
        if (item->row() >= 0 && item->column() >= 0 && !model()->data(item->row(), 0).empty()) {
          std::string name = boost::any_cast<std::string > (model()->data(item->row(), 0));
          std::string value;
          if (!model()->data(item->row(), 1).empty())
            value = boost::any_cast<std::string > (model()->data(item->row(), 1));

          std::string key;
          if(_options.find(name)!=_options.end()){
            std::string key=(*_options.find(name)).second;
            if(_dat.find(key)==_dat.end()||value.length()>0){
              _dat[key]=value;
              LOGDEBUG(" item changed name=" << (*_options.find(name)).second << " value=" << value);
              changed.emit();
            }
          }
        }
      }

      void VideoAdvanceTableView::dataChanged(Wt::WModelIndex old, Wt::WModelIndex newdata) {
        LOGDEBUG("data changed");

        std::string newd = newdata.data().empty() ? "" : boost::any_cast<std::string > (newdata.data());
        std::string oldd = old.data().empty() ? "" : boost::any_cast<std::string > (old.data());
        LOGDEBUG("boost any " << oldd);
        LOGDEBUG("boost any " << newd);
        if (old.data().empty())return;
        std::string name = boost::any_cast<std::string > (static_cast<VideoAdvanceTableModel*> (model())->data(old.row(), 0));
        LOGDEBUG("setting name=" << name);
        if (!model()->data(old.row(), 1).empty()) {
          std::string value = boost::any_cast<std::string > (static_cast<VideoAdvanceTableModel*> (model())->data(old.row(), 1));
          LOGDEBUG("setting value=" << value);
        }


        /*
        std::string name=options->currentText().narrow();
        std::string value=line->text().narrow();
        _dat[name]=value;
        LOGDEBUG("setting name="<<name<<" value="<<value<<" index "<<options->currentIndex());
         */
        //refresh();
        changed.emit();
      }

      void VideoAdvanceTableView::addOption() {
        //        _dat["please select an option"]="";
        //        refresh();
        model()->insertRow(model()->rowCount());
      }

      void VideoAdvanceTableView::removeOption() {

      }
    }
  }
}
