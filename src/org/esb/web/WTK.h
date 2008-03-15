#ifndef ORG_ESB_WEB_WTK_H
#define ORG_ESB_WEB_WTK_H

#include <iostream>
#include <list>

using namespace std;
namespace org{
namespace esb{
namespace web{
/*Prototype Table*/
class Table;
/*Prototype TableRow*/
class TableRow;
class TableColumn;


class Element{
	public:
		virtual std::string toHtml();//{return string("<").append(_name).append(">").append(_value).append("</").append(_name).append(">");}
		virtual ~Element(){};
		Element(TableColumn & el);
		Element();
		Element setValue(const char * value);
		virtual Element addElement(Element & el);
		operator const char*(){return toHtml().c_str();}
	protected:
		string _value;
		string _name;
		list<Element*>_elements;
};


class TableColumn:public Element{
	public:
		TableColumn();
		TableColumn(TableRow & row);
		~TableColumn();
};

class TableRow:public Element{
	public:
		TableRow();
		TableRow(Table&table);
		~TableRow();
		TableRow addColumn(TableColumn & col);
};

class Table: public Element{
	public:
		Table();
		~Table();
		Table addRow(TableRow & row);
		
};


}}}
#endif

