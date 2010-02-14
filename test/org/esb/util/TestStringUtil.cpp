#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Log.h"
using namespace org::esb;


void testToString(){
  std::string str="testw";
  std::string isr="1";
  std::wstring t(str.begin(), str.end());
  std::wstring in(isr.begin(), isr.end());
  int i=1;
  assert(util::StringUtil::toString(i)=="1");
  assert(util::StringUtil::toString(t)==str);
  assert(util::StringUtil::toWString(1)==in);
  assert(util::StringUtil::toWString(str)==t);
}
void testStringTokenizer(){
  std::string sql;
//  logdebug(MEC_SOURCE_DIR);
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
//    logdebug("Create Table " << next);
//    logdebug("Create Table leng" << next.size());
  }
}

void testTrim(){
  std::string teststring("   bla fasel   ");
  std::string trimmed=util::StringUtil::trim(teststring);
  LOGDEBUG(":"<<trimmed<<":");
  assert(trimmed.length()==9);
}
int main() {
  testStringTokenizer();
  testToString();
  testTrim();
}
