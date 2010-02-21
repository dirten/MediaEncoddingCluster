#ifndef ORG_ESB_WEB_FILEINFO
#define ORG_ESB_WEB_FILEINFO
#include <Wt/WContainerWidget>
#include <Wt/WScrollArea>
#include <Wt/WText>
#include <Wt/WLength>
#include <Wt/WTable>
#include <Wt/WGroupBox>
#include <Wt/Ext/ComboBox>
#include <Wt/Ext/Button>
#include <Wt/Ext/Panel>

#include <Wt/WGridLayout>
#include <Wt/WFitLayout>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"

#include "org/esb/util/Decimal.h"
#include "org/esb/util/StringUtil.h"
//#include "job.cpp"
#include "org/esb/hive/JobUtil.h"
#include "wtk/ElementContainer.h"
#include <map>
#include <list>
using namespace org::esb::sql;
using namespace org::esb::config;
namespace org {
  namespace esb {
    namespace web {

      class FileInfo : public Wt::Ext::Panel {
      public:

        FileInfo(int fileid) : Wt::Ext::Panel() {
          setTitle("File Details");
          Wt::WFitLayout * f = new Wt::WFitLayout();
          setLayout(f);
          Wt::WContainerWidget * p = new Wt::WContainerWidget();
          p->setOverflow(Wt::WContainerWidget::OverflowScroll);

          Wt::WGridLayout * l = new Wt::WGridLayout();
          p->setLayout(l);
          wtk::ElementContainer<Wt::WText> el;


          Connection con(Config::getProperty("db.connection"));
          std::string sql = "SELECT * FROM files WHERE id = ";
          sql += StringUtil::toString(fileid);
          Statement st = con.createStatement(sql.c_str());
          ResultSet rs = st.executeQuery();
          if (rs.next()) {
            el.addElement("fileid", "FileId", rs.getString("id"), l);
            el.addElement("filepath", "FilePath", rs.getString("path"), l);
            el.addElement("filename", "FileName", rs.getString("filename"), l);
            el.addElement("filesize", "FileSize", rs.getString("size"), l);
            el.addElement("streamcount", "StreamCount", rs.getString("stream_count"), l);
            el.addElement("ctype", "Type", rs.getString("container_type"), l);
            el.addElement("title", "Title", rs.getString("title"), l);
            el.addElement("author", "Author", rs.getString("author"), l);
            el.addElement("copyright", "Copyright", rs.getString("copyright"), l);
            el.addElement("comment", "Comment", rs.getString("comment"), l);
            el.addElement("album", "Album", rs.getString("album"), l);
            el.addElement("year", "Year", rs.getString("year"), l);
            el.addElement("track", "Track", rs.getString("track"), l);
            el.addElement("genre", "Genre", rs.getString("genre"), l);
            std::string d=StringUtil::toString(rs.getLong("duration")/1000000);
            el.addElement("duration", "Duration", d, l);
            el.addElement("bitrate", "Bitrate", rs.getString("bitrate"), l);
            el.addElement("insertdate", "Insertdate", rs.getString("insertdate"), l);
            std::string tmp;
            el.addElement("blank", tmp, tmp, l);
            l->setRowStretch(l->rowCount()-1,-1);
          }
        }

      private:
      };
    }
  }
}
#endif

