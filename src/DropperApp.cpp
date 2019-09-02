#define POCO_WIN32_UTF8

#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include <iostream>
#include "Dropper.h"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;

class DropperApp: public Application
{
  enum action: Poco::Int8 { PUT, GET };

  Poco::Path  _src_path;
  Poco::Path  _dst_path;

protected:
  void defineOptions(OptionSet& options)
  {
    Application::defineOptions(options);
    options.addOption(
      Option("help","h", "Display help information on command line arguments.")
        .required(false)
        .repeatable(false)
        .callback(OptionCallback<DropperApp>(this, &DropperApp::handleHelp)));

    options.addOption(
      Option("src_path", "s", "Send/Receive file path.")
        .required(true)
        .repeatable(false)
        .argument("src_path")
        .callback(OptionCallback<DropperApp>(this, &DropperApp::handle_src_path)));

    options.addOption(
      Option("dst_path", "d", "Receive/Send file path.")
        .required(true)
        .repeatable(false)
        .argument("dst_path")
        .callback(OptionCallback<DropperApp>(this, &DropperApp::handle_dst_path))); 

    options.addOption(Option("put", "put", "Send file(src_path) to dst_path."));
    options.addOption(Option("get", "get", "Download file(std_path) to dst_path."));
  }
  void displayHelp()
  {
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("get/set src_dir dst_dir");
    helpFormatter.setHeader("A simple Dropbox command line client for files transfer");
    helpFormatter.format(std::cout);
  }
  void handleHelp(const std::string& name, const std::string& value)
  {
    displayHelp();
    stopOptionsProcessing();
  }
  void handle_src_path(const std::string& name, const std::string& value)
  {
    _src_path = value;
  }
  void handle_dst_path(const std::string& name, const std::string& value)
  {
    _dst_path = value;
  }
public:
  DropperApp()
  {}

  int main(const std::vector<std::string>& args)
  {
    try
    {
      // You can change appkey and appsecret by specifying them in settings.json
      Dropper dropper("sssxezu1pku6ftg", "vi23jgrwwuzqmhd");

      dropper.load_config();
      dropper.authorize();
      dropper.tokenize();

      if (args.empty())
        std::cout << "Example: dropper.exe put /s test.txt /s /home/uploaded" << std::endl;
      else if ( args[0] == "get" )
        dropper.download_file(_src_path, _dst_path);
      else if ( args[0] == "put" )
        dropper.upload_file(_src_path, _dst_path);
      else
        std::cout << "wrong command! see help: \"dropper.exe /h\"";

      dropper.save_config();
    }
    catch(Poco::Exception& e)
    {
      remove("settings.json");
      std::cerr << e.displayText() << std::endl;
      return Application::EXIT_SOFTWARE;
    }

    return Application::EXIT_OK; 
  }
};

POCO_APP_MAIN(DropperApp)