// asp.h
#ifndef ASP_H
#define ASP_H

#include "systemc.h"

template <int N> SC_MODULE(asp)
{
	enum State {Idle = 1, Wait_Data};
	enum Opcode {Store_Init = 0, Store, XOR_A,
	XOR_A, MAC, AVE4_A, AVE4_B, AVE8_A, AVE8_B};

	sc_in<bool> valid;
	sc_in<sc_int<26> > data_in;
	sc_out<sc_int<64> > data_out;
	sc_out<bool> res_ready;
	sc_out<bool> busy;

	State current_state;
	Opcode opcode;
	int A[N];
	int B[N];
	int data_count;
	int begin_addr;
	int end_addr;
	bool mem_sel;

	void asp_func();
	void store_init();
	void store_invoke();
	void process_data();
	void xor_func();
	void mac_func();
	void ave4_func();
	void ave8_func();


	SC_CTOR(asp)
	{
		SC_THREAD(asp_func);
			sensitive << valid.pos();
	}
};


#endif
