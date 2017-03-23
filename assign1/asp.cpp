// asp.h
#include "asp.h"

template<int N>
void asp<N>::asp_func(){
	current_state = Idle;
	busy.write(false);
	Res_ready.write(false);
	wait();
	while(true){
		if (valid.read()){
			busy.write(true);
			switch (current_state){
				case Idle:
					opcode = data_in.read() >> 22;
					if (opcode == 0b0000){
						store_init();
					} else if (opcode == 0b0001){
						data_count = (data_in.read() & 511)
						current_state = Wait_Invoke;
					}
					break;
				case Wait_Invoke:
					store_invoke();
					current_state = Wait_Data;
					break;
				case Wait_Data:
					process_data();
					current_state = Idle;
				default:
					current_state = Idle;
					break;
			}
			busy.write(false);
		}
	}
}

void asp<N>::store_init(){
	if (data_in.read() & (1 << 22)) {
		for (int i = 0; i < N; i++){
			B[i] = 0;
		}
	}else{
		for (int i = 0; i < N; i++){
			A[i] = 0;
		}
	}
}

void asp<N>::store_invoke(){
	begin_address = (data_in.read() & 511);
	end_address = ((data_in.read() >> 9) & 511);
}

void asp<N>::process_data(){
	
}