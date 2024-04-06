/**
 * The given template is a guideline for your coursework only.
 * You are free to edit/create any functions and variables.
 * You can add extra C files if required.
*/


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"

#define BUTTON_PIN			16	// Pin 21 (GPIO 16)
#define CAPACITY 10//size of hashtable
//reasons for using hastable data structure aare as follows makes it compatible with my ideas 
//as each morse code value is unique and we can refere to the index 
//of each morse code letter equivvalent
//in the values class to also be unique and use the indexes of those as the key value pairs


// declare global variables e.g., the time when the button is pressed 
int pressed ;
int counter ;
uint32_t duration_ms = 0;
uint32_t interval_ms = 0;
absolute_time_t start_time;
absolute_time_t end_time;
absolute_time_t start_interval;
absolute_time_t end_interval;
int tButtonPress = 0;
bool bpressed = false;
//values will be changed later in teh program
char SignalType[4] = {'\0','\0','\0','\0'}; 
bool signalFull = true;

//used to store the contents of signal type array in string form
char morseCompare[5] = "";
//stores all possible combinations that represetn letters
char dash_Dot_Combinations[26][5] = {
    ".-\0",//a
	"-...\0",//b
	"-.-.\0",//c
	"-..\0", //d
	".\0", //e
	"..-.\0",//f 
	"--.\0",//g
	"....\0",//h
	"..\0",//I
	".---\0",//j
	"-.-\0",//k
	".-..\0",//L
    "--\0", //M
	"-.\0", //N
	"---\0", //O
	".--.\0", //P
	"--.-\0", //Q
	".-.\0", //R
	"...\0", //s
	"-\0",//t
	"..-\0",//u
	 "...-\0",//v
	 ".--\0",//w
    "-..-\0",//x
	"-.--\0",//y
	"--..\0"//z
};


char Letters[26]= {
   'a' ,
	'b' ,
	'c',
	'd',
	'e',
	'f' ,
	'g',
	'h',
	'I',
	'j',
	'k',
	'L',
   'M' ,
	'N',
	'O',
	'P',
	'Q',
	'R',
	's',
	't',
	'u',
	'v',
	'w',
   'x' ,
	'y',
	'z',
};

void decoder();
void checkButton(uint32_t newduration);
void convertCharToString();
void resetSignal();
void compareMorseCode();
void checkInterval(uint32_t newinterval);
bool signalContainsNullValues();

// --------------------------------------------------------------------
//error trapping is needed since two inputs are not inluded for possible outcomes of code

int main() {
	stdio_init_all();

	// Initialise the seven segment display.
	seven_segment_init();

	// Turn the seven segment display off when the program starts.
	seven_segment_off();

	gpio_pull_down(BUTTON_PIN); // Pull the button pin towards ground (with an internal pull-down resistor).

	//Welcome message
	printf("%s \n", "WELCOME");
	sleep_ms(200);
	//7-segment display turns on & off horizontal segment (-)
	seven_segment_welcome();
	

    //break out of while loop then jump bakc in 
	//why we are oding this becausse we dont want the serail monitor verflowing with inputs due for loops
	//harder to debug and follow code;

	while (true) {
		//Checks if button is pressed
		if (gpio_get(BUTTON_PIN));
			//Records initial time button is pressed
			start_time = get_absolute_time();
			//Records end interval time between button presses (refer to line 54[?]) [NEW]<-------------
			end_interval = start_time;
			//turns on middle segment while button is pressed
			gpio_put(SEGMENT_G, false);
			while (gpio_get(BUTTON_PIN)){
			
			//Records start interval time between button presses [NEW]<-------------
			start_interval = end_time;	
		    } 
		//Records end time button is presesed
		end_time = get_absolute_time();
		//turns off middle segment when button is released
		gpio_put(SEGMENT_G, true);
		//Finds difference between start_time and end_time to find duration of button press, storing as duration_ms
		duration_ms = absolute_time_diff_us(start_time, end_time) / 1000;
		//dont claulaste untril second and consecutivr butoon prersses
		//Finds difference between start_interval and end_interval to find duration between button presses, 
		//stored as interval_ms [NEW]<-------------
		interval_ms = absolute_time_diff_us(start_interval, end_interval) / 1000;
		
		//Debug check for duration time
	
		checkButton(duration_ms);
		if (interval_ms > 500 || signalContainsNullValues() == false) {
        	// Clear the SignalType array and add its elements to morseCompare
            convertCharToString();
			// Perform actions for interval exceeding 400 ms or when the array is not null
            compareMorseCode();
        }
	}

	return 0;
}

// Function to check if SignalType array contains null values for all
bool signalContainsNullValues() {
        for (int i = 0; i < 4; i++) {
			if (SignalType[i] == '\0') {
				return true;  // Array contains at least one null value
			}
   		}
		return false;  // Array does not contain  any null values
}
    

void checkButton(uint32_t newduration){
	bool hasadded = false;
		//if the button is pressed for less than 200ms reocrded as a dot
		//shortened since dots can be gotten relatively quickly
		if(newduration > 0 && newduration < 200){
			printf("%s \n", "Recorded as a dot");
			for(int i=0;i<4;i++){
				if(hasadded == false && SignalType[i] == '\0' ){
					SignalType[i] = '.';
					hasadded = true;
				}
				//container if it exceedds limit of 4 not needed as it is not possible to do so however if invalid combinations are give
			}
			for(int i=0;i<4;i++){
				if(SignalType[i] != '\0'){
					printf("SignalType[%d]: %c\n", i, SignalType[i]);
				}
			}
		}
		//record as dot if over 200 as this gives more room for human reaciton time
		//when pressign button for desired input for desired letter output
		else if(newduration > 200 && newduration < 700){
			printf("%s \n", "Recorded as a dash");
			for(int i=0;i<4;i++){
				if(hasadded == false && SignalType[i] == '\0' ){
					SignalType[i] = '-';
					hasadded = true;
				}
				
			}
			//used to print every dash or dot that the user has presed
			//and showd all inputs and their respective array assigments
			//for current dash or dot encoded as well as previous inputs
			//if they exist
			for(int i=0;i<4;i++){
				if(SignalType[i] != '\0'){
					printf("SignalType[%d]: %c\n", i, SignalType[i]);
				}
			}
		}
		//if button is pressed for longer than 700ms, serial console displays erro input
		//input diplays 8 for an error
		else if(newduration > 700){
			printf("%s \n", "Error input. Button preseed for longer than 700ms.");
			//code for displayign 8 for an error
			seven_segment_show(26);
			sleep_ms(300);
			seven_segment_off();
		}
}
//this is called only once time has run out or array is full
void convertCharToString(){
	// use to append the signaltype array to the morseCompare 
	// because there is a datatype mismatch between signal and big 2d array
	for (int i = 0; i < 4 && SignalType[i] != '\0'; i++) {
		char charString[2] = {SignalType[i], '\0'};
		strcat(morseCompare, charString);
    }
	
}

void resetSignal(){
	// Reset SignalType to contain all null characters
    for (int i = 0; i < 4; i++) {
        SignalType[i] = '\0';
    }
	//reset morsecompare to empty string
	strcpy(morseCompare, "");
}

void compareMorseCode(){
	// we need to reset the signalType to null characters
	bool matchFound = false;
    // comparign morseCompare with the big array and then 
    for (int i = 0; i < 26; i++) {
        // Compare morseToCompare with each Morse code sequence in dash_Dot_Combinations
    	if (strcmp(morseCompare, dash_Dot_Combinations[i]) == 0) {
			//w ewnat to retirev erepsecitve array index or dash_Dot_Combinations[i] we want to transpose this index 
			//or i to the values array in seven segment file through the fucniton seven segment show
			matchFound = true;
			if (i < sizeof(values) / sizeof(values[0])) {
				printf("This is the letter expected: %c\n", Letters[i]);
				seven_segment_show(i);
				sleep_ms(250);
				seven_segment_off();
			}
			
        }//checks if the match has been found and the values assigned in each
		//possible index in teh signalType array adn when full if the last index has been
		//reached give error and morse code string
		//this is done only when full as every 1-3 input combination possible has been specified
		//but the same cant be said for when its  4 possible comiantions
		else if(matchFound == false && signalContainsNullValues() == false && i == 25){
				printf("Error: No match found for Morse code: %s\n", morseCompare);
				break;
		}
	}
	resetSignal();
}