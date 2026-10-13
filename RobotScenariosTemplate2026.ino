/*
Skeleton program to demonstrate a simple way to handle various
scenarios for the Elec1601 robot buggy project
Ross Hutton 2026
*/

// Define all pins

/* Define other global constants, especially calibration sensitive numbers 
  and any values that are used in more than one place
*/

//Define global variables
int stopped=0;

void setup() {
  // put your setup code here, to run once:
  // setup all input pins

  // setup all output pins

  // setup servos

  // setup serial port for debugging purposes if desired

}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Insert code to measure left, middle, and right distances

  // Detect scenarios
  int scenario = 0; //the scenario is classified as unknown if no other is detected
  if(/* sensors detect scenario 1, middle of corridor */) scenario=1;
  if(/* sensors detect scenario 2, ideal position for right turn */) scenario=2;
  if(/* sensors detect scenario 3, ideal position for left turn */) scenario=3;
  if(/* sensors detect scenario 4, dead end */) scenario=4;
  if(/* sensors detect scenario 5, left side of corridor, parallel */) scenario=5;  
  if(/* sensors detect scenario 6, right side of corridor, parallel */) scenario=6;
  if(/* sensors detect scenario 7, left side of corridor, pointing at wall */) scenario=7;
  if(/* sensors detect scenario 8, right side of corridor, pointing at wall */) scenario=8;

  /*  Motion control
    This is separated from scenario detection to make stopping after 
    running a scenario cleaner, as only one scenario procedure can be executed per loop.
    It also means that if scenario tests overlap that the one with the higher number
    will have priority and execute.
  */
  if(!stopped) //if no scenario has completed and stopped the robot, process it
  {
    switch(scenario)
    {
      case 0: //Unknown scenario
      //Set the LEDs to L=off, M=off, R=off
      //Stop both servos

      stopped=1; //Stop checking for further scenarios
      break;
      
      case 1: //middle of corridor
      //Set the LEDs to L=off, M=off, R=on
      //Delay 1 second
      //Move forward 5cm
      //Stop both servos

      stopped=1; //Stop processing further scenarios
      break;
      
      case 2: //ideal position right turn
      //Set the LEDs to L=off, M=on, R=off
      //Delay 1 second
      //Turn right 90 degrees
      //Move forward until middle of corridor detected
      //Stop both servos

      stopped=1; //Stop processing further scenarios
      break;
      
      case 3: //ideal position left turn
      //Set the LEDs to L=off, M=on, R=on
      //Delay 1 second
      //Turn left 90 degrees
      //Move forward until middle of corridor detected
      //Stop both servos

      stopped=1; //Stop processing further scenarios
      break;
      
      case 4: //dead end
      //Set the LEDs to L=on, M=off, R=off
      //Delay 1 second
      //Rotate 180 degrees
      //Move forward straight at least 5cm
      //Stop both servos

      stopped=1; //Stop processing further scenarios
      break;
      
      case 5: //left side of corridor parallel
      //Set the LEDs to L=on, M=off, R=on
      //Delay 1 second
      //Rotate clockwise slightly
      //Move fwd until in middle of corridor
      //Rotate counterclockwise until parallel to walls
      //Move backwards to approximately starting position
      //Stop both servos

      stopped=1; //Stop processing further scenarios
      break;
      
      case 6: //right side of corridor parallel
      //Set the LEDs to L=on, M=on, R=off
      //Delay 1 second
      //Rotate counter clockwise slightly
      //Move fwd until in middle of corridor
      //Rotate clockwise until parallel to walls
      //Move backwards to approximately starting position
      //Stop both servos

      stopped=1; //Stop processing further scenarios
      break;
      
      case 7: //left side of corridor pointing at wall
      //Set the LEDs to L=on, M=on, R=on
      //Delay 1 second
      //Rotate clockwise until parallel
      //Stop both servos

      //The test asks for the robot to stay stopped but if you comment out the next line it should progress to scenario 4
      stopped=1; //Stop processing further scenarios
      break;
      
      case 8: //right side of corridor pointing at wall
      //Set the LEDs to L=off, M=off, R=flash
      //Delay long enough to see the flashing
      //Rotate counter clockwise until parallel
      //Stop both servos

      //The test asks for the robot to stay stopped but if you comment out the next line it should progress to scenario 5   
      stopped=1; //Stop processing further scenarios
      break;
      
      default: //this is an alternative way to process unknown scenarios - this code is redundant as case 0: handles unknown situations
      //Set the LEDs to L=off, M=off, R=off
      //Stop both servos

      stopped=1; //Stop checking for further scenarios
    }
  }
}
