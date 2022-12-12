#include <milliqan/lib.h>
#include <string>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main(int argc, char **argv) {
    
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");

    if (argc != 3) {

        spdlog::get("stderr")->info("Number of argument wrong, expect 2, get {}!", argc-1);

    } else {

        std::string runnum = argv[1];
        std::string subrunnum = argv[2];

        spdlog::get("console")->info("Start to process run {}, subrun {}", runnum, subrunnum);

        postprocess_digitizer(runnum, subrunnum);
        postprocess_triggerboard(runnum, subrunnum);

        // TODO, could also add an alternative to read a Chained TChain

    }

    return 0;
}

