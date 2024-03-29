// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "FileTreeTableNode.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <time.h>

#include "org/esb/util/Log.h"
#include <Wt/WIconPair>
#include <Wt/WStringUtil>
#include <Wt/WText>

#include "org/esb/io/File.h"
using namespace Wt;

struct FileComparator {

  bool operator() (const boost::filesystem::path& lhs, const boost::filesystem::path & rhs) const {
    org::esb::io::File a(lhs.string().c_str());
    org::esb::io::File b(rhs.string().c_str());
    if (a.isFile() && b.isDirectory())
      return false;
    if (a.isDirectory() && b.isFile())
      return true;
    return lhs.string() < rhs.string();
  }

};

FileTreeTableNode::FileTreeTableNode(const boost::filesystem::path& path)
: WTreeTableNode(Wt::fromUTF8(path.leaf()), createIcon(path)),
path_(path), _filter(NULL) {
  LOGDEBUG(path);
  label()->setTextFormat(PlainText);

  if (boost::filesystem::exists(path)) {
    /*
    if (!boost::filesystem::is_directory(path)) {
      long long int fsize = 0;
      try {
        fsize = boost::filesystem::file_size(path);
      } catch (...) {
        LOGERROR("Fehler in FSIZE:"<<path);
      }

      setColumnWidget(1, new WText(false,
          boost::lexical_cast<std::wstring > (fsize)));
      columnWidget(1)->setStyleClass("fsize");
    }*/
    //else      
    setSelectable(true);
    std::time_t t;
    try {
      t = boost::filesystem::last_write_time(path);
    } catch (...) {
      LOGERROR("last_write_time(path)");
    }
    struct tm ttm;
#if __WIN32__
    ttm = *localtime(&t);
#else
    localtime_r(&t, &ttm);
#endif

    char c[100];
    strftime(c, 100, "%b %d %Y", &ttm);

    setColumnWidget(2, new WText(c));
    columnWidget(2)->setStyleClass("date");
  } else {
    LOGDEBUG("Path does not exist:" << path)
  }
}

FileTreeTableNode::FileTreeTableNode(const boost::filesystem::path& path, org::esb::io::FileFilter & filter)
: WTreeTableNode(Wt::fromUTF8(path.leaf() == "/" ? path.root_name() : path.leaf()), createIcon(path)),
path_(path), _filter(&filter) {
  //	std::cout <<"Path:"<<path_.root_name()<<std::endl;
  LOGDEBUG(path);
  label()->setTextFormat(PlainText);

  if (boost::filesystem::exists(path)) {
    /*
    if (!boost::filesystem::is_directory(path)) {
      long long int fsize = 0;
      try {
        fsize = boost::filesystem::file_size(path);
      } catch (...) {
        LOGERROR("Fehler in FSIZE");
      }

      setColumnWidget(1, new WText(false,
          boost::lexical_cast<std::wstring > (fsize)));
      columnWidget(1)->setStyleClass("fsize");
    }*/
    //else      
    setSelectable(true);
    //	std::time_t t;
    /*
            try{
        t = boost::filesystem::last_write_time(path);
        struct tm ttm;
    #if WIN32
        ttm=*localtime(&t);
    #else
        localtime_r(&t, &ttm);
    #endif

        char c[100];
        strftime(c, 100, "%b %d %Y", &ttm);

        setColumnWidget(2, new WText(c));
        columnWidget(2)->setStyleClass("date");
            }catch(...){
            logerror("last_write_time(path)");
            }
     */
  }
}

WIconPair *FileTreeTableNode::createIcon(const boost::filesystem::path& path) {

  if (boost::filesystem::exists(path)
          && boost::filesystem::is_directory(path))
    return new WIconPair("icons/yellow-folder-closed.png",
          "icons/yellow-folder-open.png", false);
  else
    return new WIconPair("icons/document.png",
          "icons/yellow-folder-open.png", false);
}

void FileTreeTableNode::populate() {

  if (boost::filesystem::is_directory(path_)) {
    std::set<boost::filesystem::path, FileComparator> paths;
    boost::filesystem::directory_iterator end_itr;

#ifdef WIN32
    if (path_ == "/") {
      std::set<std::string> drives;
      drives.insert("A:\\");
      drives.insert("B:\\");
      drives.insert("C:\\");
      drives.insert("D:\\");
      drives.insert("E:\\");
      drives.insert("F:\\");
      drives.insert("G:\\");
      drives.insert("H:\\");
      drives.insert("I:\\");
      drives.insert("J:\\");
      drives.insert("K:\\");
      drives.insert("L:\\");
      drives.insert("M:\\");
      drives.insert("N:\\");
      drives.insert("O:\\");
      drives.insert("P:\\");
      drives.insert("Q:\\");
      drives.insert("R:\\");
      drives.insert("S:\\");
      drives.insert("T:\\");
      drives.insert("U:\\");
      drives.insert("V:\\");
      drives.insert("W:\\");
      drives.insert("X:\\");
      drives.insert("Y:\\");
      drives.insert("Z:\\");
      for (std::set<std::string>::iterator d = drives.begin(); d != drives.end(); d++) {
        try {
          boost::filesystem::file_status fs = boost::filesystem::status(*d);

          if (boost::filesystem::status_known(fs) && boost::filesystem::exists(*d)) {
            paths.insert(*d);
          }
        } catch (boost::filesystem::filesystem_error & er) {
          LOGINFO(er.what());
        }
      }
    } else
#endif
      for (boost::filesystem::directory_iterator i(path_); i != end_itr; ++i) {
        try {
          paths.insert(*i);
        } catch (boost::filesystem::filesystem_error& e) {
          std::cerr << e.what() << std::endl;
        }
      }

    for (std::set<boost::filesystem::path,FileComparator>::iterator i = paths.begin();
            i != paths.end(); ++i) {
      try {

        if (_filter != NULL && _filter->accept(org::esb::io::File(i->string().c_str()))){
          LOGDEBUG("Add Child Node:"<<(*i));
          addChildNode(new FileTreeTableNode(*i, (org::esb::io::FileFilter&) * _filter));
        }
      } catch (boost::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
      }

    }
  }



}

bool FileTreeTableNode::expandable() {
  if (!populated()) {
    return boost::filesystem::is_directory(path_);
  } else
    return WTreeTableNode::expandable();
}
