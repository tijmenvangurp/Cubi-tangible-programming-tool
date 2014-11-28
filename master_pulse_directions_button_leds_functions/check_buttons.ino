void check_buttons(){
  /*
  This function checkes the 2 buttons on the master block
  the start button: sends the robot message once to the robot
  the loop button: will also send the message once to the robot but with the indication to repeat the programm: once the programm is changed during the loop it will stop
  */
  button_state_start = digitalRead(button_start);
  if(button_state_start != lastButtonState_start && button_state_start == HIGH){
    //Serial.println("Start button pressed");
  }
  lastButtonState_start = button_state_start;

  button_state_loop = digitalRead(button_loop);
  if(button_state_loop != lastButtonState_loop && button_state_loop == HIGH){
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
