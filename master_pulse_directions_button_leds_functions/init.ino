void init_i2c (){
    /*
    This function is called only one time on startup, in an accidentel reboot (because of a memmory overflow or other reason) the system will reset all slaves
   */
  if(first_id_received == false && message_recieved == false && line_ready == false){
    line_ready = true;
    Wire.beginTransmission(0x0);
    Wire.write('S');              //Stop sending pulses
    Wire.endTransmission();       // stop transmitting
    delayMicroseconds(100); // wait a while for next transmission
    set_handshake_listening_direction_slaves('u');// all slaves listen to block above
  }
}

