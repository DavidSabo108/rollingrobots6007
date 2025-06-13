/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       rollingrobots                                             */
/*    Created:      3/25/2025, 4:48:07 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"


using namespace vex;




// A global instance of vex::brain used for printing to the V5 brain screen
//setup ports
vex::brain       Brain;
competition Competition;
controller Controller1 = controller(primary);
motor LF(PORT11, ratio18_1, true);//front motor
motor LM(PORT12, ratio18_1, false);//middle
motor LB(PORT13, ratio18_1, true);//back
motor_group LMG = motor_group(LF, LM, LB);
motor RF(PORT1, ratio18_1, false);//front motor
motor RM(PORT2, ratio18_1, true);//middle
motor RB(PORT3, ratio18_1, false);//back
motor_group RMG = motor_group(RF, RM, RB);
rotation PG = rotation(PORT4, false);
inertial Inertial = inertial(PORT5);
//setup variables
static float pi = 3.14;
static float dia = 2.75;
//float Kp = 5;
float Ki = 0.2;
float Kd = 1;
//functions




void gyroPrint() {
  float heading = Inertial.rotation(deg);
  Brain.Screen.printAt(1, 60, "heading = %.2f. degrees", heading);
}




void driveSpeed(float Lspeed, float Rspeed, int wt) {
  LMG.spin(fwd, Lspeed, pct);
  RMG.spin(fwd, Rspeed, pct);
  wait(wt, msec);
}




void driveBrake() {
  LMG.stop(brake);
  RMG.stop(brake);
}
void inchDrive(float target) {
  float x = 0;
  float past_x = 0;
  float speed = 0;//target;
  float Kp = 2;
  float Ki = 1.4;
  float Kd = 0.3;
  float f_now = 0;
  float f_last = 0;
  float integral = 0;
  float dx;
 
  PG.resetPosition();
  while (fabs(x - target) > 0.1) {
/*   if(target<0){
      while(x>target){*/
          x = PG.position(rev)*dia*pi;
          f_now = Kp * (target - x);
          f_last = Kp * (target - past_x);
          dx = (x - past_x); //+ Inertial.acceleration(xaxis))/2;
          integral = 0.5 * (f_now + f_last) * dx;
          speed = Kp * (target - x ) - Kd;// + Ki * integral;
          driveSpeed(speed, speed, 10);
          Brain.Screen.printAt(100,40,"x=%0.2f",x);
           if(fabs(target-x)<0.5 && speed <5){
              break;
          }
          past_x = x;
//}
         if (speed<5){
           break;
         }
}
/*   if(target>0){
      while(x<target){
          x = PG.position(rev)*dia*pi;
          speed = Kp*(target-x);
          driveSpeed(speed, speed, 10);
          Brain.Screen.printAt(100,40,"x=%d",x);
  } 
  }*/
  driveBrake();
}
void turn(int angle) {
  int rotation = angle;
  float x=0;
  Inertial.setRotation(0,deg);
  if(angle<0){
      while(x>rotation){
      x=Inertial.rotation(deg);
      driveSpeed(-50,50,10);
      Brain.Screen.printAt(1,100, "rotation = %d degrees", rotation);
      gyroPrint();
      }
  }
  if(angle>0){
      while(x<rotation){
      x=Inertial.rotation(deg);
      driveSpeed(50,-50,10);
      Brain.Screen.printAt(1,100, "rotation = %d degrees", rotation);
      gyroPrint();
      }
  }
  driveBrake();
  Inertial.setRotation(0,deg);
}








//competiion functions
void pre_auton(void) {
  Inertial.calibrate();
}




void autonomous(void) {
 //driveSpeed(1, 1, 10);
 inchDrive(36);


}




void usercontrol(void) {
  int leftSpeed;
  int rightSpeed;
  while (true) {
      leftSpeed = Controller1.Axis3.position();
      rightSpeed = Controller1.Axis2.position();
      driveSpeed(leftSpeed, rightSpeed, 10);
      if (Controller1.ButtonX.pressing()) {
          driveBrake();
      }
      Brain.Screen.printAt(100, 40, "hi");
    
  }
  wait(20, msec);
}




int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);




  // Run the pre-autonomous function.
  pre_auton();
  while(1) {
    
      // Allow other tasks to run
      this_thread::sleep_for(10);
  }
}

