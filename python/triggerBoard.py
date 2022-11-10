#!/usr/bin/env python2

import time
import os
from serial import Serial
from array import array
import sys
import importlib
#import numpy as np
#import ROOT
#from ROOT import TFile

class TriggerBoard():

    def __init__(self):
        self.ser = Serial("/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0",921600,timeout=1)
        self.trigger = 0b0000011
        self.outputFile = open("/home/milliqan/MilliDAQ/clock_out.txt", "a")
        self.prescale = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0] #fraction of events to pass for each trigger bit
        self.dead_time = 12 #clk ticks (8ns each) 125MHz clock
        self.coincidence_time = 12 #clk ticks (8ns each)
        self.trigger_mask = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]  #masking LVDS channels 0-63, each byte from "00" (masked) to "ff" (unmasked) controls masking of 8 channels
        self.nLayerThreshold = 1 #requirement for firing the gtNHitLayers trigger bit (greater than or equal to)
        self.nHitThreshold = 1 #requirement for firing the gtNHits trigger bit (greater than)

    def writeToFile(self, text):
        self.outputFile.write(text + '\n')

    def serialWrite(self, num):
        if hasattr(num, "__len__"):
            self.ser.write(bytearray(num))
        else:
            self.ser.write(bytearray([num]))

    def serialRead(self, numbytes):
        out = self.ser.read(numbytes)
        out = bytearray(out)
        return out

    def serialInsert(self, arg, extra):
        if not isinstance(arg, list):
            arg = [arg]
        arg.insert(0, extra)
        return arg

    def IsValid(self):
        #dumb check to make sure serial is active
        #if not self.getFirmware() == 8:
        #   return False
        return True

    def getFirmware(self):
        self.serialWrite(0)
        out = self.serialRead(1)
        self.writeToFile("got firmware version " + str(out[0])) 
        return out[0]

    def setCoincidenceTime(self, time=-1):
        if(time<0): time = self.coincidence_time
        time = self.serialInsert(time, 1)
        self.serialWrite(time)

    def setHistogram(self, histo):
        histo.insert(0, 2)
        self.serialWrite(histo)

    def toggleOutputEnable(self, toggle):
        toggle.insert(0, 3)
        self.serialWrite(toggle)
        #self.serialWrite(3)
        out = self.serialRead(1)
        return out[0]

    def toggleClockInputs(self):
        self.serialWrite(4)

    def adjustClockPhases(self):
        self.serialWrite(5)

    def setSeed(self, seed):
        seed.insert(0,6)
        self.serialWrite(seed)
    
    def setPrescale(self, prescale=-1):
        if(prescale<0): prescale = self.prescale
        psOut = []
        for scale in prescale:
            if scale > 1.0 or scale < 0.0:
                    print("bad prescale value,", scale)
                    return
            prescaleint = int((pow(2, 32) - 1) * scale)
            b4 = int(prescaleint / 256 / 256 / 256) % 256
            b3 = int(prescaleint / 256 / 256) % 256
            b2 = int(prescaleint / 256) % 256
            b1 = int(prescaleint) % 256
            psOut.extend([b1, b2, b3, b4])
            print("prescale int:", prescaleint, "Prescale:", b1, b2, b3, b4)
            print("bits sent to trigger board", psOut)
        psOut = self.serialInsert(psOut,7)
        self.serialWrite(psOut)

    def getActiveClock(self):
        self.serialWrite(8)
        out = self.serialRead(1)
        self.writeToFile("Active Clock is" + str(out[0]))
        return out[0]

    def togglePhase(self):
        self.serialWrite(9)

    def readHistogram(self):
        self.serialWrite(10)
        out = self.serialRead(32)
        #print(out)
        output = []
        for i in range(8):
            output.append(out[4*i+3] << 24 | out[4*i+2] << 16 | out[4*i+1] << 8 | out[4*i])
            
        return output

    def setDeadTime(self, time=-1):
        if(time < 0): time = self.dead_time
        time = self.serialInsert(time,11)
        self.serialWrite(time)

    def adjustClockPhaseC1(self):
        self.serialWrite(12)

    def toggleTriggerRolling(self):
        self.serialWrite(13)

    def setTriggerMask(self, mask=-1):
        if(mask < 0): 
            mask = self.trigger_mask
        mask = self.serialInsert(mask, 14)
        self.serialWrite(mask)

    def setTrigger(self, trigger=-1):
        if(trigger>=0): self.trigger = trigger
        if(trigger<0): trigger = self.trigger
        trigger = self.serialInsert(trigger, 15)
        self.serialWrite(trigger)

    def getClockCycles(self):
        self.serialWrite(16)
        out = self.serialRead(64)
        output = []
        if len(out) != 64:
            output.append(0)
            print("Error did not get 64 bytes")
        for i in range(8):
            output.append(out[8*i+7] << 56 | out[8*i+6] << 48 | out[8*i+5] << 40 | out[8*i+4] << 32 | out[8*i+3] << 24 | out[8*i+2] << 16 | out[8*i+1] << 8 | out[8*i])
        return output

    def resetClock(self):
        self.serialWrite(17)

    def syncClock(self):
        self.serialWrite(18)
        out = self.serialRead(7)
        output = out[6] << 48 | out[5] << 40 | out[4] << 32 | out[3] << 24 | out[2] << 16 | out[1] << 8 | out[0]
        return output

    def setLayerThreshold(self, layers=-1):
        if(layers < 0):
            layers = self.nLayerThreshold
        layers = self.serialInsert(layers, 19)
        self.serialWrite(layers)

    def setHitThreshold(self, hits=-1):
        if(hits < 0):
            hits = self.nHitThreshold
        hits = self.serialInsert(hits, 20)
        self.serialWrite(hits)
        
    def setZeroBiasTrigger(self, clock_cycles=-1):
        if(clock_cycles < 0):
            clock_cycles = 0
        clockOut = []
        b4 = int(clock_cycles[0] / 256 / 256 / 256) % 256
        b3 = int(clock_cycles[0] / 256 / 256) % 256
        b2 = int(clock_cycles[0] / 256) % 256
        b1 = int(clock_cycles[0]) % 256
        clockOut.extend([b1, b2, b3, b4])
        clockOut = self.serialInsert(clockOut, 21)
        self.serialWrite(clockOut)


def openTriggerBoard(name):
    if not isinstance(name, str): return "ERROR"
    trigFile = name
    trigName = trigFile.split("/")[-1]
    if(trigName.endswith(".py")): trigName = trigName.replace('.py', '')
    mod = importlib.import_module(trigName)
    tb = getattr(mod, "triggerBoard")
    out = tb.getFirmware()
    return out

def runClass(num, extra=[]):
    
    triggerBoard = TriggerBoard()

    #this gets the firmware version
    if(num == 0): 
        triggerBoard.writeToFile('Output 0 selected')
        out = triggerBoard.getFirmware()
        return out

    #set coincidene time
    elif(num == 1): 
        if(len(extra) > 0):
            triggerBoard.writeToFile('Output 1 selected')
            triggerBoard.setCoincidenceTime(extra)
        else:
            triggerBoard.setCoincidenceTime()

    #set the histogram to be read out
    elif(num == 2): 
        triggerBoard.setHistogram(extra)

    #toggle output enable   
    elif(num == 3): 
        triggerBoard.toggleOutputEnable(extra)

    #toggle clock input
    elif(num == 4):
        triggerBoard.toggleClockInputs()

    #adjust clock phases (for all clocks)
    elif(num == 5):
        triggerBoard.adjustClockPhases()

    #set random seed 
    elif(num == 6):
        triggerBoard.setSeed(extra)

    #set prescale
    elif(num == 7):
        if(len(extra)>0):
            triggerBoard.setPrescale(extra)
        else:
            triggerBoard.setPrescale()

    #get active clock
    elif(num == 8):
        out = triggerBoard.getActiveClock()
        return out

    #toggle phase up/down
    elif(num == 9):
        triggerBoard.togglePhase()

    #read active histogram
    elif(num == 10):
        out = triggerBoard.readHistogram()
        return out

    #set dead time
    elif(num == 11):
        if(len(extra) > 0):
            triggerBoard.setDeadTime(extra)
        else:
            triggerBoard.setDeadTime()

    #adjust clock phase (clock c1)
    elif(num == 12):
        triggerBoard.adjustClockPhaseC1()

    #toggle rolling of triggers
    elif(num == 13):
        triggerBoard.toggleTriggerRolling()

    #set the trigger mask
    elif(num == 14):
        if(len(extra)>0):
            triggerBoard.setTriggerMask(extra)
        else:
            triggerBoard.setTriggerMask()

    #set active trigger from config file
    #NOTE: everything set by trigger board config file should be set here!
    elif(num == 15):
        if(len(extra)>0):
            if(isinstance(extra[0], str)): 
                trigFile = extra[0]
                trigName = trigFile.split("/")[-1]
                if(trigName.endswith(".py")): trigName = trigName.replace('.py', '')
                mod = importlib.import_module(trigName)
                tb = getattr(mod, "triggerBoard")
                tb.setTrigger() #set trigger
                tb.setTriggerMask()
                tb.setPrescale()
                tb.setDeadTime()
                tb.setCoincidenceTime()
                tb.setLayerThreshold()
                tb.setHitThreshold()
                return tb.trigger
            if(isinstance(extra[0], int)):
                triggerBoard.setTrigger(extra)
                return triggerBoard.trigger
        else:
            triggerBoard.setTrigger()
            return triggerBoard.trigger


    #get clock cycles and last trigger fired
    elif(num == 16):
        n = 0
        out = triggerBoard.getClockCycles()
        return out

    #reset trigger
    elif(num == 17):
        triggerBoard.resetClock()
        triggerBoard.writeToFile("Reset Clock!!!!!")
    
    #get start time
    elif(num == 18):
        out = triggerBoard.syncClock()
        return out
    
    #set nLayerThreshold, number of layers that must be hit to trigger
    elif(num == 19):
        if(len(extra)>0):
            triggerBoard.setLayerThreshold(extra)
        else:
            triggerBoard.setLayerThreshold()

    #set nHitThreshold, number of total hits to trigger
    elif(num == 20):
        if(len(extra)>0):
            triggerBoard.setHitThreshold(extra)
        else:
            triggerBoard.setHitThreshold()

    elif(num == 21):
        if(len(extra)>0):
            triggerBoard.setZeroBiasTrigger(extra)
        else:
            triggerBoard.setZeroBiasTrigger()

    #read out the LVDS histograms
    elif(num == 98):
        out  = []
        for i in range(2,10):
            triggerBoard.setHistogram([i]) 
            out.extend(triggerBoard.readHistogram())
        return out
    
    #read out the trigger bit histograms
    elif(num == 99):
        triggerBoard.setHistogram([0])
        out = triggerBoard.readHistogram()
        return out

    else:
        triggerBoard.writeToFile("Number: " + str(num) + "is not an option")
        return -1


    return 0



if __name__ == "__main__":

    runClass(15, ['../config/triggerConfig/triggerDefault.py'])
    

    runClass(1)
    out = runClass(17)
    out = runClass(18)
    print("run start time:", out, type(out))

    #set hisgram (2) to channel [n] 
    runClass(2, [0])    
    #read out histogram
    out = runClass(10)
    print("Histogram info:", out, type(out))

    runClass(2, [1])
    out = runClass(10)
    print("Histogram info:", out, type(out))

    for i in range(2,10):
        runClass(2, [i])
        out = runClass(10)
        print("LVDS triggers", out)

    out = runClass(98)
    print(out)

    out = runClass(99)
    print(out)

    runClass(2, [0])

     
    j=0
    while(j<1):
        out = runClass(16)
        print(out, type(out))
        for i in range(8):
            print(format(out[i], '064b'))
            trigger = format(out[i], '064b')[:8]
            clock = format(out[i], '064b')[8:]
            print("Clock cycles and last trigger fired " + str(int(clock, 2)) + ', ' + str(trigger))
        j+=1


    tb = TriggerBoard()
    print("is valid: ", tb.IsValid())
