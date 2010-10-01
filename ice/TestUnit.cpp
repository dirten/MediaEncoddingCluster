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

#include <TestUnit.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

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

static const ::std::string __test__testclass__operate_name = "operate";

static const ::std::string __test__testfactory__getTestClass_name = "getTestClass";

::Ice::Object* IceInternal::upCast(::test::testclass* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::test::testclass* p) { return p; }

::Ice::Object* IceInternal::upCast(::test::testfactory* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::test::testfactory* p) { return p; }

void
test::__read(::IceInternal::BasicStream* __is, ::test::testclassPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::test::testclass;
        v->__copyFrom(proxy);
    }
}

void
test::__read(::IceInternal::BasicStream* __is, ::test::testfactoryPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::test::testfactory;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::test::testclass::operate(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::test::testclass* __del = dynamic_cast< ::IceDelegate::test::testclass*>(__delBase.get());
            __del->operate(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::test::testclass::ice_staticId()
{
    return ::test::testclass::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::test::testclass::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::test::testclass);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::test::testclass::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::test::testclass);
}

::IceProxy::Ice::Object*
IceProxy::test::testclass::__newInstance() const
{
    return new testclass;
}

::test::testclassPtr
IceProxy::test::testfactory::getTestClass(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__test__testfactory__getTestClass_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::test::testfactory* __del = dynamic_cast< ::IceDelegate::test::testfactory*>(__delBase.get());
            return __del->getTestClass(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::test::testfactory::ice_staticId()
{
    return ::test::testfactory::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::test::testfactory::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::test::testfactory);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::test::testfactory::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::test::testfactory);
}

::IceProxy::Ice::Object*
IceProxy::test::testfactory::__newInstance() const
{
    return new testfactory;
}

void
IceDelegateM::test::testclass::operate(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __test__testclass__operate_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::test::testclassPtr
IceDelegateM::test::testfactory::getTestClass(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __test__testfactory__getTestClass_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::test::testclassPtr __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(::test::__patch__testclassPtr, &__ret);
        __is->readPendingObjects();
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateD::test::testclass::operate(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::test::testclass* servant = dynamic_cast< ::test::testclass*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->operate(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __test__testclass__operate_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::test::testclassPtr
IceDelegateD::test::testfactory::getTestClass(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::test::testclassPtr& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::test::testfactory* servant = dynamic_cast< ::test::testfactory*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getTestClass(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::test::testclassPtr& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __test__testfactory__getTestClass_name, ::Ice::Normal, __context);
    ::test::testclassPtr __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPtr
test::testclass::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __test__testclass_ids[2] =
{
    "::Ice::Object",
    "::test::testclass"
};

bool
test::testclass::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__test__testclass_ids, __test__testclass_ids + 2, _s);
}

::std::vector< ::std::string>
test::testclass::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__test__testclass_ids[0], &__test__testclass_ids[2]);
}

const ::std::string&
test::testclass::ice_id(const ::Ice::Current&) const
{
    return __test__testclass_ids[1];
}

const ::std::string&
test::testclass::ice_staticId()
{
    return __test__testclass_ids[1];
}

::Ice::DispatchStatus
test::testclass::___operate(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    operate(__current);
    return ::Ice::DispatchOK;
}

static ::std::string __test__testclass_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "operate"
};

::Ice::DispatchStatus
test::testclass::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__test__testclass_all, __test__testclass_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __test__testclass_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___operate(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
test::testclass::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
test::testclass::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
test::testclass::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type test::testclass was not generated with stream support";
    throw ex;
}

void
test::testclass::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type test::testclass was not generated with stream support";
    throw ex;
}

void 
test::__patch__testclassPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::test::testclassPtr* p = static_cast< ::test::testclassPtr*>(__addr);
    assert(p);
    *p = ::test::testclassPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::test::testclass::ice_staticId(), v->ice_id());
    }
}

bool
test::operator==(const ::test::testclass& l, const ::test::testclass& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
test::operator<(const ::test::testclass& l, const ::test::testclass& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
test::testfactory::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __test__testfactory_ids[2] =
{
    "::Ice::Object",
    "::test::testfactory"
};

bool
test::testfactory::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__test__testfactory_ids, __test__testfactory_ids + 2, _s);
}

::std::vector< ::std::string>
test::testfactory::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__test__testfactory_ids[0], &__test__testfactory_ids[2]);
}

const ::std::string&
test::testfactory::ice_id(const ::Ice::Current&) const
{
    return __test__testfactory_ids[1];
}

const ::std::string&
test::testfactory::ice_staticId()
{
    return __test__testfactory_ids[1];
}

::Ice::DispatchStatus
test::testfactory::___getTestClass(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::test::testclassPtr __ret = getTestClass(__current);
    __os->write(::Ice::ObjectPtr(::IceInternal::upCast(__ret.get())));
    __os->writePendingObjects();
    return ::Ice::DispatchOK;
}

static ::std::string __test__testfactory_all[] =
{
    "getTestClass",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
test::testfactory::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__test__testfactory_all, __test__testfactory_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __test__testfactory_all)
    {
        case 0:
        {
            return ___getTestClass(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
test::testfactory::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
test::testfactory::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
test::testfactory::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type test::testfactory was not generated with stream support";
    throw ex;
}

void
test::testfactory::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type test::testfactory was not generated with stream support";
    throw ex;
}

void 
test::__patch__testfactoryPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::test::testfactoryPtr* p = static_cast< ::test::testfactoryPtr*>(__addr);
    assert(p);
    *p = ::test::testfactoryPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::test::testfactory::ice_staticId(), v->ice_id());
    }
}

bool
test::operator==(const ::test::testfactory& l, const ::test::testfactory& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
test::operator<(const ::test::testfactory& l, const ::test::testfactory& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
