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
		res_ready.write(false);
		switch (current_state){
			case Idle:
				opcode = data_in.read() >> 22;
				switch (opcode){ 
					case Store_Init: // Store initialize to 0
						store_init();
						break;
					case Store: // Store into Vector
						current_state = Wait_Data;
						data_count = (data_in.read() & 511);
						mem_sel = data_in.read() & (1 << 17);
						data_out.write(data_count);
						break;
					case XOR_A: // XOR A
						store_invoke();
						mem_sel = 0;
						xor_func();
						break;
					case XOR_B: // XOR B
						store_invoke();
						mem_sel = 1;
						xor_func();
						break;
					case MAC: // MAC
						store_invoke();
						mac_func();
						break;
					case AVE4_A: // AVE A, L = 4
						store_invoke();
						mem_sel = 0;
						ave4_func();
						break;
					case AVE4_B: // AVE B, L = 4
						store_invoke();
						mem_sel = 1;
						ave4_func();
						break;
					case AVE8_A: // AVE A, L = 8
						store_invoke();
						mem_sel = 0;
						ave8_func();
						break;
					case AVE8_B: // AVE B, L = 8
						store_invoke();
						mem_sel = 1;
						ave8_func();
						break;
					default:
						break;
				}
				break;
			case Wait_Data: // wait for data to be received
				process_data();
				if (data_count == 0){
					res_ready.write(true);
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

// 
template <int N>
void asp<N>::store_init(){
	if (data_in.read() & (1 << 17)) { // vector B
		for (int i = 0; i < N; i++){
			B[i] = 0;aw 
		}
	}else{ // vector A
		for (int i = 0; i < N; i++){
			A[i] = 0;
		}
	}
	data_out.write(65535);
}

// set m and n values
template <int N>
void asp<N>::store_invoke(){
	begin_addr = (data_in.read() & 511);
	end_addr = ((data_in.read() >> 9) & 511);
}

// store value into vector
template <int N>
void asp<N>::process_data(){
	int target_addr = ((data_in.read() >> 16) & 511);
	if (mem_sel){
		B[target_addr] = data_in.read() & 32767;
		data_out.write(B[target_addr]);
	}else{
		A[target_addr] = data_in.read() & 32767;
		data_out.write(A[target_addr]);
	}
	data_count = data_count - 1;
}

// xor function
template <int N>
void asp<N>::xor_func(){
	int output;
	if (mem_sel){ // xor across B vector
		output = B[begin_addr];;
		for (int i = begin_addr + 1; i <= end_addr; i++){
			output ^= B[i];
		}
	}else{ // xor across A vector
		output = A[begin_addr];
		for (int i = begin_addr + 1; i <= end_addr; i++){
			output ^= A[i];	
		}
	}
	data_out.write(output);
	res_ready.write(true);
}

// multiply and accumulating
template <int N>
void asp<N>::mac_func(){
	int output = 0;
	for (int i = begin_addr; i <= end_addr; i++){
		output += A[i] * B[i];
	}
	data_out.write(output);
	res_ready.write(true);
}

// average function for L = 4
template <int N>
void asp<N>::ave4_func(){
	int output = 0;
	if (mem_sel){ // ave4 across B vector
		for (int i = begin_addr; i < begin_addr + 4; i++){
			output += B[i];
		}
	} else { // ave4 across A vector
		for (int i = begin_addr; i <= begin_addr + 4; i++){
			output += A[i];
		}
	}
	res_ready.write(true);
	data_out.write(output / 4);
}

// average function for L = 8
template <int N>
void asp<N>::ave8_func(){
	int output = 0;
	if (mem_sel){ // ave4 across B vector
		for (int i = begin_addr; i < begin_addr + 8; i++){
			output += B[i];
		}
	} else { // ave4 across A vector
		for (int i = begin_addr; i <= begin_addr + 8; i++){
			output += A[i];
		}
	}
	res_ready.write(true);
	data_out.write(output / 8);
}