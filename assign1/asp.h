// asp.h
#ifndef ASP_H
#define ASP_H

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


template <int N> SC_MODULE(asp)
{
	enum State {Idle = 1, Wait_Data};
	sc_in<bool> valid;
	sc_in<sc_int<26> > data_in;
	sc_out<sc_int<64> > data_out;
	sc_out<bool> res_ready;
	sc_out<bool> busy;


	State current_state;
	int opcode;
	int output;
	int A[N];
	int B[N];
	int target_addr;
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
