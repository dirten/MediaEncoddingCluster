/* 
 * File:   TestWebApp.cpp
 * Author: HoelscJ
 *
 * Created on 13. Oktober 2010, 16:26
 */

#include <cstdlib>
#include "Wt/WServer"
#include <Wt/WApplication>

#include <Wt/WEnvironment>
#include <Wt/WBorderLayout>
#include <Wt/WFitLayout>
#include <Wt/WContainerWidget>

#include "org/esb/web/GuiBuilder.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"


#include "org/esb/web/wtk/KeyValueModel.h"
#include "org/esb/web/wtk/ComboBox.h"
#include "org/esb/av/AV.h"
#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/util/StringUtil.h"
using namespace org::esb::web;
using namespace std;

/*
 * 
 */

class TestApp : public Wt::WApplication {
public:

  TestApp(const Wt::WEnvironment & env) : Wt::WApplication(env) {
    data["bitrate"] = "500";
    data["frameSkipPropability"] = "1";
    data["bufferTime"] = "50";
    //    data["periodType"]="3";
    data["SVPL-0"] = "1";
    org::esb::av::FormatBaseStream fbs;

    Wt::Ext::Container*viewPort = new Wt::Ext::Panel(root());
    layout = new Wt::WBorderLayout();
    viewPort->setLayout(layout);


    /*
     * Combobox for the Codec Selector
     */
    KeyValueModel * codec_model = new KeyValueModel();
    AVCodec *codec = NULL;
    int a = 0;
    while ((codec = av_codec_next(codec))) {
      if (codec->encode && codec->type == CODEC_TYPE_VIDEO) {

        codec_model->addModelData(org::esb::util::StringUtil::toString(codec->id), codec->long_name);
      }
    }

   v_codec = new ComboBox();
    v_codec->setModel(codec_model);
    v_codec->setModelColumn(1);
    //v_codec->setSelectedEntry(_parameter["codec"]);
    v_codec->setTextSize(50);
    v_codec->resize(300, 40);
    v_codec->activated().connect(SLOT(this, TestApp::codecSelected));
    main_panel = new Wt::Ext::Panel();
    Wt::WFitLayout * fit = new Wt::WFitLayout();
    main_panel->setLayout(fit);

    //        main_panel->setBorder(true);
    layout->addWidget(main_panel, Wt::WBorderLayout::Center);

    //    layout->addWidget(new Wt::Ext::Panel(), Wt::WBorderLayout::Center);

    layout->addWidget(v_codec, Wt::WBorderLayout::North);



    //    root()->addWidget(builder);
  }

  void codecSelected() {
    std::string path = MEC_SOURCE_DIR;
    std::string codecid=v_codec->data(v_codec->currentIndex());
    LOGDEBUG("CodecId="<<codecid);
    std::string codecname;
    AVCodec *codec = NULL;
    int a = 0;
    while ((codec = av_codec_next(codec))) {
      if (codec->encode && codec->type == CODEC_TYPE_VIDEO && org::esb::util::StringUtil::toString(codec->id)==codecid) {
        LOGDEBUG("found codec : "<<codec->name);
        codecname=codec->name;
      }
    }
    std::string file=path;
    file+="/res/comp/";
    file+=codecname;
    file+=".gui";
    LOGDEBUG("Gui File:"<<file);
    if(!org::esb::io::File(file).exists()){
      file=path + "/res/comp/test.gui";
    }
    GuiBuilder * builder = new GuiBuilder(file, data);
    //    builder->dataChanged.connect(SLOT(this, TestApp::dataChanged));
//    Wt::WLayoutItem * item = layout->itemAt(Wt::WBorderLayout::Center);

    if (main_panel->layout()->count() > 0) {
      Wt::WLayoutItem * item = main_panel->layout()->itemAt(0);
      main_panel->layout()->removeItem(item);
      delete item->widget();
    }
    main_panel->layout()->addWidget(builder);
    main_panel->refresh();

    //layout->addWidget(builder, Wt::WBorderLayout::Center);
    //builder->setLayout(new Wt::WFitLayout());
    //builder->resize(600, 400);

  }

  void dataChanged() {
    LOGDEBUG("DataChanged");
    std::map<std::string, std::string>::iterator it = data.begin();
    for (; it != data.end(); it++) {
      LOGDEBUG("Key=" << (*it).first << " val=" << (*it).second);
    }
  }
  std::map<std::string, std::string> data;
  Wt::WBorderLayout*layout;
  Wt::Ext::Panel * main_panel;
  ComboBox * v_codec;
};

Wt::WApplication *createApp(const Wt::WEnvironment& env) {
  return new TestApp(env);
}

int main(int argc, char** argv) {
  Log::open("");
  std::string base_path = MEC_SOURCE_DIR;
  std::string docroot = (base_path + "/web");
  const char * args[] = {
    "mhive",
    "--docroot", const_cast<char*> (docroot.c_str()),
    "--http-address", "0.0.0.0",
    "--http-port", "8080",
    "--accesslog", "access.log",
    "--no-compression",
    "--deploy-path", "/"
  };
  for (int a = 0; a < 12; a++) {
    LOGDEBUG(args[a]);
  }

  Wt::WServer server("testserver");
  server.setServerConfiguration(12, const_cast<char**> (args), WTHTTP_CONFIGURATION);
  server.addEntryPoint(Wt::Application, &createApp, "/");
  server.start();
  Wt::WServer::waitForShutdown();

  return 0;
}

