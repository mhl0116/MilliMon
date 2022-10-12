#include <iostream>
#include "include/tdrstyle.C"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO 
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h" // or "../stdout_sinks.h" if no colors needed
#include "spdlog/sinks/basic_file_sink.h"

float DIGITIZER_THRESHOLD = 20; // 20 mV

// turn digitizer channel (0-15) to row and column of bar detector
// https://milliqanelog.asc.ohio-state.edu:8080/MilliQanRun3/220907_184455/channelMapping.pdf
std::map<int, int> MAP_ROW{
    {0, 1}, {1, 2}, {4, 3}, {5, 4},
    {2, 1}, {3, 2}, {6, 3}, {7, 4},
    {8, 1}, {9, 2}, {12, 3}, {13, 4},
    {10, 1}, {11, 2}, {14, 3}, {15, 4}
};

std::map<int, int> MAP_COL{
    {0, 1}, {1, 1}, {4, 1}, {5, 1},
    {2, 2}, {3, 2}, {6, 2}, {7, 2},
    {8, 3}, {9, 3}, {12, 3}, {13, 3},
    {10, 4}, {11, 4}, {14, 4}, {15, 4}
};

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

void postprocess_triggerboard(TString runnum, TString subrunnum, spdlog::logger logger) {

    setTDRStyle();

    SPDLOG_LOGGER_INFO(&logger, "Argument passed for this function is {}, {}", runnum, subrunnum);
    TString filename = "/home/hmei/data/TriggerBoard_Run" + runnum + "." + subrunnum + ".root";
    TString savename = "TriggerBoard_Run" + runnum + "_Subrun" + subrunnum + "_2supermodules_ln";

    TFile * inputData = new TFile(filename);
    TTree * events = (TTree*)inputData->Get("Events");
    int nEvents = events->GetEntries();

    ULong_t trigger = 0;
    events->SetBranchAddress("trigger", &trigger);

    TH1F* h_triggerbits;
    h_triggerbits = new TH1F("h_triggerbits","",8,1,9);

    for (int i = 0; i < nEvents; i++) {
        events->GetEntry(i);
        for (int j = 0; j < 8; j++) {
            h_triggerbits->Fill( ((trigger >> j) == 1)? j+1 : 0 );
        }
    }

    TCanvas* c = new TCanvas("c", "", 800, 800);
    h_triggerbits->Draw();
    h_triggerbits->GetXaxis()->SetTitle("Trigger bit");
    c->SaveAs("./plots/" + savename + ".pdf");

}

//void postprocess_digitizer(TString filename) {
void postprocess_digitizer(TString runnum, TString subrunnum, spdlog::logger logger) {

    setTDRStyle();

    TFile* fout = new TFile("outputs/output_run"+runnum+"_"+subrunnum+".root", "RECREATE");

    //TString filename = "/home/hmei/data/MilliQan_Run478.1_default.root";
    //TString savename = "Run478_2supermodules_ln";
    SPDLOG_LOGGER_INFO(&logger, "Argument passed for this function is {}, {}", runnum, subrunnum);
    TString filename = "/home/hmei/data/MilliQan_Run" + runnum + "." + subrunnum + "_default.root";
    TString savename = "Digitizer_Run" + runnum + "_Subrun" + subrunnum + "_2supermodules_ln";

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

    /*
    SPDLOG_LOGGER_INFO(&logger, "Start process events with ROOT TTree->Draw()");
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
    c->Close();
    */

    TCanvas* c2 = new TCanvas("c2", "", 800, 800);
    c2->SetGrid();

    SPDLOG_LOGGER_INFO(&logger, "Start process events with event loop");
    TH2F * h_channel_heatmap = new TH2F("h_channel_heatmap", "", 16, 1, 17, 16, 1, 17);
    for(int i = 0; i < nEvents; i++){
        events->GetEntry(i);
        // looper over digitizers and channels <=> loop over bars, need a map
        for (int j = 0; j < 5; j++){//loop over digitizers
            for (int k = 0; k < 16; k++){
                // if Vmax < threshold, do nothing, else fill the 2D heatmap
                std::pair<int, float> max_pulse = largest(waveform[j][k], 1024, false);
                if (max_pulse.second > DIGITIZER_THRESHOLD) {
                    int row = MAP_ROW[k] + 4*j;
                    int col = MAP_COL[k] + 4*j;
                    h_channel_heatmap->Fill(row,col);
                }
            }
        }

    }


    //fout->WriteObject(c, "plot_VMax");
    //fout->WriteObject(h_channel_heatmap, "channel_heatmap");

    h_channel_heatmap->Draw("COLZ");
    h_channel_heatmap->Draw("TEXT same");
    savename = "DigitizerHeatMap_Run" + runnum + "_Subrun" + subrunnum + "_2supermodules_ln";
    c2->SaveAs("./plots/" + savename + ".pdf");

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

    if (argc != 3) {
        std::cout << Form("Number of arguement is %d, expect 3", argc) << std::endl;
        return 0;
    }

    TString runnum = argv[1];
    TString subrunnum = argv[2];
    postprocess_digitizer(runnum, subrunnum, logger);
    postprocess_triggerboard(runnum, subrunnum, logger);

    return 0;
}

