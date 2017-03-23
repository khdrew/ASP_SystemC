// asp.h
#ifndef ASP_H
#define ASP_H

#include "systemc.h"

template <int N> SC_MODULE(asp)
{
	enum State {Idle = 1, Wait_Data};

	sc_in<bool> valid;
	sc_in<sc_int<26> > data_in;
	sc_out<sc_int<16> > data_out;
	sc_out<bool> res_ready;
	sc_out<bool> busy;

	State current_state;
	int A[N];
	int B[N];
	int opcode;
	int data_count;
	int begin_address;
	int end_address;
	bool mem_sel;

	void asp_func();
	void store_init();
	void store_invoke();
	void process_data();

	SC_CTOR(asp)
	{
		SC_THREAD(asp_func);
			sensitive << valid.pos();
	}
};


#endif
