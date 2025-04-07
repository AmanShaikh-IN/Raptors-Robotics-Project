#define motor_3_pwm 3                                                           //3
#define motor_3_dir 2                                                           //2
#define motor_4_pwm 5                                                           //6
#define motor_4_dir 4                                                           //7
#define motor_2_pwm 6                                                           //5
#define motor_2_dir 7                                                           //4
#define motor_1_pwm 9                                                           //9
#define motor_1_dir 8                                                           //8
#define receiver_channel_x A0 //channel1                                                  //Channel 1
#define receiver_channel_y A2 //channel 2
#define receiver_channel_turn A4
#define PI 3.14159

int W1, W2, W3, W4, smoothened_W1, smoothened_W2, smoothened_W3, smoothened_W4;
int left_stick_X, left_stick_Y, right_stick_turn;
int joy_value_mid_upper = 1540, joy_value_mid_lower = 1450, power;
float theta, x_component, y_component, max_val;

void setup() {
  
  Serial.begin(115200);
  pinMode(motor_1_pwm, OUTPUT);
  pinMode(motor_1_dir, OUTPUT);
  pinMode(motor_2_pwm, OUTPUT);
  pinMode(motor_2_dir, OUTPUT);
  pinMode(motor_3_pwm, OUTPUT);
  pinMode(motor_3_dir, OUTPUT);
  pinMode(motor_4_pwm, OUTPUT);
  pinMode(motor_4_dir, OUTPUT);
  pinMode(receiver_channel_x, INPUT);
  pinMode(receiver_channel_y, INPUT);
  pinMode(receiver_channel_turn, INPUT);

  smoothened_W1 = 0;
  smoothened_W2 = 0;
  smoothened_W3 = 0;
  smoothened_W4 = 0;

}

void loop() {

  right_stick_turn = pulseIn(receiver_channel_turn, HIGH);                                                 //Add a different timeout argument after reading serial monitor values
  Serial.print("\nThe current channelturn_Val is: ");
  Serial.println(right_stick_turn);

  left_stick_X = pulseIn(receiver_channel_x, HIGH);                                                        //Add a different timeout argument after reading serial monitor values
  Serial.print("\nThe current channelX_Val is: ");
  Serial.println(left_stick_X);

  left_stick_Y = pulseIn(receiver_channel_y, HIGH);                                                        //Add a different timeout argument after reading serial monitor values
  Serial.print("\nThe current channelY_Val is: ");
  Serial.println(left_stick_Y);

  if (right_stick_turn < joy_value_mid_lower){
    right_stick_turn = (right_stick_turn != 0) ? map(right_stick_turn, 980, joy_value_mid_lower, -175, 0) : 0;
  }

  else if (right_stick_turn > joy_value_mid_upper){
    right_stick_turn = map(right_stick_turn, joy_value_mid_upper, 2000, 0, 175);
  }

  //else if ((right_stick_turn > joy_value_mid_lower) && (right_stick_turn < joy_value_mid_upper)){
  else{
    right_stick_turn = 0;
  }

  Serial.print("\nThe current mapped turn value is: ");
  Serial.println(right_stick_turn);


  if (left_stick_X < joy_value_mid_lower){
    left_stick_X = (left_stick_X != 0) ? map(left_stick_X, 980, joy_value_mid_lower, -175, 0) : 0;
  }
  
  else if (left_stick_X > joy_value_mid_upper){
    left_stick_X = map(left_stick_X, joy_value_mid_upper, 2000, 0, 175);
  }

  //else if ((left_stick_X > joy_value_mid_lower) && (left_stick_X < joy_value_mid_upper)){
  else{
    left_stick_X = 0;
  }

  Serial.print("\nThe current mapped x value is: ");
  Serial.println(left_stick_X);

  if (left_stick_Y < joy_value_mid_lower){
    left_stick_Y = (left_stick_Y != 0) ? map(left_stick_Y, 980, joy_value_mid_lower, -175, 0) : 0;
  }
  
  else if (left_stick_Y > joy_value_mid_upper){
    left_stick_Y = map(left_stick_Y, joy_value_mid_upper, 2000, 0, 175);
  }

  //else if((left_stick_Y > joy_value_mid_lower) && (left_stick_Y < joy_value_mid_upper)){
  else{
    left_stick_Y = 0;
  }

  Serial.print("\nThe current mapped y value is: ");
  Serial.println(left_stick_Y);

  theta = atan2(left_stick_Y, left_stick_X);                                                              //Check if we need degrees or radians. Also if change to double is needed.
  Serial.print(theta);
  Serial.println(" radians");

  power = hypot(left_stick_X, left_stick_Y);
  Serial.print(power);
  Serial.println(" resultant");

  x_component = cos(theta - (PI/4));
  Serial.print(x_component);
  Serial.println(" x comp angle");

  y_component = sin(theta - (PI/4));
  Serial.print(y_component);
  Serial.println(" y comp angle");

  Serial.print("Validity Check: ");
  Serial.println(hypot(x_component, y_component));

  max_val = max(abs(x_component), abs(y_component));
  Serial.print(max_val);
  Serial.println(" max_val");
  
  W4 = ((power * x_component / max_val) + right_stick_turn);              //Align wheel speeds with actual wheel orientations
  W3 = ((power * y_component / max_val) - right_stick_turn);              //Max value configures for rotational + translation velocity and caps all to set upper limit
  W2 = ((power * y_component / max_val) + right_stick_turn);              //Check if adding turn before dividing value prevents unknown movement 
  W1 = ((power * x_component / max_val) - right_stick_turn);              //Adding turn afterwards can cause overshoot beyond the limit, keep cond

  if (abs(W1) > 175)
    W2 = W3 = 0;
  
  if (abs(W2) > 175)
    W1 = W4 = 0;

  W1 = constrain(W1, -175, 175);
  W2 = constrain(W2, -175, 175);
  W3 = constrain(W3, -175, 175);
  W4 = constrain(W4, -175, 175);

  Serial.print(W1);
  Serial.println(" Unfiltered W1 speed");  
  Serial.print(W2);
  Serial.println(" Unfiltered W2 speed"); 
  Serial.print(W3);
  Serial.println(" Unfiltered W3 speed"); 
  Serial.print(W4);
  Serial.println(" Unfiltered W4 speed"); 

  smoothened_W1 = (0.85 * W1) + (0.15 * smoothened_W1);
  smoothened_W2 = (0.85 * W2) + (0.15 * smoothened_W2);
  smoothened_W3 = (0.85 * W3) + (0.15 * smoothened_W3);
  smoothened_W4 = (0.85 * W4) + (0.15 * smoothened_W4);

  digitalWrite(motor_1_dir, smoothened_W1 > 0 ? LOW : HIGH);                           //Positive for a direction, negative for the other
  digitalWrite(motor_2_dir, smoothened_W2 > 0 ? LOW : HIGH);
  digitalWrite(motor_3_dir, smoothened_W3 > 0 ? LOW : HIGH);
  digitalWrite(motor_4_dir, smoothened_W4 > 0 ? LOW : HIGH);                           //Check high/low for cw and ccw

  Serial.print("\nThe speed of W1 is: ");
  Serial.println(smoothened_W1);
  Serial.print("\nThe speed of W2 is: ");
  Serial.println(smoothened_W2);
  Serial.print("\nThe speed of W3 is: ");
  Serial.println(smoothened_W3);
  Serial.print("\nThe speed of W4 is: ");
  Serial.println(smoothened_W4);

  analogWrite(motor_1_pwm, abs(smoothened_W1));
  analogWrite(motor_2_pwm, abs(smoothened_W2));
  analogWrite(motor_3_pwm, abs(smoothened_W3));
  analogWrite(motor_4_pwm, abs(smoothened_W4));
  delay(2000);
}
