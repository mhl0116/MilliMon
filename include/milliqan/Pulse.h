#ifndef Pulse_h
#define Pulse_h

#include "milliqan/mappings.h"
#include <iostream>     

class Pulse {
public : 

    float height;
    float area;
    float duriation;
    float pulseTime;
    float delay;

    float TDC;
    float TDCRollovers;

    int channel;
    int supermodule;
    int layer;
    int row;
    int column;

    Pulse();
    Pulse(Mappings mappings, int digitizerID, int channelID);
    virtual ~Pulse();
    void SetPulseProperty(float ht, float a, float dur, float pulseT, float dly);
    void SetTDC(float TDC, float TDCRollovers);
    void Print();
    bool IsShortPulse();

};

#endif
