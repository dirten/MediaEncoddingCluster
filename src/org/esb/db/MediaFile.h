/* 
 * File:   MediaFile.h
 * Author: HoelscJ
 *
 * Created on 14. Juli 2010, 11:46
 */

#ifndef MEDIAFILE_H
#define	MEDIAFILE_H

#include <string>
class MediaFile {
public:
  MediaFile();
  MediaFile(const MediaFile& orig);
  virtual ~MediaFile();
  void setId(int64_t id);
  void setFileName(std::string filename);
  void setPath(std::string path);
  void setSize(int64_t size);
  void setStreamCount(int32_t count);
  void setContainerType(std::string type);
  void setDuration(int64_t duration);
  void setBitrate(int64_t bitrate);
  
private:
  int64_t _id;
  std::string _filename;
  std::string _path;
  int64_t _size;
  int32_t _stream_count;
  std::string _container_type;
  int64_t _duration;
  int32_t _bitrate;
  int8_t _file_type;
  int64_t _parent_file_id;
};

#endif	/* MEDIAFILE_H */

