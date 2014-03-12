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

    org::esb::rpc::ProcessService::Stub nodeS(&c);
    org::esb::rpc::Node node;

    org::esb::rpc::Process proc;
    org::esb::rpc::ProcessRequest pcreq;
    org::esb::rpc::ProcessResponse pcres;

    org::esb::rpc::Controller ctrl;


    node.set_address("127.0.0.1");
    node.set_name("test node");
    *pcreq.mutable_node() = node;
    proc.set_process_name("core");
    *pcreq.mutable_process_data() = proc;
    if(argc>1&&strcmp(argv[1],"stop")==0){
      nodeS.stopProcess(&ctrl, &pcreq, &pcres, NULL);
    }else{
      nodeS.startProcess(&ctrl, &pcreq, &pcres, NULL);
    }
    if (ctrl.Failed()) {
      LOGERROR("Service request failed:" << ctrl.ErrorText());
      //break;
    }

  }
  //Log::close();
  return 0;
}

