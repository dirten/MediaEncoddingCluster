/*----------------------------------------------------------------------
 *  File    : StreamInfo.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose :
 *  Created : 02.03.2010, 20:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2010   Jan Hölscher
 *
 * This program License under the terms in the LICENSE file
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * ----------------------------------------------------------------------
 */

#include "StreamInfo.h"

#include <Wt/WFitLayout>
#include <Wt/WGridLayout>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include "wtk/ElementContainer.h"

#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace web {
      using namespace org::esb::sql;
      using namespace org::esb::config;
      StreamInfo::StreamInfo(ResultSet * rs) {
        std::string title="Stream #";
        title+=rs->getString("stream_index");
        setTitle(title);
        Wt::WFitLayout * f = new Wt::WFitLayout();
        setLayout(f);
        Wt::WContainerWidget * p = new Wt::WContainerWidget();
        p->setOverflow(Wt::WContainerWidget::OverflowAuto);
        f->addWidget(p);
        Wt::WGridLayout * l = new Wt::WGridLayout();
        p->setLayout(l);
        wtk::ElementContainer<Wt::WText> el;
        el.addElement("strid", "StreamId", rs->getString("id"), l);
        el.addElement("fileid", "FileId", rs->getString("fileid"), l);
        el.addElement("stridx", "StreamIndex", rs->getString("stream_index"), l);
        el.addElement("strtype", "StreamType", rs->getString("stream_type"), l);
        el.addElement("strcodec", "Codec", rs->getString("codec"), l);
        std::string fr=rs->getString("framerate_num");
        fr+="/";
        fr+=rs->getString("framerate_den");
        el.addElement("strfr", "FrameRate", fr, l);
        el.addElement("strstart", "StartPTS", rs->getString("start_time"), l);
        el.addElement("strfirstdts", "StartDTS", rs->getString("first_dts"), l);
        el.addElement("strdur", "Duration", rs->getString("duration"), l);
        std::string tb=rs->getString("time_base_num");
        tb+="/";
        tb+=rs->getString("time_base_den");
        el.addElement("strtb", "StreamTimebase", tb, l);
        std::string ctb=rs->getString("codec_time_base_num");
        ctb+="/";
        ctb+=rs->getString("codec_time_base_den");
        el.addElement("strctb", "CodecTimebase", ctb, l);
        el.addElement("strticks", "Ticks per Frame", rs->getString("ticks_per_frame"), l);
        std::string size=rs->getString("width");
        size+="x";
        size+=rs->getString("height");
        el.addElement("strsize", "Width x Height", size, l);

        /*
        el.addElement("filepath", "FilePath", rs.getString("path"), l)->setWordWrap(false);
        el.addElement("filename", "FileName", rs.getString("filename"), l)->setWordWrap(false);
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
        std::string d = StringUtil::toString(rs.getLong("duration") / 1000000);
        el.addElement("duration", "Duration", d, l);
        el.addElement("bitrate", "Bitrate", rs.getString("bitrate"), l);
        el.addElement("insertdate", "Insertdate", rs.getString("insertdate"), l);
        */
        std::string tmp;
        el.addElement("blank", tmp, tmp, l);
        l->setRowStretch(l->rowCount() - 1, -1);
      }

      StreamInfo::~StreamInfo() {
      }
    }
  }
}
