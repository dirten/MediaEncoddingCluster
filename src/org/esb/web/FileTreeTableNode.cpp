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

FileTreeTableNode::FileTreeTableNode(const boost::filesystem::path& path)
  : WTreeTableNode(Wt::widen(path.leaf()), createIcon(path)),
    path_(path), _filter(NULL)
{
  label()->setFormatting(WText::PlainFormatting);

  if (boost::filesystem::exists(path)) {
    if (!boost::filesystem::is_directory(path)) {
      long long int fsize = 0;
      try{
        fsize=boost::filesystem::file_size(path);
      }catch(...){
        logerror("Fehler in FSIZE");
      }
      
      setColumnWidget(1, new WText(false,
				   boost::lexical_cast<std::wstring>(fsize)));
      columnWidget(1)->setStyleClass("fsize");
    }
    //else      
    setSelectable(true);
	std::time_t t;
	try{
    t = boost::filesystem::last_write_time(path);
	}catch(...){
	    logerror("last_write_time(path)");
	}
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
  }
}

FileTreeTableNode::FileTreeTableNode(const boost::filesystem::path& path, org::esb::io::FileFilter & filter)
  : WTreeTableNode(Wt::widen(path.leaf()), createIcon(path)),
    path_(path), _filter(&filter)
{
  label()->setFormatting(WText::PlainFormatting);

  if (boost::filesystem::exists(path)) {
    if (!boost::filesystem::is_directory(path)) {
      long long int fsize = 0;
      try{
        fsize=boost::filesystem::file_size(path);
      }catch(...){
        logerror("Fehler in FSIZE");
      }
      
      setColumnWidget(1, new WText(false,
				   boost::lexical_cast<std::wstring>(fsize)));
      columnWidget(1)->setStyleClass("fsize");
    }
    //else      
    setSelectable(true);
	std::time_t t;
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

	}
}

WIconPair *FileTreeTableNode::createIcon(const boost::filesystem::path& path)
{

  if (boost::filesystem::exists(path)
      && boost::filesystem::is_directory(path))
    return new WIconPair("icons/yellow-folder-closed.png",
			 "icons/yellow-folder-open.png", false);
  else
    return new WIconPair("icons/document.png",
			 "icons/yellow-folder-open.png", false);
}

void FileTreeTableNode::populate()
{
  try {
    
    if (boost::filesystem::is_directory(path_)) {
      std::set<boost::filesystem::path> paths;
      boost::filesystem::directory_iterator end_itr;

      for (boost::filesystem::directory_iterator i(path_); i != end_itr; ++i)
	paths.insert(*i);

      for (std::set<boost::filesystem::path>::iterator i = paths.begin();
      i != paths.end(); ++i){
        if(_filter!=NULL&&_filter->accept(org::esb::io::File(i->string().c_str())))
          addChildNode(new FileTreeTableNode(*i, (org::esb::io::FileFilter&)*_filter));
      }
    }
  } catch (boost::filesystem::filesystem_error& e) {
    std::cerr << e.what() << std::endl;
  }
  
  
  
}


bool FileTreeTableNode::expandable()
{
  if (!populated()) {
    return boost::filesystem::is_directory(path_);
  } else
    return WTreeTableNode::expandable();
}
