#include "../SystemCpuInfo.h"

namespace org {
  namespace esb {
    namespace lang {

      SystemCpuInfo::~SystemCpuInfo() {
      }

      SystemCpuInfo::SystemCpuInfo() {
      }

      int SystemCpuInfo::getCpuCount() {
		  return 0;
      }

      CpuInfo SystemCpuInfo::getCpuInfo(unsigned int cpu_num) {
		  return CpuInfo();
      }

    }
  }
}
