/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/PluginContext.h"
#include "EncodingTask.h"
#include "org/esb/lang/Ptr.h"
#include "StreamData.h"

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/hive/PresetReaderJson.h"
namespace encodingtask {

  EncodingTask::EncodingTask() {
  }

  EncodingTask::~EncodingTask() {
  }

  void EncodingTask::prepare() {
    _srcuristr = getContext()->getEnvironment<std::string > ("encodingtask.src");
    std::string preset_id = getContext()->getEnvironment<std::string > ("encodingtask.profile");
    litesql::DataSource<db::Preset>profileds = litesql::select<db::Preset > (*getContext()->database, db::Preset::Uuid == preset_id);
    if (profileds.count() == 1) {
      db::Preset preset = profileds.one();
      org::esb::hive::PresetReaderJson reader(preset.data);
      org::esb::hive::PresetReaderJson::CodecList codecs = reader.getCodecList();
      org::esb::hive::PresetReaderJson::FilterList filters = reader.getFilterList();
      org::esb::hive::PresetReaderJson::Preset pre = reader.getPreset();

    } else {
      setStatus(Task::ERROR);
      setStatusMessage(std::string("Profile with id ").append(preset_id).append(" not found!"));
    }

  }

  org::esb::core::OptionsDescription EncodingTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("encodingtask");
    result.add_options()
            ("encodingtask.src", boost::program_options::value<std::string > ()->default_value(""), "Encoding task source")
            ("encodingtask.profile", boost::program_options::value<std::string > ()->default_value(""), "Encoding task profile");
    return result;
  }

  void EncodingTask::execute() {
    /*open the input file*/
    org::esb::av::FormatInputStream fis(_srcuristr);

    /*check the input is valid*/
    if (!fis.isValid()) {
      setStatus(Task::ERROR);
      setStatusMessage(std::string("Input file ").append(_srcuristr).append(" is not a valid media file!"));
      return;
    }

    /*preprocessing the input streams*/
    int scount = fis.getStreamCount();
    map<int, StreamData> stream_map;
    for (int a = 0; a < scount; a++) {
      /*getting the input stream from the file*/
      org::esb::av::AVInputStream* is = fis.getAVStream(a);

      /*create a stream data element for each stream from the input file*/
      StreamData & sdata = stream_map[is->index];

      /*create the decoder objects*/
      sdata.decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is));
      sdata.pass2decoder = boost::shared_ptr<org::esb::av::Decoder > (new org::esb::av::Decoder(is));


    }
  }

  REGISTER_TASK("EncodingTask", EncodingTask);

}
