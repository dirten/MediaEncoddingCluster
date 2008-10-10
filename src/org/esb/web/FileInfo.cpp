#ifndef ORG_ESB_WEB_FILEINFO
#define ORG_ESB_WEB_FILEINFO
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WLength>
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
#include <list>
using namespace org::esb::sql;
using namespace org::esb::config;
namespace org{
namespace esb{
namespace web{
  class FileInfo:public Wt::WContainerWidget{
    public:
      FileInfo(Wt::WContainerWidget * parent=0):Wt::WContainerWidget(parent){
//        general=new Wt::WGroupBox("General",this);
          first=true;
        main=new Wt::WTable(this);
        table=new Wt::WTable(new Wt::WGroupBox("General",main->elementAt(0,0)));
        ((Wt::WText *)new Wt::WText(table->elementAt(0,0)))->setText("Path:");
        ((Wt::WText *)new Wt::WText(table->elementAt(1,0)))->setText("File:");
        ((Wt::WText *)new Wt::WText(table->elementAt(2,0)))->setText("FileSize:");
        ((Wt::WText *)new Wt::WText(table->elementAt(3,0)))->setText("Format:");
        ((Wt::WText *)new Wt::WText(table->elementAt(4,0)))->setText("Bitrate:");
        ((Wt::WText *)new Wt::WText(table->elementAt(5,0)))->setText("Duration:");
        ((Wt::WText *)new Wt::WText(table->elementAt(6,0)))->setText("Title:");
        ((Wt::WText *)new Wt::WText(table->elementAt(7,0)))->setText("Author:");
        ((Wt::WText *)new Wt::WText(table->elementAt(8,0)))->setText("Copyright:");
        ((Wt::WText *)new Wt::WText(table->elementAt(9,0)))->setText("Comment:");
//        filepath=new Wt::WText(table->elementAt(0,1));
//        filename=new Wt::WText(table->elementAt(1,1));

//        std::string sql="select * from streams where fileid="+id;

        
/*        
        encodingbox=new Wt::WGroupBox("Encoding",this);
        profiles=new Wt::WComboBox(encodingbox);
        encode=new Wt::Ext::Button("Encode",encodingbox);
        encode->clicked.connect(SLOT(this, FileInfo::createJob));
        encodingbox->setHidden(true);
 */
//        general->setHidden(true);
      }

      void createStreamBox(ResultSet & rs, int col){
          int field=0;
        Wt::WTable * streamtable=new Wt::WTable(new Wt::WGroupBox("Stream",main->elementAt(0,col)));
        streamtable->resize(Wt::WLength(100,Wt::WLength::Percentage),150);
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Type:");
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getInt("stream_type")==0?"Video":"Audio");
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Stream#:");
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("stream_index"));
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Codec:");
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("codec"));
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Bitrate:");
        ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("bit_rate"));
        if(rs.getInt("stream_type")==0){
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Width:");
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("width"));
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Height:");
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("height"));
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Aspect Ration:");
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText("Unknown");
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Frame Rate:");
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("time_base_num")+"/"+rs.getString("time_base_den"));
        }else
        if(rs.getInt("stream_type")==1){
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Channels:");
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("channels"));
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(++field,0)))->setText("Sample Rate:");
            ((Wt::WText *)new Wt::WText(streamtable->elementAt(field,1)))->setText(rs.getString("sample_rate"));
        }
         
        streamlist.push_back(streamtable);
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
              if(!first){
              for(int a=0;a<9;a++){
//                (*it)->parent()->setHidden(true);
//                if(table->elementAt(a,1)!=NULL)
                    table->elementAt(a,1)->setHidden(true);
//                    delete table->elementAt(a,1);
                }
              
              }
              first=false;
//              Wt::WTable * table=(Wt::WTable*)main->elementAt(0,0)->parent();
            ((Wt::WText *)new Wt::WText(table->elementAt(0,1)))->setText(rs.getString("path"));
            ((Wt::WText *)new Wt::WText(table->elementAt(1,1)))->setText(rs.getString("filename"));
            ((Wt::WText *)new Wt::WText(table->elementAt(2,1)))->setText(rs.getString("size"));
            ((Wt::WText *)new Wt::WText(table->elementAt(3,1)))->setText(rs.getString("container_type"));
            ((Wt::WText *)new Wt::WText(table->elementAt(4,1)))->setText(rs.getString("bitrate"));
            ((Wt::WText *)new Wt::WText(table->elementAt(5,1)))->setText(rs.getString("duration"));
            ((Wt::WText *)new Wt::WText(table->elementAt(6,1)))->setText(rs.getString("title"));
            ((Wt::WText *)new Wt::WText(table->elementAt(7,1)))->setText(rs.getString("author"));
            ((Wt::WText *)new Wt::WText(table->elementAt(8,1)))->setText(rs.getString("copyright"));
            ((Wt::WText *)new Wt::WText(table->elementAt(9,1)))->setText(rs.getString("comment"));
//            filename->setText(rs.getString("filename"));
//            filepath->setText(rs.getString("path"));
          }
        }
        {
            /*clear StreamList*/
            list<Wt::WTable *>::iterator it=streamlist.begin();
            for(;it!=streamlist.end();it++){
                (*it)->parent()->setHidden(true);
                delete (*it)->parent();
            }
            streamlist.clear();
            PreparedStatement stmt=con.prepareStatement("select * from streams where fileid=:id");
            stmt.setInt("id", id);
            ResultSet rs=stmt.executeQuery();
            int a=1;
            while(rs.next()){
                createStreamBox(rs, a++);
            }
        }
        if(false){
          profiles->clear();
          name2id.clear();
          Statement stmt=con.createStatement("select * from profiles");
          ResultSet rs=stmt.executeQuery();
          while(rs.next()){
            profiles->addItem(rs.getString("profile_name"));
            name2id[rs.getString("profile_name")]=rs.getInt("id");
          }
        }
//        general->setHidden(false);
//        encodingbox->setHidden(false);
        refresh();
      }
      
    private:
      Wt::WText * filename;
      Wt::WText * filepath;
      Wt::WGroupBox * encodingbox;
//      Wt::WGroupBox * general;
      Wt::WComboBox * profiles;
      Wt::Ext::Button * encode;
      int file_id;
      map<std::string, int> name2id;
      list<Wt::WTable *> streamlist;
      Wt::WTable * main;
      Wt::WTable * table;
      bool first;
  };
}}}
#endif

