#include "ProcessUnitList.h"
#include "org/esb/config/config.h"
#include "org/esb/util/UUID.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
namespace encodingtask{

  ProcessUnitList::ProcessUnitList()
  {
    lastSequence=0;
    _user_data_path=org::esb::config::Config::get("hive.data_path")+"/unitlist";
    org::esb::io::File data_dir(_user_data_path+"/");
    if(!data_dir.exists()){
      data_dir.mkdirs();
    }
  }

  void ProcessUnitList::pushUnit(boost::shared_ptr<org::esb::hive::job::ProcessUnit> unit)
  {
    //_unit_list.insert(unit);
    std::string uuid=org::esb::util::PUUID();
    _unit_map[unit->_sequence]=uuid;


    org::esb::io::File file(_user_data_path+"/"+uuid);
    org::esb::io::FileOutputStream fos(&file);
    org::esb::io::ObjectOutputStream ous(&fos);
    ous.writeObject(unit);
    ous.close();


    while(_unit_map.size()&&(*_unit_map.begin()).first==lastSequence){
      std::cerr<<"lastSequence found:"<<lastSequence<<std::endl;
      std::string nuuid=(*_unit_map.begin()).second;
      org::esb::io::File infile(_user_data_path+"/"+nuuid);
      org::esb::io::FileInputStream infos(&infile);
      org::esb::io::ObjectInputStream inous(&infos);
      inous.readObject(unit);
      inous.close();
      infos.close();
      infile.deleteFile();
      if(_func)
        _func(unit);
      /*foreach(boost::shared_ptr<Packet>  p,(*_unit_list.begin())->_output_packets){
        Task::pushBuffer(Ptr<Packet>(p));
      }*/
      _unit_map.erase(_unit_map.begin());
      lastSequence++;
    }
  }

  void ProcessUnitList::addCallback(boost::function<void (boost::shared_ptr<org::esb::hive::job::ProcessUnit>unit)> func)
  {
      _func=func;
  }

//  void ProcessUnitList::writeFile(boost::shared_ptr<org::esb::hive::job::ProcessUnit>, string uuid)
//  {
//  }

//  boost::shared_ptr<org::esb::hive::job::ProcessUnit> ProcessUnitList::readFile(string uuid)
//  {
//  }
}
