#ifndef QUEUENAMEHANDLER_H
#define QUEUENAMEHANDLER_H

#include "../ProtocolCommand.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "boost/asio.hpp"

#define GET_QUEUE_NAME  "get queue_connection"

using org::esb::io::InputStream;
using org::esb::io::OutputStream;
using boost::asio::ip::tcp;

class QueueNameHandler : public org::esb::plugin::ProtocolCommand
{
  public:
    QueueNameHandler(InputStream * is, OutputStream * os, tcp::endpoint e);
    int isResponsible(char * command);
};

#endif // QUEUENAMEHANDLER_H
