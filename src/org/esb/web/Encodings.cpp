#ifndef ORG_ESB_WEB_ENCODING
#define ORG_ESB_WEB_ENCODING

namespace org {
  namespace esb {
    namespace web {

      class Encoding : public Wt::WContainerWidget {
      public:
        Encoding(int user_id) : Wt::WContainerWidget(NULL) {
			SqlTable*tab = new SqlTable(std::string("SELECT   filename, round(count(complete)/count(*)*100,2) progress,min(send) start, max(complete) complete,sum(timestampdiff(SECOND,send,complete)) \"cpu-time\" FROM process_units pu, streams s, files f where pu.target_stream=s.id and s.fileid=f.id group by fileid order by 2,f.id DESC"), this);
            tab->resize(Wt::WLength(), 300);
            tab->setColumnWidth(1,10);
            tab->setColumnWidth(2,20);
            tab->setColumnWidth(3,20);
            tab->setColumnWidth(4,10);
		}
	};
}}}
#endif


