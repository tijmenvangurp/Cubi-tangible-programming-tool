void execution_blocks(){

    // let the robot drive 
    if(!in_execution){
      drive_timeout = millis();
      // if we are here and not in a execution set the timeout for the function to come
    }
    if(robot_drive_pattern[execution_row_counter][0] =! 0){
      // we have a function because the first item out of the array is 0
      int timeout_current_function = robot_drive_pattern[pattern_rows][3];
      int current_robot_function = robot_drive_pattern[pattern_rows][0];
      if(timeout_current_function == 0){
        // we have no timeout, so a repeat block
        // speed is changed globaly and saved for each drive block, so we don't need to implement it here
        switch (current_robot_function){
        case 6:
          {
            //6 is one loop
            break;
          }
        case 7:
          {
            // 7 is the other loop
            break;
          }

        }

        execution_row_counter++; // we only have to do this function once so continue to the next function after this
      }
      else if(in_execution == false && millis()- drive_timeout < timeout_current_function){
        in_execution = true;
        // whe ave a function to drive or a delay so we need to set the motor parameters accordingly and than wait untill the time is finished
      }
      else if(millis()- drive_timeout > timeout_current_function){
        // the current time - the drive_timeout ( or the ellapsed time since we started this function) is bigger than the timeout value given for this function so 
        // we can go to the next function.
        in_execution = false; 
        execution_row_counter++;
      } 
    }
    else{
      // we have no more functions so stop the loop
      in_execution = false; 
      execution_row_counter = 0; 
      execute_driving = false;
    }
  }


}
