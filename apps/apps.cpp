#include <milliqan/lib.h>
#include <string>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main(int argc, char **argv) {
    
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");

    if (argc != 4) {

        spdlog::get("stderr")->info("Number of argument wrong, expect 3, get {}!", argc-1);

    } else {

        std::string runnum = argv[1];
        std::string subrunnum = argv[2];
        std::string type = argv[3];

        spdlog::get("console")->info("Start to process run {}, subrun {}", runnum, subrunnum);

        if (type == "DGTZ" || type == "ALL") {
            postprocess_digitizer(runnum, subrunnum);
        }
        if (type == "TRGB" || type == "ALL") {
            postprocess_triggerboard(runnum, subrunnum);
        }

        // TODO, could also add an alternative to read a Chained TChain

    }

    return 0;
}

