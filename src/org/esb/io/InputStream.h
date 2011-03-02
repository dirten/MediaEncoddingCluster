#ifndef ORG_ESB_IO_INPUTSTREAM
#define ORG_ESB_IO_INPUTSTREAM
#include <iostream>
#include <vector>
#include "org/esb/lang/Byte.h"
using namespace std;
using namespace org::esb::lang;
namespace org {
    namespace esb {
        namespace io {
            /**
             * This abstract class is the superclass of all classes representing
             * an input stream of bytes.
             *
             * <p> Applications that need to define a subclass of <code>InputStream</code>
             * must always provide a method that returns the next byte of input.
             *
             * @author  Jan Hoelscher
             * @version 1.45, 02/19/04
             * @see     BufferedInputStream
             * @see     ByteArrayInputStream
             * @see     DataInputStream
             * @see     FilterInputStream
             * @see     InputStream#read()
             * @see     OutputStream
             * @see     PushbackInputStream
             * @see     FileInputStream
             * @since   ESB-LIB-1.0.1
             */
            class InputStream {
public:

                /**
                 * Reads some number of bytes from the input stream and stores them into
                 * the buffer array <code>b</code>. The number of bytes actually read is
                 * returned as an integer.  This method blocks until input data is
                 * available, end of file is detected, or an exception is thrown.
                 *
                 * <p> If <code>b</code> is <code>null</code>, a
                 * <code>NullPointerException</code> is thrown.  If the length of
                 * <code>b</code> is zero, then no bytes are read and <code>0</code> is
                 * returned; otherwise, there is an attempt to read at least one byte. If
                 * no byte is available because the stream is at end of file, the value
                 * <code>-1</code> is returned; otherwise, at least one byte is read and
                 * stored into <code>b</code>.
                 *
                 * <p> The first byte read is stored into element <code>b[0]</code>, the
                 * next one into <code>b[1]</code>, and so on. The number of bytes read is,
                 * at most, equal to the length of <code>b</code>. Let <i>k</i> be the
                 * number of bytes actually read; these bytes will be stored in elements
                 * <code>b[0]</code> through <code>b[</code><i>k</i><code>-1]</code>,
                 * leaving elements <code>b[</code><i>k</i><code>]</code> through
                 * <code>b[b.length-1]</code> unaffected.
                 *
                 * <p> If the first byte cannot be read for any reason other than end of
                 * file, then an <code>IOException</code> is thrown. In particular, an
                 * <code>IOException</code> is thrown if the input stream has been closed.
                 *
                 * <p> The <code>read(b)</code> method for class <code>InputStream</code>
                 * has the same effect as: <pre><code> read(b, 0, b.length) </code></pre>
                 *
                 * @param      b   the buffer into which the data is read.
                 * @return     the total number of bytes read into the buffer, or
                 *             <code>-1</code> is there is no more data because the end of
                 *             the stream has been reached.
                 * @exception  IOException  if an I/O error occurs.
                 * @exception  NullPointerException  if <code>b</code> is <code>null</code>.
                 * @see        org::esb::io::InputStream#read(byte[], int, int)
                 */
                virtual int read(unsigned char * buffer, int length)=0;
                virtual int read(vector<unsigned char>&buffer)=0;
                virtual int read(string & str){std::cout << "InputStream::read not implemented!!!";return 0;}
                virtual int readUntil(string & str, std::string delim){std::cout << "InputStream::read not implemented!!!";return 0;}
                virtual int read()=0;

                /**
                 * Returns the number of bytes that can be read (or skipped over) from
                 * this input stream without blocking by the next caller of a method for
                 * this input stream.  The next caller might be the same thread or
                 * another thread.
                 *
                 * <p> The <code>available</code> method for class <code>InputStream</code>
                 * always returns <code>0</code>.
                 *
                 * <p> This method should be overridden by subclasses.
                 *
                 * @return     the number of bytes that can be read from this input stream
                 *             without blocking.
                 * @exception  IOException  if an I/O error occurs.
                 */
                virtual long long int available(bool isBlocking=false)=0;

                /**
                 * Closes this input stream and releases any system resources associated
                 * with the stream.
                 *
                 * <p> The <code>close</code> method of <code>InputStream</code> does
                 * nothing.
                 *
                 * @exception  IOException  if an I/O error occurs.
                 */
                virtual void close() {}
                ;

                /**
                 * Marks the current position in this input stream. A subsequent call to
                 * the <code>reset</code> method repositions this stream at the last marked
                 * position so that subsequent reads re-read the same bytes.
                 *
                 * <p> The <code>readlimit</code> arguments tells this input stream to
                 * allow that many bytes to be read before the mark position gets
                 * invalidated.
                 *
                 * <p> The general contract of <code>mark</code> is that, if the method
                 * <code>markSupported</code> returns <code>true</code>, the stream somehow
                 * remembers all the bytes read after the call to <code>mark</code> and
                 * stands ready to supply those same bytes again if and whenever the method
                 * <code>reset</code> is called.  However, the stream is not required to
                 * remember any data at all if more than <code>readlimit</code> bytes are
                 * read from the stream before <code>reset</code> is called.
                 *
                 * <p> The <code>mark</code> method of <code>InputStream</code> does
                 * nothing.
                 *
                 * @param   readlimit   the maximum limit of bytes that can be read before
                 *                      the mark position becomes invalid.
                 * @see     java.io.InputStream#reset()
                 */
                virtual void mark(int readlimit) {}
                ;

                /**
                 * Tests if this input stream supports the <code>mark</code> and
                 * <code>reset</code> methods. Whether or not <code>mark</code> and
                 * <code>reset</code> are supported is an invariant property of a
                 * particular input stream instance. The <code>markSupported</code> method
                 * of <code>InputStream</code> returns <code>false</code>.
                 *
                 * @return  <code>true</code> if this stream instance supports the mark
                 *          and reset methods; <code>false</code> otherwise.
                 * @see     java.io.InputStream#mark(int)
                 * @see     java.io.InputStream#reset()
                 */
                virtual bool markSupported() {return false;}
                ;

                /**
                 * Repositions this stream to the position at the time the
                 * <code>mark</code> method was last called on this input stream.
                 *
                 * <p> The general contract of <code>reset</code> is:
                 *
                 * <p><ul>
                 *
                 * <li> If the method <code>markSupported</code> returns
                 * <code>true</code>, then:
                 *
                 *     <ul><li> If the method <code>mark</code> has not been called since
                 *     the stream was created, or the number of bytes read from the stream
                 *     since <code>mark</code> was last called is larger than the argument
                 *     to <code>mark</code> at that last call, then an
                 *     <code>IOException</code> might be thrown.
                 *
                 *     <li> If such an <code>IOException</code> is not thrown, then the
                 *     stream is reset to a state such that all the bytes read since the
                 *     most recent call to <code>mark</code> (or since the start of the
                 *     file, if <code>mark</code> has not been called) will be resupplied
                 *     to subsequent callers of the <code>read</code> method, followed by
                 *     any bytes that otherwise would have been the next input data as of
                 *     the time of the call to <code>reset</code>. </ul>
                 *
                 * <li> If the method <code>markSupported</code> returns
                 * <code>false</code>, then:
                 *
                 *     <ul><li> The call to <code>reset</code> may throw an
                 *     <code>IOException</code>.
                 *
                 *     <li> If an <code>IOException</code> is not thrown, then the stream
                 *     is reset to a fixed state that depends on the particular type of the
                 *     input stream and how it was created. The bytes that will be supplied
                 *     to subsequent callers of the <code>read</code> method depend on the
                 *     particular type of the input stream. </ul></ul>
                 *
                 * <p>The method <code>reset</code> for class <code>InputStream</code>
                 * does nothing except throw an <code>IOException</code>.
                 *
                 * @exception  IOException  if this stream has not been marked or if the
                 *               mark has been invalidated.
                 * @see     java.io.InputStream#mark(int)
                 * @see     java.io.IOException
                 */
                virtual void reset() {}
                ;

                /**
                 * Skips over and discards <code>n</code> bytes of data from this input
                 * stream. The <code>skip</code> method may, for a variety of reasons, end
                 * up skipping over some smaller number of bytes, possibly <code>0</code>.
                 * This may result from any of a number of conditions; reaching end of file
                 * before <code>n</code> bytes have been skipped is only one possibility.
                 * The actual number of bytes skipped is returned.  If <code>n</code> is
                 * negative, no bytes are skipped.
                 *
                 * <p> The <code>skip</code> method of <code>InputStream</code> creates a
                 * byte array and then repeatedly reads into it until <code>n</code> bytes
                 * have been read or the end of the stream has been reached. Subclasses are
                 * encouraged to provide a more efficient implementation of this method.
                 *
                 * @param      n   the number of bytes to be skipped.
                 * @return     the actual number of bytes skipped.
                 * @exception  IOException  if an I/O error occurs.
                 */
                virtual void skip(long bytes) {}
                ;

                virtual ~InputStream() {}
                ;
            };
        }
    }
}
#endif



