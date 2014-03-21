#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H


namespace org{
  namespace esb{
    namespace model{
      struct OutputFile{
          std::string jobid;
          std::string path;
          std::string encoder_data;
          std::string status;
      };

    }
  }
}

#endif // OUTPUTFILE_H
