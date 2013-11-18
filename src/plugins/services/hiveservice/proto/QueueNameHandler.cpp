#include "QueueNameHandler.h"
#include <string.h>

QueueNameHandler::QueueNameHandler(InputStream * is, OutputStream * os, boost::asio::ip::tcp::endpoint e)
{

}

int QueueNameHandler::isResponsible(char * command) {
  if (
  strcmp(command, GET_QUEUE_NAME) == 0) {
    return CMD_PROCESS;
  } else
    if (strcmp(command, "help") == 0) {
      return CMD_HELP;
    }
  return CMD_NA;
}
