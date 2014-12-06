void send_request_to_id_or_reset(){
  /*

   */
  if(first_id_received == true && waiting_for_id == false && slave_search_direction != 0){
    send_request_to_direction();
    /*
it might be that we are still searching for blocks, in the send to request direction 
     we set waiting_for_id to true, only in the request time_out new line state we change the slave search direction to 0
     */

  }  
  else if(slave_search_direction == 0){
    // we are finished searching the structure, request sensor data / send data to robot / start over
    // Serial.println("finished start over \n\n");
    first_id_received = false;
    message_recieved = false;
    slave_search_direction = right;
    collum_counter = 0;
    row_counter = 0;
    //memset(array, 0, sizeof(array[0][0]) * m * n); http://stackoverflow.com/questions/2516096/fastest-way-to-zero-out-a-2d-array-in-c
    memset(id_array, 0, sizeof(id_array[0][0]) * rows * collums); // clear out id array, fill it with zeros
    memset(pot_array, 0, sizeof(pot_array[0][0]) * rows * collums); // clear out pot_array, fill it with zeros
    set_handshake_listening_direction_slaves('U');
     
   
    i2c_communications_delay = millis();
    //delay(400);// 2 times per second we check the whole structure this way the chance is small that we remove blocks in the middle of the discovery sequence.
    // todo: remove this delay and use the time to check if the blocks are in the right order
  }


}


