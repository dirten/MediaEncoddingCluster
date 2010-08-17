/* 
 * File:   JobDetailPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 17. August 2010, 14:46
 */

#include "JobDetailPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WText"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace web {

      JobDetailPanel::JobDetailPanel():Wt::Ext::Panel() {
        setLayout(new Wt::WFitLayout());
      }

      JobDetailPanel::JobDetailPanel(const JobDetailPanel& orig) {
      }

      JobDetailPanel::~JobDetailPanel() {
      }
      
      void JobDetailPanel::setJob(db::Job job) {
        std::string tmp="jobId:";
        tmp+=org::esb::util::StringUtil::toString(job.id);
        this->layout()->addWidget(new Wt::WText(tmp));
      }
    }
  }
}
