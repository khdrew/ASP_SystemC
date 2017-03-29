// ani.h
#ifndef ANI_H
#define ANI_H

#include "systemc.h"

#define STORE_INIT 0
#define STORE 1
#define XOR_A 2
#define XOR_B 3
#define MAC 4
#define AVE4_A 5
#define AVE4_B 6
#define AVE8_A 7
#define AVE8_B 8

SC_MODULE(ani)
{
	sc_in<bool> res_ready;
	sc_in<bool> busy;
	sc_in<sc_int<64> > from_asp;
	sc_in<sc_fifo<sc_int<32>> > D_to_NoC;

	sc_out<sc_int<26> > to_asp;
	sc_out<sc_fifo<sc_int<32>> > D_from_NoC;
	sc_out<bool> valid;
	sc_out<bool> reset;

	sc_int<32> output_NoC;
	int instruction;	
	int opcode;

    void ani_func();
    
    SC_CTOR(ani)
    {
		SC_METHOD(ani_func);
			sensitive << res_ready << busy << D_from_NoC;

    }
};

#define ANI_H