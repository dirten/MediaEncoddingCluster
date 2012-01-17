#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "inttypes.h"
#include "Poco/RegularExpression.h"
#include "Poco/String.h"
int64_t dircount = 0;
int64_t filecount = 0;

void print_file_counter() {
  std::cout << "\r Directories=" << dircount << "\t\t\t FileCount" << filecount;
}

void fileCounter(Ptr<org::esb::io::File> path) {
  if (!path->exists())return;
  if (path->isDirectory()) {
    dircount++;
    org::esb::io::FileList flist = path->listFiles();
    org::esb::io::FileList::iterator fit = flist.begin();
    for (; fit != flist.end(); fit++)
      fileCounter(*fit);
  } else {
    filecount++;
  }
  print_file_counter();
}

int main(int argc, char**argv) {
  std::string path = MEC_SOURCE_DIR;
  Ptr<org::esb::io::File> file = new org::esb::io::File(path + "/src");
  fileCounter(file);
  std::cout << std::endl;
  //  return 0;
  Log::open("");
  //	return 0;


  org::esb::io::File f(path.append("/src/./CMakeLists.txt"));
  org::esb::io::File f2("./CMakeLists.txt");
  org::esb::io::File dir(f.getFilePath());
  org::esb::io::File create("test.create");

  create.createNewFile();
  assert(create.exists());
  create.deleteFile();
  assert(!create.exists());

  assert(f.exists());
  LOGDEBUG(f.getPath());
  LOGDEBUG(f.getFilePath());
  assert(f.getPath().length() > 0);
  assert(f.getFileName().length() > 0);
  assert(f.getFilePath().length() > 0);
  assert(f.canRead());
  //canWrite is currently not implemented correctly
  //  assert(f.canWrite()==true);
  //  assert(f.isFile());
  assert(!f.isDirectory());

  assert(!dir.isFile());
  assert(dir.isDirectory());

  LOGDEBUG("GetPath" << dir.getPath());
  LOGDEBUG(dir.getFilePath() << " GetParent" << dir.getParent());
  LOGDEBUG("File GetParent" << f.getParent());
  LOGDEBUG("Relative File . " << f2.getPath());

  //  f.getExtension();


  org::esb::io::File url("http://samples.mplayerhq.hu/FLV/Ogg Theora(320x240) Vorbis(128kb 48kHz stereo)/IFTlhxwwkKE.flv");
  LOGDEBUG("Path:" << url.getPath());
  LOGDEBUG("Ext:" << url.getExtension());
  LOGDEBUG("FileName:" << url.getFileName());
  LOGDEBUG("FilePath:" << url.getFilePath());
  LOGDEBUG("Fileexist:" << url.exists());
  if (url.exists()) {
    url = url.getParent();
    LOGDEBUG("Path:" << url.getPath());
    LOGDEBUG("Ext:" << url.getExtension());
    LOGDEBUG("FileName:" << url.getFileName());
    LOGDEBUG("FilePath:" << url.getFilePath());

  }
  {
    org::esb::io::File url("/usr/local");
    LOGDEBUG("Path:" << url.getPath());
    LOGDEBUG("Ext:" << url.getExtension());
    LOGDEBUG("FileName:" << url.getFileName());
    LOGDEBUG("FilePath:" << url.getFilePath());
    LOGDEBUG("Fileexist:" << url.exists());
    LOGDEBUG("Directory:" << url.isDirectory());
  }
  {
    org::esb::io::File url("/media/video/ChocolateFactory.t");
    LOGDEBUG("Path:" << url.getPath());
    LOGDEBUG("Ext:" << url.getExtension());
    LOGDEBUG("FileName:" << url.getFileName());
    LOGDEBUG("FilePath:" << url.getFilePath());
    LOGDEBUG("Fileexist:" << url.exists());
    org::esb::io::File wld(url.getFilePath());
    org::esb::io::FileList list = wld.listFiles();
    LOGDEBUG("FilesCount" << list.size());
    std::string reg=url.getPath();
    reg=Poco::replace(reg,".","\\.");
    reg=Poco::replace(reg,"*",".*");
    LOGDEBUG("Regex:"<<reg);
    Poco::RegularExpression re(reg);

    foreach(Ptr<org::esb::io::File> file, list) {
      if (re.match(file->getPath())) {
        LOGDEBUG("File Found:" << file->getPath());
      }
    }
  }
  Log::close();
  return 0;
}
