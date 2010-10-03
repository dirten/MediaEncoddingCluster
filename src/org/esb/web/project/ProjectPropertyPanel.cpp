#include "ProjectPropertyPanel.h"

#include "Wt/Ext/ToolBar"
#include "Wt/Ext/Button"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "Wt/WFitLayout"
#include "org/esb/util/StringUtil.h"
namespace org{
  namespace esb{
    namespace web{

      ProjectPropertyPanel::ProjectPropertyPanel():Wt::Ext::Panel(){
        setTitle("Project Properties");
        setTopToolBar(new Wt::Ext::ToolBar());

        setLayout(new Wt::WFitLayout());

        Wt::WContainerWidget * cont = new Wt::WContainerWidget();
        Wt::WGridLayout * l = new Wt::WGridLayout();
        cont->setLayout(l);

        _el.addElement("id", "Project Id", "", static_cast<Wt::WGridLayout*>(cont->layout()))->setEnabled(false);
        _el.addElement("name", "Project Name", "", static_cast<Wt::WGridLayout*>(cont->layout()));
        _el.addElement("outdir", "Output Directory", "", static_cast<Wt::WGridLayout*>(cont->layout()))->setEnabled(false);

        Wt::Ext::Button * selectOutDirectory = new Wt::Ext::Button("Select Directory");
        selectOutDirectory->clicked().connect(SLOT(this, ProjectPropertyPanel::openOutfolder));
        l->addWidget(selectOutDirectory, 2, 2);

        l->addWidget(new Wt::WText(), l->rowCount(), 0);
        l->setRowStretch(l->rowCount() - 1, 1);

        layout()->addWidget(cont);
      }
      ProjectPropertyPanel::~ProjectPropertyPanel(){
        
      }
      
      void ProjectPropertyPanel::save(){
        _project->name=_el.getElement("name")->text().narrow();
        _project->outdirectory=_el.getElement("outdir")->text().narrow();        
      }

      void ProjectPropertyPanel::setProject(Ptr<db::Project> p){
        _project=p;
        _el.getElement("id")->setText(org::esb::util::StringUtil::toString(p->id));
        _el.getElement("name")->setText(static_cast<std::string>(p->name));
        _el.getElement("outdir")->setText(static_cast<std::string>(p->outdirectory));
      }
      void ProjectPropertyPanel::openOutfolder() {
        _dc = new DirectoryChooser("Select Outfolder");
        _dc->selected.connect(SLOT(this, ProjectPropertyPanel::selectedOutfolder));
        _dc->canceled.connect(SLOT(_dc.get(), Wt::Ext::Dialog::accept));
        _dc->show();
      }

      void ProjectPropertyPanel::selectedOutfolder(std::string path) {
        _el.getElement("outdir")->setText(path);
        _dc->accept();
      }
    }
  }
}