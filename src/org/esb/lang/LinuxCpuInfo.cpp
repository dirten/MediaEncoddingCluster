#include "SystemCpuInfo.h"

#include "org/esb/util/Log.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"

#include <iostream>
#include <fstream>
#include <string.h>
namespace org {
  namespace esb {
    namespace lang {

      std::string read_proc_fs(std::string filename) {
        std::ifstream is;
        is.open(filename.c_str(), std::ifstream::in);
        std::string cpudata;
        while (!is.eof()) {
          char buffer[101];
          memset(&buffer, 0, sizeof (buffer));
          is.read(buffer, 100);
          cpudata.append(buffer);
        }
        is.close();
        return cpudata;
      }

      SystemCpuInfo::~SystemCpuInfo() {
        std::vector<CpuInfo*>::iterator it=_processors.begin();
        for(;it!=_processors.end();it++)
          delete (*it);
      }

      SystemCpuInfo::SystemCpuInfo() {
        LOGDEBUG("SystemInfo");
        std::string cpudata = read_proc_fs("/proc/cpuinfo");
        parseCpuInfo(cpudata);
        Log::close();
      }

      int SystemCpuInfo::getCpuCount() {
        return _processors.size();
      }

      CpuInfo SystemCpuInfo::getCpuInfo(int cpu_num) {
        if (cpu_num > _processors.size())
          LOGINFO("no cpu fond for number " << cpu_num);
        return *_processors[cpu_num];
      }

      void SystemCpuInfo::parseCpuInfo(std::string& data) {
        int start = -1;
        int last_start = -1;
        bool hasMoreProcessor = false;
        while (hasMoreProcessor) {
          start = data.find("processor", start + 1);
          if (start == -1)break;
          last_start = start;
          int end = data.find("processor", start + 1);
          parseProcessor(data.substr(start, end - start));
        }
      }

      void SystemCpuInfo::parseProcessor(std::string data) {
        std::map<std::string, std::string> datamap = this->parseLines(data);
        CpuInfo *info = new CpuInfo();
        info->num = atoi(std::string("processor").c_str());
        info->freq = atof(datamap[std::string("cpu MHz")].c_str());
        info->family=atoi(datamap[std::string("cpu family")].c_str());
        info->model=atoi(datamap[std::string("model")].c_str());
        info->step=atoi(datamap[std::string("stepping")].c_str());
        info->cores=atoi(datamap[std::string("cpu cores")].c_str());
        _processors.push_back(info);
      }

      std::map<std::string, std::string> SystemCpuInfo::parseLines(std::string data) {
        std::map<std::string, std::string> result;
        org::esb::util::StringTokenizer t1(data, "\n");
        while (t1.hasMoreTokens()) {
          std::string line = t1.nextToken();
          org::esb::util::StringTokenizer t2(line, ":");
          if (!t2.countTokens() == 2)continue;
          std::string key = org::esb::util::StringUtil::trim(t2.nextToken(), "\t ");
          std::string val = org::esb::util::StringUtil::trim(t2.nextToken());
          result[key] = val;
        }
        return result;
      }
    }
  }
}
