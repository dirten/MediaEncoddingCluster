/* 
 * File:   ProfileGroupTest.cpp
 * Author: HoelscJ
 *
 * Created on 8. September 2010, 16:36
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::config::Config::setProperty("db.url", "host=localhost;user=root;port=3306;database=example");

  org::esb::hive::DatabaseService::start(MEC_SOURCE_DIR);
  if (org::esb::hive::DatabaseService::databaseExist()) {
    org::esb::hive::DatabaseService::dropDatabase();
  }
  org::esb::hive::DatabaseService::createDatabase();
  org::esb::hive::DatabaseService::createTables();

  {

    db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
    db::ProfileGroup root(db);
    root.name = "root group";
    root.update();

    db::ProfileGroup child1(db);
    db::ProfileGroup child2(db);
    db::ProfileGroup child3(db);
    db::ProfileGroup child4(db);
    db::ProfileGroup child5(db);

    child1.name="child1";
    child2.name="child2";
    child3.name="child3";
    child4.name="child4";
    child5.name="child5";


    child1.update();
    child2.update();
    child3.update();
    child4.update();
    child5.update();


    root.childrens().link(child1);
    root.childrens().link(child2);
    root.childrens().link(child3);

    child3.childrens().link(child4);
    child3.childrens().link(child5);

    db::Profile p1(db);
    db::Profile p2(db);
    db::Profile p3(db);
    db::Profile p4(db);
    p1.name="testprofile1";
    p2.name="testprofile2";
    p3.name="testprofile3";
    p4.name="testprofile4";

    p1.update();
    p2.update();
    p3.update();
    p4.update();

    child1.profiles().link(p1);
    child2.profiles().link(p1);

    child4.profiles().link(p2);
    child5.profiles().link(p3);
    child5.profiles().link(p4);

  }
  {

    db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
    db::ProfileGroup root=litesql::select<db::ProfileGroup>(db,db::ProfileGroup::Name=="root group").one();
    vector<db::ProfileGroup> childs=root.childrens().get().all();
    assert(childs.size()==3);

    assert(childs[0].name=="child1");
    assert(childs[1].name=="child2");
    assert(childs[2].name=="child3");

    vector<db::ProfileGroup> childsfrom3=childs[2].childrens().get().all();
    vector<db::ProfileGroup>::iterator it=childsfrom3.begin();

    assert(childsfrom3.size()==2);
    assert(childsfrom3[0].name=="child4");
    assert(childsfrom3[1].name=="child5");

    assert(childsfrom3[0].parent().get().one().name=="child3");
    assert(childsfrom3[1].parent().get().one().name=="child3");
    assert(childsfrom3[1].parent().get().one().parent().get().one().name=="root group");

    assert(childs[0].profiles().get().one().name=="testprofile1");
    assert(childs[1].profiles().get().one().name=="testprofile1");
    assert(childsfrom3[0].profiles().get().one().name=="testprofile2");
    vector<db::Profile> profiles=childsfrom3[1].profiles().get().all();
    assert(profiles.size()==2);
    assert(profiles[0].name=="testprofile3");
    assert(profiles[1].name=="testprofile4");


  }
  org::esb::hive::DatabaseService::dropDatabase();
  org::esb::hive::DatabaseService::stop();

  Log::close();
  org::esb::config::Config::close();
  return 0;
}

