// asp_test.cpp

#include "asp.cpp"
#include "systemc.h"


SC_MODULE (input_generate) 
{
	sc_out<bool> valid;
	sc_out<sc_int<26> > data_in;

	void in_gen();
	void reset_valid_flag();
	int i;

	SC_CTOR(input_generate) 
	{
		SC_THREAD(in_gen);
	}
};


void input_generate::in_gen()
{


	data_in.write(0);  // initialize values
	reset_valid_flag();	
	
	data_in.write(131072); // initialize vector B to 0
	reset_valid_flag();

	data_in.write(4194404); // opcode store 100 words to vector A - 0001 0000 0 00000000 001100100
	reset_valid_flag();
	for (i = 0; i < 100; i++){
		data_in.write((i << 16) | i);
		reset_valid_flag();
	}

	
	data_in.write(4325476); // opcode store 100 words to vector B - 0001 0000 1 00000000 001100100
	reset_valid_flag();
	for (i = 0; i < 100; i++){
		data_in.write((i << 16) | i);
		reset_valid_flag();
	}	
		
	
	data_in.write(8389633); // xor A from 0 to 1
	reset_valid_flag();
	
	data_in.write(12598282); // xor B from 10 to 30
	reset_valid_flag();

	data_in.write(16782337); // multiply sum 1 to 10
	reset_valid_flag();

	data_in.write(20971530); // ave A L4 at 10
	reset_valid_flag();

	data_in.write(25165834); // ave B L4 at 10
	reset_valid_flag();

	data_in.write(29360138); // ave A L8 at 10
	reset_valid_flag();

	data_in.write(33554442); // ave B L8 at 10
	reset_valid_flag();


}

void input_generate::reset_valid_flag(){
	wait(10, SC_NS);
	valid.write(false); // clock fall
	wait(10, SC_NS);
	valid.write(true); // clock rise
}


SC_MODULE(top)
{
	// module to test
	asp<100> my_asp;
	// test input signal
	sc_signal<bool> t_valid;
	sc_signal<bool> t_reset;
	sc_signal<sc_int<26> > t_data_in;
	// test output signal
	sc_signal<sc_int<64> > t_data_out;
	sc_signal<bool> t_res_ready;
	sc_signal<bool> t_busy;
	// input generator
	input_generate in_gen;

	SC_CTOR(top)
		: my_asp("my_asp"),
		  in_gen("in_gen")
	{
		// test module signal mapping
		// inputs
		my_asp.valid(t_valid);
		my_asp.reset(t_reset);
		my_asp.data_in(t_data_in);
		// output
		my_asp.data_out(t_data_out);
		my_asp.res_ready(t_res_ready);
		my_asp.busy(t_busy);

		// genertator signal mapping
		in_gen.valid(t_valid);
		in_gen.data_in(t_data_in);

		t_reset = 0;
	}
}; 

SC_MODULE_EXPORT(top);
