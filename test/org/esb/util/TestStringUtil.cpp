#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
using namespace org::esb;

int main() {
  std::string sql;
  logdebug(MEC_SOURCE_DIR);
  std::string path=MEC_SOURCE_DIR;
  path.append("/sql/hive-0.0.3.sql");
  io::File f(path);
  io::FileInputStream(&f).read(sql);
  util::StringTokenizer st(sql, ";");
  int tok = st.countTokens();
  for (int i = 0; i < tok - 1; i++) {
    std::string t = st.nextToken();
    t = util::StringUtil::trim(t, *new std::string("\n"));
    std::string next = util::StringUtil::trim(t);
    logdebug("Create Table " << next);
    logdebug("Create Table leng" << next.size());
  }
}
