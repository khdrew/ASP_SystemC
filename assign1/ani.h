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
	// TO/FROM ASP
	sc_in<bool> res_ready;
	sc_in<bool> busy;
	sc_in<sc_uint<64> > from_asp;
	
	sc_out<sc_uint<26> > to_asp;
	sc_out<bool> valid;
	sc_out<bool> reset;

	// TO/FROM ANI
	sc_in<sc_uint<32> > d_to_NoC;
	
	sc_out<sc_uint<32> > d_from_NoC;

	// FIFO
	sc_fifo<int> fifo_from_NoC;
	sc_fifo<int> fifo_to_NoC;

	int instruction;
	
    void to_NoC_func();
    void from_NoC_func();
    
    SC_CTOR(ani)
    {
		SC_METHOD(from_NoC_func); // RECIEVE INSTRUCTION
			sensitive << d_from_NoC;
		SC_THREAD(to_NoC_func); // SENDING RESULT
			sensitive << res_ready << busy;

		sc_fifo<sc_uint<32>> d_to_NoC (10);
		sc_fifo<sc_uint<32>> d_from_NoC (10);
		reset = 0;
    }
};

#define ANI_H