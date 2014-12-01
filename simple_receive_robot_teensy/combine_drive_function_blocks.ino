void combine_drive_function_blocks(){
  /*
just here for refference
   const byte function_id_block = 0; 
   const byte left_wheel_direction = 1; 
   const byte right_wheel_direction = 2; 
   const byte driving_time = 3; 
   const byte left_wheel_speed = 4; 
   const byte right_wheel_speed = 5;
   const byte forward = 1;
   const byte backwards = 0;
   */
  int level = robot_drive_pattern_counter;
  int combined_id_nummerator = robot_drive_pattern[level][0] + robot_drive_pattern[level+1][0];
  // in case one is forward/backwards and the otherone left/right, make a forwards turn, or backwards turn
  switch (combined_id_nummerator){
  case driving_forwards_backwards*2:
    {
      if(robot_drive_pattern[level][left_wheel_direction] == robot_drive_pattern[level][left_wheel_direction]){
        // in case both are driving forward or both are driving backwards: add the time driving forward/backwards
        robot_drive_pattern[level][driving_time] += robot_drive_pattern[level+1][driving_time];
      }
      else{
        // one is forward the otherone is backward
        int result_combined_time_forward_backward = 0;
        if(robot_drive_pattern[level][left_wheel_direction] == forwards){
          // level block = forwards en level+1 = backwards
          result_combined_time_forward_backward = robot_drive_pattern[level][driving_time] - robot_drive_pattern[level+1][driving_time];
          if(result_combined_time_forward_backward >= 0){
            // result is driving forward
            robot_drive_pattern[level][left_wheel_direction] = forwards;
            robot_drive_pattern[level][right_wheel_direction] = forwards;
            robot_drive_pattern[level][3] = result_combined_time_forward_backward;
          }
          else{
            // result is driving backwards
            robot_drive_pattern[level][left_wheel_direction] = backwards;
            robot_drive_pattern[level][right_wheel_direction] = backwards;
            robot_drive_pattern[level][3] = (result_combined_time_forward_backward *-1);
          }
        }
        else{
          // level block = backwards en level+1 = forwards
          result_combined_time_forward_backward = robot_drive_pattern[level+1][driving_time] - robot_drive_pattern[level][driving_time];
          if(result_combined_time_forward_backward >= 0){
            // result is driving forward
            robot_drive_pattern[level][left_wheel_direction] = forwards;
            robot_drive_pattern[level][right_wheel_direction] = forwards;
            robot_drive_pattern[level][driving_time] = result_combined_time_forward_backward;
          }
          else{
            // result is driving backwards
            robot_drive_pattern[level][left_wheel_direction] = backwards;
            robot_drive_pattern[level][right_wheel_direction] = backwards;
            robot_drive_pattern[level][driving_time] = (result_combined_time_forward_backward *-1);
          }
        }
      }
    }

  case 5:
    {
      // one is forward/backwards the otherone is turning
      if(robot_drive_pattern[level][0] = 2){
        // level block was forward/backwards
        // high time driving forward, and high time turning, is long corner allmost driving a circle --> time to execute is long, speed difference motors is not to big
        // low time to drive forward and high time turning, is short corner --> time to execute is short, speed difference motors is big
        // high time driving forward, and low time turning, is long corner --> difference speed motor is small
        //TODO on master block check if there more than 3 blocks when that is the case the combination may only be forward/backward block left/right block and speed block

        // left/right block knob will make the difference in speed from motor
        int speed_setting = round(map(robot_drive_pattern[level+1][driving_time],min_time,max_time,local_speed_set,0));
        if(robot_drive_pattern[level+1][left_wheel_direction] == backwards){
          // turning left
          robot_drive_pattern[level][left_wheel_speed] = speed_setting;
        }
        else{
          // turning right
          robot_drive_pattern[level][right_wheel_speed] = speed_setting;
        }
      }
      else{
        // level block was left/right
        int speed_setting = round(map(robot_drive_pattern[level][driving_time],min_time,max_time,local_speed_set,0));
        if(robot_drive_pattern[level][left_wheel_direction] == backwards){
          // turning left
          robot_drive_pattern[level][function_id_block] = driving_forwards_backwards; 
          robot_drive_pattern[level][driving_time] = robot_drive_pattern[level+1][driving_time];
          robot_drive_pattern[level][left_wheel_direction] = robot_drive_pattern[level+1][left_wheel_direction];
          robot_drive_pattern[level][right_wheel_direction] = robot_drive_pattern[level+1][right_wheel_direction];
          robot_drive_pattern[level][left_wheel_speed] = speed_setting;
        }
        else{
          // turning right
          robot_drive_pattern[level][function_id_block] = driving_forwards_backwards; 
          robot_drive_pattern[level][driving_time] = robot_drive_pattern[level+1][driving_time];
          robot_drive_pattern[level][left_wheel_direction] = robot_drive_pattern[level+1][left_wheel_direction];
          robot_drive_pattern[level][right_wheel_direction] = robot_drive_pattern[level+1][right_wheel_direction];
          robot_drive_pattern[level][right_wheel_speed] = speed_setting;

        }

      }

    }

  }


}




