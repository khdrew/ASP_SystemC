// ani.h
#ifndef ANI_H
#define ANI_H

#include "systemc.h"
#include "queue"
#define NOT_STORING 0
#define STORING 1

using namespace std;

SC_MODULE(ani)
{
	// TO/FROM ASP
	sc_in<bool> res_ready;
	sc_in<bool> busy;
	sc_in<sc_int<64> > from_asp;
	sc_out<sc_int<26> > to_asp;
	sc_out<bool> valid;
	sc_out<bool> reset;

	// TO/FROM ANI
	sc_in<bool> pop;
	sc_in<sc_int<32> > d_from_NoC;
	sc_out<sc_int<32> > d_to_NoC;

	int state;
	int instruction;
	int t_instruction;
	int data_count;
	int t_packet;
	queue<int> queue_to_NoC;
	
	void pop_queue();
	void to_NoC_func();
	void from_NoC_func();
	void prep_t_packet();
	void send_to_asp();

	SC_CTOR(ani)
	{
		SC_THREAD(to_NoC_func); // SENDING RESULT
			sensitive << res_ready << busy;
		SC_METHOD(from_NoC_func); // RECIEVE INSTRUCTION
			sensitive << d_from_NoC;
		SC_METHOD(pop_queue);
			sensitive << pop;
	}
};

#endif

