// ani_test.cpp

#include "asp.cpp"
#include "ani.h"
#include "systemc.h"

SC_MODULE (input_generate) 
{
	sc_out<sc_int<32> > d_from_NoC;

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
	// (3 << 30) shifting valid and legacy bit into the packet

	d_from_NoC.write((3 << 30) | 0);  // initialize values
	reset_valid_flag();	
	
	d_from_NoC.write((3 << 30) | 131072); // initialize vector B to 0
	reset_valid_flag();

	d_from_NoC.write((3 << 30) | 4194404); // opcode store 100 words to vector A - 0001 0000 0 00000000 001100100
	reset_valid_flag();
	d_from_NoC.write((3 << 30) | 4245504); // send invoke - 0001 0000 001100100 000000000
	reset_valid_flag();
	for (i = 0; i < 100; i++){
		d_from_NoC.write((3 << 30) | (i << 16) | i);
		reset_valid_flag();
	}

	
	d_from_NoC.write((3 << 30) | 4325476); // opcode store 100 words to vector B - 0001 0000 1 00000000 001100100
	reset_valid_flag();
	d_from_NoC.write((3 << 30) | 4245504); // send invoke - 0001 0000 001100100 000000000
	reset_valid_flag();
	for (i = 0; i < 100; i++){	
		d_from_NoC.write((3 << 30) | (i << 16) | i);
		reset_valid_flag();
	}	
		
	
	d_from_NoC.write((3 << 30) | 8389633); // xor A from 0 to 1
	reset_valid_flag();
	
	d_from_NoC.write((3 << 30) | 12598282); // xor B from 10 to 30
	reset_valid_flag();

	d_from_NoC.write((3 << 30) | 16782337); // multiply sum 1 to 10
	reset_valid_flag();

	d_from_NoC.write((3 << 30) | 20971530); // ave A L4 at 10
	reset_valid_flag();

	d_from_NoC.write((3 << 30) | 25165834); // ave B L4 at 10
	reset_valid_flag();

	d_from_NoC.write((3 << 30) | 29360138); // ave A L8 at 10
	reset_valid_flag();

	d_from_NoC.write((3 << 30) | 33554442); // ave B L8 at 10
	reset_valid_flag();
}

void input_generate::reset_valid_flag(){
	wait(10, SC_NS);
	d_from_NoC.write(0); // clock fall
	wait(10, SC_NS);
	
}

SC_MODULE(top)
{
	// modules to test
	asp<100> my_asp;
	ani my_ani;

	// test signal
	sc_signal<bool> t_valid;
	sc_signal<bool> t_reset;
	sc_signal<sc_int<26> > t_data_in;
	sc_signal<sc_int<64> > t_data_out;
	sc_signal<bool> t_res_ready;
	sc_signal<bool> t_busy;
	sc_signal<sc_int<32> > t_d_to_NoC;
	sc_signal<sc_int<32> > t_d_from_NoC;
	



	// input generator
	input_generate in_gen;

	SC_CTOR(top)
		: my_asp("my_asp"),
		  my_ani("my_ani"),
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
		my_ani.d_to_NoC(t_d_to_NoC); // READ RESULT
		// genertator signal mapping
		in_gen.d_from_NoC(t_d_from_NoC);

	}
}; 

SC_MODULE_EXPORT(top);
