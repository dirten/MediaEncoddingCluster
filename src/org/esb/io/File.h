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
		    bool createNewFile();
		    bool deleteFile();
		    long length();
		    bool mkDir();
		    bool mkDirs();
		    bool renameTo(File * dest);
		    bool setLastModified(long date);
		    long getLastModified();
		    bool setReadOnly();
		private:
		    const char * _filename;
	    };
	}
    }
}
#endif
