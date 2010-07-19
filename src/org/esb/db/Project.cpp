/* 
 * File:   Project.cpp
 * Author: HoelscJ
 * 
 * Created on 14. Juli 2010, 11:46
 */

#include "Project.h"

Project::Project() {
}

Project::Project(const Project& orig) {
}

Project::~Project() {
}

void Project::addInputMediaFile(boost::shared_ptr<MediaFile> file){
  _inputMediaFileList.push_back(file);
}

