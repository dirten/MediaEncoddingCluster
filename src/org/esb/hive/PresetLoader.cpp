/* 
 * File:   PresetToDB.cpp
 * Author: HoelscJ
 * 
 * Created on 26. Oktober 2010, 11:08
 */
#include "PresetLoader.h"
#include "DatabaseService.h"
namespace org {
  namespace esb {
    namespace hive {

      PresetLoader::PresetLoader(PresetReader & preset) : _preset(preset) {

      }

      void PresetLoader::load() {
        db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
        db.begin();
        org::esb::hive::PresetReader::Preset p = _preset.getPreset();

        db::Profile profile(db);
        profile.name = p["name"];

        profile.update();
        std::map<std::string, std::string> preset_parameter;
        preset_parameter["file_format"] = "id";

        std::map<std::string, std::string>::iterator it=preset_parameter.begin();
        for(;it!=preset_parameter.end();it++){
          setParameter(profile,(*it).first,p[(*it).second]);
          p.erase((*it).second);
        }
        it=p.begin();
        for(;it!=p.end();it++){
          setParameter(profile,(*it).first,(*it).second);
        }
        PresetReader::CodecList clist=_preset.getCodecList();
        
        it=clist["video"].begin();
        for(;it!=clist["video"].end();it++){
          setParameter(profile,(*it).first,(*it).second);
        }
        it=clist["audio"].begin();
        for(;it!=clist["audio"].end();it++){
          setParameter(profile,(*it).first,(*it).second);
        }
        PresetReader::FilterList flist=_preset.getFilterList();
        PresetReader::FilterList::iterator flist_it=flist.begin();
        std::map<std::string, std::string> filter_name;
        filter_name["resize"]="Resize Filter";
        filter_name["deinterlace"]="Deinterlace Filter";
        for(;flist_it!=flist.end();flist_it++){
          db::Filter filter(db);
          filter.filterid=(*flist_it).first;
          filter.filtername=filter_name[filter.filterid];
          filter.update();
          profile.filter().link(filter);
          std::map<std::string, std::string>::iterator param=(*flist_it).second.begin();
          for(;param!=(*flist_it).second.end();param++){
            db::FilterParameter fp(db);
            fp.fkey=(*param).first;
            fp.fval=(*param).second;
            fp.update();
            filter.parameter().link(fp);
          }
        }
        db.commit();
      }

      PresetLoader::~PresetLoader() {

      }

      void PresetLoader::setParameter(db::Profile p, std::string key, std::string val) {
          db::ProfileParameter param(p.getDatabase());
          param.name = key;
          param.val = val;
          param.update();
          p.params().link(param);
      }
    }
  }
}
