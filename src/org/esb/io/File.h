#ifndef ORG_ESB_IO_FILE_H
#define ORG_ESB_IO_FILE_H
namespace org{
    namespace esb{
	namespace io{
	    class File{
		public:
		    File(const char * filename);
		    File();
		    ~File();
		    const char * getPath();
		    bool canRead();
		    bool canWrite();
		    bool exist();
		    bool isFile();
		    bool isDirectory();
		private:
		    const char * _filename;
	    };
	}
    }
}
#endif
