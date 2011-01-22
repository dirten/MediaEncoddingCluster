/* 
 * File:   RpcClientTest.cpp
 * Author: HoelscJ
 *
 * Created on 12. Januar 2011, 20:29
 */

#include <cstdlib>
#include "org/esb/net/TcpSocket.h"
#include "org/esb/rpc/Client.h"
#include "org/esb/util/Log.h"
#include "org/esb/av/FormatBaseStream.h"

#include "org/esb/rpc/Channel.h"
#include "org/esb/rpc/rpc.pb.h"
#include "org/esb/rpc/Controller.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  org::esb::av::FormatBaseStream::initialize();
  //org::esb::rpc::Client c("localhost", 6000);

  org::esb::net::TcpSocket socket("localhost", 6000);
  try {
    socket.connect();
  } catch (std::exception & ex) {
    std::cout << ex.what() << std::endl;
  }
  if (socket.isConnected()) {
    org::esb::rpc::Channel c(&socket);

    org::esb::rpc::NodeService::Stub nodeS(&c);
    org::esb::rpc::Node node;
    org::esb::rpc::NodeList nodelist;


    org::esb::rpc::Controller ctrl;

    node.set_address("127.0.0.1");
    node.set_name("test node");
    if(argc>1&&strcmp(argv[1],"unregister")==0){
      if(argc>2)
        node.set_name(argv[2]);
      nodeS.unregisterNode(&ctrl, &node, &nodelist, NULL);
    }else{
      if(argc>1)
        node.set_name(argv[1]);
      nodeS.registerNode(&ctrl, &node, &nodelist, NULL);
    }
    if (ctrl.Failed()) {
      LOGERROR("Service request failed:" << ctrl.ErrorText());
      //break;
    }
    LOGDEBUG(nodelist.DebugString());

  }
  Log::close();
  return 0;
}

