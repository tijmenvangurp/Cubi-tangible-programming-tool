# Final-Master-project--Cubi

For my final master project I programmed several micro controllers to make a tangible programming tool for children.


Through literature studies, user tests, and design probes, there is examined how to fit a tangible programming tool into the educational environment. The first two tests explore how concepts of programming can be taught. The final test explores Cubi’s usefulness for a group setting of 8 and 9 year olds. Furthermore, literature about existing tangible programming tools has been explored in this study, to support design decisions and to present a vision of the future of programming in education.

The program exists out of 3 parts

1: The master_pulsedirections-button_leds (I know this name sounds terible but at least for me it was clear) was the main controlling item. This microcontroler was the start block where each block was connected to. Via I2C the slave structure is explored end send to robot (simple_receive_robot_teensy)

2: the Slave_pulse_directions is the code for the slave blocks, which send their corresponding ID to the master when they receive a pulse from their neighbor.

3: The simple_receive_robot_teensy is a piece of code that translated the string of ID's in a driving behavior. 
