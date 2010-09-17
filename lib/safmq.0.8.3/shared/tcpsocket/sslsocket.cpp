/*
 Copyright 2004-2008 Matthew J. Battey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software distributed
	under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied. See the License for the
	specific language governing permissions and limitations under the License.




This software implements a platform independent C++ interface to TCP/IP socket
communications.
*/
#include "sslsocket.h"

#ifdef _WIN32
	#define EWOULDBLOCK             WSAEWOULDBLOCK
	#define EINPROGRESS             WSAEINPROGRESS
	#define EALREADY                WSAEALREADY
	#define ENOTSOCK                WSAENOTSOCK
	#define EDESTADDRREQ            WSAEDESTADDRREQ
	#define EMSGSIZE                WSAEMSGSIZE
	#define EPROTOTYPE              WSAEPROTOTYPE
	#define ENOPROTOOPT             WSAENOPROTOOPT
	#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
	#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
	#define EOPNOTSUPP              WSAEOPNOTSUPP
	#define EPFNOSUPPORT            WSAEPFNOSUPPORT
	#define EAFNOSUPPORT            WSAEAFNOSUPPORT
	#define EADDRINUSE              WSAEADDRINUSE
	#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
	#define ENETDOWN                WSAENETDOWN
	#define ENETUNREACH             WSAENETUNREACH
	#define ENETRESET               WSAENETRESET
	#define ECONNABORTED            WSAECONNABORTED
	#define ECONNRESET              WSAECONNRESET
	#define ENOBUFS                 WSAENOBUFS
	#define EISCONN                 WSAEISCONN
	#define ENOTCONN                WSAENOTCONN
	#define ESHUTDOWN               WSAESHUTDOWN
	#define ETOOMANYREFS            WSAETOOMANYREFS
	#define ETIMEDOUT               WSAETIMEDOUT
	#define ECONNREFUSED            WSAECONNREFUSED
	#define ELOOP                   WSAELOOP
	#define EHOSTDOWN               WSAEHOSTDOWN
	#define EHOSTUNREACH            WSAEHOSTUNREACH
	#define EPROCLIM                WSAEPROCLIM
	#define EUSERS                  WSAEUSERS
	#define EDQUOT                  WSAEDQUOT
	#define ESTALE                  WSAESTALE
	#define EREMOTE                 WSAEREMOTE
	typedef int socklen_t;
	#define SOCERRNO				h_errno

	#ifdef min
		#undef min
		#define min _cpp_min
	#endif
	#ifdef max
		#undef max
		#define max _cpp_max
	#endif


#else
	#include <netdb.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <sys/time.h>
	#include <fcntl.h>
	#include "timezone_hack.h"
	#define SOCERRNO				errno
#endif

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#include <iostream>
#include <iomanip>

#include "SSLLocking.h"

namespace tcpsocket {

struct _SSLInitializer {
	SSLLocking	*lock;

	_SSLInitializer() {
		SSL_library_init();
		SSL_load_error_strings();
		lock = new SSLLocking;
	}

	~_SSLInitializer() {
		delete lock;
	}
};
static _SSLInitializer _initializer;

SSLContext	SSLContext::defctx;

struct X509Data {
	X509Data() {
		certificate = NULL;
	}
	X509Data(const X509Data& cert) {
		certificate = cert.certificate;
	}
	X509Data(X509* cert) {
		certificate = cert;
	}

	X509*	certificate;
};

struct SSLSocketData {
	SSLSocketData() {
		ctx = NULL;
		ssl = NULL;
	}

	SSLContext*	ctx;
	SSL*		ssl;
	int			mrc;
};

/**
Constructs an empty X509Certificate holder.  Use <code>loadPEMFile()</code>
or <code>loadDERFile()</code> to initialize the certificate.

@see #PEMLoad(const std::string&)
@see #DERLoad(const std::string&)
*/
X509Certificate::X509Certificate()
{
	certificate = new X509Data;
}


/**
Constructs an X509Certificate object from the
masked X509Data structrure.
@param certificate the source data
*/
X509Certificate::X509Certificate(const X509Data* certificate)
{
	this->certificate = new X509Data(*certificate);
}

/**
Destroys the certificate object
*/
X509Certificate::~X509Certificate()
{
	if (certificate->certificate)
		X509_free(certificate->certificate);
	delete certificate;
}

/**
Providse the X509 certificate serial number
@return the X509 certificate serial number
*/
std::string X509Certificate::getSerialNumber()
{
	BIO*	bp = BIO_new(BIO_s_mem());
	char	buffer[1024];
	i2a_ASN1_INTEGER(bp,X509_get_serialNumber(certificate->certificate));
	memset(buffer,0,sizeof(buffer));
	BIO_read(bp,buffer,sizeof(buffer));
	buffer[sizeof(buffer)-1]=0;
	BIO_free(bp);
	return buffer;
}
/**
Provides the Issuer Name
@return the Issuer Name
*/
std::string X509Certificate::getIssuerName()
{
	BIO* bp = BIO_new(BIO_s_mem());

	char		buffer[64];
	std::string ret;
	int			read;

	X509_NAME_print_ex(bp, X509_get_issuer_name(certificate->certificate), 0, XN_FLAG_SEP_CPLUS_SPC);

	while ((read = BIO_read(bp,buffer,sizeof(buffer))) == sizeof(buffer)) {
		ret.append(buffer,read);
	}
	if (read > 0)
		ret.append(buffer,read);

	BIO_free(bp);
	return ret;
}
/**
Provides the subject name
@return The subject name
*/
std::string X509Certificate::getSubjectName()
{
	BIO* bp = BIO_new(BIO_s_mem());

	char		buffer[64];
	std::string ret;
	int			read;

	X509_NAME_print_ex(bp, X509_get_subject_name(certificate->certificate), 0, XN_FLAG_SEP_CPLUS_SPC);

	while ((read = BIO_read(bp,buffer,sizeof(buffer))) == sizeof(buffer)) {
		ret.append(buffer,read);
	}
	if (read > 0)
		ret.append(buffer,read);

	BIO_free(bp);
	return ret;
}

/**
Provides the certificate not before date
@return The certificate not before date
*/
std::string X509Certificate::getNotBefore()
{
	BIO*	bp = BIO_new(BIO_s_mem());
	char	buffer[1024];
	ASN1_TIME_print(bp,X509_get_notBefore(certificate->certificate));
	memset(buffer,0,sizeof(buffer));
	BIO_read(bp,buffer,sizeof(buffer));
	buffer[sizeof(buffer)-1]=0;
	BIO_free(bp);
	return buffer;
}
/**
Provides the certificate not after date
@return The certificate not after date
*/
std::string X509Certificate::getNotAfter()
{
	BIO*	bp = BIO_new(BIO_s_mem());
	char	buffer[1024];
	ASN1_TIME_print(bp,X509_get_notAfter(certificate->certificate));
	memset(buffer,0,sizeof(buffer));
	BIO_read(bp,buffer,sizeof(buffer));
	buffer[sizeof(buffer)-1]=0;
	BIO_free(bp);
	return buffer;
}

/*
Helps convert a generalized time string to a time_t value
*/
time_t X509Certificate::make_time_t(const std::string& generalizedTime)
{
	struct tm	theTime;
	time_t		t = time_t();

	if (generalizedTime.length() >= 14) {
		// From http://www.obj-sys.com/asn1tutorial/node14.html
		// GeneralizedTime

		//  Type GeneralizedTime takes values of the year, month, day, hour, time, minute,second, and second fraction in any of three forms.

		//   1. Local time only. ``YYYYMMDDHHMMSS.fff'', where the optional fff is accurate to three decimal places.
		//   2. Universal time (UTC time) only. ``YYMMDDHHMMSS.fffZ''.
		//   3. Difference between local and UTC times. ``YYYYMMDDHHMMSS.fff+-HHMM''.

		theTime.tm_year = atoi(generalizedTime.substr(0,4).c_str()) - 1900;
		theTime.tm_mon = atoi(generalizedTime.substr(4,2).c_str()) - 1;
		theTime.tm_mday = atoi(generalizedTime.substr(6,2).c_str());
		theTime.tm_hour = atoi(generalizedTime.substr(8,2).c_str());
		theTime.tm_min = atoi(generalizedTime.substr(10,2).c_str());
		theTime.tm_sec = atoi(generalizedTime.substr(12,2).c_str());
		theTime.tm_isdst = -1;
		
		t = mktime(&theTime);

		if (generalizedTime.length() > 14) {
			std::string::size_type next = 14;

			if (generalizedTime[next] == '.') {
				// skip the milliseconds, they aren't usable in a time_t
				next += 4;
			}

			if (generalizedTime.length() >= next) {
				if (generalizedTime[next] == 'Z') {
					t -= _timezone;
				} else if (	(generalizedTime[next] == '+' || generalizedTime[next] == '-' )
								&& generalizedTime.length() >= next+5) {
					long hours = atoi(generalizedTime.substr(next,3).c_str());
					long minutes = atoi((generalizedTime[next] + generalizedTime.substr(next+3,2)).c_str());
					long tz = hours * 3600 + minutes * 60;
					
					t -= _timezone + tz;
				}
			}
		}
	}

	return t;
}

/**
Provides the Certificate Not Before time value as a time_t.
@return The Not Before time as a time_t
*/
time_t X509Certificate::getNotBefore_time_t()
{
	ASN1_GENERALIZEDTIME* pgt = NULL;
	
	if (ASN1_TIME_to_generalizedtime(X509_get_notBefore(certificate->certificate), &pgt) != NULL) {
		time_t t = make_time_t((const char*)pgt->data);

		ASN1_GENERALIZEDTIME_free(pgt);
		return t;
	}
	return 0;
}

/**
Provides the Certificate Not After time value as a time_t.
@return The Not After time as a time_t
*/
time_t X509Certificate::getNotAfter_time_t()
{
	ASN1_GENERALIZEDTIME* pgt = NULL;
	
	if (ASN1_TIME_to_generalizedtime(X509_get_notAfter(certificate->certificate), &pgt) != NULL) {
		time_t t = make_time_t((const char*)pgt->data);

		ASN1_GENERALIZEDTIME_free(pgt);
		return t;
	}
	return 0;

}

/**
Converts the certificate to a string
@return The PEM string representation of the certificate
*/
std::string X509Certificate::toString()
{
	BIO* bp = BIO_new(BIO_s_mem());

	char		buffer[512];
	std::string ret;
	int			read;

	PEM_write_bio_X509(bp, certificate->certificate);
	while ((read = BIO_read(bp,buffer,sizeof(buffer))) == sizeof(buffer)) {
		ret.append(buffer,read);
	}
	if (read > 0)
		ret.append(buffer,read);

	BIO_free(bp);
	return ret;
}

/**
Provides a pointer to the X509 data structure
@return An OpenSSL <code>X509*</code> pointer cast to <code>void*</code>
*/
void* X509Certificate::getX509()
{
	return certificate->certificate;
}


int passwordCallback(char *buf, int size, int rwflag, void *u)
{
	int len = 0;

	if (u != NULL) {
		const char*	passwd = (const char*)u;

		len = strlen(passwd);
		if (len > size) len = size;
		memcpy(buf, passwd, len);
	}	
	return len;
}


/**
Loads am X509 certificate from a PEM file.
@param the name of the file to load.
@param password [optional] a password controlling access to the file.

@return the success of loading the file
*/
X509Certificate::LoadResult X509Certificate::loadPEMFile(const std::string& filename, const char* password)
{
	if (filename.length() == 0)
		return invalidFile;

	if (certificate->certificate) {
		X509_free(certificate->certificate);
		certificate->certificate = NULL;
	}

	FILE*	f = fopen(filename.c_str(), "rb");
	if (f) {
		if (password)
			certificate->certificate = PEM_read_X509(f, &certificate->certificate, passwordCallback, (void*)password);
		else
			certificate->certificate = PEM_read_X509(f, &certificate->certificate, NULL, NULL);

		if (!certificate->certificate) {
			return invalidFile;
		}
		fclose(f);
	} else {
		return fileNotFound;
	}
	return noError;
}

/**
Loads am X509 certificate from a DER file.
@param the name of the file to load.

@return the success of loading the file
*/
X509Certificate::LoadResult X509Certificate::loadDERFile(const std::string& filename)
{
	if (filename.length() == 0)
		return invalidFile;

	if (certificate->certificate) {
		X509_free(certificate->certificate);
		certificate->certificate = NULL;
	}

	FILE*	f = fopen(filename.c_str(), "rb");
	if (f) {
		certificate->certificate = d2i_X509_fp(f, NULL);
		if (!certificate->certificate) {
			return invalidFile;
		}
		fclose(f);
	} else {
		return fileNotFound;
	}
	return noError;
}


struct PrivateKeyData {
	PrivateKeyData() {
		pkey = NULL;
	}

	EVP_PKEY*	pkey;
};



/**
Constructs the private key
*/
PrivateKey::PrivateKey()
{
	pk = new PrivateKeyData();
}

/**
Destroys the private key
*/
PrivateKey::~PrivateKey()
{
	if (pk->pkey) {
		EVP_PKEY_free(pk->pkey);
	}

	delete pk;
}

/**
Loads a private key from a PEM file.
@param the name of the file to load.
@param password [optional] a password controlling access to the file.

@return the success of loading the file
*/
PrivateKey::LoadResult PrivateKey::loadPEMFile(const std::string& filename, const char* password)
{
	if (filename.length() == 0)
		return invalidFile;

	if (pk->pkey) {
		EVP_PKEY_free(pk->pkey);
		pk->pkey = NULL;
	}
	FILE* f = fopen(filename.c_str(), "rb");
	if (f) {
		if (password) {
			pk->pkey = PEM_read_PrivateKey(f, &pk->pkey, passwordCallback, (void*)password);
		} else {
			pk->pkey = PEM_read_PrivateKey(f, &pk->pkey, NULL, NULL);
		}
		fclose(f);

		if (!pk->pkey) {
			return invalidFile;
		}
	} else {
		return fileNotFound;
	}
	return noError;
}

/**
Loads a private key from a DER file.
@param the name of the file to load.
@return the success of loading the file
*/
PrivateKey::LoadResult PrivateKey::loadDERFile(const std::string& filename)
{
	if (filename.length() == 0)
		return invalidFile;

	if (pk->pkey) {
		EVP_PKEY_free(pk->pkey);
		pk->pkey = NULL;
	}
	FILE* f = fopen(filename.c_str(), "rb");
	if (f) {
		pk->pkey = d2i_PrivateKey_fp(f, &pk->pkey);
		if (!pk->pkey) {
			return invalidFile;
		}
		fclose(f);
	} else {
		return fileNotFound;
	}
	return noError;
}

/**
Provides a pointer to the EVP_PKEY data structure
@return An OpenSSL <code>EVP_PKEY*</code> pointer cast to <code>void*</code>
*/

void* PrivateKey::get()
{
	return pk->pkey;
}

/**
Constructs the certificate authority information.
@param caFile [in, optional] specifies the PEM file containing trusted certificates
@param caDirectory [in, optional] specifies the directory containing trusted certificate PEM files 
*/
CertificateAuthority::CertificateAuthority(const std::string& caFile, const std::string& caDirectory)
{
	this->caFile = caFile;
	this->caDirectory = caDirectory;
}

/**
Provides the name of the file used as a list of trusted certificates.
@return the name of the file used as a list of trusted certificates.
*/
std::string CertificateAuthority::getAuthorityFile()
{
	return caFile;
}

/**
Provides the directory used as a list of trusted certificates.
@return the directory used as a list of trusted certificates.
*/
std::string CertificateAuthority::getAuthorityDirectory()
{
	return caDirectory;
}

/**
Determines which file should be used as a list of trusted certificates.
@param caFile [in] specifies the PEM file containing trusted certificates
*/
void CertificateAuthority::setAuthorityFile(const std::string& caFile)
{
	this->caFile = caFile;
}

/**
Determines which directory should be used as a source of trusted certificates.
@param caDirectory [in] specifies the directory containing trusted certificate PEM files 
*/
void CertificateAuthority::setAuthorityDirectory(const std::string& caDirectory)
{
	this->caDirectory = caDirectory;
}


struct SSLContextData {
	SSLContextData() {
		ctx = NULL;
	}

	SSL_CTX	*ctx;
};

/**
Default constructor.  Establishes an empty context that will
utilize default CA and SSL settings
*/
SSLContext::SSLContext()
{
	init();
}

/**
Constructs the context specifying the SSL version, Private Key, X509 Certificate
and Certificate Authority.

@param ver The SSL Version to use.
@param pk The private key to use when connecting
@param cert The X509 certificate to supply when connecting
@param ca The Certificate Authority list indicating the list of trusted certificates.
*/
SSLContext::SSLContext(ssl_ver ver, PrivateKey* pk, X509Certificate* cert, CertificateAuthority *ca)
{
	init();
	setSSLVersion(ver);
	setPrivateKey(pk);
	setX509Certificate(cert);
	setCertificateAuthority(ca);
}

void SSLContext::init()
{
	pdata = new SSLContextData;
	pdata->ctx = SSL_CTX_new(SSLv23_method());
	ver = sslv23;
}

/// Destroys the context
SSLContext::~SSLContext()
{
	SSL_CTX_free(pdata->ctx);
	delete pdata;
}

/**
Allocates a <code>SSLSocket</code> socket using the current context 
from the supplied socket handle.

@param socket_handle [in] A connected socket handle to be used in SSL data exchagne
@return An SSL controlled socket object
*/
SSLSocket SSLContext::allocateSocket(int socket_handle)
{
	return SSLSocket(socket_handle, this);
}

/**
Allocates a client <code>SSLSocket</code> connected to the destination host and 
port.

@param host [in] The DNS or IP address of the destination host
@param port [in] The tcp/ip port number of the destination host.
@return A connected SSLSocket.
@exception SSLSocketException
@exception SocketException
*/
SSLSocket SSLContext::allocateSocket(const char* host, short port) throw (SSLSocketException,SocketException)
{
	return SSLSocket(host, port, this);
}

/**
Allocates a socket and binds it to the port and address specified.

@param bindPort [in] The local port to bind to
@param bindAddress [in, optional] A 4-byte long represented TCP/IP address to bind to
@param listenerBacklog [in, optional] The number of backlogged listeners to maintain
@return A bound SSL server socket
@exception SocketException
*/
SSLServerSocket	SSLContext::allocateServerSocket(short bindPort, in_addr_t bindAddress, int listenerBacklog) throw (SocketException)
{
	return SSLServerSocket(bindPort, this, bindAddress, listenerBacklog);
}

/**
Allocates an SSL* object cast to void*, based on this context
@return A void* cast SSL* object
*/
void* SSLContext::allocateSSL()
{
	return SSL_new(pdata->ctx);
}

/**
Sets the private key used in this context
@param pk The private key to be used in this context
*/
void SSLContext::setPrivateKey(PrivateKey *pk)
{
	if (pk) {
		::SSL_CTX_use_PrivateKey(pdata->ctx, (EVP_PKEY*)pk->get());
	}
	this->pk = pk;
}

/**
Provides the private key used in this context.
@return The private key used in this context.
*/
PrivateKey* SSLContext::getPrivateKey()
{
	return pk;
}

/**
Sets the X509 certificate used to identify this endpoint.
@param cert [in] The certificate used to identify this endpoint.
*/
void SSLContext::setX509Certificate(X509Certificate *cert)
{
	if (cert) {
		::SSL_CTX_use_certificate(pdata->ctx, (X509*)cert->getX509());
	}
	this->cert = cert;
}

/**
Provides the X509 certificate used to identify this endpoint.
@return The X509 certificate used to identify this endpoint.
*/
X509Certificate* SSLContext::getX509Certificate()
{
	return cert;
}

/**
Sets the details about the certificate authority used to validate certificates
@param ca [in] The certificate authroity details to use
*/
void SSLContext::setCertificateAuthority(CertificateAuthority* ca)
{
	this->ca = ca;
	if (ca) {
		std::string file = ca->getAuthorityFile();
		std::string dir = ca->getAuthorityDirectory();

		if (file.length() || dir.length()) {
			if (!SSL_CTX_load_verify_locations(pdata->ctx, file.length()?file.c_str():NULL, dir.length()?dir.c_str():NULL)) {
				ERR_print_errors_fp(stderr);
			}
		}
	}
}

/**
Retrieves the certificate authority details used in certificate validataion
@return The certificate authority details used to validate certificates
*/
CertificateAuthority* SSLContext::getCertificateAuthority()
{
	return ca;
}

/**
Sets the SSL version used in new connections.
@param ver [in] The SSL version to use
*/
void SSLContext::setSSLVersion(ssl_ver ver)
{
	switch (ver) {
		case sslv2:
			SSL_CTX_set_ssl_version(pdata->ctx, SSLv2_method());
			break;
		case sslv3:
			SSL_CTX_set_ssl_version(pdata->ctx, SSLv3_method());
			break;
		case sslv23:
			SSL_CTX_set_ssl_version(pdata->ctx, SSLv23_method());
			break;
		case tlsv1:
			SSL_CTX_set_ssl_version(pdata->ctx, TLSv1_method());
			break;
	}	
	this->ver = ver;
}

/**
Provides the SSL version used to establish connections
@return The current SSL version.
*/
SSLContext::ssl_ver SSLContext::getSSLVersion()
{
	return this->ver;
}

/**
Sets the session ID for the context.
@param sid A binary buffer containing the application identifier
@param sid_len The size of the buffer
*/
void SSLContext::setSessionID(const unsigned char* sid, unsigned int sid_len)
{
	SSL_CTX_set_session_id_context(pdata->ctx, sid, sid_len);
}

/**
Sets the depth of X509 certificate chain verification
@param verifyDepth the depth of X509 certificate chain verification.
*/
void SSLContext::setVerifyDepth(int verifyDepth)
{
	this->verifyDepth = verifyDepth;
	SSL_CTX_set_verify_depth(pdata->ctx, verifyDepth);
}

/**
Provides the current certificate chain verification depth.
@return the current certificate chain verification depth
*/
int SSLContext::getVerifyDepth()
{
	return verifyDepth;
}

static int verify_callback(X509_STORE_CTX* ctx, void* arg)
{
	((SSLContext*)arg)->VerifyPeerCertificate(ctx);
	return 1;
}

/**
Call to enable verification routienes
@param requireCert [in, optional] Indicates wheter a client certificate is required for new connections
*/
void SSLContext::enableVerification(bool requireCert)
{
	SSL_CTX_set_verify(pdata->ctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE | (requireCert?SSL_VERIFY_FAIL_IF_NO_PEER_CERT:0), NULL);
	SSL_CTX_set_cert_verify_callback(pdata->ctx, verify_callback, this);
}
/**
Disables certificate verification
*/
void SSLContext::disableVerification()
{
	SSL_CTX_set_verify(pdata->ctx, SSL_VERIFY_NONE, NULL);
	SSL_CTX_set_cert_verify_callback(pdata->ctx, NULL, NULL);
}

/**
Called to verify the certificate of the peer
*/
int SSLContext::VerifyPeerCertificate(void* x509_store_ctx)
{
	X509_STORE_CTX*	store_ctx = (X509_STORE_CTX*)x509_store_ctx;
	int	res = X509_verify_cert(store_ctx);
	return res;
}


// /////////////////////////////////////////////////////////////////////////////
// SSLSocket
// /////////////////////////////////////////////////////////////////////////////
/**
Constructs the SSLSocket object, usable for copy destinations.
*/
SSLSocket::SSLSocket()
{
	init();
}

/**
Constructs the SSLSocket and uses the provided socket handle for communications
@param socket_handle [in] A previously created socket handle
@param pCtx [in] A previously created SSLContext
@param type [in, default = con_client] Indicates wether the socket is a client or server
*/
SSLSocket::SSLSocket(int socket_handle, SSLContext* pCtx, con_type type)
{
	init();
	init_ssl(pCtx);

	m_socket = socket_handle;

	if (!pdata->ssl) {
		throw SocketException(-1);
	}

	pdata->mrc = SSL_set_fd(pdata->ssl, m_socket);
	if (pdata->mrc < 1)
		throw SSLSocketException(getErrorMessage().c_str());

	if (type == con_client)
		SSL_set_connect_state(pdata->ssl);
	else 
		SSL_set_accept_state(pdata->ssl);
}

/**
Copy constructor, copies the socket handle
@param s [in] The source socket
*/	
SSLSocket::SSLSocket(const SSLSocket& s)
{
	init();
	*this = s;
}

/**
Constructs the socket and opens a connection to a server.
@param host [in] The address of the server or DNS name
@param port [in] The TCP port to connect to
@param version [in] The SSL Protocol version to use
*/
SSLSocket::SSLSocket(const char* host, short port, SSLContext* pCtx) throw (SSLSocketException, SocketException) 
		: Socket(host,port)
{
	init();
	init_ssl(pCtx);

	if (!pdata->ctx || !pdata->ssl) {
		throw SSLSocketException(getErrorMessage().c_str());
	}

	pdata->mrc = SSL_set_fd(pdata->ssl,m_socket);
	if (pdata->mrc < 0) {
		close();
		throw SSLSocketException(getErrorMessage().c_str());
	}

	SSL_set_connect_state(pdata->ssl);	
}
/**
Destroys the SSL socket
*/
SSLSocket::~SSLSocket()
{
	// Do not free the SSL object, except on close.
	delete pdata;
}
/**
Copy oeprator, copies the socket handle and SSL information
@param s [in] The source socket
@return A reference tot his object.
*/	
SSLSocket& SSLSocket::operator=(const SSLSocket& s)
{
	pdata->ctx = s.pdata->ctx;
	pdata->ssl = s.pdata->ssl;
	m_socket = s.m_socket;
	m_throwOnClose = s.m_throwOnClose;
	return *this;
}

/**
Sends data over the socket.  Sends exactly <code>length</code> bytes.

@param sendBuffer [in] A buffer containing the data to be sent
@param length [in] The number of bytes in the buffer
@return The number of bytes sent, -1 on error, and 0 if the connection was closed
@exception SocketException if the connection was closed and the throw on close flag was set.
*/
size_t SSLSocket::send(const char* sendBuffer, size_t length)	throw (SSLSocketException)
{
	size_t	sent = 0;
	int		isent;

	while ( sent < length ) {
		pdata->mrc = isent = SSL_write(pdata->ssl,sendBuffer+sent,length-sent);
		if (isent == 0) {
			if (m_throwOnClose)
				throw SocketException(ECONNRESET);
			break;
		} else if (isent < 0) {
			int err = SSL_get_error(pdata->ssl,isent);
			if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
				sleep(200);
			} else {
				throw SSLSocketException(getErrorMessage().c_str());
			}
		} else {
			sent += isent;
		}
	}
	return sent;
}

/**
Receives data from the socket.  Receives exactly <code>length</code> bytes or
less if the socket is closed early.

@param sendBuffer [out] A buffer receiving the data 
@param length [in] The maximum number of bytes in the buffer
@return The number of bytes received, -1 on error, and 0 if the connection was closed
@exception SocketException if the connection was closed and the throw on close flag was set.
*/
size_t SSLSocket::receive(char* readBuffer, size_t length)		throw (SSLSocketException)
{
	size_t	nread = 0;
	int		iread;

	while ( nread < length ) {
		pdata->mrc = iread = SSL_read(pdata->ssl,readBuffer+nread,length-nread);
		if (iread == 0) {
			if (m_throwOnClose)
				throw SocketException(ECONNRESET);
			break;
		} else if (iread < 0) {
			int err = SSL_get_error(pdata->ssl,iread);
			if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
				sleep(10);
			} else
				throw SSLSocketException(getErrorMessage().c_str());
		} else {
			nread += iread;
		}
	}
	return nread;
}

/**
Receives data from the socket.  Receives upto <code>length</code> bytes.

@param sendBuffer [out] A buffer receiving the data 
@param length [in] The maximum number of bytes in the buffer
@return The number of bytes received, -1 on error, and 0 if the connection was closed
@exception SocketException if the connection was closed and the throw on close flag was set.
*/
size_t SSLSocket::receiveSome(char* readBuffer, size_t length)	throw (SSLSocketException)
{
	int	nread = 0;
	int	iread;
	
	while ( nread == 0 ) {
		pdata->mrc = iread = SSL_read(pdata->ssl,readBuffer+nread,length-nread);
		if (iread == 0) {
			int err = SOCERRNO;
			if (m_throwOnClose)
				throw SocketException(ECONNRESET);
			break;
		} else if (iread < 0) {
			int err = SSL_get_error(pdata->ssl,iread);
			if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
				sleep(10);
			} else
				throw SSLSocketException(getErrorMessage().c_str());
		} else {
			nread += iread;
		}
	}
	return nread;
}

/**
Closes the socket handle, and frees the SSL structures
*/
void SSLSocket::close() throw (SocketException)
{
	if (pdata->ssl) 
		SSL_free(pdata->ssl);
	pdata->ssl = NULL;

	Socket::close();
}

/**
Clones this object.
@return a new SSLSocket constructed from this object.
*/
Socket* SSLSocket::clone() const 
{
	return new SSLSocket(*this);
}

#include <stdio.h>
/**
Retreives the certificate of the peer.<br>
<b>Note:</b> The returned object must be freed by the caller.

@return the X509 cetificate of the peer.
*/
X509Certificate* SSLSocket::getPeerCertificate()
{
	if (pdata->ssl) {
		X509*	cert = SSL_get_peer_certificate(pdata->ssl);
		if (cert == NULL) {
			if (!SSL_is_init_finished(pdata->ssl)) {
				if (SSL_in_connect_init(pdata->ssl)) {
					pdata->mrc = SSL_connect(pdata->ssl);
				} else if (SSL_in_accept_init(pdata->ssl)) {
					pdata->mrc = SSL_accept(pdata->ssl);
				}
				if (pdata->mrc <= 0) {
					int err = SSL_get_error(pdata->ssl, pdata->mrc);
					fprintf(stderr, "\n\nrc: %d SSL_get_error(): %d\n", pdata->mrc, err);

					//long	lerr;
					//char	buffer[1024];
					//while (lerr = ERR_get_error()) {
						//fprintf(stderr, "Error: %ld -- %s\n", lerr, ERR_error_string(err, buffer));
					//}
					//
					ERR_print_errors_fp(stderr);

					fprintf(stderr,"\n\n");
				} else {
					cert = SSL_get_peer_certificate(pdata->ssl);
					if (cert != NULL) {
						X509Data	dcert(cert);
						return new X509Certificate(&dcert);
					}
				}
			}
		}
	} 
	return NULL;
}



/**
Gets the result of performing a certificate verification.
@return The verify result.
*/
int SSLSocket::getVerifyResult()
{
	return SSL_get_verify_result(pdata->ssl);
}

static int error_print(const char* str, size_t len, void* u)
{
	((std::string*)u)->append(str);
	return 1;
}

/**
Gets the SSL error message
@return The SSL error message
*/
std::string SSLSocket::getErrorMessage()
{
	std::string ret;
	if (pdata->ssl) {
		ERR_print_errors_cb(error_print,&ret);
	}
	return ret;
}

/**
Gets the SSL object cast as void*.
@return the SSL object cast as void*.
*/
void* SSLSocket::getSSL()
{
	return pdata->ssl;
}

/**
Gets the SSLContext Object associated with the socket

@return The SSLCOntext Object associated with the socket.
*/
SSLContext* SSLSocket::getCTX()
{
	return pdata->ctx;
}

void SSLSocket::init()
{
	pdata = new SSLSocketData();
}

void SSLSocket::init_ssl(SSLContext* pCtx)
{
	pdata->ctx = pCtx;
	if (pCtx)
		pdata->ssl = (SSL*)pCtx->allocateSSL();
}

// /////////////////////////////////////////////////////////////////////////////

/**
Constructs the server socket binding to the port and optionally using the bind address and listener backlog
@param bindPort [in] The TCP port to bind to
@param pCtx [in] The SSL Context used to establish the SSL Environment
@param bindAddress [in,optional] Optional address to bind to, if not specified all local address
									will be bound.
@param listenerBacklog [in,optional] Optional depth of the listener queue.
@throw SocketException on an error
*/
SSLServerSocket::SSLServerSocket(short bindPort, SSLContext* pCtx, in_addr_t bindAddress, int listenerBacklog) throw (SocketException)
{
	init(bindPort, pCtx, bindAddress, listenerBacklog);
}

/**
Releases the server socket
@throw SocketException on an error
*/
SSLServerSocket::~SSLServerSocket() throw (SocketException)
{
	close();
}

/**
Accepts a connection from the server socket.
@return A socket connected to a client
@throw SocketException on an error
*/
SSLSocket SSLServerSocket::acceptConnection() throw (SocketException)
{
	int	soc = ::accept(m_socket,NULL,NULL);
	if (soc < 0)
		throw SocketException(SOCERRNO);

	return SSLSocket(soc, pCtx, con_server);
}

/**
Closes the server socket.  Can be called out of band to cause acceptConnection()
to return immediately.
*/
void SSLServerSocket::close() throw (SocketException)
{
	Socket::close();
	m_socket = -1; 
}

/**
Provides the underlying socket handle.
@return The underlying socket handle
*/
int SSLServerSocket::getSocket()
{
	return SSLSocket::getSocket();
}


/**
Provides the SSLContext associated with the server
@return The curretn SSLContext
*/
SSLContext* SSLServerSocket::getCTX()
{
	return pCtx;
}

/**
Sets the server context
@param pCtx the new SSL context for the server socket
*/
void SSLServerSocket::setCTX(SSLContext* pCtx)
{
	this->pCtx = pCtx;
}


void SSLServerSocket::init(short bindPort, SSLContext* pCtx, in_addr_t bindAddress, int listenerBacklog) throw (SocketException)
{
	struct sockaddr_in	saddr;

	this->pCtx = pCtx;

	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(bindPort);
	saddr.sin_addr.s_addr = bindAddress;

	int	iReuse = 1;
	::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*) &iReuse, sizeof(int));

	if (::bind(m_socket, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) {
		int err = SOCERRNO;
		close();
		throw SocketException(err);
	}

	if (::listen(m_socket, listenerBacklog) != 0) {
		int err = SOCERRNO;
		close();
		throw SocketException(err);
	}
}

}
