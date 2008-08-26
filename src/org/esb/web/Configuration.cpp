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
#include <Wt/WLengthValidator>

#include "org/esb/sql/Connection.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/util/Properties.h"
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

  /*
   * The following code is functionally equivalent to:
   *
   *   exampleTabs->addTab(helloWorldExample(), "Hello world");
   *
   * However, we optimize here for memory consumption (it is a homepage
   * after all, and we hope to be slashdotted some day)
   *
   * Therefore, we wrap all the static content (including the tree
   * widgets), into WViewWidgets with static models. In this way the
   * widgets are not actually stored in memory on the server.
   *
   * For the tree list example (for which we cannot use a view with a
   * static model, since we allow the tree to be manipulated) we use
   * the defer utility function to defer its creation until it is
   * loaded.
   */


      Wt::WTable * table=new Wt::WTable(this);
      Wt::WTable * table2=new Wt::WTable(this);
      Wt::WTable * table3=new Wt::WTable(this);

      exampleTabs->addTab(table,"Database");
      exampleTabs->addTab(table2,"Directories");
      exampleTabs->addTab(table3,"System");
      exampleTabs->setTabEnabled(0,false);
      exampleTabs->setTabEnabled(1,false);
      exampleTabs->setTabEnabled(2,false);
      
      
      Wt::WLabel *hostLabel = new Wt::WLabel("Host: ", table->elementAt(0, 0));
      hostLabel->setToolTip("hier ein testtooltip");
      table->elementAt(0, 0)->resize(Wt::WLength(14, Wt::WLength::FontEx), Wt::WLength());
      host = new Wt::WLineEdit(table->elementAt(0, 1));
//      Wt::WLengthValidator *v=new Wt::WLengthValidator(this);
//      v->setMinimumLength(4);
//      host->setValidator(v);

      hostLabel->setBuddy(host);

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

      new Wt::WBreak(this);


      Wt::WPushButton *testButton = new Wt::WPushButton("Test Connection", table->elementAt(4, 0));
      testButton->clicked.connect(SLOT( this, Configuration::checkDbConnection));

      saveButton = new Wt::WPushButton("Save Configuration", this);
      saveButton->disable();
      saveButton->clicked.connect(SLOT( this, Configuration::saveConfig));

      new Wt::WBreak(this);

      log=new Wt::WTextArea(this);
      log->decorationStyle().setBackgroundColor(Wt::WColor(255,255,255));
      log->decorationStyle().setForegroundColor(Wt::WColor(0,0,0));
      log->decorationStyle().setBorder(Wt::WBorder(Wt::WBorder::Hidden),WWidget::All);
      log->setColumns(100);
      log->setRows(30);
      log->disable();
    }

    Wt::WWidget * home(){
      return this;
    }

  private:
      Wt::WLineEdit * host;
      Wt::WLineEdit * user;
      Wt::WLineEdit * passwd;
      Wt::WLineEdit * database;
      Wt::WTextArea * log;
      Wt::WPushButton *saveButton;
      util::Properties props;
      
  private:
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
        sql::Connection con(host->text().narrow().c_str(),database->text().narrow().c_str(),user->text().narrow().c_str(),passwd->text().narrow().c_str());
        log->setText(log->text()+" successfull connected to Database\n");
        props.setProperty("host",host->text().narrow());
        props.setProperty("user",user->text().narrow());
        props.setProperty("passwd",passwd->text().narrow());
        props.setProperty("database",database->text().narrow());
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

