#Author: Lucas Martiniano <lucasmartiniano@protonmail.com>
##Date  : 29-10-2020
##
##RUN A TIME SERIES DATA ANALYSIS ON A GIVEN BINARY FILE

#importing all necessary plotting libraries
import matplotlib.pyplot as plt
from numpy import uint8
from pandas.plotting import lag_plot
from pandas.plotting import autocorrelation_plot
from pandas import Series


def CreatePlots():
    #crete main figure
    fig = plt.figure(figsize=(19.0, 10.0))
    fig.suptitle('TIME SERIES ANALYSIS - {}'.format(argv[1]), fontsize=18)
    
    #subplots creation
    runSequence = plt.subplot2grid((13,11), (0,0), rowspan=6, colspan=5)
    histogram = plt.subplot2grid((13,11), (0,6), rowspan=6, colspan=5)
    lag = plt.subplot2grid((13,11), (7,0), rowspan=6, colspan=5)
    auto = plt.subplot2grid((13,11), (7,6), rowspan=6, colspan=5)

    return fig, runSequence, histogram, lag, auto

def RunSeqPlot(runSequence):
    #RUN SEQUENCE PLOT
    runSequence.plot(data[:1024], marker='.', markersize=4, linewidth=1)
    runSequence.set_xlabel('data index[]', fontsize=14)
    runSequence.set_ylabel('byte value', fontsize=14)

def HistrogramPlot(histogram): #takes way too much time; use threads? gpu?
    #HISTOGRAM PLOT
    histogram.hist(data)
    histogram.set_xlabel('byte value', fontsize=14)
    histogram.set_ylabel('frequency', fontsize=14)

def LagPlot(lag):
    #LAG PLOT
    dataSeries = Series(data[:4096], dtype=uint8)
    lag_plot(dataSeries, ax=lag, marker='.', s=9.5)

def AutocorrelationPlot(auto):
    #AUTOCORRELATION PLOT
    autocorrelation_plot(data[:1024], ax=auto)

def ChiSquared():
    #PERFORMING CHI-SQUARED TEST
    import chisquaredtest
    resultChi = chisquaredtest.initialize(argv[1])
    
    print('Chi Squared Test Results:')
    for topic in resultChi:
    #    print('{} - {}'.format(topic, resultChi[topic]))
        pass

def StartSubplots():
    fig, runSequence, histogram, lag, auto = CreatePlots()
    
    RunSeqPlot(runSequence)
    HistrogramPlot(histogram)
    LagPlot(lag)
    AutocorrelationPlot(auto)

    return fig

#getting .BIN file path & data
from sys import argv
with open(argv[1], 'rb') as f:
    data = bytearray(f.read())

fig = StartSubplots()
   
#show everything together
plt.show()

#SAVING 
from time import strftime
outFile = strftime("%Y %m %d - %H %M %S")

#fig.savefig("{}.png".format(outFile))

##timestamps
## t0 - t1
#print('t0-t1: {:.5f}'.format(t1-t0))
## t1 - t2
#print('t1-t2: {:.5f}'.format(t2-t1)) #*****
## t2 - t3
#print('t2-t3: {:.5f}'.format(t3-t2))
## t3 - tfinal
#print('t3-t4: {:.5f}'.format(tfinal-t3)) #*
##total
#print('total time: {}'.format(tfinal-t0))
