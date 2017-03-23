// asp.h
#ifndef ASP_H
#define ASP_H
enum State {Idle = 1, Wait_Invoke, Wait_Data};

template <int N>
SC_MODULE(asp)
{
	sc_in<bool> valid;
	sc_in<sc_bv<26> > data_in;
	sc_out<sc_int<16> > data_out;
	sc_out<bool> Res_ready;
	sc_out<bool> busy;

	State current_state;
	int A[N];
	int B[N];
	int opcode;
	int data_count;
	int begin_address;
	int end_address;

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
