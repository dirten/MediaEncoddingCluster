#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H


namespace org{
  namespace esb{
    namespace model{
      struct OutputFile{
          std::string jobid;
          std::string uuid;
          std::string path;
          std::string filename;
          std::string encoder_data;
          std::string status;
      };

    }
  }
}

#endif // OUTPUTFILE_H
