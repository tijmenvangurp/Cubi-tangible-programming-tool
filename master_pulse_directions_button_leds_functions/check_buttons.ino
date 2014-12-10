void check_buttons(){
  /*
  This function checkes the 2 buttons on the master block
   the start button: sends the robot message once to the robot
   the loop button: will also send the message once to the robot but with the indication to repeat the programm: once the programm is changed during the loop it will stop
   */
  button_state_start = digitalRead(button_start);
  if(button_state_start != lastButtonState_start && button_state_start == HIGH){
    // Serial.println("Start button pressed");
    // TODO
    // set code in execution, wait untill we have received a command that it is finnished untill we send new pot data
    // send stop command when in running and button is pressed again.
    if(errors == false && waiting_to_send_pot_values == false){
      // but all lights to white
      Wire.beginTransmission(0x0); // broadcast to all ID's
      Wire.write('W');
      Wire.endTransmission();
      
      in_loop = false;
      waiting_to_send_pot_values = true;
      Serial.print(slave_summary);
      Serial.print("$$");
      // light up this part of led strip
      for(int i=NUMPIXELS/2;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0,100,0)); 
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
    }
  }
  lastButtonState_start = button_state_start;

  button_state_loop = digitalRead(button_loop);
  if(button_state_loop != lastButtonState_loop && button_state_loop == HIGH){


    if(errors == false && waiting_to_send_pot_values == false && in_loop == false){
      // but all lights to white
      Wire.beginTransmission(0x0); // broadcast to all ID's
      Wire.write('W');
      Wire.endTransmission();
      
      waiting_to_send_pot_values = true;
      in_loop = true;
      Serial.print(slave_summary);
      Serial.print("??");
      // light up this part of led strip
      for(int i=0;i<NUMPIXELS/2;i++){
        pixels.setPixelColor(i, pixels.Color(0,100,0)); 
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
    }
    else if(in_loop == true){
      // but all lights to white
      Wire.beginTransmission(0x0); // broadcast to all ID's
      Wire.write('W');
      Wire.endTransmission();
      
      in_loop = false;
      Serial.print("!!");
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(100,100,100)); 
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
    }
    // Serial.println("loop button pressed");
  }
  lastButtonState_loop= button_state_loop;

}




/*	buttonState = digitalRead(buttonPin);
 	
 	if (buttonState != lastButtonState && buttonState == HIGH ) {
 	Serial.println(slave_summary);
 	}
 	
 	lastButtonState = buttonState;
 	*/
//if(starting){
//set_handshake_listening_direction_slaves('U');
//Serial.println("starting");
//starting = false;
//}
// when code is is finished and we are waiting for a new message, make the line ready








