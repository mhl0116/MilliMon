from triggerBoard import TriggerBoard
import time
import numpy as np
import matplotlib.pyplot as plt
import array

tb = TriggerBoard()

tb.trigger = 0b11111111
tb.dead_time = 100
tb.coincidence_time = 20
tb.nLayerThreshold = 3 #requirement for firing the gtNHitLayers trigger bit (greater than or equal to)
tb.nHitThreshold = 0 #requirement for firing the gtNHits trigger bit (greater than)
tb.prescale = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0] #fraction of events to pass for each trigger bit

tb.resetClock()
tb.setTrigger() #set trigger
tb.setTriggerMask()
tb.setPrescale()
tb.setDeadTime()
tb.setCoincidenceTime()
tb.setLayerThreshold()
tb.setHitThreshold()
tb.toggleOutputEnable([1])
tb.syncClock()

time.sleep(1)

#print ("Firmware version is {}".format(tb.getFirmware()) )

a_time = []
a_trigger = []
a_count = []

lasttime = 0
while True:

    trigger_time = tb.getClockCycles()
    for i in range(len(trigger_time)):

        trigger_time_i = trigger_time[i]
        #print (trigger_time_i)
        trigger_bits = hex(trigger_time_i)[-2:]
        timestamp = hex(trigger_time_i)[:-2]

        #print (timestamp, trigger_bits)

        if timestamp != "0x" and timestamp != '0':
            a_time.append(int(timestamp, base=16)/50.0e6)
            a_count.append(len(a_time))
        if trigger_bits != "00" and trigger_bits != 'x0':
            a_trigger.append(trigger_bits)

    if len(a_time) > 0:
        if lasttime != a_time[-1]:
            print (a_time[-1], a_trigger[-1])
            lasttime = a_time[-1]

    #time.sleep(5e-3)
    #print ('#####')

    if len(a_time) == 1000:
        break

x,y = np.array(a_time), np.array(a_count) 
# The actual curve fitting happens here
coef = np.polyfit(x,y,1)
poly1d_fn = np.poly1d(coef)
# poly1d_fn is now a function which takes in x and returns an estimate for y
plt.plot(x,y, 'yo', x, poly1d_fn(x), '--k') #'--k'=black dashed line, 'yo' = yellow circle marker
plt.text(4, 200, "fitted trigger rate: {:.3f} Hz".format(coef[0]))

#plt.xlim(0, 5)
#plt.ylim(0, 12)
plt.xlabel('Time stamp (s)')
plt.ylabel('Number of events')
plt.savefig("../plots/test_sim_triggerrate.png")

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
