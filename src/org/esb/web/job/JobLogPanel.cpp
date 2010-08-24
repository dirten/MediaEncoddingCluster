/* 
 * File:   JobLogPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 17. August 2010, 15:33
 */

#include "JobLogPanel.h"
#include "JobLogTableModel.h"
namespace org {
  namespace esb {
    namespace web {

      JobLogPanel::JobLogPanel():Wt::Ext::TableView() {
        setModel(new JobLogTableModel());
        setColumnWidth(0, 140);
        setColumnWidth(1, 900);
        setAutoExpandColumn(1);

      }

      JobLogPanel::JobLogPanel(const JobLogPanel& orig) {
      }

      JobLogPanel::~JobLogPanel() {
      }
      void JobLogPanel::setJob(db::Job job) {
        static_cast<JobLogTableModel*>(model())->setJob(job);
      }
      void JobLogPanel::refresh() {

      }
    }
  }
}
