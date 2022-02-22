
// Base stuff
#include <docopt.h>

#include <spdlog/spdlog.h>
#include <map>

#include "Base.h"
#include "StartQt.h"
//#include "gRPCServerAsync.h"


static const char USAGE[] = R"(
   Usage:
      BaseRun [CONFIGDIR]
)";


int main(int argc, char *argv[])
{
   std::map<std::string, docopt::value> args =
      docopt::docopt(USAGE, {argv + 1, argv + argc},
                     true,           // show help if requested
                     "BaseRun 0.1"); // version string

   const std::string configRoot(args["CONFIGDIR"] ? args["CONFIGDIR"].asString() : "");

   base::Base base(configRoot);
   base.start();
   auto qtThread = std::thread([&base, &argc, &argv](){
      uiadapter::qt::startQt(base, argc, argv);
   });
   /*
   auto grpcThread = std::thread([&base](){
      uiadapter::googlerpc::startAsyncServer();
   });
   */
   qtThread.join();
}
