#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <utility>
// define your own namespace to hold constants
namespace constants
{

    /*
    if(samFreq == 3.2) {
      dgtz.SAMFrequency = 0L;
      dgtz.secondsPerSample = 1. / (3.2e9);
    }
    else if(samFreq == 1.6) {
      dgtz.SAMFrequency = 1L;
      dgtz.secondsPerSample = 1. / (1.6e9);
    }
    else if(samFreq == 0.8) {
      dgtz.SAMFrequency = 2L;
      dgtz.secondsPerSample = 1. / (0.8e9);
    }
    else if(samFreq == 0.4) {
      dgtz.SAMFrequency = 3L;
      dgtz.secondsPerSample = 1. / (0.4e9);
    }
    */
    inline constexpr int nDigitizers {5}; 
    inline constexpr int nChannels {16}; 
    inline constexpr int nSamples {1024}; 
    inline constexpr int SAMFrequency {3};  
    inline constexpr double nanosecondsPerSample {(1.0 / 3.2) * (1 << SAMFrequency)}; 

    inline constexpr std::pair sideband_range {std::make_pair(0,50)};  
    inline constexpr int nConsecSamples {3};  
    inline constexpr int nConsecSamplesEnd {3};  
    inline constexpr int lowThresh {1};  
    inline constexpr int highThresh {10};  
    // ... other related constants
    inline constexpr int nTriggerBits {8};  
    inline constexpr double secondsPerCycle {20.0/1e9}; 

}
#endif
