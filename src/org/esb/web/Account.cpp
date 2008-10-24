#ifndef ORG_ESB_WEB_ACCOUNT
#define ORG_ESB_WEB_ACCOUNT

#include "Wt/WContainerWidget"
#include <Wt/Ext/LineEdit>
#include <Wt/Ext/Button>
#include <Wt/WGroupBox>
#include <Wt/WTable>
#include <Wt/WLabel>

#include "SqlUtil.h"
namespace org {
  namespace esb {
    namespace web {

      class Account : public Wt::WContainerWidget {
      public:

        Account(int user_id) : Wt::WContainerWidget(NULL) {
          _user_id = user_id;
          Wt::WGroupBox * group = new Wt::WGroupBox("Account Information", this);
          Wt::WTable *t = new Wt::WTable(group);
          int i = 0;
          loadMap();

          buildElement("id", "Account Id", t, i++)->setEnabled(false);
          buildElement("auth_name", "Authentication Name", t, i++);
          buildElement("auth_passwd", "Authentication Password", t, i++)->setEchoMode(Wt::Ext::LineEdit::Password);
          buildElement("first_name", "First Name", t, i++);
          buildElement("last_name", "Last Name", t, i++);
          buildElement("email", "eMail Address", t, i++);

          Wt::Ext::Button * saveBut = new Wt::Ext::Button("Save", t->elementAt(i, 1));
          saveBut->clicked.connect(SLOT(this, Account::saveMap));
        }
      private:
        int _user_id;
        std::map<std::string, std::string> sqldata;
        std::map<std::string, Wt::Ext::LineEdit*> elements;

        void saveMap() {
          std::map<std::string, std::string> data;
          std::map<std::string, Wt::Ext::LineEdit*>::iterator it = elements.begin();
          for (; it != elements.end(); it++) {
            data[(*it).first] = (*it).second->text().narrow();
          }
          SqlUtil::map2sql("user", data);
        }

        void loadMap() {

          SqlUtil::sql2map("user", _user_id, sqldata);
        }

        Wt::Ext::LineEdit * buildElement(std::string name, std::string label, Wt::WTable * table, int row) {
          Wt::WLabel * elementLabel = new Wt::WLabel(label, table->elementAt(row, 0));
          table->elementAt(row, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
          Wt::Ext::LineEdit * element = new Wt::Ext::LineEdit(table->elementAt(row, 1));
          element->setText(config::Config::getProperty((char*) name.c_str()));
          element->resize(Wt::WLength(30, Wt::WLength::FontEx), Wt::WLength());
          elementLabel->setBuddy(element);
          element->setText(sqldata[name]);
          elements[name] = element;
          return element;
        }
      };
    }
  }
}

#endif

