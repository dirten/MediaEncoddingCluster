#ifndef JOB_H
#define JOB_H

namespace org{
  namespace esb{
    namespace model{

      struct Job{
          std::string uuid;
          std::string outputfile;
          std::string graph;
          std::string graphstatus;
          std::string status;
          time_t created;
          time_t begintime;
          time_t endtime;
          int8_t progress;
          std::string infile;
          std::string outfile;
          std::string graphname;
      };
    }
  }
}

#endif // JOB_H
