#include "DeinterlaceFilterPanel.h"


namespace org{
  namespace esb{
    namespace web{
      DeinterlaceFilterPanel::DeinterlaceFilterPanel():BaseFilterPanel(){
        setTitle("Deinterlace Filter");

      }
      DeinterlaceFilterPanel::~DeinterlaceFilterPanel(){

      }
      std::string DeinterlaceFilterPanel::getId(){
        return "deinterlace";
      }
    }
  }
}

