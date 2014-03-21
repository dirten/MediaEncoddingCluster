#ifndef JOB_H
#define JOB_H

namespace org{
  namespace esb{
    namespace model{

      struct Job{
          std::string uuid;
          std::string outputfile;
          std::string graph;
          std::string status;
      };
    }
  }
}

#endif // JOB_H
