#include <iostream>
#include "include/tdrstyle.C"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO 
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h" // or "../stdout_sinks.h" if no colors needed
#include "spdlog/sinks/basic_file_sink.h"

std::pair<int, float> largest(float arr[], int n, bool debug=false)
{
    int i;
    int imax = 0;
     
    // Initialize maximum element
    float max = arr[0];
 
    // Traverse array elements
    // from second and compare
    // every element with current max
    for (i = 1; i < n; i++) {
        if (debug) std::cout << i << ", " << arr[i] << std::endl;
        if (arr[i] > max) {
            max = arr[i];
            imax = i;
        }
    }
 
    return std::make_pair(imax, max);
}

//void postprocess_digitizer(TString filename) {
void postprocess_digitizer(TString runnum, spdlog::logger logger) {

    setTDRStyle();

    //TString filename = "/home/hmei/data/MilliQan_Run478.1_default.root";
    //TString savename = "Run478_2supermodules_ln";
    SPDLOG_LOGGER_INFO(&logger, "Argument passed for this function is {}", runnum);
    TString filename = "/home/hmei/data/MilliQan_Run" + runnum + ".1_default.root";
    TString savename = "Run" + runnum + "_2supermodules_ln";

    TFile * inputData = new TFile(filename);
    TTree * events = (TTree*)inputData->Get("Events");

    int nEvents = events->GetEntries();
    float vmax_per_channel[5][16]; // currently 4 digitizers, each has 16 channels
                                    
    Float_t waveform[5][16][1024] = {0};
    Int_t TDC[5][8] = {0};
    events->SetBranchAddress("digitizers.waveform[16][1024]", waveform);
    events->SetBranchAddress("digitizers.TDC[8]", TDC);

    TCanvas* c = new TCanvas("c", "", 1600, 1600);
    c->Divide(4,4);

    TH1F* h_waveforms[5][16];
    Float_t xbins[1000];

    int nbins = 10;
    int xlow = 0;
    int xhigh = 10;

    std::map<int, int> colors_per_digitizer { {1, 2}, {2, 8}, {3, 4}, {4, 5}, {5,1} };

    for (int i = 0; i < 16; i++) {
        //c->cd(i+1);
        TPad *p = (TPad*)c->cd(i+1);
        p->SetLogy();
        //p->SetLogx();
        auto legend = new TLegend(0.5,0.7,0.88,0.9);
        for (int j = 0; j < 5; j++) {

            h_waveforms[j][i] = new TH1F(Form("h_waveform_digi%d_channel%d", j+1, i+1),"",nbins,xlow,xhigh);
            //h_waveforms[j][i] = new TH1F(Form("h_waveform_digi%d_channel%d", j+1, i+1),"",nbins,xbins);

            SPDLOG_LOGGER_INFO(&logger, "Process digitizer {}, channel {}", j, i);
            events->Draw(Form("log( Max$(digitizers.waveform[%d][%d]) )>>%s", j,i,h_waveforms[j][i]->GetName()), "1", "goff"); 
            h_waveforms[j][i]->SetLineColor(colors_per_digitizer[j+1]);
            h_waveforms[j][i]->SetMinimum(0.1);
            h_waveforms[j][i]->GetXaxis()->SetTitle("Log_{e}(Vmax) (mV)");
            h_waveforms[j][i]->GetYaxis()->SetTitle("Events");
            h_waveforms[j][i]->Draw("same");

            legend->AddEntry(h_waveforms[j][i]->GetName(),Form("Digitizer %d, channel %d", j+1, i+1),"l");
        }
        legend->Draw("same");

    }

    //c->SaveAs("./plots/" + savename + ".png");
    c->SaveAs("./plots/" + savename + ".pdf");


}

int main(int argc, char **argv) {

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern("[source %s] [%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
    //file_sink->set_level(spdlog::level::trace);
    file_sink->set_level(spdlog::level::info);

    spdlog::sinks_init_list sink_list = { file_sink, console_sink };

    spdlog::logger logger("multi_sink", sink_list.begin(), sink_list.end());
    logger.set_level(spdlog::level::info);
    logger.set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%4!l%$] [%s:%!:%#] %v");

    for (int i = 0; i < argc; i++) {
        SPDLOG_LOGGER_INFO(&logger, "Argument {} for this program is: {}", i, argv[i]);
    }

    if (argc != 2) {
        std::cout << Form("Number of arguement is %d, expect 2", argc) << std::endl;
        return 0;
    }

    TString runnum = argv[1];
    postprocess_digitizer(runnum, logger);

    return 0;
}

/*
TCanvas* c2 = new TCanvas("c2", "", 1600, 1600);
TH1F* h_channelmap_max0 = new TH1F("h_channelmap_max0","",17,1,18);
for(int i = 0; i < nEvents; i++){
    events->GetEntry(i);

    TString TDC_local = Form("TDC for event %d", i+1);
    for (int j = 0; j < 5; j++) {

        TDC_local += Form(" %d", TDC[j][0]);

        for (int k = 0; k < 16; k++) {
            //pair<int, float> max_pulse = largest(waveform[0][0], 1024, true);
            //if (j != 1 || k != 3) continue;
            pair<int, float> max_pulse = largest(waveform[j][k], 1024, false);
            cout << "max voltage for event " << i+1 << ", digitizer " << j+1 << ", channel " << k+1 << " is: " << max_pulse.second << " at position " << max_pulse.first << endl;
            if (max_pulse.second == 0 && j == 2) h_channelmap_max0->Fill(k+1);
        }
    }
    //cout << TDC_local << endl;

}
h_channelmap_max0->Draw("COLZ");
c2->SaveAs("./plots/h_channelmap_max0.pdf");
*/
