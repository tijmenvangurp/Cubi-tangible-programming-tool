void get_pot_values(){
  /*
  This function checkes if the blocks has a knob, because there are only 2 blockes witout a knob I check if these 2 ID's are not there
   */  
  if(received_id != 65 && received_id != 75 ){
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

