// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "FileTreeTable.h"
#include "FileTreeTableNode.h"

//#include <Wt/WMessage>
#include <Wt/WWidget>
#include <Wt/WText>
#include <Wt/WTree>

using namespace Wt;

FileTreeTable::FileTreeTable(const boost::filesystem::path& path, WContainerWidget *parent) : WTreeTable(parent) {
//    addColumn("Size", 80);
//    addColumn("Modified", 110);

//    header(1)->setStyleClass("fsize");
//    header(2)->setStyleClass("date");

    setTreeRoot(new FileTreeTableNode(path), "File");
    
    treeRoot()->setImagePack("icons/");
    treeRoot()->expand();
}

FileTreeTable::FileTreeTable(const boost::filesystem::path& path,org::esb::io::FileFilter & filter, WContainerWidget *parent) : WTreeTable(parent) {
//    addColumn("Size", 80);
//    addColumn("Modified", 110);

//    header(1)->setStyleClass("fsize");
//    header(2)->setStyleClass("date");

    setTreeRoot(new FileTreeTableNode(path, filter), "File");
    using namespace Wt;
    
    tree()->setSelectionMode(ExtendedSelection);
    treeRoot()->setImagePack("icons/");
    treeRoot()->expand();
    
}
