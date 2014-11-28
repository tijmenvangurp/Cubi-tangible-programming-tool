void construct_message(){
  /*
  This function creates a string package of ID's to send to the robot, and a package of bytes of the potvalues.
   It also checkes if all the blockes are still in the same position, if they are in a new position the message should be send, otherwise not. 
   */
  boolean errors = false;

  memset(potmeter_values_to_send,0,sizeof(potmeter_values_to_send)); // clear out prepared buffer array
  potmeter_array_counter = 0;
  prepare_message(2000);// 2000 is start message robot knows this 1 to 1024 are reserved for pot values
  /*
  potvalues will be send in a byte array becayse the numbers are sometimes 4 numbers long, if it would be a string it would complicate the algarithm for receiving
   values 1 tm 1025 will be reserved for potvalues
   2000 is start message
   3000 will be new collum
   4000 will be new row
   5000 will be the end of the message
   */
  int check_position_loop_blocks [4][1];
  memset(check_position_loop_blocks, 0, sizeof(check_position_loop_blocks[0][0]) * 4 * 1); // clear out pot_array, fill it with zeros
  int position_loop_blocks_counter = 0;
  if(id_array[0][0] =!0){
    // we have at least one block so start message
    slave_summary = "++";
  }

  for (int level = 0; level < rows; level++) {
    if(id_array[level][0] =!0){
      // at this point check the message if all the blocks are in the right order
      int id_catogary_block = round(floor((id_array[level][0]/10)));
      // check if loop a or loop b is in tha house and save in seperate array
      if (id_catogary_block == 6 || id_catogary_block == 7){
        check_position_loop_blocks [position_loop_blocks_counter][0] = id_array[level][0];
        check_position_loop_blocks [position_loop_blocks_counter][1] = id_catogary_block;
        position_loop_blocks_counter++;
      }
      for (int count = 0; count < collums; count++) {
        int current_id = id_array[level][count];
        if (current_id != 0) {
          slave_summary += (String)current_id;
          // potmeter value is prepared to send 
          // todo: only add potnumbers to those blocks that have a knob 
          if(id_catogary_block == 6 || id_catogary_block == 7){
            if(current_id%2 == 0){
              // all even blocks will have a knob
              prepare_message(pot_array[level][count]);
            }
            else{
              // all odd blocks won't have a knob, but inorder that both messages are same length we ad a 0
              prepare_message(0);
            }
          }
          else{
            prepare_message(pot_array[level][count]);
          }
          if(count < collums ){
            if(id_array[level][count +1 ] != 0){
              // if on the current count +1 is not 0 than ad a comma
              slave_summary += ",,";
              prepare_message (3000);// 3000 will be new collum
            }
          }
        }
        else{
          // when there is no id anymore in this collum stop the for loop
          count = collums;
        }
      }
    }
    if(level < rows && id_array[0][0]!= 0){
      if (id_array[level + 1][0] != 0) {
        slave_summary += "..";
        prepare_message (4000);// 4000 will be new row
      }
      else {
        //no more blocks
        slave_summary += "**";
        prepare_message (5000);// 5000 will be the end of the message
        level = rows;
      }
    }
    else if(level == rows){
      // for the case we have actually the ammount of blocks that we have levels also finish the slave summary
      slave_summary += "**";
      prepare_message (5000);// 5000 will be the end of the message
    }
  }

  // than check the order of the loops in the array: should be either aa bb or abba or baab 
  // in the meantime check if there is something in between the loop? or just let it loop with nothing? 
  // if they are not both available check if a or b is in tha house
  // than check if the loop is closed
  // handle errors and don't send message to robot
  if(position_loop_blocks_counter =!0){

    switch (position_loop_blocks_counter){
    case 1:
      {
        // only one loop block send blink command to block
        errors = true;
        error_message(check_position_loop_blocks[0][0],'B');
        break;
      }
    case 2:
      {
        // only 2 blocks check if both from the same loop
        if (check_position_loop_blocks[0][1] =! check_position_loop_blocks[1][1]){
          // 2 loop blocks from different kind
          errors = true;
          error_message(check_position_loop_blocks[0][0],'B');
          error_message(check_position_loop_blocks[1][0],'B');
        }
      }
    case 3:
      {
        int counter = 0; 
        for(int i = 0 ; i < 3; i++){
          if(check_position_loop_blocks[i][1] == 6){
            counter++; 
          }
        }
        if(counter == 1){
          // there was only one a
          errors = true;
          for(int i = 0 ; i < 3; i++){
            // find a and send error message to a
            if(check_position_loop_blocks[i][1] == 6){
              error_message(check_position_loop_blocks[i][0],'B');
              break;
            }
          }
        }
        else{
          // there was only one b
          errors = true;
          for(int i = 0 ; i < 3; i++){
            // 
            if(check_position_loop_blocks[i][1] == 7){
              error_message(check_position_loop_blocks[i][0],'B');
              break;
            }
          }
        }
        break;
      }
    case 4:
      {
        // only wrong possibility is abab once there are 2 after eachother the same its ok
        if(check_position_loop_blocks[0][1] != check_position_loop_blocks[1][1] && check_position_loop_blocks[2][1] != check_position_loop_blocks[3][1]){
          error_message(check_position_loop_blocks[1][0],'B');
          error_message(check_position_loop_blocks[3][0],'B');
        }

      }
    }
  }

  if (slave_summary != robot_message){
    Serial.print(slave_summary);
    waiting_to_send_pot_values = true;
    delayMicroseconds(100);
    robot_message = slave_summary;
  }

  // Serial.print(slave_summary);
  //Serial.println(freeRam());
  // request_sensor_data();

  slave_search_direction = none;
}


void prepare_message( int value){
  // send the two bytes that comprise an integer
  value_to_add(highByte(value)); // send the high byte
  value_to_add(lowByte(value)); // send the low byte
}

void value_to_add(int val){
  /*
  This functoin adds the high or low significant byte of the prepare message function
   */
  potmeter_values_to_send [potmeter_array_counter] = val;
  potmeter_array_counter++;
}
































