add_executable(BaseRun "main.cpp")
target_link_libraries(BaseRun
   Base
   docopt
   spdlog::spdlog
)