// ani.h

SC_MODULE(ani)
{
	sc_in<bool> res_ready;
	sc_in<bool> busy;
	sc_in<sc_int<64> > asp_in;
	sc_in<sc_fifo<sc_int<32>> > D_to_NoC;

	sc_out<sc_int<26> > asp_out;
	sc_out<sc_fifo<sc_int<32>> > D_from_NoC;
	sc_out<bool> valid;
	sc_out<bool> reset;


    void ani_func();
    
    SC_CTOR(ani)
    {
		SC_THREAD(ani_func);
			sensitive << res_ready;
    }
};


