#include <bifrost/array.h>
#include <bifrost/common.h>
#include <bifrost/ring.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

//Global Variables
#define FREQUENCY_CHANNELS 16
#define TIME_SAMPLES 256
#define RATE_Gbps 8

uint8_t generatePacket(uint32_t * data_o){
    static uint8_t value = 23;
    for(int i = 0; i<FREQUENCY_CHANNELS*TIME_SAMPLES;i++){
        data_o[i] = value++;
    }
    return 1;
}

uint8_t generatePackets(){
    uint32_t packets = 0;

    double required_rate = (double)RATE_Gbps*1024*1024*1024;
    double packet_size = FREQUENCY_CHANNELS*TIME_SAMPLES*sizeof(uint32_t)*8;
    double seconds_per_packet = packet_size/required_rate;
    printf("%f %f %f\n",packet_size,required_rate,seconds_per_packet);
    clock_t begin_benchmark = clock();
    clock_t startPacketCreate = clock();
    while(1){
        uint32_t * data_out = new uint32_t[FREQUENCY_CHANNELS*TIME_SAMPLES];
        generatePacket(data_out);
        packets++;
        if(packets%100000==0){
            clock_t end_benchmark = clock();
            double time_spent = (double)(end_benchmark - begin_benchmark) / CLOCKS_PER_SEC;
            double rate = ((double)100000*FREQUENCY_CHANNELS*TIME_SAMPLES*sizeof(uint32_t)*8)/time_spent/1024/1024/1024;
            printf("Rate: %f Gbps, Packets: %i, Time: %fs\n",rate,packets, time_spent);
            begin_benchmark = clock();
        }    
        delete[] data_out;

        clock_t endPacketCreate = clock();
        double time_passed = (double)(endPacketCreate - startPacketCreate) / CLOCKS_PER_SEC;
        while(time_passed < seconds_per_packet){
            endPacketCreate = clock();
            time_passed = (double)(endPacketCreate - startPacketCreate) / CLOCKS_PER_SEC;
        }
        startPacketCreate = clock();
        
    }
}
int main()
{
    // printf() displays the string inside quotation
    printf("Hello Worldasd!\n");
    generatePackets();
    return 0;
}

