#ifndef ORG_ESB_WEB_LOGIN
#define ORG_ESB_WEB_LOGIN
#include "org/esb/db/hivedb.hpp"
#include "org/esb/hive/DatabaseService.h"
#include "Wt/WContainerWidget"
#include <Wt/Ext/Button>
#include <Wt/Ext/LineEdit>

#include "Wt/Ext/Dialog"

namespace org {
  namespace esb {
    namespace web {

      class Login : public Wt::Ext::Dialog {
      public:

        Login() : Wt::Ext::Dialog("Login") {
          _authenticated = false;
          resize(400, 215);
          Wt::WGroupBox * group = new Wt::WGroupBox("Login", this->contents());
          group->resize(Wt::WLength(),140);
          Wt::WTable *t = new Wt::WTable(group);
          int i = 0;
          buildElement("auth_name", "Authentication Name", t, i++);
          Wt::Ext::LineEdit * pass=buildElement("auth_passwd", "Authentication Password", t, i++);
          pass->setEchoMode(Wt::Ext::LineEdit::Password);
          pass->keyWentUp().connect(SLOT(this, Login::enter));


         
          msg=new Wt::WText(t->elementAt(i, 0));
          
          t->elementAt(i, 0)->setAttributeValue("colspan","2");
          Wt::Ext::Button * saveBut = new Wt::Ext::Button("Login");
          this->addButton(saveBut);
          saveBut->clicked().connect(SLOT(this, Login::authenticate));
          
        }
        void enter(Wt::WKeyEvent ev) {
//          logdebug("KeyCode"<<ev.keyCode());
          if(ev.keyCode()==13){
            authenticate();
          }
        }
        void authenticate() {
          using namespace org::esb;
          db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
          std::string name=elements["auth_name"]->text().narrow();
          std::string pass=elements["auth_passwd"]->text().narrow();
          if(litesql::select<db::User>(db,db::User::Authname==name&&db::User::Authpass==pass).count()>0){
            db::User user=litesql::select<db::User>(db,db::User::Authname==name&&db::User::Authpass==pass).one();
            _authenticated = true;
            user_id = user.id;
            LOGDEBUG("User authenticated");
            authenticated.emit(user);
            msg->setText("<b>Login successfull!</b>");
          }else{
            msg->setText("<b>Name or Password are invalid!</b>");
            LOGDEBUG("User not authenticated");
          }
          /*
          User user
          std::string sql = "select * from user where auth_name=:name and auth_passwd=:passwd";
          sql::Connection con(std::string(config::Config::getProperty("db.connection")));
          sql::PreparedStatement stmt = con.prepareStatement(sql.c_str());
          stmt.setString("name", elements["auth_name"]->text().narrow());
          stmt.setString("passwd", elements["auth_passwd"]->text().narrow());
          sql::ResultSet rs = stmt.executeQuery();
          if (rs.next()) {
            _authenticated = true;
            user_id = rs.getInt("id");
            logdebug("User authenticated");
            authenticated.emit();
            msg->setText("Login successfull");
          }else {
            msg->setText("Name or Password are invalid");
            logdebug("User not authenticated");
          }*/
        }

        bool isAuthenticated() {
          return _authenticated;
        }

        int getUserId() {
          return user_id;
        }

        Wt::Signal<db::User>authenticated;
      private:
        std::map<std::string, Wt::Ext::LineEdit*> elements;
        int user_id;
        bool _authenticated;
        Wt::WText * msg;
        Wt::Ext::LineEdit * buildElement(std::string name, std::string label, Wt::WTable * table, int row) {
          Wt::WLabel * elementLabel = new Wt::WLabel(label, table->elementAt(row, 0));
          table->elementAt(row, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          Wt::Ext::LineEdit * element = new Wt::Ext::LineEdit(table->elementAt(row, 1));
          element->setText(config::Config::getProperty((char*) name.c_str()));
          element->resize(Wt::WLength(30, Wt::WLength::FontEx), Wt::WLength());
          elementLabel->setBuddy(element);
          elements[name] = element;
          return element;
        }

      };
    }
  }
}
#endif

