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
		if (reset.read()){
			mem_sel = 1;
			store_init();
			mem_sel = 0;
			store_init();
		}else if (valid.read()){
			busy.write(true);
			res_ready.write(false);
			switch (current_state){
				case Idle:
					opcode = data_in.read() >> 22;
					switch (opcode){ 
						case STORE_INIT: // Store initialize to 0
							store_init();
							break;
						case STORE: // Store into Vector
							current_state = Wait_Invoke;
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
							data_out.write(0xFFFFFF); // error opcode value
							break;
					}
					break;
				case Wait_Invoke: // wait for invoke packet
					store_invoke();
					current_state = Wait_Data;
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
			wait(2.5, SC_NS); // delay for simulation
			busy.write(false);
			res_ready.write(true);
		}		
	}
}

// 
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
	data_out.write(0x01);
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
	target_addr = ((data_in.read() >> 16) & 511);
	if (mem_sel){
		B[target_addr] = data_in.read() & 65535;
		data_out.write(B[target_addr]);
	}else{
		A[target_addr] = data_in.read() & 65535;
		data_out.write(A[target_addr]);
	}
	data_count = data_count - 1;
}

// xor function
template <int N>
void asp<N>::xor_func(){
	if (mem_sel){ // xor across B vector
		output = B[begin_addr];
		for (int i = begin_addr + 1; i <= end_addr; i++){
			output = output ^ B[i];
		}
	}else{ // xor across A vector
		output = A[begin_addr];
		for (int i = begin_addr + 1; i <= end_addr; i++){
			output = output ^ A[i];	
		}
	}
	data_out.write(output);
}

// multiply and accumulating
template <int N>
void asp<N>::mac_func(){
	output = 0;
	for (int i = begin_addr; i <= end_addr; i++){
		output = output + A[i] * B[i];
	}
	data_out.write(output);
}

// average function for L = 4
template <int N>
void asp<N>::ave4_func(){
	output = 0;
	if (mem_sel){ // ave4 across B vector
		for (int i = begin_addr; i < begin_addr + 4; i++){
			output += B[i];
		}
	} else { // ave4 across A vector
		for (int i = begin_addr; i <= begin_addr + 4; i++){
			output += A[i];
		}
	}
	data_out.write(output / 4);
}

// average function for L = 8
template <int N>
void asp<N>::ave8_func(){
	output = 0;
	if (mem_sel){ // ave4 across B vector
		for (int i = begin_addr; i < begin_addr + 8; i++){
			output += B[i];
		}
	} else { // ave4 across A vector
		for (int i = begin_addr; i <= begin_addr + 8; i++){
			output += A[i];
		}
	}
	data_out.write(output / 8);
}

