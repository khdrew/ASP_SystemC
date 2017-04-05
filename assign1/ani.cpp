// ani.cpp

#include "ani.h"

// result recieved from ASP to be sent to NoC
void ani::to_NoC_func()
{
	state = NOT_STORING; // initialize variables
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
		}else{
			valid.write(false);
		}
	}
}

void ani::prep_t_packet(){
	t_packet = ((instruction & (65535 << 18)) | t_packet); // add valid, legacy, ports etc
	queue_to_NoC.push(t_packet);
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

// new instruction recieved from NoC to be sent to ASP
void ani::from_NoC_func(){
	if (!busy.read()){ // if not busy send to asp
		t_instruction = d_from_NoC.read();
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
			queue_to_NoC.push((instruction & (65535 << 18)) | 1); // send access granted
		}else{
			valid.write(false);
			queue_to_NoC.push(0); // send access not granted
		}
	}else{
		queue_to_NoC.push(0); // send access not granted
	}
}

