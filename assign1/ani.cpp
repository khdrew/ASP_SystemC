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
			// t_packet = (from_asp.read() >> 48);
			// if (t_packet > 0){
			// 	t_packet = t_packet | (3 << 16); // concatenate packet number
			// 	prep_t_packet(); // add other relevant information and send
			// }
			// t_packet = ((from_asp.read() >> 32) & 65535);
			// if (t_packet > 0){
			// 	t_packet = t_packet | (2 << 16);
			// 	prep_t_packet();
			// }
			// t_packet = ((from_asp.read() >> 16) & 65535);
			// if (t_packet > 0){
			// 	t_packet = t_packet | (1 << 16);
			// 	prep_t_packet();
			// }
			t_packet = (from_asp.read() & 65535); // send data or access granted
			prep_t_packet(); // add other relevant information and send
		}
	}
}

void ani::prep_t_packet(){
	t_packet = ((instruction & (65535 << 18)) | t_packet); // add valid, legacy, ports etc
	d_to_NoC.write(t_packet);
}

// new instruction recieved from NoC to be sent to ASP
void ani::from_NoC_func()
{
	t_instruction = d_from_NoC.read();
	to_asp.write(t_instruction & 67108863);
	if (t_instruction >> 31) { // check valid
		valid.write(true);
		if (state == NOT_STORING){
			if (((t_instruction >> 22) & 15) == 1){
				state = NOT_STORING;
				data_count = (t_instruction & 511) + 1;
			}else {
				instruction = t_instruction;
			}
		} else {
			data_count = data_count - 1;
		}
	}else{
		valid.write(false);
	}
	// to_asp.write(d_from_NoC.read() & 67108863);
	// if ((d_from_NoC.read() >> 30) == 3) { // check valid and legacy bit for valid instruction
	// 	// fifo_from_NoC.write(d_from_NoC.read()); // add to queue
	// 	to_asp.write(d_from_NoC.read() & 67108863);
	// 	instruction = d_from_NoC.read(); 






	// 	// send_to_asp(); // check to send to asp
	// }
}

// void ani::pop_queue()
// {
// 	if (read_NoC.read()){
// 		d_to_NoC.write(fifo_to_NoC.read());
// 	}
// }

// void ani::send_to_asp(){
// 	if (!busy.read()){ // if not busy send to asp
// 		if (fifo_from_NoC.nb_read(f_packet)){
// 			// Store port information to be sent back to sender
// 			if (state != STORING){ // store port information if not storing/receiving data packets
// 				if (f_packet >> 22 & 15 == 1){ // enter storing state
// 					data_count = (f_packet & 511) + 1;
// 					state = STORING;
// 				}
// 				instruction = f_packet;
// 			}else if (data_count > 1){ // exit storing state when all data packets recieved
// 				data_count = data_count - 1;
// 			}else{
// 				state = NOT_STORING;
// 			}
// 			valid.write(false);
// 			wait(2.5,SC_NS);
// 			to_asp.write(f_packet);
// 			wait(2.5,SC_NS);
// 			valid.write(true);
// 		}
// 	}
// }

