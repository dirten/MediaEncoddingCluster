#include "MainApplication.h"
#include <iostream>

#include "Poco/Util/HelpFormatter.h"
#include "Poco/Environment.h"
#include "subsystemone.h"
#include "Poco/Path.h"
#include "Poco/File.h"



using namespace Poco;
using namespace Poco::Util;
MainApplication::MainApplication(int argc, char ** argv)
{



    /*get base path*/
    Path path=new Path();
    if(path.tryParse(argv[0]))
        path.makeAbsolute().makeParent().makeParent();

    config().setString("base_path",path.toString());
    config().setString("logging.path",config().getString("base_path"));

    addSubsystem(new SubSystemOne());

    std::string pluginDir=Poco::Environment::get(std::string("MHIVE_PLUGIN_DIR"),config().getString("base_path")+"plugins");
    //std::string pluginDir=Poco::Environment::get(std::string("PWD"));
}
/*
int MainApplication::run()
{
    return Poco::Util::ServerApplication::run();
}
*/
void MainApplication::initialize(Poco::Util::Application & self)
{
    //logger().getChannel()->close();
    //logger().setProperty("path","/tmp/testsample.log");
    logger().information("initialize");

    loadConfiguration();
    Poco::Util::ServerApplication::initialize(self);
}

void MainApplication::defineOptions(Poco::Util::OptionSet & options)
{
    logger().information("defineOptions");
    Poco::Util::ServerApplication::defineOptions(options);

    options.addOption(
                Poco::Util::Option("help","h","display Help").
                required(false).
                repeatable(false).
                callback(Poco::Util::OptionCallback<MainApplication>(this,&MainApplication::handleHelp))
                );

    options.addOption(
                Poco::Util::Option("client","c","start client services").
                required(false).
                repeatable(false).
                group("client").
                callback(Poco::Util::OptionCallback<MainApplication>(this,&MainApplication::handleOption))
                );

    options.addOption(
                Poco::Util::Option("server","s","start server services").
                required(false).
                repeatable(false).
                group("server").
                callback(Poco::Util::OptionCallback<MainApplication>(this,&MainApplication::handleOption))
                );

}

void MainApplication::handleHelp(const std::string & name,const std::string & value)
{
    _helpRequested=true;
    logger().information("handleHelp");
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Media Encoding Cluster start options");
    helpFormatter.format(std::cout);

    stopOptionsProcessing();
}

void MainApplication::handleOption(const std::string & name,const std::string & value)
{
    //ServerApplication::handleOption(name,value);
    logger().information("handleOption key"+name+" = "+value);
    if(name=="help")
        handleHelp("","");
}

int MainApplication::main(const std::vector<std::string> &args)
{

    logger().information("main log");
    if(!_helpRequested){
        waitForTerminationRequest();
    }
    return EXIT_OK;
}
//POCO_SERVER_MAIN(MainApplication)
