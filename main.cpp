
// Base stuff
#include <docopt.h>
#include <spdlog/spdlog.h>

#include <map>
#include <string>

#include "Base.h"
#include "StartQt.h"

static const char USAGE[] = R"(
   Usage:
      NomidiBackend CONFIGDIR [-v | --verbose]

   Options:
      -v --verbose  Verbose Output
)";

int main(int argc, char *argv[])
{
   std::map<std::string, docopt::value> args =
       docopt::docopt(USAGE, {argv + 1, argv + argc},
                      true,                   // show help if requested
                      "NomidiBackend 0.1");   // version string

   const std::string configRoot(args["CONFIGDIR"] ? args["CONFIGDIR"].asString()
                                                  : "");

   const auto verbose = args["--verbose"].asBool();
   if (verbose)
   {
      spdlog::info("Set loglevel to debug");
      spdlog::set_level(spdlog::level::debug);
   }

   spdlog::info("Using Config Dir: '{}'", configRoot);

   const std::string rtServerRpcBindAddr("tcp://*:55555");
   const std::string rtServerSignalBindAddr("tcp://*:55556");
   const std::string loaderServerRpcBindAddr("tcp://*:55557");
   const std::string loaderServerSignalBindAddr("tcp://*:55558");
   base::Base base(configRoot, rtServerRpcBindAddr, rtServerSignalBindAddr,
                   loaderServerRpcBindAddr, loaderServerSignalBindAddr);
   base.start();
   auto qtThread = std::thread(
       [=, &argc, &argv]() { uiadapter::qt::startQt(
         std::string(rtServerRpcBindAddr).replace(rtServerRpcBindAddr.find("*"), 1, "localhost"),
         std::string(rtServerSignalBindAddr).replace(rtServerSignalBindAddr.find("*"), 1, "localhost"),
         std::string(loaderServerRpcBindAddr).replace(loaderServerRpcBindAddr.find("*"), 1, "localhost"),
         std::string(loaderServerSignalBindAddr).replace(loaderServerSignalBindAddr.find("*"), 1, "localhost"),
         argc, argv); });
   if (0 != pthread_setname_np(qtThread.native_handle(), "NMBE-Qt"))
   {
      spdlog::error("Could not set thread name: NMBE-Qt");
   }
   qtThread.join();
}
