void handle_id_message(){

  char c = Serial2.read();
  incomming_id = c;
  c = Serial2.read();
  incomming_id += c;
  // Serial.print("incomming Id = ");
  //   Serial.println(incomming_id);
  switch (c) {
  case '+':
    Serial.println("new id message");
    memset(id_array, 0, sizeof(id_array[0][0]) * rows * collums); // clear out id array so we can fill it again
    collum_counter = 0;
    row_counter = 0;
    break;
  case '.':
 //   Serial.println("new id line");
    row_counter++;
    collum_counter = 0;
    break;
  case ',':{
 //   Serial.println("new id collum");
    collum_counter++;
 //   Serial.println(collum_counter);
    break;
  }
  case '*':
    Serial.println("End id message");
    Serial2.print('G');
    waiting_for_pot_values = true; 
    break;
  default:
    //Serial.print(c);
    id_array [row_counter][collum_counter] = incomming_id.toInt();
    Serial.print(id_array [row_counter][collum_counter]);
    Serial.print(" Row: ");
    Serial.print(row_counter);
    Serial.print(" Collum: ");
    Serial.println(collum_counter);
    break;
  }

}

//void print_out(String toPrint){
//  Serial.println(toPrint);
//}




