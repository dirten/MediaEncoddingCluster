#ifndef ORG_ESB_WEB_LOGIN
#define ORG_ESB_WEB_LOGIN

#include "Wt/WContainerWidget"
#include <Wt/Ext/Button>

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

namespace org {
  namespace esb {
    namespace web {

      class Login : public Wt::WContainerWidget {
      public:

        Login(Wt::WContainerWidget*parent = 0) : Wt::WContainerWidget(parent) {
          _authenticated = false;
          Wt::WGroupBox * group = new Wt::WGroupBox("Login", this);
          Wt::WTable *t = new Wt::WTable(group);
          int i = 0;
          buildElement("auth_name", "Authentication Name", t, i++);
          Wt::Ext::LineEdit * pass=buildElement("auth_passwd", "Authentication Password", t, i++);
          pass->setEchoMode(Wt::Ext::LineEdit::Password);
          pass->keyWentUp.connect(SLOT(this, Login::enter));
          msg=new Wt::WText(t->elementAt(i, 0));
          Wt::Ext::Button * saveBut = new Wt::Ext::Button("Login", t->elementAt(i, 1));
          saveBut->clicked.connect(SLOT(this, Login::authenticate));
          
        }
        void enter(Wt::WKeyEvent ev) {
//          logdebug("KeyCode"<<ev.keyCode());
          if(ev.keyCode()==13){
            authenticate();
          }
        }
        void authenticate() {
          using namespace org::esb;
          std::string sql = "select * from user where auth_name=:name and auth_passwd=:passwd";
          sql::Connection con(config::Config::getProperty("db.connection"));
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
          }
        }

        bool isAuthenticated() {
          return _authenticated;
        }

        int getUserId() {
          return user_id;
        }

        Wt::Signal<void>authenticated;
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

