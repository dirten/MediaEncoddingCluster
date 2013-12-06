#include "subsystemone.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
SubSystemOne::SubSystemOne()
{
}

void SubSystemOne::initialize(Poco::Util::Application & self)
{
    //Poco::Util::Subsystem::initialize(self);
}

const char * SubSystemOne::name()const
{
    return "SubSystemOne";
}
void SubSystemOne::uninitialize()
{

}
void SubSystemOne::defineOptions(Poco::Util::OptionSet & options)
{
    Poco::Util::Subsystem::defineOptions(options);

    options.addOption(
                Poco::Util::Option("subsystem","a","start subsystem").
                required(false).
                repeatable(false).
                group("server").
                callback(Poco::Util::OptionCallback<SubSystemOne>(this,&SubSystemOne::handleOption))
                );

}

void SubSystemOne::handleOption(const std::string & name,const std::string & value)
{

}

