/*
 *  ColumnConfig.h
 *  test2
 *
 *  Created by Jan Hölscher on 26.07.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef ORG_ESB_WEB_COLUMNCONFIG
#define ORG_ESB_WEB_COLUMNCONFIG
#include "org/esb/db/hivedb.hpp"
#include <string>
class ColumnConfig{
  litesql::FieldType _field;
//  litesql::Expr _expr;
  std::string _name;
  std::string _fieldname;
  int _width;
  public:
  ColumnConfig(litesql::FieldType field,std::string name, int width,std::string func=std::string());
//  ColumnConfig(const litesql::Expr & expr,std::string name, int width);
  litesql::FieldType getFieldType(){return _field;}
//  litesql::Expr getExpr(){return _expr;}
  std::string getFieldName(){return _fieldname;}
  std::string getHeader(){return _name;}
  int getWidth(){return _width;}
};
#endif

