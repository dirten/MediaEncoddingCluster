// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `TestUnit.ice'

#ifndef __TestUnit_h__
#define __TestUnit_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace test
{

class testclass;

class testfactory;

}

}

namespace test
{

class testclass;
bool operator==(const testclass&, const testclass&);
bool operator<(const testclass&, const testclass&);

class testfactory;
bool operator==(const testfactory&, const testfactory&);
bool operator<(const testfactory&, const testfactory&);

}

namespace IceInternal
{

::Ice::Object* upCast(::test::testclass*);
::IceProxy::Ice::Object* upCast(::IceProxy::test::testclass*);

::Ice::Object* upCast(::test::testfactory*);
::IceProxy::Ice::Object* upCast(::IceProxy::test::testfactory*);

}

namespace test
{

typedef ::IceInternal::Handle< ::test::testclass> testclassPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::test::testclass> testclassPrx;

void __read(::IceInternal::BasicStream*, testclassPrx&);
void __patch__testclassPtr(void*, ::Ice::ObjectPtr&);

typedef ::IceInternal::Handle< ::test::testfactory> testfactoryPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::test::testfactory> testfactoryPrx;

void __read(::IceInternal::BasicStream*, testfactoryPrx&);
void __patch__testfactoryPtr(void*, ::Ice::ObjectPtr&);

}

namespace IceProxy
{

namespace test
{

class testclass : virtual public ::IceProxy::Ice::Object
{
public:

    void operate()
    {
        operate(0);
    }
    void operate(const ::Ice::Context& __ctx)
    {
        operate(&__ctx);
    }
    
private:

    void operate(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<testclass> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testclass> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testclass*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<testclass*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class testfactory : virtual public ::IceProxy::Ice::Object
{
public:

    ::test::testclassPtr getTestClass()
    {
        return getTestClass(0);
    }
    ::test::testclassPtr getTestClass(const ::Ice::Context& __ctx)
    {
        return getTestClass(&__ctx);
    }
    
private:

    ::test::testclassPtr getTestClass(const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<testfactory> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<testfactory> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<testfactory*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<testfactory*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace test
{

class testclass : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void operate(const ::Ice::Context*) = 0;
};

class testfactory : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::test::testclassPtr getTestClass(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace test
{

class testclass : virtual public ::IceDelegate::test::testclass,
                  virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void operate(const ::Ice::Context*);
};

class testfactory : virtual public ::IceDelegate::test::testfactory,
                    virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::test::testclassPtr getTestClass(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace test
{

class testclass : virtual public ::IceDelegate::test::testclass,
                  virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void operate(const ::Ice::Context*);
};

class testfactory : virtual public ::IceDelegate::test::testfactory,
                    virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::test::testclassPtr getTestClass(const ::Ice::Context*);
};

}

}

namespace test
{

class testclass : virtual public ::Ice::Object
{
public:

    typedef testclassPrx ProxyType;
    typedef testclassPtr PointerType;
    
    testclass() {}
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void operate(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___operate(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class testfactory : virtual public ::Ice::Object
{
public:

    typedef testfactoryPrx ProxyType;
    typedef testfactoryPtr PointerType;
    
    testfactory() {}
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::test::testclassPtr getTestClass(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___getTestClass(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
