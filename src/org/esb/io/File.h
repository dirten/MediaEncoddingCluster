#ifndef ORG_ESB_IO_FILE_H
#define ORG_ESB_IO_FILE_H
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include "org/esb/util/Log.h"

#include "FileFilter.h"
namespace org {
  namespace esb {
    namespace io {
      typedef std::list<boost::shared_ptr<File> > FileList;

      class File {
      public:
        /**
         * Creates a new <code>File</code> instance by converting the given
         * pathname string into an abstract pathname.  If the given string is
         * the empty string, then the result is the empty abstract pathname.
         *
         * @param   pathname  A pathname string
         * @throws  NullPointerException
         *          If the <code>pathname</code> argument is <code>null</code>
         */
        File(const char * filename);
        ~File();
        /**
         * Converts this abstract pathname into a pathname string.  The resulting
         * string uses the {@link #separator default name-separator character} to
         * separate the names in the name sequence.
         *
         * @return  The string form of this abstract pathname
         */

        const std::string getPath();
        const std::string getFileName();
        const std::string getFilePath();
        /**
         * Tests whether the application can read the file denoted by this
         * abstract pathname.
         *
         * @return  <code>true</code> if and only if the file specified by this
         *          abstract pathname exists <em>and</em> can be read by the
         *          application; <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
         *          method denies read access to the file
         */

        bool canRead();
        /**
         * Tests whether the application can modify the file denoted by this
         * abstract pathname.
         *
         * @return  <code>true</code> if and only if the file system actually
         *          contains a file denoted by this abstract pathname <em>and</em>
         *          the application is allowed to write to the file;
         *          <code>false</code> otherwise.
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
         *          method denies write access to the file
         */

        bool canWrite();
        /**
         * Tests whether the file or directory denoted by this abstract pathname
         * exists.
         *
         * @return  <code>true</code> if and only if the file or directory denoted
         *          by this abstract pathname exists; <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
         *          method denies read access to the file or directory
         */

        bool exists();
        /**
         * Tests whether the file denoted by this abstract pathname is a normal
         * file.  A file is <em>normal</em> if it is not a directory and, in
         * addition, satisfies other system-dependent criteria.  Any non-directory
         * file created by an application is guaranteed to be a normal file.
         *
         * @return  <code>true</code> if and only if the file denoted by this
         *          abstract pathname exists <em>and</em> is a normal file;
         *          <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
         *          method denies read access to the file
         */

        bool isFile();
        /**
         * Tests whether the file denoted by this abstract pathname is a
         * directory.
         *
         * @return <code>true</code> if and only if the file denoted by this
         *          abstract pathname exists <em>and</em> is a directory;
         *          <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
         *          method denies read access to the file
         */

        bool isDirectory();
        /**
         * Atomically creates a new, empty file named by this abstract pathname if
         * and only if a file with this name does not yet exist.  The check for the
         * existence of the file and the creation of the file if it does not exist
         * are a single operation that is atomic with respect to all other
         * filesystem activities that might affect the file.
         * <P>
         * Note: this method should <i>not</i> be used for file-locking, as
         * the resulting protocol cannot be made to work reliably. The 
         * {@link java.nio.channels.FileLock FileLock}
         * facility should be used instead. 
         *
         * @return  <code>true</code> if the named file does not exist and was
         *          successfully created; <code>false</code> if the named file
         *          already exists
         *
         * @throws  IOException
         *          If an I/O error occurred
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
         *          method denies write access to the file
         *
         * @since 1.2
         */

        bool createNewFile();
        /**
         * Deletes the file or directory denoted by this abstract pathname.  If
         * this pathname denotes a directory, then the directory must be empty in
         * order to be deleted.
         *
         * @return  <code>true</code> if and only if the file or directory is
         *          successfully deleted; <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkDelete}</code> method denies
         *          delete access to the file
         */

        bool deleteFile();
        /**
         * Returns the length of the file denoted by this abstract pathname.
         * The return value is unspecified if this pathname denotes a directory.
         *
         * @return  The length, in bytes, of the file denoted by this abstract
         *          pathname, or <code>0L</code> if the file does not exist
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
         *          method denies read access to the file
         */

        long length();
        /**
         * Creates the directory named by this abstract pathname.
         *
         * @return  <code>true</code> if and only if the directory was
         *          created; <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
         *          method does not permit the named directory to be created
         */

        bool mkdir();
        /**
         * Creates the directory named by this abstract pathname, including any
         * necessary but nonexistent parent directories.  Note that if this
         * operation fails it may have succeeded in creating some of the necessary
         * parent directories.
         *
         * @return  <code>true</code> if and only if the directory was created,
         *          along with all necessary parent directories; <code>false</code>
         *          otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkRead(java.lang.String)}</code>
         *          method does not permit verification of the existence of the 
         *          named directory and all necessary parent directories; or if
         *          the <code>{@link 
         *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
         *          method does not permit the named directory and all necessary
         *          parent directories to be created
         */

        bool mkdirs();
        /**
         * Renames the file denoted by this abstract pathname.
         * 
         * <p> Many aspects of the behavior of this method are inherently
         * platform-dependent: The rename operation might not be able to move a
         * file from one filesystem to another, it might not be atomic, and it
         * might not succeed if a file with the destination abstract pathname
         * already exists.  The return value should always be checked to make sure
         * that the rename operation was successful.
         *
         * @param  dest  The new abstract pathname for the named file
         * 
         * @return  <code>true</code> if and only if the renaming succeeded;
         *          <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
         *          method denies write access to either the old or new pathnames
         * 
         * @throws  NullPointerException  
         *          If parameter <code>dest</code> is <code>null</code>
         */

        bool renameTo(File * dest);
        /**
         * Sets the last-modified time of the file or directory named by this
         * abstract pathname.
         *
         * <p> All platforms support file-modification times to the nearest second,
         * but some provide more precision.  The argument will be truncated to fit
         * the supported precision.  If the operation succeeds and no intervening
         * operations on the file take place, then the next invocation of the
         * <code>{@link #lastModified}</code> method will return the (possibly
         * truncated) <code>time</code> argument that was passed to this method.
         *
         * @param  time  The new last-modified time, measured in milliseconds since
         *               the epoch (00:00:00 GMT, January 1, 1970)
         *
         * @return <code>true</code> if and only if the operation succeeded;
         *          <code>false</code> otherwise
         *
         * @throws  IllegalArgumentException  If the argument is negative
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
         *          method denies write access to the named file
         *
         * @since 1.2
         */

        bool setLastModified(long date);

        long getLastModified();
        /**
         * Marks the file or directory named by this abstract pathname so that
         * only read operations are allowed.  After invoking this method the file
         * or directory is guaranteed not to change until it is either deleted or
         * marked to allow write access.  Whether or not a read-only file or
         * directory may be deleted depends upon the underlying system.
         *
         * @return <code>true</code> if and only if the operation succeeded;
         *          <code>false</code> otherwise
         *
         * @throws  SecurityException
         *          If a security manager exists and its <code>{@link
         *          java.lang.SecurityManager#checkWrite(java.lang.String)}</code>
         *          method denies write access to the named file
         *
         * @since 1.2
         */

        bool setReadOnly();


        FileList listFiles();
        FileList listFiles(FileFilter & filter);
      private:
        //                const char * _filename;
        boost::filesystem::path _full_path;
        logger("io.file");
      };
    }
  }
}
#endif
