/* 
 * File:   FormatPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 17:10
 */

#include "FormatPanel.h"
#include "Wt/WFitLayout"
#include "Wt/WContainerWidget"
#include "Wt/WGridLayout"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/util/StringTokenizer.h"
namespace org {
  namespace esb {
    namespace web {

      FormatPanel::FormatPanel(Ptr<db::Profile> profile) : _profile(profile), Wt::Ext::Panel() {
        
        setLayout(new Wt::WFitLayout());
        Wt::WContainerWidget * main = new Wt::WContainerWidget();
        Wt::WGridLayout * grid = new Wt::WGridLayout();
        main->setLayout(grid);
        layout()->addWidget(main);

        Wt::Ext::ComboBox * v_format = _elcb.addElement("v_format", "Container Fromat", _profile->format.value(), grid);
        v_format->setTextSize(50);
        v_format->resize(150, Wt::WLength::Auto);

        AVOutputFormat *ofmt = NULL;
        int a = 0;

        while ((ofmt = av_oformat_next(ofmt))) {
          v_format->addItem(ofmt->long_name);
          a++;
        }

        Wt::Ext::ComboBox * v_format_ext = _elcb.addElement("v_format_ext", "File Extension", _profile->formatext.value(), grid);
        v_format_ext->setTextSize(50);
        v_format_ext->resize(150, Wt::WLength::Auto);
        v_format_ext->setEditable(true);
        grid->addWidget(new Wt::WText(), grid->rowCount(), 0);
        grid->setRowStretch(grid->rowCount() - 1, 1);
        v_format->activated().connect(SLOT(this, FormatPanel::setAvailableExtensions));

      }

      void FormatPanel::setAvailableExtensions() {
        Wt::Ext::ComboBox * v_format = _elcb.getElement("v_format");
        Wt::Ext::ComboBox * v_format_ext = _elcb.getElement("v_format_ext");
        v_format_ext->clear();
        std::string longname = v_format->currentText().narrow();
        AVOutputFormat *ofmt = NULL;
        while ((ofmt = av_oformat_next(ofmt))) {
          if (longname == ofmt->long_name) {
            if (ofmt->extensions && std::strlen(ofmt->extensions) > 0) {
              org::esb::util::StringTokenizer tok(ofmt->extensions, ",");
              while (tok.hasMoreTokens()) {
                v_format_ext->addItem(tok.nextToken());
              }
              v_format_ext->setCurrentIndex(0);
            }
          }
        }
      }

      FormatPanel::~FormatPanel() {
      }
    }
  }
}
