
#include "ProfileCreator.h"
#include "Wt/Ext/Panel"
#include "Wt/Ext/CheckBox"
#include "Wt/Ext/TabWidget"
#include "Wt/Ext/Button"
#include "Wt/WBorderLayout"
#include "Wt/WGridLayout"
#include "Wt/WFitLayout"
#include "Wt/WContainerWidget"
#include "Wt/WText"
#include "Wt/WObject"
#include <Wt/WApplication>

#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

#include "SqlUtil.h"
namespace org {
  namespace esb {
    namespace web {
      ProfileCreator::ProfileCreator() : Wt::Ext::Dialog("ProfileCreator") {

      }

      ProfileCreator::ProfileCreator(db::Profile&profile) : Wt::Ext::Dialog("ProfileCreator") {
        LOGTRACEMETHOD("ProfileCreator")
        std::string res = std::string(org::esb::config::Config::getProperty("hive.base_path"));
        res.append("/res/profile_creator");
        Wt::WApplication::instance()->messageResourceBundle().use(res.c_str(), false);
        LOGDEBUG("loading resource bundle" << res << " for Locale:" << Wt::WApplication::instance()->locale());
        resize(700, 430);
        setBorder(false);
        Wt::WBorderLayout *layout = new Wt::WBorderLayout();
        setLayout(layout);

        tab = new Wt::Ext::TabWidget();
        {
          LOGTRACEMETHOD("GeneralPanel")
          tab->addTab(new GeneralPanel(profile), "General");
        }
        {
          LOGTRACEMETHOD("FilePanel")
          tab->addTab(new FilePanel(profile), "Format");
        }
        {
            LOGTRACEMETHOD("VideoPanel")
          tab->addTab(new VideoPanel(profile), "Video");
        }
        {
          LOGTRACEMETHOD("AudioPanel")
          tab->addTab(new AudioPanel(profile), "Audio");
        }
//        tab->currentChanged.connect(SLOT(this, ProfileCreator::setContextText));

        layout->addWidget(tab, Wt::WBorderLayout::Center);
        Wt::Ext::Panel * previewPanel = new Wt::Ext::Panel();
        previewPanel->resize(330, Wt::WLength::Auto);
        previewPanel->setResizable(true);
        previewPanel->layout()->addWidget(new Wt::WText(Wt::WString::tr("profile_creator_general")));
        layout->addWidget(previewPanel, Wt::WBorderLayout::East);


        Wt::Ext::Panel * bot = new Wt::Ext::Panel();
        bot->resize(Wt::WLength::Auto, 30);
        bot->setLayout(new Wt::WFitLayout());
        bot->layout()->setContentsMargins(0, 0, 0, 0);


        Wt::WContainerWidget * cont = new Wt::WContainerWidget();
        cont->setLayout(new Wt::WGridLayout());
        cont->layout()->setContentsMargins(2, 2, 0, 0);
        bot->layout()->addWidget(cont);

        addButton(new Wt::Ext::Button("Save"));
        buttons().back()->clicked.connect(SLOT(this, ProfileCreator::save));

        addButton(new Wt::Ext::Button("Cancel"));
        buttons().back()->clicked.connect(SLOT(this, ProfileCreator::cancel));
        //	_el.getElement("id").setText(profile.id);
        int tab_count = tab->count();
        for (int a = 0; a < tab_count; a++) {
          BasePanel * panel = static_cast<BasePanel*> (tab->panel(a));
          panel->setChangedListener();
        }
      }

      void ProfileCreator::setContextText(int tab_index) {
        LOGDEBUG(tab_index);
      }

      void ProfileCreator::cancel() {

        canceled.emit();

        this->done(Rejected);
        //        delete this;
      }

      void ProfileCreator::save() {
        LOGDEBUG("try saving panels");

        int tab_count = tab->count();
        std::map<std::string, std::string>data;
        for (int a = 0; a < tab_count; a++) {
          BasePanel * panel = static_cast<BasePanel*> (tab->panel(a));
          std::map<std::string, std::string>pd = panel->getKeyValue();
          data.insert(pd.begin(), pd.end());
        }
        //        SqlUtil::map2sql("profiles", data);
        saved.emit();
        this->done(Accepted);
        //        delete this;
      }

      void ProfileCreator::setProfile(int id) {
        std::map<std::string, std::string> sqldata;
        SqlUtil::sql2map("profiles", id, sqldata);
        int tab_count = tab->count();
        for (int a = 0; a < tab_count; a++) {
          BasePanel * panel = static_cast<BasePanel*> (tab->panel(a));
          panel->setKeyValue(sqldata);
        }
      }

      void ProfileCreator::show() {
        Wt::Ext::Dialog::show();
      }

      ProfileCreator::~ProfileCreator() {
        LOGDEBUG("freeing ProfileCreator");
      }

      /**
      * GeneralPanel
      * @return
      */
      ProfileCreator::BasePanel::BasePanel(db::Profile&profile) : Wt::Ext::Panel(), _profile(profile) {
        _cont = new Wt::WContainerWidget();
        _cont->setLayout(new Wt::WGridLayout());

        setLayout(new Wt::WFitLayout());
        layout()->addWidget(_cont);
      }
      void ProfileCreator::BasePanel::setChangedListener(){
        LOGDEBUG("setChangedListener");
        std::map<std::string, Wt::Ext::LineEdit*> lel = _el.getElements();
        std::map<std::string, Wt::Ext::LineEdit*>::iterator it = lel.begin();
        for (; it != lel.end(); it++) {
          (*it).second->keyPressed.connect(SLOT(this,ProfileCreator::BasePanel::changed));
          (*it).second->keyWentUp.connect(SLOT(this,ProfileCreator::BasePanel::changed));
        }
        std::map<std::string, Wt::Ext::ComboBox*> lec = _elcb.getElements();
        std::map<std::string, Wt::Ext::ComboBox*>::iterator itc = lec.begin();
        for (; itc != lec.end(); itc++) {
          (*itc).second->keyPressed.connect(SLOT(this,ProfileCreator::BasePanel::changed));
          (*itc).second->activated.connect(SLOT(this,ProfileCreator::BasePanel::changed));

        }

      }
      /*
      ProfileCreator::BasePanel::BasePanel() : Wt::Ext::Panel() {
      _cont = new Wt::WContainerWidget();
      _cont->setLayout(new Wt::WGridLayout());

      setLayout(new Wt::WFitLayout());
      layout()->addWidget(_cont);
      }
      */
      void ProfileCreator::BasePanel::addWidget(Wt::WWidget *widget, int row, int col) {
        static_cast<Wt::WGridLayout*> (_cont->layout())->addWidget(widget, row, col);
      }

      std::map<std::string, std::string> ProfileCreator::BasePanel::getKeyValue() {
        std::map<std::string, std::string> data;

        std::map<std::string, Wt::Ext::LineEdit*> lel = _el.getElements();
        std::map<std::string, Wt::Ext::LineEdit*>::iterator it = lel.begin();
        for (; it != lel.end(); it++) {
          data[(*it).first] = (*it).second->text().narrow();
        }
        std::map<std::string, Wt::Ext::ComboBox*> lec = _elcb.getElements();
        std::map<std::string, Wt::Ext::ComboBox*>::iterator itc = lec.begin();
        for (; itc != lec.end(); itc++) {
          data[(*itc).first] = (*itc).second->text().narrow();
        }
        return data;
      }

      void ProfileCreator::BasePanel::setKeyValue(std::map<std::string, std::string> data) {
        std::map<std::string, Wt::Ext::LineEdit*> lel = _el.getElements();
        std::map<std::string, std::string>::iterator it = data.begin();
        for (; it != data.end(); it++) {
          std::string key = it->first;
          if (lel.find(key) != lel.end()) {
            lel[key]->setText(it->second);
          }
        }
        std::map<std::string, Wt::Ext::ComboBox*> lec = _elcb.getElements();
        std::map<std::string, std::string>::iterator itc = data.begin();
        for (; itc != data.end(); itc++) {
          std::string key = itc->first;
          if (lec.find(key) != lec.end()) {
            int item_count = lec[key]->count();
            for (int a = 0; a < item_count; a++) {
              if (lec[key]->itemText(a).narrow() == itc->second)
                lec[key]->setCurrentIndex(a);
            }
            LOGDEBUG("found key:" << key << " item count:" << lec[key]->count() << " currentIndex:" << lec[key]->currentIndex());
            if (lec[key]->currentIndex() == -1 || lec[key]->count() == 0) {
              lec[key]->addItem(itc->second);
              lec[key]->setCurrentIndex(lec[key]->count() - 1);
            }
          }
        }
      }

      ProfileCreator::BasePanel::~BasePanel() {
        LOGDEBUG("freeing basePanel");
      }

      /**
      * GeneralPanel
      * @return
      */
      ProfileCreator::GeneralPanel::GeneralPanel(db::Profile&profile) : BasePanel(profile) {
        Wt::WGridLayout*l = static_cast<Wt::WGridLayout*> (_cont->layout());
        _el.addElement("id", "Profile Id", profile.id, l)->setEnabled(false);
        _el.addElement("profile_name", "Profile Name", profile.name, l);
        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, -1);      
      }

      void ProfileCreator::GeneralPanel::changed(){
        LOGDEBUG("GeneralPanel changed");
        _profile.name=(std::string)_el.getElement("profile_name")->text().narrow();
      }

      std::map<std::string, std::string> ProfileCreator::GeneralPanel::getKeyValue() {
        return BasePanel::getKeyValue();
      }

      void ProfileCreator::GeneralPanel::setKeyValue(std::map<std::string, std::string> data) {
        BasePanel::setKeyValue(data);
      }

      ProfileCreator::GeneralPanel::~GeneralPanel() {
        LOGDEBUG("freeing generalPanel");

      }

      /**
      * FilePanel
      * @return
      */

      ProfileCreator::FilePanel::FilePanel(db::Profile & profile) : BasePanel(profile) {
        Wt::WGridLayout*l = static_cast<Wt::WGridLayout*> (_cont->layout());
        Wt::Ext::ComboBox * v_format = _elcb.addElement("v_format", "Container Fromat", profile.format, l);
        v_format->setTextSize(50);
        v_format->resize(180, Wt::WLength::Auto);
        AVOutputFormat *ofmt = NULL;
        int a = 0;

        while ((ofmt = av_oformat_next(ofmt))) {
          v_format->addItem(ofmt->long_name);
          //          formatid2formatidx[p->id] = a++;
          //          formatname2formatidx[ofmt->long_name] = a;
          //          formatidx2formatname[a] = ofmt->long_name;
          a++;
        }
        Wt::Ext::ComboBox * v_format_ext = _elcb.addElement("v_format_ext", "File Extension", profile.formatext, l);
        v_format_ext->setTextSize(50);
        v_format_ext->resize(180, Wt::WLength::Auto);
        v_format_ext->setEditable(true);
        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, -1);
        v_format->activated.connect(SLOT(this, ProfileCreator::FilePanel::setAvailableExtensions));
      }

      void ProfileCreator::FilePanel::changed(){
        LOGDEBUG("FilePanel changed");
        _profile.format=(std::string)_elcb.getElement("v_format")->text().narrow();
        _profile.formatext=(std::string)_elcb.getElement("v_format_ext")->text().narrow();
      }

      void ProfileCreator::FilePanel::setAvailableExtensions() {
        Wt::Ext::ComboBox * v_format = _elcb.getElement("v_format");
        Wt::Ext::ComboBox * v_format_ext = _elcb.getElement("v_format_ext");
        v_format_ext->clear();
        std::string longname = v_format->currentText().narrow();
        AVOutputFormat *ofmt = NULL;
        while ((ofmt = av_oformat_next(ofmt))) {
          if (longname == ofmt->long_name) {
            if (ofmt->extensions && std::strlen(ofmt->extensions) > 0) {
              org::esb::util::StringTokenizer tok(ofmt->extensions, ",");
              while (tok.hasMoreTokens()) {
                v_format_ext->addItem(tok.nextToken());
              }
              v_format_ext->setCurrentIndex(0);
            }
          }
        }
      }

      std::map<std::string, std::string> ProfileCreator::FilePanel::getKeyValue() {
        return BasePanel::getKeyValue();

      }

      void ProfileCreator::FilePanel::setKeyValue(std::map<std::string, std::string> data) {

        BasePanel::setKeyValue(data);
        setAvailableExtensions();
        BasePanel::setKeyValue(data);
      }

      ProfileCreator::FilePanel::~FilePanel() {
        LOGDEBUG("freeing filePanel");
      }

      /**
      * GeneralPanel
      * @return
      */

      ProfileCreator::VideoPanel::VideoPanel(db::Profile&profile) : BasePanel(profile) {
        Wt::WGridLayout*l = static_cast<Wt::WGridLayout*> (_cont->layout());
        Wt::Ext::ComboBox * v_codec = _elcb.addElement("v_codec", "Codec", "", l);
        //        Wt::Ext::CheckBox * v_codec = _elchkb.addElement("v_aspect_ration", "Codec", "", l);
        v_codec->setTextSize(50);
        //        v_codec->resize(180, Wt::WLength::Auto);
        AVCodec *p = NULL;
        int a = 0;
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_VIDEO) {
            v_codec->addItem(p->long_name);
            if(p->id==(int)profile.vcodec){
              v_codec->setCurrentIndex(a);
            }
            a++;
          }
        }
        v_codec->activated.connect(SLOT(this, ProfileCreator::VideoPanel::setPredefinedCodecFlags));

        _el.addElement("v_bitrate", "Video Bitrate", profile.vbitrate, l);
        Wt::Ext::ComboBox * v_framerate = _elcb.addElement("v_framerate", "Video Framerate", profile.vframerate, l);
        v_framerate->setTextSize(50);
        //        v_codec->resize(180, Wt::WLength::Auto);
        org::esb::util::StringTokenizer st(org::esb::config::Config::getProperty("framerates"), ",");

        int c = st.countTokens();
        v_framerate->addItem("same as source");
        for (int a = 1; a <= c; a++) {
          std::string t = st.nextToken();
          org::esb::util::StringTokenizer st2(t, "#");
          if (st2.countTokens() == 2) {
            st2.nextToken();
            std::string t2 = st2.nextToken();
            v_framerate->addItem(t2);
            if(t2==(std::string)profile.vframerate)
              v_framerate->setCurrentIndex(a);
          }
        }
        /*
        Wt::Ext::ComboBox * v_aspect = _elcb.addElement("_v_keep_aspect_ratio", "Aspect Ratio", "", l);
        v_aspect->setTextSize(50);
        v_aspect->addItem("Rescale to width and height defined below");
        v_aspect->addItem("Keep Aspect Ratio from Source");*/
        //        v_aspect->addItem("Use 4:3");
        //        v_aspect->addItem("Use 16:9");

        /*        Wt::Ext::ComboBox * v_deinterlace = _elcb.addElement("_v_deinterlace", "Deinterlace", "", l);
        v_deinterlace->setTextSize(50);
        v_deinterlace->addItem("No");
        v_deinterlace->addItem("Yes");*/

        //_el.addElement("v_width", "Video Width", profile.vwidth, l);
        //_el.addElement("v_height", "Video Height", profile.vheight, l);

        Wt::Ext::ComboBox * vpre = _elcb.addElement("_vpre", "Predefined Flags", "", l);
        vpre->setTextSize(50);

        _el.addElement("v_extra", "Extra Flags", profile.vextra, l);

        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, -1);

        setPredefinedCodecFlags();
      }
      void ProfileCreator::VideoPanel::changed(){
        LOGDEBUG("VideoPanel changed");
        //	_profile.vcodec=atoi(_el.getElement("v_codec")->text().narrow().c_str());
        AVCodec *p = NULL;
        int a = 0;
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_VIDEO && _elcb.getElement("v_codec")->currentText().narrow() == std::string(p->long_name)) {
            _profile.vcodec = (int)p->id;
          }
        }
        _profile.vframerate=_elcb.getElement("v_framerate")->text().narrow();
        _profile.vbitrate=atoi(_el.getElement("v_bitrate")->text().narrow().c_str());
        _profile.vwidth=atoi(_el.getElement("v_width")->text().narrow().c_str());
        _profile.vheight=atoi(_el.getElement("v_height")->text().narrow().c_str());
        _profile.vextra=_el.getElement("v_extra")->text().narrow();

      }

      void ProfileCreator::VideoPanel::setPredefinedCodecFlags() {
        Wt::Ext::ComboBox * v_codec = _elcb.getElement("v_codec");
        Wt::Ext::ComboBox * vpre = _elcb.getElement("_vpre");
        vpre->activated.connect(SLOT(this, ProfileCreator::VideoPanel::setSelectedPredifinedCodecFlags));
        std::string longname = v_codec->currentText().narrow();
        LOGDEBUG( "LonName=" << longname);
        AVCodec *p = NULL;
        int a = 0;
        vpre->clear();
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_VIDEO && longname == p->long_name) {
            LOGINFO( "retriving extra flags for codec id " << p->id);
            db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
            vector<db::CodecPreset> presets=litesql::select<db::CodecPreset>(db, db::CodecPreset::Codecid==p->id).all();
            //            std::string sql = "SELECT * FROM codec WHERE codec_id=:id";
            //            org::esb::sql::Connection con(std::string(config::Config::getProperty("db.connection")));
            //            org::esb::sql::PreparedStatement pstmt = con.prepareStatement(sql.c_str());
            //            pstmt.setInt("id", p->id);
            //            org::esb::sql::ResultSet rs = pstmt.executeQuery();
            for (vector<db::CodecPreset>::iterator it=presets.begin() ;it!=presets.end();it++) {
              vpre->addItem((std::string)(*it).name);
            }
            //            std::map<std::string, std::string> data;
            //            SqlUtil::sql2map("codec","codec_id", p->id, data);
          }
        }
      }

      void ProfileCreator::VideoPanel::setSelectedPredifinedCodecFlags() {
        Wt::Ext::ComboBox * vpre = _elcb.getElement("_vpre");
        std::string name = vpre->currentText().narrow();
        LOGDEBUG( "Name=" << name);
        db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
        db::CodecPreset preset=litesql::select<db::CodecPreset>(db, db::CodecPreset::Name==name).one();

        //        std::map<std::string, std::string> data;
        //        SqlUtil::sql2map("codec", "name", name, data);
        Wt::Ext::LineEdit * v_extra = _el.getElement("v_extra");
        v_extra->setText((std::string)preset.preset);
        LOGDEBUG( "extra=" << preset);
      }

      std::map<std::string, std::string> ProfileCreator::VideoPanel::getKeyValue() {
        std::map<std::string, std::string> data = BasePanel::getKeyValue();
        std::string vcodec = data["v_codec"];
        AVCodec *p = NULL;
        int a = 0;
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_VIDEO && vcodec == p->long_name) {
            data["v_codec"] = org::esb::util::StringUtil::toString(p->id);
          }
        }

        int idx=_elcb.getElement("_v_keep_aspect_ratio")->currentIndex();
        data["v_keep_aspect_ratio"]=org::esb::util::StringUtil::toString(idx);

        int idx_deinterlace=_elcb.getElement("_v_deinterlace")->currentIndex();
        data["v_deinterlace"]=org::esb::util::StringUtil::toString(idx_deinterlace);

        return data;
      }

      void ProfileCreator::VideoPanel::setKeyValue(std::map<std::string, std::string> data) {
        std::string vcodec = data["v_codec"];
        AVCodec *p = NULL;
        int a = 0;
        while ((p = av_codec_next(p))) {
          if (p->encode &&
            p->type == CODEC_TYPE_VIDEO &&
            vcodec == org::esb::util::StringUtil::toString(p->id)) {
              data["v_codec"] = p->long_name;
          }
        }

        BasePanel::setKeyValue(data);
        setPredefinedCodecFlags();
        _elcb.getElement("_v_keep_aspect_ratio")->setCurrentIndex(atoi(data["v_keep_aspect_ratio"].c_str()));
        _elcb.getElement("_v_deinterlace")->setCurrentIndex(atoi(data["v_deinterlace"].c_str()));
      }

      ProfileCreator::VideoPanel::~VideoPanel() {
        LOGDEBUG("freeing videoPanel");

      }

      /**
      * GeneralPanel
      * @return
      */
      ProfileCreator::AudioPanel::AudioPanel(db::Profile&profile) : BasePanel(profile) {
        Wt::WGridLayout*l = static_cast<Wt::WGridLayout*> (_cont->layout());
        Wt::Ext::ComboBox * a_codec = _elcb.addElement("a_codec", "Audio Codec", "", l);
        a_codec->setTextSize(50);

        AVCodec *p = NULL;
        int a = 0;
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_AUDIO) {
            a_codec->addItem(p->long_name);
            if(p->id==(int)profile.acodec){
              a_codec->setCurrentIndex(a);
            }
            a++;
          }
        }

        Wt::Ext::ComboBox * a_channels = _elcb.addElement("a_channels", "Audio Channels", "", l);
        a_channels->setTextSize(50);
        a_channels->addItem("1");
        a_channels->addItem("2");
        a_channels->setCurrentIndex(profile.achannels==1?0:1);
        Wt::Ext::ComboBox * a_bitrate = _elcb.addElement("a_bitrate", "Audio Bitrate", "", l);
        StringTokenizer stabr(org::esb::config::Config::getProperty("audiobitrates"), ",");
        int c3 = stabr.countTokens();
        for (int a = 0; a < c3; a++) {
          std::string t = stabr.nextToken();
          a_bitrate->addItem(t);
          if(t==org::esb::util::StringUtil::toString(profile.abitrate))
            a_bitrate->setCurrentIndex(a);
        }
        Wt::Ext::ComboBox * a_samplerate = _elcb.addElement("a_samplerate", "Audio Samplerate", "", l);
        StringTokenizer stsr(org::esb::config::Config::getProperty("samplerates"), ",");
        int c2 = stsr.countTokens();
        //        a_samplerate->addItem("same as source");
        for (int a = 0; a < c2; a++) {
          std::string t = stsr.nextToken();
          a_samplerate->addItem(t);
          if(t==org::esb::util::StringUtil::toString(profile.asamplerate))
            a_samplerate->setCurrentIndex(a);
        }
        _el.addElement("a_extra", "Extra Flags", profile.aextra, l);

        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, -1);

      }
      void ProfileCreator::AudioPanel::changed(){
        LOGDEBUG("AudioPanel changed");
        AVCodec *p = NULL;
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_AUDIO && _elcb.getElement("a_codec")->currentText().narrow() == p->long_name) {
            _profile.acodec = (int)p->id;
          }
        }
        _profile.achannels=atoi(_elcb.getElement("a_channels")->currentText().narrow().c_str());
        _profile.abitrate=atoi(_elcb.getElement("a_bitrate")->currentText().narrow().c_str());
        _profile.asamplerate=atoi(_elcb.getElement("a_samplerate")->currentText().narrow().c_str());
        _profile.aextra=(std::string)_el.getElement("a_extra")->text().narrow();
      }

      std::map<std::string, std::string> ProfileCreator::AudioPanel::getKeyValue() {
        std::map<std::string, std::string> data = BasePanel::getKeyValue();
        std::string acodec = data["a_codec"];
        AVCodec *p = NULL;
        while ((p = av_codec_next(p))) {
          if (p->encode && p->type == CODEC_TYPE_AUDIO && acodec == p->long_name) {
            data["a_codec"] = org::esb::util::StringUtil::toString(p->id);
          }
        }
        return data;

      }

      void ProfileCreator::AudioPanel::setKeyValue(std::map<std::string, std::string> data) {
        std::string acodec = data["a_codec"];
        AVCodec *p = NULL;
        while ((p = av_codec_next(p))) {
          if (p->encode &&
            p->type == CODEC_TYPE_AUDIO &&
            acodec == org::esb::util::StringUtil::toString(p->id)) {
              data["a_codec"] = p->long_name;
          }
        }
        BasePanel::setKeyValue(data);
      }

      ProfileCreator::AudioPanel::~AudioPanel() {
        LOGDEBUG("freeing audioPanel");

      }
    }
  }
}

