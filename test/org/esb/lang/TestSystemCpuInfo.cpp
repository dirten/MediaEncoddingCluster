/* 
 * File:   TestSystemCpuInfo.cpp
 * Author: jhoelscher
 *
 * Created on 15. Dezember 2009, 17:19
 */

#include <stdlib.h>

#include "org/esb/lang/SystemCpuInfo.h"
#include "org/esb/util/Log.h"

/*
 * 
 */
int main(int argc, char** argv) {

  /**
   *         info->family=atoi(std::string("cpu family").c_str());
        info->model=atoi(std::string("model").c_str());
        info->step=atoi(std::string("stepping").c_str());
        info->cores=atoi(std::string("cpu cores").c_str());

   */
  org::esb::lang::SystemCpuInfo sci;
  int c = sci.getCpuCount();
  LOGDEBUG("CpuCount:" << c);
  for (int i = 0; i < c; i++) {
    LOGDEBUG(i<<"Cpu family" << sci.getCpuInfo(i).family);
    LOGDEBUG(i<<"Cpu model" << sci.getCpuInfo(i).model);
    LOGDEBUG(i<<"Cpu step" << sci.getCpuInfo(i).step);
    LOGDEBUG(i<<"Cpu cores" << sci.getCpuInfo(i).cores);
    LOGDEBUG(i<<"Cpu freq" << sci.getCpuInfo(i).freq);
  }
  return (EXIT_SUCCESS);
}

