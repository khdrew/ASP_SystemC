// asp.h
#include "asp.h"

template<int N>
void asp<N>::asp_func(){
	current_state = Idle;
	data_out.write(0);
	busy.write(false);
	res_ready.write(false);
	while(true){
		wait();
		busy.write(true);
		switch (current_state){
			case Idle:
				opcode = data_in.read() >> 22;
				switch (opcode){ 
					case 0: // Store initialize to 0
						store_init();
						break;
					case 1: // Store into Vector
						current_state = Wait_Data;
						data_count = (data_in.read() & 511);
						mem_sel = data_in.read() & (1 << 17);
						data_out.write(data_count);
						break;
					case 2: // XOR A
						store_invoke();
						mem_sel = data_in.read() & (1 << 17);
						xor_func();
						break;
					case 3: // XOR B
						store_invoke();
						mem_sel = data_in.read() & (1 << 17);
						xor_func();
						break;
					case 4: // MAC
						store_invoke();
						mac_func();
						break;
					case 5: // AVE A
						store_invoke();
						mem_sel = data_in.read() & (1 << 17);
						ave_func();
						break;
					case 6: // AVE B
						store_invoke();
						mem_sel = data_in.read() & (1 << 17);
						ave_func();
						break;	
					default:
						break;
				}
				break;
			case Wait_Data: // wait for data to be received
				process_data();
				if (data_count == 0){
					current_state = Idle;	
				}
				break;
			default:
				current_state = Idle;
				break;
		}
		busy.write(false);		
	}
}

template <int N>
void asp<N>::store_init(){
	if (data_in.read() & (1 << 17)) { // vector B
		for (int i = 0; i < N; i++){
			B[i] = 0;
		}
	}else{ // vector A
		for (int i = 0; i < N; i++){
			A[i] = 0;
		}
	}
	data_out.write(65535);
}

template <int N>
void asp<N>::store_invoke(){
	begin_address = (data_in.read() & 511);
	end_address = ((data_in.read() >> 9) & 511);
}

template <int N>
void asp<N>::process_data(){
	int target_address = ((data_in.read() >> 16) & 511);
	if (mem_sel){
		B[target_address] = data_in.read() & 32767;
		data_out.write(B[target_address]);
	}else{
		A[target_address] = data_in.read() & 32767;
		data_out.write(A[target_address]);
	}
	data_count = data_count - 1;
}

template <int N>
void asp<N>::xor_func(){
	
}

template <int N>
void asp<N>::mac_func(){
	
}

template <int N>
void asp<N>::ave_func(){
	
}
