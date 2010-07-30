/*
 *  ColumnConfig.cpp
 *  test2
 *
 *  Created by Jan Hölscher on 26.07.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColumnConfig.h"

ColumnConfig::  ColumnConfig(litesql::FieldType field,std::string name, int width, std::string func):_field(field),_name(name),_width(width){
    _fieldname=field.fullName();
}
/*
ColumnConfig::  ColumnConfig(const litesql::Expr& expr,std::string name, int width):_expr(expr),_name(name),_width(width){
    _fieldname=_expr.asString();
}
*/


