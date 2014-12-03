void execution_blocks(){
    // let the robot drive

  if(robot_drive_pattern[execution_row_counter][function_id_block] =! 0){
    // we have a function because the first item out of the array is not 0

    int timeout_current_function = robot_drive_pattern[execution_row_counter][driving_time];
    if(in_execution == false){
    int current_robot_function = robot_drive_pattern[execution_row_counter][0];
/*
handle loops
*/
    if(current_robot_function == loop_a || current_robot_function == loop_b){
      // if it is one of the repeat blocks handle it here
      // we need a boolean for in repeat
      // execution_row_counter is the start or end of the loop_b/a once it arrives at the end a counter should count how often the loop is done
      int repeat_x_times = robot_drive_pattern[execution_row_counter][1]; // if repeat_x_times is not 0 than its start of loop a

      if(current_robot_function == loop_a ){
        if(repeat_x_times != 0 && loop_a_running == false){
          // its the first time we are here, set loop_a_counter
          loop_a_counter = repeat_x_times;
          start_loop_a = execution_row_counter; // save current position for the start of the loop
          loop_a_running = true;
        }
        else if(repeat_x_times == 0){
          // this is the last block of the loop so start over if loopcounter is not yet 0
          if(loop_a_counter != 0){
            loop_a_counter --;
          // repeat_x_times was 0 so this means we are at the end of the block
            execution_row_counter = start_loop_a;
          }
          else if(loop_b_counter != 0){
            // done with the loop
            // if this loop is inside another loop
            loop_a_running = false;
            // when b counter is not 0 this loop is inside the b loop, reset loop a so that it will start over when b loop is restarted
          }
        }
      }
      else{
        // it was loop_b
        if(repeat_x_times != 0 && loop_b_running == false){
          // its the first time we are here, set loop_a_counter
          loop_b_counter = repeat_x_times;
          start_loop_b = execution_row_counter; // save current position for the start of the loop
          loop_b_running = true;
        }
        else if(repeat_x_times == 0){
          // this is the last block of the loop so start over if loopcounter is not yet 0
          if(loop_b_counter != 0){
            loop_b_counter --;
            // repeat_x_times was 0 so this means we are at the end of the block
            execution_row_counter = start_loop_b;
          }
          else if(loop_a_counter != 0){
            // done with the loop
            // if this loop is inside another loop
            loop_b_running = false;
            // when a counter is not 0 this b loop is inside the a loop, reset loop b so that it will start over when a loop is restarted
          }
        }
      }
      // check if this is not 0
      // if it is 0 it is the end of the repeat
      execution_row_counter++; // we only have to do this function once so continue to the next function after this
    }
    else{
      // if not in execution set new parameters for motors
      in_execution = true;
      // just to be clear about what the robot functions are
      int motor_left_direction = robot_drive_pattern[execution_row_counter][left_wheel_direction];
      int motor_right_direction = robot_drive_pattern[execution_row_counter][right_wheel_direction];
      int motor_speed_left_wheel =  robot_drive_pattern[execution_row_counter][left_wheel_speed];
      int motor_speed_right_wheel = robot_drive_pattern[execution_row_counter][right_wheel_speed];

      drive(motor_left_direction, motor_right_direction ,motor_speed_left_wheel,motor_speed_right_wheel);
      drive_timeout = millis();
      // whe ave a function to drive or a delay so we need to set the motor parameters accordingly and than wait untill the time is finished
    }
}
    else if(millis()- drive_timeout > timeout_current_function){
      // the current time - the drive_timeout (or the ellapsed time since we updated the motor state) is bigger than the timeout value given for this function so
      // we can go to the next function.
      in_execution = false;
      drive(0, 0 ,0,0);// stop driving
      execution_row_counter++;
// go to the next funtion
    }
  }
  else{
    // we have no more functions so stop the loop
    in_execution = false;
    execution_row_counter = 0; // set execution row counter
    loop_a_counter = 0;
    loop_b_counter = 0;
    execute_driving = false;
// TODO when master block is in repeat mode keep execute driving
  }

}
