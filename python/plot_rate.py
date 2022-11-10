#open input file
#read line by line time and trigger bit
#plot and fit

def fill_array(timestamp, triggerbit_decoded, l_time, l_count, index):

    if triggerbit_decoded[index] == '1':

        l_time[7-index].append(float(timestamp))

        if len(l_count[7-index]) == 0:
            l_count[7-index].append(1)
        else:
            l_count[7-index].append( l_count[7-index][-1] + 1)

fname_in = open("test_long.txt", "r")
content_in = fname_in.readlines()

l_time = [[] for _ in range(8)] 
l_count = [[] for _ in range(8)] 

#my_hexdata = "40"
num_of_bits = 8
scale = 16
#bin(int(my_hexdata, scale))[2:].zfill(num_of_bits)

for i in range(len(content_in)):
    timestamp, triggerbit = content_in[i].strip('\n').split() 
    triggerbit_decoded = bin(int(triggerbit[1:], scale))[2:].zfill(num_of_bits)

    for j in range(8):
        fill_array(timestamp, triggerbit_decoded, l_time, l_count, j)

for i in range(8):
    print ("number of triggers for bit {} is {}".format(i+1, len(l_time[i])))
