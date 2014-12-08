void construct_driving_pattern(){

  memset( robot_drive_pattern, 0, sizeof(robot_drive_pattern[0][0])* pattern_rows * pattern_collums);
  global_speed_robot = normal_speed;
  robot_drive_pattern_counter = 0;  // is increased in the get function block by id function
  // when we have blocks in the id array, make the driving array for what the robot should do
  for (int level = 0; level < rows; level++) {
    //Serial.println(id_array [level][0]);
       if(id_array [level][0] != 0){
      // Serial.println("there are blocks to process");
      // there is a block in this level ( if it is a speed block the function get function block id will handle the rest )

      local_speed_set = global_speed_robot; // if local speed is not the same as global speed local speed is set to global before we continue this row
      // if there is a local speed setting it will be changed automatically      
      get_function_block_by_id(id_array [level][0], pot_array[level][0]);
     //Serial.print("outside the function it is ");
    //  Serial.println(robot_drive_pattern[level][6]);



      if(id_array [level][1] != 0){
        // there is more than one block this level start combining blocks
        robot_drive_pattern_counter--;
       // Serial.println("multiple blocks this level start combining" );
        // Serial.println(robot_drive_pattern_counter);
        // we handle it as a whole collum we dont want 2 the same entries in the array so we need to set the counter one back so we overwrite the first call to the function

          int current_number_of_collums = 0;
        local_speed_robot[0] = 0;
        local_speed_robot[1] = 0;
        int nr_of_speed_blocks = 0;
        /*
 --Check Number of speed blocks
         -- check number of collums in this level
         */
        for(int current_collum = 0; current_collum < collums ; current_collum++){
          // check if there are more cullums
          int current_function_id = round(floor((id_array [level][current_collum]/10)));
          if(current_function_id != 0){
            current_number_of_collums++;
            if( current_function_id == speed_function){
              // save value of speed blocks
              local_speed_robot[nr_of_speed_blocks] = round(map(pot_array[level][current_collum], begin_knob, end_knob, minimum_speed, maximum_speed));
              nr_of_speed_blocks++;
            }
          }
          else{
            // there are no more blocks this collum
            break;
          }
        }// finished checking nr of speed blocks
        /*
When there are speed blocks save speed value local or global
         */
        if(nr_of_speed_blocks > 0 ){
       //   Serial.print("There where ");
       //   Serial.print(nr_of_speed_blocks);
       //   Serial.println(" nr of speed blocks");
          if(current_number_of_collums == nr_of_speed_blocks ){
            // there are at least 2 blocks, and nr of blocks is the same as number of speed blocks so combine speed and set global
            global_speed_robot = round(((local_speed_robot[0]+local_speed_robot[1])/2));
        //    Serial.print("only 2 speed blocks this level set global speed to");
        //    Serial.println(global_speed_robot);
          }
          else if(nr_of_speed_blocks == 2){
            // there are 2 speed blocs but more than 2 blocks in this collum so combine speed values and set local speed
            local_speed_set = round(((local_speed_robot[0]+local_speed_robot[1])/2));
        //    Serial.print("2 speed blocks combined Set local speed to:");
       //     Serial.println(local_speed_set);
          }
          else{
            // there is only one speed block, but in total 2 blocks at this collum set this local
            local_speed_set = local_speed_robot[0];
         //   Serial.print("Set local speed to:");
        //    Serial.println(local_speed_set);
          }
        }
        /*
check if there other blocks than speed blocks, if so store in execute_driving array
         */
        // nr of blocks to combine is current_number_of_collums - nr_of_speed_blocks
        if(current_number_of_collums - nr_of_speed_blocks > 0){
          // there are non speed blocks to process
          boolean combine_functions = false;
        //  Serial.print("nr of collums: ");
       //   Serial.println(current_number_of_collums);
        //  Serial.print("nr of speed blocks: ");
       //   Serial.println(nr_of_speed_blocks);
          for(int current_collum = 0 ; current_collum < current_number_of_collums ; current_collum++ ){
            if(id_array [level][current_collum]!= 0 ){
              // there is a block to process
              int id_function = round(floor((id_array [level][current_collum]/10)));
              if(id_function != speed_function){
                // skipp all blocks that have a speed setting
                if(!combine_functions){
                  // just save normaly
                  get_function_block_by_id(id_array [level][current_collum], pot_array[level][current_collum]);
                  // we had one non speed block to process, so next time combine_functions
                  combine_functions = true;
                }
                else{
                  // we already have a setting saved so combine current reading with previous and decrease counter
                  get_function_block_by_id(id_array [level][current_collum], pot_array[level][current_collum]); // saves it on new line
                  // combine current block with previous
                  //Serial.println(robot_drive_pattern_counter);
                  robot_drive_pattern_counter -= 2; // make sure the next block overwrites the current values by decreasing counter
                  combine_drive_function_blocks();
                  robot_drive_pattern_counter++;
                }
              }
            }
            else{
              // no more blocks in this collum
              break;
            }
          }// end for loop collums
        }
        else{
        //  Serial.println("nr of speed blocks was the same as current number of blocks in collum");
          //nr of speed blocks was the same as current number of blocks in collum
        }
      }
      else{
        // there was just one block in this level go to next level
        // Serial.println("just one block this level");
      }
    }
    else{
     // Serial.println("no more blocks to process");
      // no more id's in the message
     
      execute_driving = true;
      start_constructing_patern = false; // only need to construct the pattern once, is set to true with a new message
      break; // not necesarry to look for more id's if ther are no more so break the for loop
    }
  }// end for loop
//  Serial.println("Finished constructing driving pattern");

}





