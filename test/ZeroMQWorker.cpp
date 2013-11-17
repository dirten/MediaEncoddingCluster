
#include "zmq.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
int main (int argc, char *argv[])
{
    zmq::context_t context(1);

    //  Socket to receive messages on
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.connect("tcp://localhost:5557");

    //  Socket to send messages to
    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.connect("tcp://localhost:5558");

    //  Process tasks forever
    while (1) {
        zmq::message_t message;
        int workload;           //  Workload in msecs

        receiver.recv(&message);
        std::cout << "message received"<<std::flush;
        std::istringstream iss(static_cast<char*>(message.data()));
        iss >> workload;
        std::cout << "wait " << workload <<std::flush;

        //  Do the work
        sleep(workload);

        //  Send results to sink
        message.rebuild();
        sender.send(message);

        //  Simple progress indicator for the viewer
        std::cout << "." << std::flush;
    }
    return 0;
}
