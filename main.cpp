
// Base stuff
#include <docopt.h>

#include <spdlog/spdlog.h>
#include <map>

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
                     true,           // show help if requested
                     "NomidiBackend 0.1"); // version string

   const std::string configRoot(args["CONFIGDIR"] ? args["CONFIGDIR"].asString() : "");
   
   const auto verbose = args["--verbose"].asBool();
   if (verbose)
   {
      spdlog::info("Set loglevel to debug");
      spdlog::set_level(spdlog::level::debug);
   }


   spdlog::info("Using Config Dir: '{}'", configRoot);
   base::Base base(configRoot);
   base.start();
   auto qtThread = std::thread([&base, &argc, &argv](){
      uiadapter::qt::startQt(base, argc, argv);
   });
   if (0 != pthread_setname_np(qtThread.native_handle(), "NMBE-Qt"))
   {
      spdlog::error("Could not set thread name: NMBE-Qt");
   }
   qtThread.join();
}
