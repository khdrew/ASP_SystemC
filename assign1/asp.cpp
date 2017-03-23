// asp.h
#include "asp.h"

template<int N>
void asp<N>::asp_func(){
	current_state = Idle;
	data_out.write(0);
	busy.write(false);
	res_ready.write(false);
	wait();
	while(true){
		if (valid.read()){
			busy.write(true);
			switch (current_state){
				case Idle:
					opcode = data_in.read() >> 22;
					if (opcode == 0){	// initialize all vectors to 0 based on mem select
						store_init();
					} else if (opcode == 1){
						data_count = (data_in.read() & 511);
						mem_sel = data_in.read() & (1 << 17);
						data_out.write((data << 1) | mem_sel);
						current_state = Wait_Invoke;
					}
					break;
				case Wait_Data:
					process_data();
					// current_state = Idle;
				default:
					current_state = Idle;
					break;
			}
			busy.write(false);
		}
	}
};

template <int N>
void asp<N>::store_init(){
	if (data_in.read() & (1 << 17)) {
		for (int i = 0; i < N; i++){
			B[i] = 0;
		}
	}else{
		for (int i = 0; i < N; i++){
			A[i] = 0;
		}
	}
};

template <int N>
void asp<N>::store_invoke(){
	begin_address = (data_in.read() & 511);
	end_address = ((data_in.read() >> 9) & 511);
};

template <int N>
void asp<N>::process_data(){
	
};
