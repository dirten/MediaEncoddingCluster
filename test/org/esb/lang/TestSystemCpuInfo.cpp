/* 
 * File:   TestSystemCpuInfo.cpp
 * Author: jhoelscher
 *
 * Created on 15. Dezember 2009, 17:19
 */

#include <stdlib.h>

#include "org/esb/util/Log.h"
#include "org/esb/lang/SystemCpuInfo.h"

/*
 * 
 */

int main(int argc, char** argv) {

  Log::open("");

  /**
   *         info->family=atoi(std::string("cpu family").c_str());
        info->model=atoi(std::string("model").c_str());
        info->step=atoi(std::string("stepping").c_str());
        info->cores=atoi(std::string("cpu cores").c_str());

   */
  org::esb::lang::SystemCpuInfo sci;
  int c = sci.getCpuCount();
  std::cout<<"CpuCount:" << c<<std::endl;
  for (int i = 0; i < c; i++) {
    std::cout<<i<<"Cpu family" << sci.getCpuInfo(i).family<<std::endl;
    std::cout<<i<<"Cpu model" << sci.getCpuInfo(i).model<<std::endl;
    std::cout<<i<<"Cpu step" << sci.getCpuInfo(i).step<<std::endl;
    std::cout<<i<<"Cpu cores" << sci.getCpuInfo(i).cores<<std::endl;
    std::cout<<i<<"Cpu freq" << sci.getCpuInfo(i).freq<<std::endl;
  }
  return (EXIT_SUCCESS);
}

