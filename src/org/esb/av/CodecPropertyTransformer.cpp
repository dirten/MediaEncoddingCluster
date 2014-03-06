/* 
 * File:   CodecPropertyTransformer.cpp
 * Author: jholscher
 * 
 * Created on 21. Oktober 2010, 11:54
 */

#include <vector>

#include "CodecPropertyTransformer.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace av {

      CodecPropertyTransformer::CodecPropertyTransformer() {
        init();
      }
      /*
      CodecPropertyTransformer::CodecPropertyTransformer(std::vector<db::StreamParameter> p) {
        init();
        std::vector<db::StreamParameter>::iterator it = p.begin();
        std::map<std::string, std::string> data;
        for (; it != p.end(); it++) {
          data.insert(std::pair<std::string, std::string>((*it).name,(*it).val));
        }
        process(data);
      }*/

      CodecPropertyTransformer::CodecPropertyTransformer(std::map<std::string, std::string> p) {
        init();
        process(p);
      }

      CodecPropertyTransformer::~CodecPropertyTransformer() {
      }

      void CodecPropertyTransformer::process(std::map<std::string, std::string> data) {
        std::map<std::string, std::string>::iterator it = data.begin();
        /**special handling for the time_base*/
        std::string frame_rate_num = "0";
        std::string frame_rate_den = "0";
        for (; it != data.end(); it++) {
          LOGDEBUG("Processing Property " << (*it).first<<"="<<(*it).second);
          std::string key;
          std::string value;
          if (_result.count((*it).first) > 0) {
            key=(*it).first;
            value=(*it).second;
            //_result[(*it).first] = (*it).second;
          } else
            if (_prop2parent.count((*it).first) > 0) {
            //LOGDEBUG("Properyt parent found : " << _prop2parent[(*it).first]);
              key=_prop2parent[(*it).first];
              value=(*it).first;
            if ((*it).second.length() > 0 && (*it).second != "0"){
              //_result[_prop2parent[(*it).first]].append("+").append((*it).first);
            }
            else
              if ((*it).second.length() > 0 && (*it).second == "0"){
                //_result[_prop2parent[(*it).first]].append("-").append((*it).first);
              }
          } else {
            LOGWARN("Property \"" << (*it).first << "\" with value \"" << (*it).second << "\" is unknown!");
            //_result[key] = value;
          }

          if(_options.count(key)&&_options[key]->type==FF_OPT_TYPE_FLAGS){
            LOGDEBUG("result.key.substr "<<value.substr(0,1));
            if(value.substr(0,1)=="-"||value.substr(0,1)=="+"){
              value=value.substr(1);
            }
            if((*it).second=="0"||(*it).second=="false"){
              if(value.substr(0,1)!="-")
                _result[key].append("-");
                _result[key].append(value);
            }else{
              if(value.substr(0,1)!="+")
                _result[key].append("+");
                _result[key].append(value);
            }
            LOGDEBUG("result.key.substr "<<_result[key]);
          }else{
            if(value!="0"&&value!="0.0")
              _result[key] = value;
          }
        }
        //_result["time_base"] = frame_rate_num + "/" + frame_rate_den;
      }

      void CodecPropertyTransformer::init() {
        /** this will be only enabled when support for 2pass encoding is well tested*/
        _result["multipass"] = "0";
        _result["width"] = "0";
        _result["height"] = "0";
        const AVOption * option = NULL;
        AVCodecContext * codec = avcodec_alloc_context3(NULL);
        std::map<std::string, std::string> unit2name;
        while (option = av_next_option(codec, option)) {
          if (option->flags & AV_OPT_FLAG_ENCODING_PARAM || option->flags == 0) {
            std::string name = option->name;
            std::string unit = option->unit != NULL ? option->unit : "";
            if (option->offset == 0) {
              _prop2parent[name] = unit;
            } else {
              _result[name] = "";
              _options[name] = option;
            }
          }
        }
        av_free(codec);
      }

      std::map<std::string, std::string> CodecPropertyTransformer::getCodecProperties() {
        std::map<std::string, std::string>::iterator it = _result.begin();
        std::map<std::string, std::string> result;
        for (; it != _result.end(); it++) {
          if ((*it).second.length() > 0)
            result[(*it).first] = (*it).second;
        }
        return result;
      }

      const AVOption* CodecPropertyTransformer::getOption(std::string name) {
        const AVOption * result = NULL;
        if (_result.count(name) > 0) {
          result = _options[name];
        } else
          if (_prop2parent.count(name) > 0) {
          LOGDEBUG("Properyt parent found : " << _prop2parent[name]);
          result = _options[_prop2parent[name]];
        } else {
          LOGWARN("Property \"" << name << "\" is unknown!");
        }
        return result;
      }

      std::string CodecPropertyTransformer::toString() {
        std::map<std::string, std::string>::iterator it = _result.begin();
        std::string result;
        for (; it != _result.end(); it++) {
          if ((*it).second.length() > 0)
            result += (*it).first + "=" + (*it).second + ";";
        }
        return result;
      }
    }
  }
}

