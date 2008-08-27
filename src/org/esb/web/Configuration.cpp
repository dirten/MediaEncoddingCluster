#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WTextArea>
#include <Wt/WColor>
#include <Wt/WBorder>
#include <Wt/WTabWidget>
#include <Wt/WGroupBox>
#include <Wt/WLengthValidator>

#include "org/esb/sql/Connection.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/util/Properties.h"

#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "org/esb/lang/Thread.h"
#include <map>
#include <string>
using namespace org::esb;
namespace org{
namespace esb{
namespace web{
class Configuration : public Wt::WContainerWidget{
  public:
    Configuration():Wt::WContainerWidget(0){
    
    //  Wt::WContainerWidget * result=new Wt::WContainerWidget();
    Wt::WTabWidget *exampleTabs = new Wt::WTabWidget(this);
    exampleTabs->enableBrowserHistory("example");

      saveButton = new Wt::WPushButton("Save Configuration", this);
      saveButton->disable();
      saveButton->clicked.connect(SLOT( this, Configuration::saveConfig));


      exampleTabs->addTab(createDbConfigPage(),"Database Config");
//      exampleTabs->addTab(createDirectoryPage(),"Directories");
      exampleTabs->addTab(createSystemPage(this),"System");


      }

Wt::WWidget * createDirectoryPage(){
  Wt::WTable * result=new Wt::WTable(this);
  return result;
}


Wt::WWidget * createSystemPage(Wt::WContainerWidget * parent){
  Wt:WContainerWidget * cont=new Wt::WContainerWidget();
  Wt::WGroupBox * hive =new Wt::WGroupBox("Hive", cont);
//  Wt::WGroupBox * box2 =new Wt::WGroupBox("System", cont);
  Wt::WTable * result=new Wt::WTable(hive);
  buildElement("hive.datadir","Hive Data Directory:",result,0);
  buildElement("hive.port","Hive Listener Port:",result,1);
  buildElement("hive.start","Hive Autostart",result,2);
  buildElement("test3","testlabel",result,3);

  Wt::WGroupBox * box2 =new Wt::WGroupBox("System", cont);
  Wt::WTable * result2=new Wt::WTable(box2);
  buildElement("hive.datadir","Hive Data Directory:",result2,0);
  buildElement("hive.port","Hive Listener Port:",result2,1);
  buildElement("hive.start","Hive Autostart",result2,2);
  buildElement("test3","testlabel",result2,3);
//  box2->setHidden(true);
  return cont;
}

Wt::WWidget * createDbConfigPage(){

      Wt::WTable * result=new Wt::WTable();
      result->decorationStyle().setBorder(Wt::WBorder(Wt::WBorder::Dotted,Wt::WBorder::Thin),WWidget::All);
      buildElement("host","Host:",result,0);
      buildElement("user","Username:",result,1);
      buildElement("passwd","Password:",result,2);
      buildElement("database","Database:",result,3);
      
            
/*      
      Wt::WLabel *hostLabel = new Wt::WLabel("Host: ", table->elementAt(0, 0));
      hostLabel->setToolTip("hier ein testtooltip");
      table->elementAt(0, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
      host = new Wt::WLineEdit(table->elementAt(0, 1));
      hostLabel->setBuddy(host);

*/
//      Wt::WLengthValidator *v=new Wt::WLengthValidator(this);
//      v->setMinimumLength(4);
//      host->setValidator(v);

/*
      Wt::WLabel *userLabel = new Wt::WLabel("Username: ", table->elementAt(1, 0));
      table->elementAt(1, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
      user = new Wt::WLineEdit(table->elementAt(1, 1));
      userLabel->setBuddy(user);

      Wt::WLabel *passwdLabel = new Wt::WLabel("Password: ", table->elementAt(2, 0));
      table->elementAt(2, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
      passwd = new Wt::WLineEdit(table->elementAt(2, 1));
      passwdLabel->setBuddy(passwd);

      Wt::WLabel *dbLabel = new Wt::WLabel("Database: ", table->elementAt(3, 0));
      table->elementAt(3, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
      database = new Wt::WLineEdit(table->elementAt(3, 1));
      dbLabel->setBuddy(database);
*/
      new Wt::WBreak(this);


      Wt::WPushButton *testButton = new Wt::WPushButton("Test Connection", result->elementAt(4, 0));
      testButton->clicked.connect(SLOT( this, Configuration::checkDbConnection));
/*
      saveButton = new Wt::WPushButton("Save Configuration", this);
      saveButton->disable();
      saveButton->clicked.connect(SLOT( this, Configuration::saveConfig));
*/
      new Wt::WBreak(this);

      log=new Wt::WTextArea(this);
      log->decorationStyle().setBackgroundColor(Wt::WColor(255,255,255));
      log->decorationStyle().setForegroundColor(Wt::WColor(0,0,0));
      log->decorationStyle().setBorder(Wt::WBorder(Wt::WBorder::Hidden),WWidget::All);
      log->setColumns(100);
      log->setRows(30);
      log->disable();

      return result;
    }
        
    Wt::WWidget * home(){
      return this;
    }

  private:
/*      Wt::WLineEdit * host;
      Wt::WLineEdit * user;
      Wt::WLineEdit * passwd;
      Wt::WLineEdit * database;*/
      Wt::WTextArea * log;
      Wt::WPushButton *saveButton;
      util::Properties props;
      std::map<std::string,Wt::WLineEdit*> elements;
  private:

    void buildElement(std::string name, std::string label, Wt::WTable * table,int row){
      Wt::WLabel * elementLabel = new Wt::WLabel(label, table->elementAt(row, 0));
      table->elementAt(row, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
      Wt::WLineEdit * element = new Wt::WLineEdit(table->elementAt(row, 1));
      elementLabel->setBuddy(element);
      elements[name]=element;
    }

    void saveConfig(){
      io::File file("config.txt");
      io::FileOutputStream fos(&file);
      props.save(&fos);
      fos.close();
      log->setText(log->text()+"Configuration saved successfull \n");
    }
    
    void checkDbConnection(){
//      if(host->validate()==Wt::WValidator::Valid){
      log->setText(log->text()+"check Connection : ");
      try{
        sql::Connection con(elements["host"]->text().narrow().c_str(),
                            elements["database"]->text().narrow().c_str(),
                            elements["user"]->text().narrow().c_str(),
                            elements["passwd"]->text().narrow().c_str());
                            
        log->setText(log->text()+" successfull connected to Database\n");
        props.setProperty("host",elements["host"]->text().narrow());
        props.setProperty("user",elements["user"]->text().narrow());
        props.setProperty("passwd",elements["passwd"]->text().narrow());
        props.setProperty("database",elements["database"]->text().narrow());
        saveButton->enable();
      }catch(sql::SqlException & ex){
        log->setText(log->text()+ex.what()+"\n");
        saveButton->disable();
      }
//      }else{
//        log->setText("Hostname cannot be null\n");      
//      }
    }
};
}}}

