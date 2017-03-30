// ani_test.cpp

#include "asp.cpp"
#include "ani.h"
#include "systemc.h"

SC_MODULE (input_generate) 
{
	sc_out<sc_uint<32> > data_in;

	void in_gen();
	int i;

	SC_CTOR(input_generate) 
	{
		SC_THREAD(in_gen);
	}
};


void input_generate::in_gen()
{

	data_in.write(0);  // initialize values
	data_in.write(131072); // initialize vector B to 0
	data_in.write(4194404); // opcode store 100 words to vector A - 0001 0000 0 00000000 001100100
	data_in.write(4245504); // send invoke - 0001 0000 001100100 000000000
	for (i = 0; i < 100; i++){
		data_in.write((i << 16) | i);
	}
	data_in.write(4325476); // opcode store 100 words to vector B - 0001 0000 1 00000000 001100100
	data_in.write(4245504); // send invoke - 0001 0000 001100100 000000000
	for (i = 0; i < 100; i++){	
		data_in.write((i << 16) | i);
	}	
	data_in.write(8389633); // xor A from 0 to 1
	data_in.write(12598282); // xor B from 10 to 30
	data_in.write(16782337); // multiply sum 1 to 10
	data_in.write(20971530); // ave A L4 at 10
	data_in.write(25165834); // ave B L4 at 10
	data_in.write(29360138); // ave A L8 at 10
	data_in.write(33554442); // ave B L8 at 10
	
}



SC_MODULE(top)
{
	// modules to test
	asp<100> my_asp;
	ani my_ani;

	// test signal
	sc_signal<bool> t_valid;
	sc_signal<bool> t_reset;
	sc_signal<sc_uint<26> > t_data_in;
	sc_signal<sc_uint<64> > t_data_out;
	sc_signal<bool> t_res_ready;
	sc_signal<bool> t_busy;
	sc_signal<sc_uint<32> > t_d_to_NoC;
	sc_signal<sc_uint<32> > t_d_from_NoC;
	



	// input generator
	input_generate in_gen;

	SC_CTOR(top)
		: my_asp("my_asp"),
		  in_gen("in_gen")
	{
		// test ASP signal mapping
		// inputs
		my_asp.valid(t_valid);
		my_asp.reset(t_reset);
		my_asp.data_in(t_data_in);
		// output
		my_asp.data_out(t_data_out);
		my_asp.res_ready(t_res_ready);
		my_asp.busy(t_busy);

		// test ANI signal mapping
		// ASP mapping
		// to ASP
		my_ani.valid(t_valid);
		my_ani.reset(t_reset);
		my_ani.to_asp(t_data_in);
		// from ASP
		my_ani.from_asp(t_data_out);
		my_ani.res_ready(t_res_ready);
		my_ani.busy(t_busy);

		// test NoC mapping
		my_ani.d_from_NoC(t_d_from_NoC);
		my_ani.t_d_to_NoC(t_d_to_NoC); // READ RESULT
		// genertator signal mapping
		in_gen.data_in(t_d_from_NoC);

		t_reset = 0;
	}
}; 

SC_MODULE_EXPORT(top);
