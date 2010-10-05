/* 
 * File:   AudioPanel.cpp
 * Author: HoelscJ
 * 
 * Created on 4. Oktober 2010, 17:13
 */

#include "AudioPanel.h"
namespace org {
  namespace esb {
    namespace web {

      AudioPanel::AudioPanel(db::Profile & p):_profile(p), Wt::Ext::Panel() {
      }


      AudioPanel::~AudioPanel() {
      }
    }
  }
}
