/* 
 * File:   CtrlWindow.cpp
 * Author: HoelscJ
 * 
 * Created on 6. Dezember 2010, 15:13
 */

#include "CtrlWindow.h"
#include "wx/wx.h"

namespace org {
  namespace esb {
    namespace ui {

      CtrlWindow::CtrlWindow() {
      }

      CtrlWindow::~CtrlWindow() {
      }

      class MyFrame : public wxFrame {
      public:


        MyFrame(){}
        MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, -1, title, pos, size) {
          wxMenu *menuFile = new wxMenu;

          menuFile->Append(ID_About, _("&About..."));
          menuFile->AppendSeparator();
          menuFile->Append(ID_Quit, _("E&xit"));

          wxMenuBar *menuBar = new wxMenuBar;
          menuBar->Append(menuFile, _("&File"));

          SetMenuBar(menuBar);

          CreateStatusBar();
          SetStatusText(_("Welcome to wxWidgets!"));
        }
        enum {
          ID_Quit = 1,
          ID_About,
        };

        void OnQuit(wxCommandEvent& WXUNUSED(event)) {
          Close(true);
        }

        void OnAbout(wxCommandEvent& WXUNUSED(event)) {
          wxMessageBox(_("This is a wxWidgets Hello world sample"),
                  _("About Hello World"),
                  wxOK | wxICON_INFORMATION, this);
        }


        //DECLARE_EVENT_TABLE()
      };

      class MyApp : public wxApp {

        bool OnInit() {
          MyFrame *frame = new MyFrame(_("Hello World"), wxPoint(50, 50), wxSize(450, 340));
          frame->Show(true);
          SetTopWindow(frame);
          return true;
        }

      };

    }
  }
}
IMPLEMENT_APP(org::esb::ui::MyApp)

/*int main() {
  org::esb::ui::MyApp app;
}*/
