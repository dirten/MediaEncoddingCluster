#include "WTK.h"
#include <iostream>
using namespace org::esb::web;
using namespace std;

Element::Element(){

}

Element::Element(TableColumn & el){
	el.addElement(*this);
}

Element Element::setValue(const char * value){
	_value=string(value);
	return * this;
}

Element Element::addElement(Element & el){
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
		return string("<").append(_name).append(">").append(_value).append("</").append(_name).append(">");
	}else{
		return _value;
	}
	
}

/*-------------------------------------------------------------------------------------*/

Table::Table(){
	_name="table";
}

Table::~Table(){
	

}

Table Table::addRow(TableRow & row){	
	_elements.push_back(&row);
	return *this;
}

/*-------------------------------------------------------------------------------------*/

TableRow::TableRow(){

}

TableRow::TableRow(Table & table){
	_name="tr";
	table.addElement(*this);
}

TableRow TableRow::addColumn(TableColumn & col){
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


