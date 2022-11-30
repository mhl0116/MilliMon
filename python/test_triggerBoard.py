from triggerBoard import TriggerBoard
import time
import numpy as np
import matplotlib.pyplot as plt
import array
import datetime

nEvents = 4000000

tb = TriggerBoard()

tb.trigger = 0b11111111
#tb.trigger = 0b01000000
tb.dead_time = 100
tb.coincidence_time = 20
tb.nLayerThreshold = 3 #requirement for firing the gtNHitLayers trigger bit (greater than or equal to)
tb.nHitThreshold = 3 #requirement for firing the gtNHits trigger bit (greater than)
#tb.prescale = [1.0, 0.01, 1.0, 0.01, 0.01, 0.01, 0.1, 1.0] #fraction of events to pass for each trigger bit
tb.prescale = [1.0, 0.001, 1.0, 0.001, 0.01, 0.01, 0.1, 1.0] #fraction of events to pass for each trigger bit
#tb.prescale = [1.0]*8 #fraction of events to pass for each trigger bit

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

l_time = [[]]*5 
l_trigger = [[]]*5 
l_count = [[]]*5 

a_time = [] 
a_trigger = [] 
a_count = [] 

my_hexdata = "40"
num_of_bits = 8
scale = 16
#bin(int(my_hexdata, scale))[2:].zfill(num_of_bits)

#lasttime = 0

fw = open("test.txt", "w")

while True:

    trigger_time = tb.getClockCycles()
    for i in range(len(trigger_time)):

        trigger_time_i = trigger_time[i]
        trigger_bits = hex(trigger_time_i)[-2:]
        timestamp = hex(trigger_time_i)[:-2]

        if timestamp != "0x" and timestamp != '0':
            a_time.append(int(timestamp, base=16)/50.0e6)
            a_count.append(len(a_time))
        if trigger_bits != "00" and trigger_bits != 'x0':
            a_trigger.append(trigger_bits)

    if len(a_time) % 1000 == 0:
        print ("{}: Acquired {} events already, last fired triggers {}.".format( datetime.datetime.now(),  len(a_time), a_trigger[-1] ))

    if len(a_time) == nEvents and len(a_trigger) == nEvents:
        for i in range(len(a_trigger)):
            fw.write("{} {}\n".format(a_time[i], a_trigger[i]) )
        break

'''
x,y = np.array(a_time), np.array(a_count) 
coef = np.polyfit(x,y,1)
poly1d_fn = np.poly1d(coef)
plt.plot(x,y, 'yo', x, poly1d_fn(x), '--k') #'--k'=black dashed line, 'yo' = yellow circle marker
plt.text(4, 200, "fitted trigger rate: {:.3f} Hz".format(coef[0]))

#plt.xlim(0, 5)
#plt.ylim(0, 12)
plt.xlabel('Time stamp (s)')
plt.ylabel('Number of events')
plt.savefig("../plots/test_sim_triggerrate_prescale0.1.png")
'''
