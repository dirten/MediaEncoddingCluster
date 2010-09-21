#include "ProfilesForm.h"

#include <Wt/WTable>
#include <Wt/Ext/Button>
#include "org/esb/av/AV.h"
#include "org/esb/util/Decimal.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/config/config.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "SqlUtil.h"
namespace org {
  namespace esb {
    namespace web {

      ProfilesForm::ProfilesForm(Wt::WContainerWidget * parent) : Wt::WContainerWidget(parent) {
        //        Wt::WTable * t = new Wt::WTable(parent);
        int i = 0;
        Wt::WGridLayout * l = new Wt::WGridLayout();
        //        l->setVerticalSpacing(3);
        //        l->setHorizontalSpacing(0);
        //        l->setMinimumSize(Wt::WLength::Auto, 30);
        setLayout(l);

        _el.addElement("id", "Profile Id", "", l)->setEnabled(false);
        _el.addElement("profile_name", "Profile Name", "", l);
        Wt::Ext::ComboBox * v_format = _elcb.addElement("v_format", "Container Fromat", "", l);
        v_format->setTextSize(50);
        AVOutputFormat *ofmt = NULL;
        int a = 0;

        while ((ofmt = av_oformat_next(ofmt))) {
          v_format->addItem(ofmt->long_name);
          //					formatid2formatidx[p->id] = a++;
          formatname2formatidx[ofmt->long_name] = a;
          formatidx2formatname[a] = ofmt->long_name;
          a++;
        }
        Wt::Ext::ComboBox * v_codec = _elcb.addElement("v_codec", "Codec", "", l);
        v_codec->setTextSize(50);
        {
          AVCodec *p = NULL;
          int a = 0;
          while ((p = av_codec_next(p))) {
            if (p->encode && p->type == CODEC_TYPE_VIDEO) {
              v_codec->addItem(p->long_name);
              vcodecid2vcodecidx[p->id] = a++;
              codecname2codecid[p->long_name] = p->id;
              codecid2codecname[p->id] = p->long_name;
            }
          }
        }
        _el.addElement("v_bitrate", "Video Bitrate", "", l);
        Wt::Ext::ComboBox * v_framerate = _elcb.addElement("v_framerate", "Video Framerate", "", l);
        StringTokenizer st(org::esb::config::Config::getProperty("framerates"), ",");

        int c = st.countTokens();
        v_framerate->addItem("same as source");
        for (int a = 1; a <= c; a++) {
          std::string t = st.nextToken();
          StringTokenizer st2(t, "#");
          if (st2.countTokens() == 2) {
            st2.nextToken();
            std::string t2 = st2.nextToken();
            v_framerate->addItem(t2);
            framerates2idx[t2] = a;
          }
        }
        //        _el.getElement("v_framerate", "Video Framerate", "", t->elementAt(i++, 0));
        _el.addElement("v_width", "Video Width", "", l);
        _el.addElement("v_height", "Video Height", "", l);
        //        _el.addElement("v_height", "Video Height", "", l);
        Wt::Ext::ComboBox * a_codec = _elcb.addElement("a_codec", "Audio Codec", "", l);
        a_codec->setTextSize(50);
        {
          AVCodec *p = NULL;
          int a = 0;
          while ((p = av_codec_next(p))) {
            if (p->encode && p->type == CODEC_TYPE_AUDIO) {
              a_codec->addItem(p->long_name);
              acodecid2acodecidx[p->id] = a++;
              codecname2codecid[p->long_name] = p->id;
              codecid2codecname[p->id] = p->long_name;
            }
          }
        }
        _el.addElement("a_channels", "Audio Channels", "", l);
        //_el.addElement("a_bitrate", "Audio Bitrate", "", l);
        Wt::Ext::ComboBox * a_bitrate = _elcb.addElement("a_bitrate", "Audio Bitrate", "", l);
        StringTokenizer stabr(org::esb::config::Config::getProperty("audiobitrates"), ",");
        int c3 = stabr.countTokens();
        //        a_samplerate->addItem("same as source");
        for (int a = 0; a < c3; a++) {
          std::string t = stabr.nextToken();
          a_bitrate->addItem(t);
          audiobitrates2idx[t] = a;
        }

        //        _el.addElement("a_samplerate", "Audio Samplerate", "", l);
        Wt::Ext::ComboBox * a_samplerate = _elcb.addElement("a_samplerate", "Audio Samplerate", "", l);
        StringTokenizer stsr(org::esb::config::Config::getProperty("samplerates"), ",");
        int c2 = stsr.countTokens();
        //        a_samplerate->addItem("same as source");
        for (int a = 0; a < c2; a++) {
          std::string t = stsr.nextToken();
          a_samplerate->addItem(t);
          samplerates2idx[t] = a;
        }


        i++;
        i++;
        int btrow = l->rowCount();
        Wt::Ext::Button *cancel = new Wt::Ext::Button("Cancel");
        l->addWidget(cancel, btrow, 0);
        Wt::Ext::Button *save = new Wt::Ext::Button("Save");
        l->addWidget(save, btrow, 1);
        cancel->clicked().connect(SLOT(this, ProfilesForm::cancelProfile));
        save->clicked().connect(SLOT(this, ProfilesForm::saveProfile));

      }

      void ProfilesForm::cancelProfile() {
        profileCanceled.emit();
      }

      void ProfilesForm::setProfile(int profile_id) {
        if (profile_id > 0) {
          using namespace org::esb::sql;
          using namespace org::esb::config;
          Connection con(std::string(Config::getProperty("db.connection")));
          Statement stmt = con.createStatement(std::string("select * from profiles where id=").append(Decimal(profile_id).toString()).c_str());
          ResultSet rs = stmt.executeQuery();
          if (rs.next()) {
            _el.getElement("id")->setText(rs.getString("id"));
            _el.getElement("profile_name")->setText(rs.getString("profile_name"));
            ////			elements["v_format"]->setText(rs.getString("v_format"));
            //			elements["v_codec"]->setText(codecid2codecname[rs.getInt("v_codec")]);
            ((Wt::Ext::ComboBox*)_elcb.getElement("v_format"))->setCurrentIndex(formatname2formatidx[rs.getString("v_format")]);
            ((Wt::Ext::ComboBox*)_elcb.getElement("v_codec"))->setCurrentIndex(vcodecid2vcodecidx[rs.getInt("v_codec")]);
            //			((Wt::Ext::ComboBox*)elements["v_codec"])->setCurrentIndex(vcodecid2vcodecidx[rs.getInt("v_codec")]);
            _el.getElement("v_bitrate")->setText(rs.getString("v_bitrate"));
            ((Wt::Ext::ComboBox*)_elcb.getElement("v_framerate"))->setCurrentIndex(framerates2idx[rs.getString("v_framerate")]);

            //            _el.getElement("v_framerate")->setText(rs.getString("v_framerate"));
            _el.getElement("v_width")->setText(rs.getString("v_width"));
            _el.getElement("v_height")->setText(rs.getString("v_height"));
            _el.getElement("a_channels")->setText(rs.getString("a_channels"));
            //			elements["a_codec"]->setText(codecid2codecname[rs.getInt("a_codec")]);
            ((Wt::Ext::ComboBox*)_elcb.getElement("a_codec"))->setCurrentIndex(acodecid2acodecidx[rs.getInt("a_codec")]);
            _el.getElement("a_bitrate")->setText(rs.getString("a_bitrate"));
            //            _el.getElement("a_samplerate")->setText(rs.getString("a_samplerate"));
            ((Wt::Ext::ComboBox*)_elcb.getElement("a_bitrate"))->setCurrentIndex(audiobitrates2idx[rs.getString("a_bitrate")]);
            ((Wt::Ext::ComboBox*)_elcb.getElement("a_samplerate"))->setCurrentIndex(samplerates2idx[rs.getString("a_samplerate")]);
            //			elements["a_codec"]->setText(rs.getString("a_codec"));

          }
        } else {
          //            std::map<std::string, Wt::Ext::LineEdit*>::iterator elit = elements.begin();
          //            for (; elit != elements.end(); elit++) {
          //              (*elit).second->setText("");
          //            }
        }
      }

      void ProfilesForm::saveProfile() {
        bool update = atoi(_el.getElement("id")->text().narrow().c_str()) > 0 ? true : false;
        std::map<std::string, std::string> data;
        std::map<std::string, Wt::Ext::LineEdit*> elem = _el.getElements();
        std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elem.begin();

        for (; it != elem.end(); it++) {
          std::string value = (*it).second->text().narrow();
          if ((*it).first == "a_codec" || (*it).first == "v_codec")value = org::esb::util::Decimal(codecname2codecid[value]).toString();
          //					if ((*it).first == "v_format" || (*it).first == "v_codec")value = org::esb::util::Decimal(codecname2codecid[value]).toString();
          data[(*it).first] = value;
        }
        std::map<std::string, Wt::Ext::ComboBox*> elem_cb = _elcb.getElements();
        std::map<std::string, Wt::Ext::ComboBox*>::iterator itcb = elem_cb.begin();

        for (; itcb != elem_cb.end(); itcb++) {
          std::string value = (*itcb).second->text().narrow();
          if ((*itcb).first == "a_codec" || (*itcb).first == "v_codec")value = org::esb::util::Decimal(codecname2codecid[value]).toString();
          data[(*itcb).first] = value;
        }

        SqlUtil::map2sql("profiles", data);
        profileSaved.emit();
        //				but->setEnabled(false);
        //				setEnabled(false);
        return;
      }

    }
  }
}

