#ifndef ORG_ESB_WEB_PROFILESELECTOR
#define ORG_ESB_WEB_PROFILESELECTOR

#include "Wt/WContainerWidget"
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/Button>
#include "org/esb/hive/JobUtil.h"

namespace org {
  namespace esb {
    namespace web {

      class ProfileSelector : public Wt::WContainerWidget {
      public:

        ProfileSelector(Wt::WContainerWidget*parent = 0) : Wt::WContainerWidget(parent) {
          //		        	encodingbox=new Wt::WGroupBox("Encoding",this);
          profiles = new Wt::Ext::ComboBox(this);
          encode = new Wt::Ext::Button("Encode", this);
          encode->clicked.connect(SLOT(this, ProfileSelector::createJob));
          //        			encodingbox->setHidden(true);
          Connection con(std::string(Config::getProperty("db.connection")));

          Statement stmt = con.createStatement("select * from profiles");
          ResultSet rs = stmt.executeQuery();
          while (rs.next()) {
            profiles->addItem(rs.getString("profile_name"));
            name2id[rs.getString("profile_name")] = rs.getInt("id");
          }
        }

        void setFileId(int id) {
          file_id = id;
        }
      private:

        void createJob() {
          try{
          int pid = name2id[profiles->currentText().narrow()];
          if(pid<=0)return;
          std::string file = Decimal(file_id).toString();
          std::string profile = Decimal(pid).toString();
          char * jobarg[] = {"", "", (char*) file.c_str(), (char*) profile.c_str()};
          std::cout << "FileId:" << jobarg[2] << ":" << std::endl;
          std::cout << "ProfileId:" << jobarg[3] << ":" << std::endl;
          jobcreator(3, jobarg);
          }catch(...){
            logerror("Fehler im create job");
          }

        }

        Wt::Ext::ComboBox * profiles;
        Wt::Ext::Button * encode;
        map<std::string, int> name2id;
        int file_id;
      };
    }
  }
}
#endif


