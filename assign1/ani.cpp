// ani.cpp

#include "ani.h"

// result recieved from ASP to be sent to NoC
void ani::to_NoC_func()
{
	state = NOT_STORING; // initialize variables
	// data_count = 0;
	while(1){
		wait();
		if (!busy.read() || res_ready.read()){
			// check length of packet with relevant information
			// refer to report section on NoC packet sending
			t_packet = (from_asp.read() >> 48);
			if (t_packet > 0){
				t_packet = t_packet | (3 << 16); // concatenate packet number
				prep_t_packet(); // add other relevant information and send
			}
			t_packet = ((from_asp.read() >> 32) & 65535);
			if (t_packet > 0){
				t_packet = t_packet | (2 << 16);
				prep_t_packet();
			}
			t_packet = ((from_asp.read() >> 16) & 65535);
			if (t_packet > 0){
				t_packet = t_packet | (1 << 16);
				prep_t_packet();
			}
			t_packet = (from_asp.read() & 65535); // send data or access granted
			prep_t_packet(); // add other relevant information and send
			send_to_asp();
		}else{
			valid.write(false);
		}
	}
}

void ani::prep_t_packet(){
	t_packet = ((instruction & (65535 << 18)) | t_packet); // add valid, legacy, ports etc
	queue_to_NoC.push(t_packet);
}

// new instruction recieved from NoC to be sent to ASP
void ani::from_NoC_func()
{
	queue_from_NoC.push(d_from_NoC.read());
	send_to_asp();
	// t_instruction = d_from_NoC.read();
	// to_asp.write(t_instruction & 67108863);
	// if (t_instruction >> 31) { // check valid
	// 	if (state == NOT_STORING){
	// 		if (((t_instruction >> 22) & 15) == 1){
	// 			state = NOT_STORING;
	// 			data_count = (t_instruction & 511) + 1;
	// 		}else {
	// 			instruction = t_instruction;
	// 		}
	// 	} else {
	// 		data_count = data_count - 1;
	// 	}
	// 	valid.write(true);
	// }else{
	// 	valid.write(false);
	// }
}

void ani::pop_queue()
{
	if (pop.read()){
		if (!queue_to_NoC.empty()){
			d_to_NoC.write(queue_to_NoC.front());
			queue_to_NoC.pop();
		}
	}
}

void ani::send_to_asp(){
	if (!busy.read()){ // if not busy send to asp
		if (!queue_from_NoC.empty){
			t_instruction = queue_from_NoC.front();
			queue_from_NoC.pop();
			to_asp.write(t_instruction & 67108863);
			if (t_instruction >> 31) { // check valid
				if (state == NOT_STORING){ // do not store data/invoke information
					if (((t_instruction >> 22) & 15) == 1){
						state = NOT_STORING;
						data_count = (t_instruction & 511) + 1;
					}else { // save the instruction containing port information
						instruction = t_instruction;
					}
				} else { // count data packets
					data_count = data_count - 1;
				}
				valid.write(true);
			}else{
				valid.write(false);
			}
		}
	}
}

