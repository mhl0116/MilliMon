import time
import subprocess
import socket
import json
import logging
import pandas as pd

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

formatter = logging.Formatter('%(asctime)s:%(name)s:%(message)s')

file_handler = logging.FileHandler('../logs/run_monitoring.log')
file_handler.setLevel(logging.INFO)
file_handler.setFormatter(formatter)

stream_handler = logging.StreamHandler()
stream_handler.setFormatter(formatter)

logger.addHandler(file_handler)
logger.addHandler(stream_handler)

def DumpToDB(info_to_dump):

    # send to db
    print('Sending data to influxDB...')
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(
            #json.dumps({'metric_name': 'milliqan_online_monitoring', 'value1': 10, 'value2': random.randint(1, 10)}).encode(),
            json.dumps(info_to_dump).encode(),
            ('localhost', 8094)
        )
        #print('Sending data to influxDB...')
        sock.close()

    except socket.error as e:
        print('Got error: {}'.format(e))

class MilliMon():

    root_file_path = "/home/milliqan/data/"
    digitizer_file_prefix = "MilliQan_Run"
    triggerboard_file_prefix = "TriggerBoard_Run"
    cpp_app_path = "/home/hmei/MilliAna/build/apps/apps"
    nLayers = 4
    nSuperModules = 4
    sleep = 5

    def __init__(self, tag):

        self.lastRunNum = -1
        self.lastSubrunNum = -1
        self.tag = tag
        if tag == "DGTZ":
            self.prefix = MilliMon.digitizer_file_prefix 
            logger.info(f"Start monitoring process for digitizer outputs")
        if tag == "TRGB":
            self.prefix = MilliMon.triggerboard_file_prefix 
            logger.info(f"Start monitoring process for triggerboard outputs")

    def GetRunNumber(self):

        logger.info(f"Start get run and subrun numbers")

        cmd_get_run_num = f"ls -lrht {MilliMon.root_file_path}{self.prefix}* | tail -n 1 | awk -F '_' '{{print $2}}' | awk -F '.' '{{print $1}}' | sed 's/[^0-9]*//g'"
        cmd_get_subrun_num = f"ls -lrht {MilliMon.root_file_path}{self.prefix}* | tail -n 1 | awk -F '_' '{{print $2}}' | awk -F '.' '{{print $2}}' | sed 's/[^0-9]*//g'"

        runnum = subprocess.run(cmd_get_run_num, stdout=subprocess.PIPE, shell=True)
        subrunnum = subprocess.run(cmd_get_subrun_num, stdout=subprocess.PIPE, shell=True)
        runnum_decoded =  runnum.stdout.decode('utf-8').strip('\n')
        subrunnum_decoded =  subrunnum.stdout.decode('utf-8').strip('\n')
    
        logger.info(f"Run number: {runnum_decoded}, subrun number: {subrunnum_decoded}")
        return runnum_decoded, subrunnum_decoded

    def RootFileChecker(self):
        '''
        Runs periodically, to check if new file from Digitizer and trigger board appear on disk
        If yes, send needed information (run, subrun number) to cpp Runner
        '''
        
        logger.info(f"Get last run and subrun number by checking {MilliMon.root_file_path}")
        tmp_lastRunNum, tmp_lastSubrunNum = -1, -1 
        while ( (tmp_lastRunNum == self.lastRunNum) and (tmp_lastSubrunNum == self.lastSubrunNum)):
            # no new file arrived yet, keep waiting
            if (tmp_lastRunNum != -1 and tmp_lastSubrunNum != -1):
                logger.info(f"New file not produced for run {self.lastRunNum} and subrun {self.lastSubrunNum}")
                logger.info(f"Wait for {MilliMon.sleep} second")
                time.sleep(MilliMon.sleep)
            tmp_lastRunNum, tmp_lastSubrunNum = self.GetRunNumber()

        self.lastRunNum = tmp_lastRunNum
        self.lastSubrunNum = tmp_lastSubrunNum
        logger.info(f"New file for run {self.lastRunNum} and subrun {self.lastSubrunNum} produced")

    def RunCppAnalyer(self):
        '''
        Run cpp based analyzer taking digitizer/trigger board output root files as input
        save (for now) csv files on disk for next stage influxdb/grafana and/or plotting
        '''
        logger.info(f"Run cpp analyzer for run {self.lastRunNum} and subrun {self.lastSubrunNum}")
        cmd_run_cpp_analyzer = f"{MilliMon.cpp_app_path} {self.lastRunNum} {self.lastSubrunNum} {self.tag}"
        subprocess.run(cmd_run_cpp_analyzer, shell=True)

    def DataGather(self):
        '''
        Process csv made by the cpp code
        1. Send organized data to influxDB
        2. Make plots when needed, and save somewhere
        ''' 
        if self.tag == "DGTZ":
            self.GatherDGTZData()
        if self.tag == "TRGB":
            self.GatherTRGBData()

    def GatherDGTZData(self):
        pass

        info_to_dump = {}
        # get DF
        # TODO set a project root path in the beginning
        fname = f"../data/Digitizer_run{self.lastRunNum}_subrun{self.lastSubrunNum}.csv"
        df = pd.read_csv(fname)

        for i in range(MilliMon.nLayers):
            info_to_dump[f"sb_mean_max_layer_{i}"] =  df[df.xxx == xxx].sb_mean.max
            info_to_dump[f"sb_rms_max_layer_{i}"] = 
            info_to_dump[f"nPulse_max_layer_{i}"] = 
            info_to_dump[f"pulse_height_max_layer_{i}"] = 
            info_to_dump[f"pulse_area_max_layer_{i}"] = 
            info_to_dump[f"pulse_duriation_max_layer_{i}"] = 
            info_to_dump[f"sb_mean_min_layer_{i}"] = 
            info_to_dump[f"sb_rms_min_layer_{i}"] = 
            info_to_dump[f"nPulse_min_layer_{i}"] = 
            info_to_dump[f"pulse_height_min_layer_{i}"] = 
            info_to_dump[f"pulse_area_min_layer_{i}"] = 
            info_to_dump[f"pulse_duriation_min_layer_{i}"] = 

            ## Use a new script or function to make plot from subRun i to i+N every 1 hour

        for i in range(MilliMon.nSuperModules):
            self.ProcessBla()

        # Plot 1-4, sideband_mean, sideband_rms for layer 1-4
        # Plot 5-8, pulse height max and min for layer 1-4
        # Plot 9-12, pulse area max and min for layer 1-4
        # Plot 13-16, pulse duriation max and min for layer 1-4
        # Plot 17-20, nPulse for layer 1-4
        # Then double the plot by 2 for supermodule 1-4

        # For now, 40 plots, each plot has 8 curves
        # For now, will have a python dictionary with 320 keys

        DumpToDB(info_to_dump)
        #DumpPlots()

    def GatherTRGBData(self):
        pass

mon = MilliMon("DGTZ")
mon.RootFileChecker()
mon.RunCppAnalyer()
mon.DataGather()
