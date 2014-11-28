void save_id(){
  
  /*
  When there is a message received it will be saved accordingly in the 2D array
  Also the potmeter values of the block will be requested from the slaves to create a 2D array of potvalues
  */

 if(first_id_received == false && message_recieved == false){
    send_pulses_down();
  } else if(first_id_received == false && message_recieved == true){
    first_id_received = true;
    id_array [0][0] = received_id;
    get_pot_values();
    slave_search_direction = right;
    digitalWrite(led_pin, LOW);
    message_recieved = false;
  }  else if(message_recieved == true){
    // the received ID was not the first ID, add as collum, or row
    if (slave_search_direction == new_line) {
      // when receive an id on a new row, save it accordingly
      slave_search_direction = right;
      row_counter ++;// save it in next row
      collum_counter = 0;// save it in first position
      id_array [row_counter][collum_counter] = received_id;
      get_pot_values();

    }	else{
      // append on the collum because search direction was not new line
      collum_counter ++;
      id_array [row_counter][collum_counter] = received_id;
      get_pot_values();
    }
    message_recieved = false;
    waiting_for_id = false;
  }

}
