#include "ResizeFilterPanel.h"

namespace org{
  namespace esb{
    namespace web{

      ResizeFilterPanel::ResizeFilterPanel():BaseFilterPanel(){
        setTitle("Resize Filter");

      }
      ResizeFilterPanel::~ResizeFilterPanel(){

      }
      std::string ResizeFilterPanel::getId(){
        return "resize";
      }
    }
  }
}