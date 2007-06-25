#ifndef ORG_ESB_LANG_BYTE
#define ORG_ESB_LANG_BYTE
#include <stdint.h>
namespace org{
namespace esb{
namespace lang{
class Byte {
    private:
	uint8_t _byte;
    public:
	~Byte();
    /**
     * Constructs a newly allocated <code>Byte</code> object that
     * represents the specified <code>byte</code> value.
     *
     * @param value	the value to be represented by the 
     *			<code>Byte</code>.
     */
	Byte(char byte);

    /**
     * Constructs a newly allocated <code>Byte</code> object that
     * represents the <code>byte</code> value indicated by the
     * <code>String</code> parameter. The string is converted to a
     * <code>byte</code> value in exactly the manner used by the
     * <code>parseByte</code> method for radix 10.
     *
     * @param s		the <code>String</code> to be converted to a 
     *			<code>Byte</code>
     * @exception	NumberFormatException If the <code>String</code> 
     *			does not contain a parsable <code>byte</code>.
     */
	Byte(char&bytes);
    /**
     * Returns the value of this <code>Byte</code> as a
     * <code>byte</code>.
     */
	uint8_t byteValue();

    /**
     * Returns the value of this <code>Byte</code> as a
     * <code>short</code>.
     */
	short shortValue();
    /**
     * Returns the value of this <code>Byte</code> as an
     * <code>int</code>.
     */
	int intValue();
    /**
     * Returns the value of this <code>Byte</code> as a
     * <code>long</code>.
     */
	long longValue();
    /**
     * Returns the value of this <code>Byte</code> as a
     * <code>double</code>.
     */
	double doubleValue();
    /**
     * Returns the value of this <code>Byte</code> as a
     * <code>float</code>.
     */
	float floatValue();
    /**
     * Returns a <code>String</code> object representing this
     * <code>Byte</code>'s value.  The value is converted to signed
     * decimal representation and returned as a string, exactly as if
     * the <code>byte</code> value were given as an argument to the
     * {@link java.lang.Byte#toString(byte)} method.
     *
     * @return  a string representation of the value of this object in
     *          base&nbsp;10.
     */
	char toString();
};

}}}
#endif


