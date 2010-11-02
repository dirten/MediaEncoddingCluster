/* 
 * File:   PresetFileWriterTest.cpp
 * Author: HoelscJ
 *
 * Created on 2. November 2010, 16:19
 */


#include "org/esb/hive/PresetFileWriter.h"

#include <map>
using namespace std;
using namespace org::esb::hive;

/*
 * 
 */
int main(int argc, char** argv) {
  std::string file = std::string(MEC_SOURCE_DIR) + "/test.preset";
  PresetFileWriter writer(file);

  PresetFileWriter::Preset preset;
  preset.insert(std::pair<std::string, std::string > ("name", "PresetFileWriter Test"));
  preset.insert(std::pair<std::string, std::string > ("id", "mp4"));
  preset.insert(std::pair<std::string, std::string > ("fileExtension", "mp4"));
  preset.insert(std::pair<std::string, std::string > ("author", "testAuthor"));
  writer.setPreset(preset);


  PresetFileWriter::CodecList codecs;
  std::multimap<std::string, std::string> video;
  video.insert(std::pair<std::string, std::string > ("codec_id", "libx264"));
  video.insert(std::pair<std::string, std::string > ("b", "1000"));
  video.insert(std::pair<std::string, std::string > ("g", "250"));
  video.insert(std::pair<std::string, std::string > ("cmp", "chroma"));

  std::multimap<std::string, std::string> audio;
  audio.insert(std::pair<std::string, std::string > ("codec_id", "mp2"));
  audio.insert(std::pair<std::string, std::string > ("ab", "128000"));
  audio.insert(std::pair<std::string, std::string > ("ar", "44100"));
  audio.insert(std::pair<std::string, std::string > ("ac", "2"));

  codecs.insert(std::pair<std::string, std::multimap<std::string, std::string> >("video", video));
  codecs.insert(std::pair<std::string, std::multimap<std::string, std::string> >("audio", audio));


  writer.setCodecList(codecs);

  PresetFileWriter::FilterList filter;

  std::multimap<std::string, std::string> resize;
  resize.insert(std::pair<std::string, std::string > ("width", "320"));
  resize.insert(std::pair<std::string, std::string > ("height", "240"));

 
  filter.insert(std::pair<std::string, std::multimap<std::string, std::string> >("resize", resize));

  writer.setFilterList(filter);

  writer.write();
  return 0;
}

