      #include <reg51.h>

//Defines
/*************************************************************************************/
// Assigning dipswitch states to represent values, defining them so it makes
// the overall program simple and reduces time 
#define dpsw_000  0		
#define dpsw_001	1		
#define dpsw_010	2
#define dpsw_011	3
#define dpsw_100	4
#define dpsw_101	5
#define dpsw_110	6
#define dpsw_111	7

//Global Variables
/*************************************************************************************/
unsigned int dpsw_state = 0;
unsigned int rpm_array [3];
unsigned int negative_edge_counter = 0;
unsigned int avg_rpm = 0;
char seven_seg_code[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned int counter = 0;
unsigned int displaycounter = 0;
int output_rpm;
int last_error;


//Pin Declarations
/*************************************************************************************/
sbit dpsw_1 = P1^0;
sbit dpsw_2 = P1^1;
sbit dpsw_3 = P1^2;

sbit pwm_output = P2^0;
sbit seven_seg_sw1 = P2^3;
sbit seven_seg_sw2 = P2^2;
sbit seven_seg_sw3 = P2^1;



// Function Declarations
/*************************************************************************************/
void pwm_output_to_motor(int);
void check_dpsw_value();
//void motorRPM();
void edge_counter();
void delay(int);
void encoder_feedback_control(int);
void display_rpm(int displaycounter);


// Interrupts
/*************************************************************************************/
void initialize_interrupt(){
	// Using external falling edge Interrupt to count encoder pulses
	IT0 = 1;					// Configure INT0 for falling edge on P3.2
	EX0 = 1; 					// Enable EX0 Interrupt
	EA = 1;						// Enable All-Interrupt flag
	
	
	// Using timer1 to generate an Interrupt every 50ms
	TMOD = 0x11; 			// Enabling in split mode
	TH1 = 0x4B;				// counting from 0x4BFF to oxFFFF takes 50ms, after 50ms Interrupt routine is called
	TL1 = 0xFF;
	TR1 = 1;					// Turn on timer 1
	ET1 = 1;					// Enable timer 1 Interrupt
}	


void falling_edge_interrupt() interrupt 0{
	// This function is a counter for every falling edge that comes from the encoder signal
	negative_edge_counter++;
}




void interrupt_50ms() interrupt 3{
	int rpm; 					// rpm variable
	
	display_rpm(displaycounter);
	
	if (counter >=  18){  
		/* Normally value should be 20 but since the frequency of our code and
		the mikroboard are slightly different, to offset this difference
		we have increased the count
		
	
		/* In accordance with our motor, as per the datasheet, we have 2 periods of encoder signal
		per revolution of the motor. Thus in 2 periods of the encoder signal we have 2 falling edges
		So 1 revolution has 2 Falling edges. Let X = # of falling edges detected in 1s, 
		then, X = #Falling edges detected in 1 s, so we have 
		X [Falling edge/sec] * 60 [sec/min] * 1/2 [revolution/falling edge], this can be condensed to
		X*30 */
		//rpm = ((negative_edge_counter)*45/24);
		rpm = ((negative_edge_counter)*30);
		
		// updating the rpm array, rpm_array [2] = current rpm
		rpm_array[0] = rpm_array[1];
		rpm_array[1] = rpm_array[2];
		rpm_array[2] = rpm;
		
		//add last three RPM's which are stored in the array, then divide by 3 to get 
		avg_rpm = (rpm_array [0] + rpm_array [1] + rpm_array [2])/3;

		negative_edge_counter = 0; //reset negative edge counter
		counter = 0; 							 //reset 1 second counter for interrup_50ms
	}
	TH1 = 0x4B;
	TL1 = 0xFF; 								// Reset timer
	if (displaycounter > 2){
		displaycounter = 0;
	}
	counter++; 		// Increment counter to track num of 50ms delays
	displaycounter++;
}





//Main File
/*************************************************************************************/
void main(void){
	
	initialize_interrupt();				//initialize Interrupt
	dpsw_1 = 1;
	dpsw_2 = 1;
	dpsw_3 = 1;
	
	pwm_output = 0;								//Initialize P2^0 as pwmoutput
	seven_seg_sw1 = 0;									// 7_seg_sw1,2,3 = P2^1,2,3 respectively
	seven_seg_sw2 = 0;
	seven_seg_sw3 = 0;
	P0 = 0x00;										//Clearing port 0

    while (1) {
		 switch (dpsw_state){ //dip switch state machine
				case dpsw_000:
					 encoder_feedback_control(0); //0% 
					 break;
				case dpsw_001:
					 encoder_feedback_control(480); //20% 
					 break;
				case dpsw_010:
					 encoder_feedback_control(1200); //50% 
					 break;
				case dpsw_011:
					 encoder_feedback_control(1440); //60% 
					 break;
				case dpsw_100:
					 encoder_feedback_control(1680); //70% 
					 break;
				case dpsw_101:
					 encoder_feedback_control(1920); //80% 
					 break;
				case dpsw_110:
					 encoder_feedback_control(2160); //90%
					 break;
				case dpsw_111:
					 encoder_feedback_control(2400); //100% 
					 break;
				default:
					 encoder_feedback_control(0);
				}
	   }

}




//Function Definitions
/*************************************************************************************/

void encoder_feedback_control(int output_rpm){
	// This function is responsible for calculating the error and then setting the 
	// output based on the error and desired RPM
	int error = output_rpm - avg_rpm;
	int der = error - last_error;
	int x = output_rpm/24;
	 int output_dutycycle = (x); //+(error)/24 + (der)/24;
	// int output_dutycycle = (output_rpm - avg_rpm);
	// The output is output_rpm dutycycle*gain, from trial and error we have
	// deduced that an appropriate gain value is 1.6
	last_error = error;
	pwm_output_to_motor(output_dutycycle);
	//display_rpm();

}
void pwm_output_to_motor(int dutycycle){
	// This function is responsible for writing the dutycycle to Port2 Pin0
	
	if (dutycycle <= 0){
		pwm_output = 0;
	}
	else if (dutycycle >= 100){
		pwm_output = 1;
	}
	else{
		// Default value is high, and we delay depending on dutycycle
		pwm_output = 0;
		pwm_output = 1;
		delay(dutycycle);
		pwm_output = 0;
		delay(100-dutycycle);
		pwm_output = 1;
	}
	// Check dipswitch state
	check_dpsw_value();
}
void check_dpsw_value(){
	/* This function checks the value of the dpsw
	Keep in mind, P1 has internal pull-up resistors, this means that the
	default value of each pin is high, i.e. 000 = high high high, and 
	001 = high high low*/
	// 000 ==> 111
	if (dpsw_1 == 1 && dpsw_2 == 1 && dpsw_3 == 1){
		dpsw_state = dpsw_000;
	}
	// 001 ==> 110
	else if (dpsw_1 == 1 && dpsw_2 == 1 && dpsw_3 == 0){
		dpsw_state = dpsw_001;
	}
	// 010 ==> 101
	else if (dpsw_1 == 1 && dpsw_2 == 0 && dpsw_3 == 1){
		dpsw_state = dpsw_010;
	}
	// 011 ==> 100
	else if (dpsw_1 == 1 && dpsw_2 == 0 && dpsw_3 == 0){
		dpsw_state = dpsw_011;
	}
	// 100 ==> 011
	else if (dpsw_1 == 0 && dpsw_2 == 1 && dpsw_3 == 1){
		dpsw_state = dpsw_100;
	}
	// 101 ==> 010
	else if (dpsw_1 == 0 && dpsw_2 == 1 && dpsw_3 == 0){
		dpsw_state = dpsw_101;
	}
	// 110 ==> 001
	else if (dpsw_1 == 0 && dpsw_2 == 0 && dpsw_3 == 1){
		dpsw_state = dpsw_110;
	}
	// 111 ==> 000
	else if (dpsw_1 == 0 && dpsw_2 == 0 && dpsw_3 == 0){
		dpsw_state = dpsw_111;
	}
}
void display_rpm(int displaycounter){
	
	/* this function displays the average rpm in 3 7segment displays
	only the first 3 digits are displayed, i.e. 1440 rpm = 144 on display and
	480 rpm = 048 on display
	
	The array used is seven_seg_code[] where each index has a value that
	corresponds to that index. For ex seven_seg_code[5] = 0x92 = 1001 0010
	(xgfedcba) = 5 on 7 segment
	
	We can extract the first 3 digits of the rpm by the divide (/) and modulo(%)
	functions. For ex. let x = 234/10 = 23, then first digit = 23/10 = 2
	second digit = 23%10 = 3 and third digit = 234%10 = 4*/
	int digit1;
	int digit2;
	int digit3;
	
	int temp1;		//Temp vals used in calculation
	int temp2;
	
	if (avg_rpm<=0){
		// If rpm is 0 or less than 0 then display 0 in all segments
		digit1 = seven_seg_code[0];
		digit2 = seven_seg_code[0];
		digit3 = seven_seg_code[0];
	}
	else if (avg_rpm > 0 && avg_rpm<10){
		// For ex. rpm = 8, then display avg rpm in 3rd segment
		digit1 = seven_seg_code[0];
		digit2 = seven_seg_code[0];
		digit3 = seven_seg_code[avg_rpm];
	}
	else if (avg_rpm >= 10 && avg_rpm < 100){
		// For ex. rpm = 83, 83/10 = 8 -> Segment 3, 83%10 = 3 -> Segment 2
		digit1 = seven_seg_code[0];
		digit2 = seven_seg_code[0];
		digit3 = seven_seg_code[avg_rpm/10];
	}
	else if (avg_rpm >= 100 && avg_rpm <1000){
		/* For ex. rpm = 673, then let temp1 = 673/10 = 67 then 
		temp1/10 = 6 -> segment1
		temp1%10 = 7 -> segment2
		673%10 = 3   -> segment3*/
		
		temp1 = avg_rpm/10;
		digit1 = seven_seg_code[0];
		digit2 = seven_seg_code[temp1/10];
		digit3 = seven_seg_code[temp1%10];
	}
	else if (avg_rpm >= 1000 && avg_rpm < 10000){
	/* For ex, rpm = 1467, then let temp1 = 1467/10 = 146 and temp2 = 146/10 = 14
		temp2/10 = 1 -> segment 1
		temp2%10 = 4 -> segment 2
		temp1%10 = 6 -> segment 3*/
		
		temp1 = avg_rpm/10;
		temp2 = temp1/10;
		digit1 = seven_seg_code[temp2/10];
		digit2 = seven_seg_code[temp2%10];
		digit3 = seven_seg_code[temp1%10];
	}

	if (displaycounter == 1){
	//turn on switch 1, turn off switch 2 and 3, and display 1st digit on seg1
	seven_seg_sw1 = 1;	//P2^3 = 1
	seven_seg_sw2 = 0;
	seven_seg_sw3 = 0;
	P0 = digit1;
	}
	//delay(5);

	if (displaycounter == 2){
	//turn on switch 2, turn off switch 1 and 3, and display 2nd digit on seg2
	seven_seg_sw1 = 0;
	seven_seg_sw2 = 1;
	seven_seg_sw3 = 0;
	P0 = digit2;
	}
	//delay(5);

	if (displaycounter == 3){
	//turn on switch 3, turn off switch 1 and 2, and display 3rd digit on seg3
	seven_seg_sw1 = 0;
	seven_seg_sw2 = 0;
	seven_seg_sw3 = 1;
	P0 = digit3;
	}
	//delay(5);

}
//void delay(int num_of_100us_delay){
//	// This function is used to create a delay for the duty cycle. Each delay is 100us
//	// thus for a delay of 1ms we call delay(10)
//	int i;
//	for(i=0;i<num_of_100us_delay;i++){
//		TMOD = 0x11;		// For 100us delay of 11.0592MHz oscillator with 12 oscillator periods per machine
//		TH0 = 0xFF;			// cycles, we need 92 machine cycles. FFFF - FFA3 = 5C (HEX) = 92 (DEC)
//		TL0 = 0xA3;
//		TR0 = 1;
//		while (TF0 == 0){
//			// While the counter has not reached max value, do nothing
//		}
//		TF0 = 0;				// After count has maxed, clear overflow flag and timer
//		TR0 = 0;
//	}
//}


//***************EDITS***********************************/

void delay(int num_of_100us_delay){
	// This function is used to create a delay for the duty cycle. Each delay is 100us
	// thus for a delay of 1ms we call delay(10)
	int i;
	for(i=0;i<num_of_100us_delay;i++){
		TMOD = 0x11;		// For 100us delay of 11.0592MHz oscillator with 12 oscillator periods per machine
		TH0 = 0xFF;			// cycles, we need 92 machine cycles. FFFF - FFA3 = 5C (HEX) = 92 (DEC)
		TL0 = 0xA3;
		TR0 = 1;
		while (TF0 == 0){
			// While the counter has not reached max value, do nothing
		}
		TF0 = 0;				// After count has maxed, clear overflow flag and timer
		TR0 = 0;
	}
}