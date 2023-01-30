# MilliMon

- gcc (GCC) 8.3.0
- cmake version 3.22.2
- required external packages
    + spdlog
    + ROOT
- extras
    + influxDB
    + grafana
    + telegraf
- python related
    + conda and pandas

## Basic structure

- cpp code to process ROOT files from digitizer and trigger board 
- python to parse and further process csv file, send to influxDB, then grafana
- need some sort of grafana scripting to manipulate info

## TODOs

- Add matching info, matching rate and/or trigger info matching to bit
- Should give all values going to grafana an initial value, in case something wrong with daq
- How to forward localhost at daqpc to some static page at, say my lxplus website
- Add diagnostic code to exmaine csv dataframe, could serve as basis for python based analysis
- HV/LV monitoring
- DAQ PC info
- Would be nice to monitoring data flux inside milliDAQ code
