
// Base stuff
#include <docopt.h>
#include <spdlog/spdlog.h>

#include <map>
#include <string>

#include "Base.h"
#include "ReplaceAsteriskToLocalhost.h"
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
   const std::string address(args["ADDRESS"] ? args["ADDRESS"].asString()
                                             : "tcp://*");

   const auto verbose = args["--verbose"].asBool();
   if (verbose)
   {
      spdlog::info("Set loglevel to debug");
      spdlog::set_level(spdlog::level::debug);
   }

   spdlog::info("Using Config Dir: '{}'", configRoot);

   std::string rtServerRpcBindAddr;
   std::string rtServerSignalBindAddr;
   std::string loaderServerRpcBindAddr;
   std::string loaderServerSignalBindAddr;

   if (address.rfind("tcp", 0) == 0)
   {
      rtServerRpcBindAddr        = fmt::format("{}:55555", address);
      rtServerSignalBindAddr     = fmt::format("{}:55556", address);
      loaderServerRpcBindAddr    = fmt::format("{}:55557", address);
      loaderServerSignalBindAddr = fmt::format("{}:55558", address);
   }
   else if (address.rfind("ipc", 0) == 0)
   {
      rtServerRpcBindAddr     = fmt::format("{}/rtServerRpc", address);
      rtServerSignalBindAddr  = fmt::format("{}/rtServerSignal", address);
      loaderServerRpcBindAddr = fmt::format("{}/loaderServerRpc", address);
      loaderServerSignalBindAddr =
          fmt::format("{}/loaderServersignal", address);
   }
   else
   {
      spdlog::error("illegal address str added");
      return -1;
   }
   base::Base base(configRoot, rtServerRpcBindAddr, rtServerSignalBindAddr,
                   loaderServerRpcBindAddr, loaderServerSignalBindAddr);
   base.start();
   auto qtThread = std::thread([=, &argc, &argv]() {
      uiadapter::qt::startQt(
          util::replaceAsteriskToLocalhost(rtServerRpcBindAddr),
          util::replaceAsteriskToLocalhost(rtServerSignalBindAddr),
          util::replaceAsteriskToLocalhost(loaderServerRpcBindAddr),
          util::replaceAsteriskToLocalhost(loaderServerSignalBindAddr), argc,
          argv);
   });
   if (0 != pthread_setname_np(qtThread.native_handle(), "NMBE-Qt"))
   {
      spdlog::error("Could not set thread name: NMBE-Qt");
   }
   qtThread.join();
}
