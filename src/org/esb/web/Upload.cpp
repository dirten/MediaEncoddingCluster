#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WText>
namespace org{
namespace esb{
namespace web{
class Upload:public Wt::WContainerWidget{
  public:
    Upload():Wt::WContainerWidget(){
       

      addWidget(new Wt::WText("Upload Home"));

      upload=new Wt::WFileUpload(this);
      status=new Wt::WText("", this);
  // Try to catch the fileupload change signal to trigger an upload.
  // We could do like google and at a delay with a WTimer as well...
      upload->changed.connect(SLOT(upload, Wt::WFileUpload::upload));

  // React to a succesfull upload.
      upload->uploaded.connect(SLOT(this, Upload::uploaded));

  // React to a fileupload problem.
      upload->fileTooLarge.connect(SLOT(this, Upload::fileTooLarge));

//      return new Wt::WText("Upload Home");
    }
  private:
    void uploaded(){
      status->setText("Uploaded!!!");    
    }
    void fileTooLarge(){
      status->setText("File to large!!!");
    }

    Wt::WFileUpload * upload;
    Wt::WText * status;
};
}}}

