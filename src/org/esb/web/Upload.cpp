#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WText>
namespace org{
namespace esb{
namespace web{
class Upload{
  public:
    Wt::WWidget * home(){
      Wt::WContainerWidget *result=new Wt::WContainerWidget();

      result->addWidget(new Wt::WText("Upload Home"));

      upload=new Wt::WFileUpload(result);
      status=new Wt::WText("", result);
  // Try to catch the fileupload change signal to trigger an upload.
  // We could do like google and at a delay with a WTimer as well...
      upload->changed.connect(SLOT(upload, Wt::WFileUpload::upload));

  // React to a succesfull upload.
      upload->uploaded.connect(SLOT(this, Upload::uploaded));

  // React to a fileupload problem.
      upload->fileTooLarge.connect(SLOT(this, Upload::fileTooLarge));

      return result;
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

