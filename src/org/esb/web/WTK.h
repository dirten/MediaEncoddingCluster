#ifndef ORG_ESB_WEB_WTK_H
#define ORG_ESB_WEB_WTK_H

#include <iostream>
#include <list>
#include "org/esb/util/Properties.h"
using namespace std;
namespace org{
namespace esb{
namespace web{
/*Prototype Table*/
class Table;
/*Prototype TableRow*/
class TableRow;
class TableColumn;

using namespace org::esb::util;
class Element{
	public:
		virtual std::string toHtml();//{return string("<").append(_name).append(">").append(_value).append("</").append(_name).append(">");}
		virtual ~Element(){};
		Element(Element & el);
		Element();
		virtual Element & setValue(const char * value);
		Element & setAttribute(const char * key,const char * val);
		virtual Element & addElement(Element & el);
		operator const char*(){return toHtml().c_str();}
	protected:
		string _value;
		string _name;
		list<Element*>_elements;
		Properties _attr;
		string buildAttributes();
};


class TableColumn:public Element{
	public:
		TableColumn();
		TableColumn(TableRow & row);
//		TableColumn(TableRow & row);
		~TableColumn();
};

class TableRow:public Element{
	public:
		TableRow();
		TableRow(Table&table);
		TableRow(TableColumn col);
		~TableRow();
		TableRow & addColumn(TableColumn & col);
};

class Table: public Element{
	public:
		Table();
		~Table();
		void addRow(TableRow row);
		
};

class Input: public Element{
	public:
		Input();
};

class LabledTableRow: public TableRow{
	public:
		LabledTableRow(const char * label, Element & el);
		void setLabel(const char * label);
		void setElement(Element & element);
	private:
		const char * _label;
		Element & _element;
};
}}}
#endif

