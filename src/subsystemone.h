#ifndef SUBSYSTEMONE_H
#define SUBSYSTEMONE_H
#include "Poco/Util/Subsystem.h"
class SubSystemOne: public Poco::Util::Subsystem
{
public:
    SubSystemOne();
    virtual void initialize(Poco::Util::Application & self);
    virtual void uninitialize();
    virtual void defineOptions(Poco::Util::OptionSet & options);
    virtual const char * name()const;
    void handleOption(const std::string & name,const std::string & value);
};

#endif // SUBSYSTEMONE_H
