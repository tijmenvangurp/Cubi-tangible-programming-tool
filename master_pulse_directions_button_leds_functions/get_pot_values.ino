void get_pot_values(){
  /*
  This function checkes if the current ID is odd or even. When its even it will collect the sensor data from that block.  
   */
  if(received_id % 2 == 0 ){
    // when there are no leftovers when recieved id is devided by 2 than it is even. all even id's will have a knob' all odd values wont have a knob
    pot_array [row_counter][collum_counter] = sensor_data(received_id) +1;// plus 1 so the sensor data can never be a 0 this so i can use a if(pot_array [row_counter][collum_counter]== 0) to check if there are no potvalues
  }
  else{
    pot_array [row_counter][collum_counter] = 0;
  }
}

int sensor_data(int which){
  /*
  This function combines 2 incomming bytes over the I2C bus. It does this by first receiving the highest significant byte,
  shift this 8 bytes to the left and combines this with the lowest significant byte. 
   */
  int val;
  Wire.requestFrom (which, 2);
  val = Wire.read ();
  val <<= 8;
  val |= Wire.read ();
  //Serial.println (val, DEC);
  delayMicroseconds(10);
  return val;
}

