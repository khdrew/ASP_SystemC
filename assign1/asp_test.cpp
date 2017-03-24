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
	reset_valid_flag();
	data_in.write(0);  // initialize values
	
	reset_valid_flag();	
	data_in.write(131072); // initialize vector B to 0
	
	reset_valid_flag();
	data_in.write(4325386); // opcode store 10 words to vector B
	for (i = 0; i < 15; i++){
		reset_valid_flag();
		data_in.write(i + 1);
	}		
		
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
	asp<20> my_asp;
	// test input signal
	sc_signal<bool> t_valid;
	sc_signal<sc_int<26> > t_data_in;
	// test output signal
	sc_signal<sc_int<16> > t_data_out;
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
		my_asp.data_in(t_data_in);
		// output
		my_asp.data_out(t_data_out);
		my_asp.res_ready(t_res_ready);
		my_asp.busy(t_busy);

		// genertator signal mapping
		in_gen.valid(t_valid);
		in_gen.data_in(t_data_in);
	}
}; 

SC_MODULE_EXPORT(top);
