from triggerBoard import TriggerBoard

tb = TriggerBoard()

#tb.resetClock()
#tb.setTrigger() #set trigger
#tb.setTriggerMask()
#tb.setPrescale()
#tb.setDeadTime()
#tb.setCoincidenceTime()
#tb.setLayerThreshold()
#tb.setHitThreshold()
#tb.toggleOutputEnable([1])
#tb.syncClock()

print ("Firmware version is {}".format(tb.getFirmware()) )
#print (tb.getClockCycles())

#out1  = []
#for i in range(2,10):
#    tb.setHistogram([i]) 
#    out1.extend(tb.readHistogram())
#
#print (out1)
#
#tb.setHistogram([0])
#out2 = tb.readHistogram()
#
#print (out2)
