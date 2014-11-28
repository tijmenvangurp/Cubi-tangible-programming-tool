
void get_function_block_by_id(byte id, int pot_value,byte level, boolean global){
  /*
  motor_assignment[0]= indication what variables there are
   2 = driving forward/backwards
   3 = turning left/right
   4 = delay
   5 = speed
   6 = loop a
   7 = loop b
   motor_assignment[0]= function
   motor_assignment[1]= direction left motor (1 = forward, 0 = backwards)  ||  the speed we should drive this direction || the ammount of time to repeat
   motor_assignment[2]= direction right motor (1 = forward, 0 = backwards)
   motor_assignment[3]= the ammount of time to drive this direction
   motor_assignment[4]= global speed value:  the speed for this block <-- we need this here because we change the speed setting globaly
   
   */
  // changing the time will influence the distance robot will drive
  static int max_time = 5000; 
  static int min_time = 100;
  static int middle_knob = 513;
  static int end_knob = 1025;
  static int begin_knob = 1;

  robot_drive_pattern[level][0] = round(floor((id/10)));

  switch  (round(floor((id/10)))){
  case 2:
    {
      //drive forward/backward
      if(pot_value < middle_knob){
        //drive forward
        int time_to_drive_forward = round(map(pot_value,begin_knob,middle_knob,max_time,min_time));
        // both motors should go forward
        robot_drive_pattern[level][0] = 2;
        robot_drive_pattern[level][1] = 1;
        robot_drive_pattern[level][2] = 1;
        robot_drive_pattern[level][3] = time_to_drive_forward;
        robot_drive_pattern[level][4] = speed_robot;
      }
      else{
        //drive backward
        // both motors should go forward
        int time_to_drive_backward = round(map(pot_value,middle_knob,end_knob,min_time,max_time));
        robot_drive_pattern[level][0] = 2;
        robot_drive_pattern[level][1] = 0;
        robot_drive_pattern[level][2] = 0;
        robot_drive_pattern[level][3] = time_to_drive_backward;
        robot_drive_pattern[level][4] = speed_robot;
      }
      break;
    }
  case 3:
    {      
      if(pot_value < middle_knob){
        // drive left
        int time_to_drive_left = round(map(pot_value,begin_knob,middle_knob,max_time,min_time));
        robot_drive_pattern[level][0] = 3;
        robot_drive_pattern[level][1] = 1;
        robot_drive_pattern[level][2] = 0;
        robot_drive_pattern[level][3] = time_to_drive_left;
        robot_drive_pattern[level][4] = speed_robot;

      }
      else{
        // drive right
        int time_to_drive_right = round(map(pot_value,middle_knob,end_knob,min_time,max_time));
        robot_drive_pattern[level][0] = 3;
        robot_drive_pattern[level][1] = 0;
        robot_drive_pattern[level][2] = 1;
        robot_drive_pattern[level][3] = time_to_drive_right;
        robot_drive_pattern[level][4] = speed_robot;
      }

      break;
    }
  case 4:
    {
      int delay_time = round(map(pot_value,begin_knob,end_knob,min_time,max_time));
      robot_drive_pattern[level][0] = 4;
      robot_drive_pattern[level][1] = delay_time;

      // delay
      break;
    }
  case 5:
    {
      //speed
      if(global){
        speed_robot = round(map(pot_value,begin_knob,end_knob,0,255));
        robot_drive_pattern[level][0] = 5;
      }
      else{
        robot_drive_pattern[level][0] = 5;
        robot_drive_pattern[level][3] = round(map(pot_value,begin_knob,end_knob,0,255));
      }
      break;
    }
  case 6:
    {
      // loop A:

      if (pot_value == 0){
        // we have the first loop section
        for(int i  = level; i < rows; i++ ){
          int current_id_section = round(floor((id_array [i][0]/10)));
          if(current_id_section == 6){
            pot_value = pot_array [i][0];
            pot_array[i][0] = 0; 
          }
        }
      }      
      int repeat_x_times = round(map(pot_value,begin_knob,end_knob,0,9));
      robot_drive_pattern[level][0] = 6;
      robot_drive_pattern[level][1] = repeat_x_times;
      break;
    }
  case 7:
    { // loop A:
      boolean save_potvalue = false;
      if (pot_value == 0){
        // this means we should save the potmeter value of the equevilant of potmeter 6 
        save_potvalue = true;      
      }
      unsigned int function_blocks_in_loop_counter = 0; 
      for(int i  = level; i < rows; i++ ){
        int current_id_section = round(floor((id_array [i][0]/10)));
        if( current_id_section == 6 ){

          // we have problem: there is an loop block too soon
          // send error message back to master block with loop block that is in the wrong position
          String error_message = "R";
          error_message += (String) id_array [i][0];
          Serial2.print(error_message);
          // break all the code
          block_in_wrong_position = true; 
          break;
        }
        if( current_id_section == 7 && function_blocks_in_loop_counter > 0 ){
          // succes we have found the next loop section and we have had blocks to repeat in between

        }
        else if(current_id_section == 7){
          // the loop blocks are next to eachother, nothing to loop maybe break here?
          // or send error command back to master block
          if(save_potvalue){
            // because we want only the first block to have the repeat value we switch them around here
            pot_value = pot_array [i][0];
            pot_array [i][0] = 0; 
          }
        }
        if(current_id_section < 6){
          // we have a block inside the loop that we can loop
          function_blocks_in_loop_counter++;
        }
      }
      int repeat_x_times = round(map(pot_value,begin_knob,end_knob,0,9));
      robot_drive_pattern[level][0] = 7;
      robot_drive_pattern[level][1] = repeat_x_times;
      break;
    }
  default:
    break;
  }


}




