#include "Div.h"
namespace org {
  namespace esb {
    namespace web {
      namespace wtk {

        Div::Div(const std::string& style, WContainerWidget *parent)
        : Wt::WContainerWidget(parent) {
          setStyleClass(style);
        }
      };
    }
  }
}

