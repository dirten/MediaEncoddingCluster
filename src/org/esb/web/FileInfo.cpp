#ifndef ORG_ESB_WEB_FILEINFO
#define ORG_ESB_WEB_FILEINFO
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WGroupBox>
#include <Wt/WComboBox>
#include <Wt/Ext/Button>

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"

//#include "job.cpp"
#include "org/esb/hive/JobUtil.h"
#include <map>
using namespace org::esb::sql;
using namespace org::esb::config;
namespace org{
namespace esb{
namespace web{
  class FileInfo:public Wt::WContainerWidget{
    public:
      FileInfo(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){
        infobox=new Wt::WGroupBox("FileInfo",this);
        Wt::WTable * table=new Wt::WTable(infobox);
        filename=new Wt::WText(table->elementAt(0,0));
        filepath=new Wt::WText(table->elementAt(1,0));
        encodingbox=new Wt::WGroupBox("Encoding",this);
        profiles=new Wt::WComboBox(encodingbox);
        encode=new Wt::Ext::Button("Encode",encodingbox);
        encode->clicked.connect(SLOT(this, FileInfo::createJob));
        encodingbox->setHidden(true);
        infobox->setHidden(true);
      }
      void createJob(){
        int pid=name2id[profiles->currentText().narrow()];
        std::string file=Decimal(file_id).toString();
        std::string profile=Decimal(pid).toString();
        char * jobarg[]={"","",(char*)file.c_str(),(char*)profile.c_str()};
        std::cout <<"FileId:"<<jobarg[2]<<":"<<std::endl;
        std::cout <<"ProfileId:"<<jobarg[3]<<":"<<std::endl;
    	jobcreator(3, jobarg);

      }
      void setData(int id){
        file_id=id;
        Connection con(Config::getProperty("db.connection"));
        {
          PreparedStatement stmt=con.prepareStatement("SELECT * FROM files where id=:id");
          stmt.setInt("id", id);
          ResultSet rs=stmt.executeQuery();
          if(rs.next()){
            filename->setText(rs.getString("filename"));
            filepath->setText(rs.getString("path"));
          }
        }
        {
          profiles->clear();
          name2id.clear();
          Statement stmt=con.createStatement("select * from profiles");
          ResultSet rs=stmt.executeQuery();
          while(rs.next()){
            profiles->addItem(rs.getString("profile_name"));
            name2id[rs.getString("profile_name")]=rs.getInt("id");
          }
        }
        infobox->setHidden(false);
        encodingbox->setHidden(false);
      }
      
    private:
      Wt::WText * filename;
      Wt::WText * filepath;
      Wt::WGroupBox * encodingbox;
      Wt::WGroupBox * infobox;
      Wt::WComboBox * profiles;
      Wt::Ext::Button * encode;
      int file_id;
      map<std::string, int> name2id;
  };
}}}
#endif

