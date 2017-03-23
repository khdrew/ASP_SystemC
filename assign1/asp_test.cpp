// asp_test.cpp

#include "asp.h"
#include "asp.cpp"
#include "systemc.h"

using namespace std;

SC_MODULE (input_generate) 
{
   sc_out<bool> valid;
   sc_out<sc_int<26> > data_in;

   void in_gen();

   SC_CTOR(input_generate) 
   {
      SC_THREAD(in_gen);
   }
};

void input_generate::in_gen()
{
	data_in.write(0);  // initialize values
	valid.write(false);
	
	wait(10, SC_NS);
	valid.write(true); // initialize vector A to 0

	wait(10, SC_NS);
	valid.write(false); // reset

	wait(10, SC_NS);
	data_in.write(131072); // initialize vector B to 0
	valid.write(true);

	wait(10, SC_NS);
	valid.write(false); // reset

	wait(100, SC_NS);
	data_in.write(4325386); // opcode store 10 words to vector B
	valid.write(true);
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