#include "WTK.h"
#include <iostream>
#include <vector>
#include <map>
using namespace org::esb::web;
using namespace std;

Element::Element(){

}

Element::Element(Element & el){
	el.addElement(*this);
}

Element & Element::setValue(const char * value){
	_value=string(value);
	return * this;
}

Element & Element::setAttribute(const char * key,const char * val){
	_attr.setProperty(key, val);
	return * this;
}

Element & Element::addElement(Element & el){
	_elements.push_back(&el);
	return * this;
}

string Element::toHtml(){
	list<Element*>::iterator it;
	for(it=_elements.begin();it!=_elements.end();it++){
		Element * el=*it;
		_value.append(el->toHtml());
	}
	if(strlen(_name.c_str())>0){
		return string("<").append(_name).append(buildAttributes()).append(">").append(_value).append("</").append(_name).append(">");
	}else{
		return _value;
	}	
}
string Element::buildAttributes(){
	string result=" ";
	vector<std::pair<std::string,std::string> > attr=_attr.toArray();
	vector<std::pair<std::string,std::string> >::iterator it;
	for(it=attr.begin();it!=attr.end();it++){
		pair<std::string,std::string> el=*it;
		result.append(el.first).append("=");
		result.append("\"").append(el.second).append("\" ");
	}
	return result;
}

/*-------------------------------------------------------------------------------------*/

Table::Table(){
	_name="table";
}

Table::~Table(){
	

}

Table& Table::addRow(TableRow & row){	
//    return addElement((Element&)row);
	_elements.push_back(&row);
	return *this;
}

/*-------------------------------------------------------------------------------------*/

TableRow::TableRow(){
	_name="tr";
}

TableRow::TableRow(Table & table){
	_name="tr";
	table.addElement(*this);
}

TableRow::TableRow(TableColumn  col){
	_name="tr";
	addElement(col);
}

TableRow & TableRow::addColumn(TableColumn & col){
	_elements.push_back(&col);
	return *this;
}

TableRow::~TableRow(){


}

/*-------------------------------------------------------------------------------------*/

TableColumn::TableColumn(){
	_name="td";
}

TableColumn::TableColumn(TableRow & row){
	_name="td";
	row.addElement(*this);
}

TableColumn::~TableColumn(){


}
/*
std::string TableColumn::toHtml(){
    return Element::toHtml();
}
*/
/*-------------------------------------------------------------------------------------*/
Input::Input(){
	_name="input";
}
/*-------------------------------------------------------------------------------------*/
LabledTableRow::LabledTableRow(const char * label, Element & el):_label(label), _element(el){


}

void LabledTableRow::setLabel(const char * label){


}

void LabledTableRow::setElement(Element & el){


}
