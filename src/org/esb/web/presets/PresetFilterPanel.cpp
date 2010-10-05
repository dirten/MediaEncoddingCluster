/* 
 * File:   FilterPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 17:13
 */

#include "PresetFilterPanel.h"
namespace org {
  namespace esb {
    namespace web {

      FilterPanel::FilterPanel(db::Profile & p):_profile(p), Wt::Ext::Panel() {
      }


      FilterPanel::~FilterPanel() {
      }
    }
  }
}
