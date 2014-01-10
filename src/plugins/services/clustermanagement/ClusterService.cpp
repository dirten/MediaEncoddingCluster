#include "ClusterService.h"

#include "thrift/TProcessor.h"
#include "thrift/server/TSimpleServer.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/transport/TServerTransport.h"
#include "thrift/transport/TServerSocket.h"
#include "thrift/transport/THttpServer.h"
#include "thrift/transport/TTransportUtils.h"
#include "thrift/protocol/TProtocol.h"
#include "thrift/protocol/TJSONProtocol.h"
#include "thrift/protocol/TBinaryProtocol.h"

#include "org/esb/thrift/proto_types.h"
#include "org/esb/thrift/proto_constants.h"
#include "org/esb/thrift/Calculator.h"
#include "org/esb/lang/Ptr.h"


namespace plugin{
  using apache::thrift::TProcessor;
  using apache::thrift::server::TSimpleServer;
  using apache::thrift::transport::TBufferedTransport;
  using apache::thrift::transport::TServerTransport;
  using apache::thrift::transport::TServerSocket;
  using apache::thrift::transport::THttpServerTransportFactory;
  using apache::thrift::transport::TTransportFactory;
  using apache::thrift::protocol::TProtocolFactory;
  using apache::thrift::protocol::TBinaryProtocolFactory;
  using apache::thrift::protocol::TJSONProtocolFactory;
  using apache::thrift::transport::TBufferedTransportFactory;

  class CalculatorHandler : virtual public CalculatorIf
  {
    public:
      CalculatorHandler(){

      }
      int32_t add(int32_t a, int32_t b){
        return a+b;
      }
  };

  ClusterService::ClusterService()
  {
  }
  void ClusterService::startService()
  {
    run();
  }

  void ClusterService::stopService()
  {

  }

  void ClusterService::run()
  {
    int port = 9090;
    Ptr<CalculatorHandler>handler(new CalculatorHandler());
    Ptr<TProcessor> processor(new CalculatorProcessor(handler));
    Ptr<TServerTransport> serverTransport(new TServerSocket(port));

    //Ptr<TServerTransport> httpTransport(new THttpServer(serverTransport));
    //Ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());

    Ptr<TTransportFactory> transportFactory(new THttpServerTransportFactory());
    //Ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    Ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
  }

  org::esb::core::OptionsDescription ClusterService::getOptionsDescription()
  {
    org::esb::core::OptionsDescription result("auservice");

    return result;
  }

}
