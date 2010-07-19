/* 
 * File:   Project.h
 * Author: HoelscJ
 *
 * Created on 14. Juli 2010, 11:46
 */

#ifndef PROJECT_H
#define	PROJECT_H

#include "MediaFile.h"
#include <boost/shared_ptr.hpp>
#include <list>
class Project {
public:
  Project();
  Project(const Project& orig);
  virtual ~Project();

  void addInputMediaFile(boost::shared_ptr<MediaFile> file);

private:
  std::list<boost::shared_ptr<MediaFile> > _inputMediaFileList;
};

#endif	/* PROJECT_H */

