/* COMP2215 Task 5---SKELETON */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "rios.h"
#include "image.h"
#include "ruota.h"


#define MENU_STATE 0 
#define COUNTDOWN_STATE 1
#define DRAW_STATE 2
#define GUESS_STATE 3
#define RELEASE_STATE_WIN 4
#define RELEASE_STATE_LOSE 5
#define LOSE_STATE 6
#define DEMO_STATE 7

#define CENTRE SWC

#define LEFT SWW
#define RIGHT SWE
#define UP SWN
#define DOWN SWS

#define DEMO_TIME 400


int gameState;
uint16_t random_seed;
int gameMode;
int score = 0;

// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Menu --------------------------------------------------
// ------------------------------------------------------------------------------------------------

void menuState();
volatile counter=0;

int firstMenu=1;
int number = 0;
void menuState(){
    if(firstMenu){
        clear_screen();
        display_string_xy("WELCOME TO THE LAFORTUNA DANCE MAT",40,30);
        display_string_xy("Left Click - Normal Mode",40,100);
        display_string_xy("Right Click - Inverse Mode",40,120);

        firstMenu=0;
        counter=0;
 	} else if(!firstMenu){
 		if(counter >= DEMO_TIME){
 			gameState = DEMO_STATE;
			firstMenu=1;
 		}else if (get_switch_state(_BV(LEFT))) {
 			countdownSetUp();
			gameState=COUNTDOWN_STATE;
			gameMode=0;
			firstMenu=1;
		}else if(get_switch_state(_BV(RIGHT))) {
 			countdownSetUp();
			gameState=COUNTDOWN_STATE;
			firstMenu=1;
			gameMode=1;
		}
 	}
}

// ------------------------------------------------------------------------------------------------
// --------------------------------  Random Number Generation  ------------------------------------
// ------------------------------------------------------------------------------------------------

/* functions rand_init and rand are taken from Giacomo Meanti's
   Space Invaders game from 2014-2015
*/

uint16_t rand_init(void) {
    //ADC conversion from unused pins should give random results.
    //an amplification factor of 200x is used.
    ADMUX |= _BV(REFS0) | _BV(MUX3) | _BV(MUX1) | _BV(MUX0);
    //Prescaler
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1);
    //Ena+ble and start
    ADCSRA |= _BV(ADEN) | _BV(ADSC);
    //Wait until complete
    while(! (ADCSRA & _BV(ADIF))) {
        _delay_ms(2);
    }
    //Read result
    uint16_t res = ADCL;
    res |= ADCH << 8;
    //Disable
    ADCSRA &= ~_BV(ADEN);
    
    //The XOR should increase the randomness?
    //since the converted number is only 10 bits
    return res ^ 0xACE1u;
}

//http://en.wikipedia.org/wiki/Linear_feedback_shift_register
uint16_t rand(void) {
    unsigned lsb = random_seed & 1;  /* Get lsb (i.e., the output bit). */
    random_seed >>= 1;               /* Shift register */
    if (lsb == 1)             /* Only apply toggle mask if output bit is 1. */
        random_seed ^= 0xB400u;        /* Apply toggle mask, value has 1 at bits corresponding
                             * to taps, 0 elsewhere. */
    return random_seed;
}


// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Demo State  --------------------------------------
// ------------------------------------------------------------------------------------------------
void demoState();

int firstDemo=1;

void demoState(){
	if(get_switch_state(_BV(LEFT)) || get_switch_state(_BV(RIGHT)) || get_switch_state(_BV(UP)) || get_switch_state(_BV(DOWN)) || get_switch_state(_BV(CENTRE))){
        gameState=MENU_STATE;
        firstDemo=1;
        return;
    }if(firstDemo){
    	clear_screen();
    	display_string_xy("Press anything to play", WIDTH/3 - 15, HEIGHT/2);
    	firstDemo=0;
    	counter=0;
    }else if (counter > 30){
    	image randomRectangle;
    	randomRectangle.left = rand()%240;
    	randomRectangle.top = rand()%320;
    	randomRectangle.height = rand()%100;
    	randomRectangle.width = rand()%100;
    	fill_image(randomRectangle,rand()%65535);
    	counter=0;

    	display_string_xy("Press anything to play", WIDTH/3 - 15, HEIGHT/2);
    }
}

// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Countdown State  --------------------------------------
// ------------------------------------------------------------------------------------------------

void countdownSetUp();
void countdownState();

void countdownSetUp(){
	clear_screen();
	counter=0;
}

int first3=1;
int first2=1;
int first1=1;
int firstGo=1;

int timeToComplete;

void countdownState(){
	timeToComplete = 160;
	score=0;
	if(counter <= 40 && first3){
		display_string_xy("3", WIDTH/2 - 2, HEIGHT/2);
		first3=0;
	}else if(counter > 40 && counter <= 80 && first2){
		clear_screen();
		display_string_xy("2", WIDTH/2 - 2, HEIGHT/2);
		first2=0;
	}else if(counter > 80 && counter <= 120 && first1){
		clear_screen();
		display_string_xy("1", WIDTH/2 - 2, HEIGHT/2);
		first1=0;
	}else if(counter > 120 && counter <= 160 && firstGo ){
		clear_screen();
		display_string_xy("GO", WIDTH/2 - 5, HEIGHT/2);
		firstGo=0;
	}else if(counter > 160){
		gameState=DRAW_STATE;
		first3=1;
		first2=1;
		first1=1;
		firstGo=1;
	}
}



// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Draw State  -------------------------------------------
// ------------------------------------------------------------------------------------------------

void drawState();
char* getInstruction();
uint8_t currentInstruction;
char* output;

image currentImage;
uint16_t* colour;

void drawState(){
	clear_screen();
	output = getInstruction();
	
	display_string_xy(output, WIDTH/2 - 12, HEIGHT/2);
	score += 1;
	gameState=GUESS_STATE;

	counter = 0;

}



char* getInstruction(){
	int number = rand()%5;
	if(number == 0){
		if(gameMode){
			currentInstruction = RIGHT;
		}else{
			currentInstruction = LEFT;
		}
		return "Left";
	}else if(number == 1){
		if(gameMode){
			currentInstruction = LEFT;
		}else{
			currentInstruction = RIGHT;
		}
		return "Right";
	}else if(number == 2){
		if(gameMode){
			currentInstruction = DOWN;
		}else{
			currentInstruction = UP;
		}
		return "Up";
	}else if(number == 3){
		if(gameMode){
			currentInstruction = UP;
		}else{
			currentInstruction = DOWN;
		}
		return "Down";
	}else if(number == 4){
		currentInstruction = CENTRE;
		return "Centre";
	}else{
		return "error";
	}
}

// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Guess State  -------------------------------------------
// ------------------------------------------------------------------------------------------------

void guessState();
void checkReleaseWin();
void checkReleaseLose();

void guessState(){
	if (get_switch_state(_BV(currentInstruction))) {
		gameState = RELEASE_STATE_WIN;
	}else if(get_switch_state(_BV(LEFT)) || get_switch_state(_BV(RIGHT)) || get_switch_state(_BV(UP)) || get_switch_state(_BV(DOWN)) || get_switch_state(_BV(CENTRE))){
		gameState = RELEASE_STATE_LOSE;
	}else if(counter > timeToComplete){
		gameState = LOSE_STATE;
	}
}

void checkReleaseWin(){
	if (!get_switch_state(_BV(currentInstruction))) {
		win_temp();
		timeToComplete -= 4;
		gameState = DRAW_STATE;
	}else if(counter > timeToComplete){
		gameState = RELEASE_STATE_LOSE;
	}
}

void checkReleaseLose(){
	if (!get_switch_state(_BV(CENTRE))) {
		gameState = LOSE_STATE;
	}
}

// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Lose State  -------------------------------------------
// ------------------------------------------------------------------------------------------------

void loseState();

int firstLose = 1;
void loseState(){
	if(firstLose){
		clear_screen();
		display_string("UNLUCKY YOU LOSE!!!\n\nPress Enter to go back to menu");
		char char_score[6555];
		sprintf(char_score, "%d", score-1);
		display_string_xy("YOU SCORED: ", 40,100);
		display_string_xy(char_score, 110,100);
		firstLose=0;
		counter=0;
	}else{
		if(counter >= DEMO_TIME){
			gameState=DEMO_STATE;
			firstLose=1;
		}else if (get_switch_state(_BV(CENTRE))) {
			gameState=MENU_STATE;
			firstLose=1;
		}
	}
	
}



// ------------------------------------------------------------------------------------------------
// ---------------------------------------- Main --------------------------------------------------
// ------------------------------------------------------------------------------------------------


int displayFrame(int state);

volatile flag=0;

int displayFrame(int state){
    counter++;
    flag=1;
    return state;
}

int main(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
      
    init_lcd(); //Initiate lcd screen
    os_init_scheduler(); //Initiate scheduler
    os_init_ruota();

    random_seed = rand_init();
   	gameState=MENU_STATE;
   	os_add_task(displayFrame,            25, 1); //This is going to start counter which is going to be important
   	sei();

   	while(1){
            if(flag){
                cli();
                flag=0;
                sei();
                switch(gameState){
                    case MENU_STATE:
                        menuState();
                        break;
                    case COUNTDOWN_STATE:
                    	countdownState();
                    	break;
                    case DRAW_STATE:
                    	drawState();
                    	break;
                    case GUESS_STATE:
                    	guessState();
                    	break;
                    case RELEASE_STATE_WIN:
                    	checkReleaseWin();
                    	break;
                    case RELEASE_STATE_LOSE:
                    	checkReleaseLose();
                    	break;
                    case LOSE_STATE:
                    	loseState();
                    	break;
                    case DEMO_STATE:
                    	demoState();
                    	break;
                }
                    
            }
   };


}









