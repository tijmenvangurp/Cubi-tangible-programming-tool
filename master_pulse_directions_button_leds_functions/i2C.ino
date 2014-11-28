void send_request_to_direction()
{
  /*
  This function does 2 things
   first: it sends a command to all blocks to change their pulse listening direction (all blocks will change their pulse listening line)
   second: it sends a command to the last discoverd block to send pulses to a given direction
   */

  /*
    Serial.print("search direction  = ");
   Serial.print(slave_search_direction);
   Serial.print(" ID = ");
   Serial.println(received_id);
   */

  switch(slave_search_direction) {
  case right:
    set_handshake_listening_direction_slaves('L');
    requestNeighbour_id_side(received_id,'R');
    break;
  case left:
    set_handshake_listening_direction_slaves('R');
    requestNeighbour_id_side(received_id,'L');
    break;
  case new_line:
    set_handshake_listening_direction_slaves('U');
    requestNeighbour_id_side(received_id,'D');
    break;
  }
}

void receiveEvent(int howMany){
  /*
  This function handles incomming bytes from the I2C, it is autmaticly called upon by the wire I2C library
   */
  message_recieved = true;
  received_id = Wire.read();    // receive byte as an integer

  for(int i = 0; i < howMany ; i++){
    Wire.read ();// flush buffer probably not nessesary, but if there was more than one byte to be send we make shure that these bytes are removed
  }
}

void requestNeighbour_id_side (int block_id, char side){
  /*
 This is a sub function of the send request to direction function
   */
  //  Serial.println("Requesting ID of block's neighbor.");
  byte error;
  Wire.beginTransmission(block_id);
  Wire.write(side);
  error = Wire.endTransmission(true);
  /*
  if(error =! 1){
   Serial.print("error in wire transmissionrequest neighbour error = ");
   Serial.println(error);
   }
   */
  waiting_for_id = true;
  // Serial.print("time between 2 requests = ");
  // Serial.println(micros() - waiting_for_id_timeout);
  waiting_for_id_timeout = micros();
  // after the request is send we give some time for the pulses to land, if we dont receive an id in time we search to the next direction
  //  Serial.println("Set timeout: "+ (String) block_id + " " + (String)side);
}

void set_handshake_listening_direction_slaves(char side){
  /*
 This is a sub function of the send request to direction function
   */
  char message[2];
  message[0] = 'I';
  message[1] = side;
  byte error;
  Wire.beginTransmission(0x0); // broadcast to all ID's
  Wire.write(message);
  error = Wire.endTransmission();
  /*
  if(error =! 1){
   Serial.print("error in wire transmission set handshake error = ");
   Serial.println(error);
   }
   */
  delayMicroseconds(100);// just for safety, give all blocks time to recover before I send a new message on the line
  // Serial.println(F("Finished setting listening direction "));
}

void error_message(int block_id, char side){
  Wire.beginTransmission(block_id);
  Wire.write(side);
  Wire.endTransmission(true);
  delayMicroseconds(100);

}





