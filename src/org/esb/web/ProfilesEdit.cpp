#ifndef ORG_ESB_WEB_PROFILES_EDIT
#define ORG_ESB_WEB_PROFILES_EDIT
#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WGroupBox>

#include <Wt/Ext/LineEdit>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/Button>

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/config/config.h"
#include "org/esb/util/Decimal.h"

#include "org/esb/av/AV.h"

#include "SqlUtil.h"
#include <map>
#include <string>

using namespace org::esb::sql;
using namespace org::esb::config;
namespace org{
namespace esb{
namespace web{
  class ProfilesEdit: public Wt::WContainerWidget{
    public:
      ProfilesEdit(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){
      if(true){
        resize(400,Wt::WLength());

        if(true){
          Wt::WGroupBox * group =new Wt::WGroupBox("General", this);
          Wt::WTable *t=new Wt::WTable(group);
          int i=0;
          buildElement("id", "Profile Id",t,i++);
          elements["id"]->setEnabled(false);
          buildElement("profile_name", "Profile Name",t,i++);

          Wt::WLabel * elementLabel = new Wt::WLabel("Container Format", t->elementAt(i, 0));
          t->elementAt(i, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          Wt::Ext::ComboBox * element = new Wt::Ext::ComboBox(t->elementAt(i, 1));
         
            AVOutputFormat *ofmt=NULL;
            while((ofmt= av_oformat_next(ofmt))) {
//                if((name == NULL || strcmp(ofmt->name, name)<0) &&strcmp(ofmt->name, last_name)>0){
	          element->addItem(ofmt->long_name);
//                }
            }
            elementLabel->setBuddy(element);
            elements["v_format"]=element;
            i++;
//          buildElement("v_format", "File Format",t,i++);
        }
        if(true){
          Wt::WGroupBox * group =new Wt::WGroupBox("Video", this);
          Wt::WTable *t=new Wt::WTable(group);
          int i=0;

          Wt::WLabel * elementLabel = new Wt::WLabel("Codec", t->elementAt(i, 0));
          t->elementAt(i, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          Wt::Ext::ComboBox * element = new Wt::Ext::ComboBox(t->elementAt(i, 1));
//          element->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());

  	      AVCodec *p=NULL;
  	      int a=0;
	      while((p= av_codec_next(p))) {
		    if(p->encode&&p->type==CODEC_TYPE_VIDEO){
	          element->addItem(p->name);
	          vcodecid2vcodecidx[p->id]=a++;
	          codecname2codecid[p->name]=p->id;
	          codecid2codecname[p->id]=p->name;
		    }
	      }
          elementLabel->setBuddy(element);
          elements["v_codec"]=element;
          
          i++;
          buildElement("v_bitrate", "Bitrate",t,i++);
          buildElement("v_framerate", "Framerate",t,i++);
          buildElement("v_width", "Width",t,i++);
          buildElement("v_height", "Height",t,i++);
        }
        if(true){
          Wt::WGroupBox * group =new Wt::WGroupBox("Audio", this);
          Wt::WTable *t=new Wt::WTable(group);
          int i=0;

          Wt::WLabel * elementLabel = new Wt::WLabel("Codec", t->elementAt(i, 0));
          t->elementAt(i, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          Wt::Ext::ComboBox * element = new Wt::Ext::ComboBox(t->elementAt(i, 1));
  	      AVCodec *p=NULL;
  	      int a=0;
	      while((p= av_codec_next(p))) {
		    if(p->encode&&p->type==CODEC_TYPE_AUDIO){
	          element->addItem(p->name);
	          acodecid2acodecidx[p->id]=a++;
	          codecname2codecid[p->name]=p->id;
	          codecid2codecname[p->id]=p->name;
		    }
	      }
          elementLabel->setBuddy(element);
          elements["a_codec"]=element;
          i++;
          buildElement("a_channels", "Channels",t,i++);
          buildElement("a_bitrate", "Bitrate",t,i++);
          buildElement("a_samplerate", "Samplerate",t,i++);
//          buildElement("v_height", "Height",t,i++);
//          elements["a_codec"]->setText("mp2");
        }
      if(true){
        but=new Wt::Ext::Button("save",this);
        but->setEnabled(false);
        but->clicked.connect(SLOT(this, ProfilesEdit::saveData));
      }
      }
      }
	void setData(int profile_id){
      if(profile_id>0){
        Connection con(std::string(Config::getProperty("db.connection")));
        Statement stmt=con.createStatement(std::string("select * from profiles where id=").append(Decimal(profile_id).toString()).c_str());
        ResultSet rs=stmt.executeQuery();
        if(rs.next()){
			elements["id"]->setText(rs.getString("id"));
			elements["profile_name"]->setText(rs.getString("profile_name"));
////			elements["v_format"]->setText(rs.getString("v_format"));
//			elements["v_codec"]->setText(codecid2codecname[rs.getInt("v_codec")]);
			((Wt::Ext::ComboBox*)elements["v_codec"])->setCurrentIndex(vcodecid2vcodecidx[rs.getInt("v_codec")]);
//			((Wt::Ext::ComboBox*)elements["v_codec"])->setCurrentIndex(vcodecid2vcodecidx[rs.getInt("v_codec")]);
			elements["v_bitrate"]->setText(rs.getString("v_bitrate"));
			elements["v_framerate"]->setText(rs.getString("v_framerate"));
			elements["v_width"]->setText(rs.getString("v_width"));
			elements["v_height"]->setText(rs.getString("v_height"));
			elements["a_channels"]->setText(rs.getString("a_channels"));
//			elements["a_codec"]->setText(codecid2codecname[rs.getInt("a_codec")]);
			((Wt::Ext::ComboBox*)elements["a_codec"])->setCurrentIndex(acodecid2acodecidx[rs.getInt("a_codec")]);
			elements["a_bitrate"]->setText(rs.getString("a_bitrate"));
			elements["a_samplerate"]->setText(rs.getString("a_samplerate"));
//			elements["a_codec"]->setText(rs.getString("a_codec"));
          
        }
      }else{
        std::map<std::string,Wt::Ext::LineEdit*>::iterator elit=elements.begin();
	    for(;elit!=elements.end();elit++){
	      (*elit).second->setText("");
	    }
      }
	}
	void setEnabled(bool e){
      std::map<std::string,Wt::Ext::LineEdit*>::iterator elit=elements.begin();
	  for(;elit!=elements.end();elit++){
	    (*elit).second->setEnabled(e);
	  }
	  elements["id"]->setEnabled(false);
	  but->setEnabled(e);
	}
	
	Wt::Signal<void> profileSaved;
	
    private:
      Wt::Ext::Button * but;
      std::map<std::string,Wt::Ext::LineEdit*> elements;
	  std::map<int, std::string> codecid2codecname;
	  std::map<std::string,int> codecname2codecid;
	  std::map<int,int> acodecid2acodecidx;
	  std::map<int,int> vcodecid2vcodecidx;
      void buildElement(std::string name, std::string label, Wt::WTable * table,int row){
        Wt::WLabel * elementLabel = new Wt::WLabel(label, table->elementAt(row, 0));
        table->elementAt(row, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
        Wt::Ext::LineEdit * element = new Wt::Ext::LineEdit(table->elementAt(row, 1));
//      if(config::Config::hasProperty(name))
        element->setText(config::Config::getProperty((char*)name.c_str()));
        elementLabel->setBuddy(element);
        elements[name]=element;
    }
    
    void saveData(){
      bool update=atoi(elements["id"]->text().narrow().c_str())>0?true:false;
      std::map<std::string, std::string> data;
      std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();

      for (; it != elements.end(); it++) {
        std::string value=(*it).second->text().narrow();
	if((*it).first=="a_codec"||(*it).first=="v_codec")value=Decimal(codecname2codecid[value]).toString();
        data[(*it).first] = value;
      }
      SqlUtil::map2sql("profiles", data);
	  profileSaved.emit();
	  but->setEnabled(false);
	  setEnabled(false);
      return;
    }
  };
}}}
#endif
