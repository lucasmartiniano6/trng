//Author: Lucas Martiniano <lucasmartiniano@protonmail.com>
//Date  : 28-10-2020
//
//WINDOWS IMPLEMENTATION TO RECEIVE SERIAL DATA AND WRITE IT TO A .BIN FILE

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

//SERIAL MACROS
#define BAUDRATE 250000     //baud in bps
#define BYTES_MAX 20000     // number Of bytes to receive before closing connection

//PROGRESS BAR MACROS
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

//update log file accordinly
void update_log(FILE *logfile, double time, int datasize, int stopbits, int parity){
    fprintf(logfile, "\nbaudrate %i", BAUDRATE);
    fprintf(logfile, "\nbps      %.3f", (double)(BYTES_MAX*8)/time);
    fprintf(logfile, "\ntime     %.3f", time);
    fprintf(logfile, "\ndatasize %i", datasize);
    fprintf(logfile, "\nstopbits %i", stopbits);
    fprintf(logfile, "\nparity   %i", parity);
}

//update the progress bar accordingly
void printProgress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

//receives the serial data, dumps it into a binary file and return duration of time
double start_recv(HANDLE hComm, FILE* outFile)
{
    uint8_t tempChar;            //Temporary byte used to store the data
    DWORD bytesRead;             //Number of bytes read
    unsigned int countBytes = 0; //Counter

    float currentPercentage = 0.0;        //Progress Bar percentage 0.0-1.0
    const float unitPer= 1.0 / BYTES_MAX; //just the basic math for percentage

    clock_t tStart = clock(); //measure start time

    do
    {
        ReadFile( hComm,            //Handle of the Serial port
             &tempChar,             //Temporary byte
             sizeof(tempChar),      //Size of tempChar
             &bytesRead,            //Number of bytes read
             NULL);

        fputc(tempChar, outFile);   //Write byte to file
        countBytes++;               //Increment counter

        currentPercentage = countBytes * unitPer; //Basic percentage math
        printProgress(currentPercentage);

        if (countBytes>=BYTES_MAX){          //byte limit reached
            return ( (double) (clock()-tStart) / CLOCKS_PER_SEC ); //measure and return time elapsed in seconds
        }
    }while (bytesRead > 0);

}

int main()
{

    HANDLE hComm; //main serial communication structure

    hComm = CreateFile("\\\\.\\COM4",                 //port name
                      GENERIC_READ | GENERIC_WRITE, //Read/Write
                      0,                            // No Sharing
                      NULL,                         // No Security
                      OPEN_EXISTING,// Open existing port only
                      0,            // Non Overlapped I/O
                      NULL);        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE)
        printf("Error in opening serial port\n");
    else
        printf("Serial port opened successfully\n");

    DCB dcbSerialParams = { 0 }; // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    GetCommState(hComm, &dcbSerialParams); //GET CURRENT STATE

    dcbSerialParams.BaudRate = BAUDRATE;  // Setting BaudRate (bps)
    dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
    dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None

    SetCommState(hComm, &dcbSerialParams); //SET NEW STATE


    FILE *outFile = fopen("outputSerial.bin", "wb"); //create binary file

    FILE *logfile = fopen("specs.log", "w"); //create log file

    double durationSec = start_recv(hComm, outFile); //receive data from COM - return duration of time in seconds

    printf("\nTime elapsed: %.3f seconds", durationSec);

    fclose(outFile); //close binary file
    printf("\nBinary file updated - %i bytes written", BYTES_MAX );

    CloseHandle(hComm);//Closing the Serial Port
    printf("\nConnection terminated");

    update_log(logfile, durationSec, dcbSerialParams.ByteSize, dcbSerialParams.StopBits,
            dcbSerialParams.Parity);

    fclose(logfile);

    return 0;
}
