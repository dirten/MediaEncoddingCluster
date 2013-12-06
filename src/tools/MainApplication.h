#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include "Poco/Util/ServerApplication.h"
class MainApplication : public Poco::Util::ServerApplication
{
  public:
    MainApplication(int argc, char ** argv);
    virtual void initialize(Poco::Util::Application & self);
    virtual void defineOptions(Poco::Util::OptionSet & options);
    virtual int main(const std::vector<std::string> &args);
    void handleHelp(const std::string & name,const std::string & value);
    void handleOption(const std::string & name,const std::string & value);
    //int run();
private:
    bool _helpRequested;
};
#endif // MAINAPPLICATION_H
