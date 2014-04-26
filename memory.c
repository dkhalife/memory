//**
//  Hardware Connections and Constants
//**
#define		ON			1
#define		OFF			0

#define		SOUND1		1200
#define		SOUND2		1400
#define		SOUND3		1600
#define		SOUND4		1800
#define		SOUND5		2000
#define		SOUND6		500

#define		LED1		RB0_bit
#define		LED2		RB1_bit
#define		LED3		RB2_bit
#define		LED4		RB3_bit
#define		LED5		RB4_bit

#define		BUTTONS		PORTA
#define		BTN1		4
#define		BTN2		3
#define		BTN3		2
#define		BTN4		1
#define		BTN5		0

#define		ACTIONS		PORTD
#define		RECORD		2
#define		PLAY		3
#define		RANDOM		1

#define		DELAY		250
#define		RAND_L		25
#define		START_ADR	2

//**
//  Variable Definitions
//**
unsigned int i = 0;
unsigned char Seed=1;
unsigned int cursor = 0;
unsigned int recording = 0;
unsigned char sequence[50];
unsigned int oldstate[8] = {0,0,0,0,0,0,0,0};

//**
//  Pseudo-Random Number
//**
unsigned char prand(int Lim, int Start){
	return rand()%Lim+Start;
}

//**
//  Save via I2C
//**
void Save(){
	I2C1_Start();			     // Start BIT
	I2C1_Wr(0xA0);			     // ControlCode[4](1010) - BlockSelect[1] - ChipSelect[2](A1A0) - R/W[1](1/0) --> ACK
	I2C1_Wr(0x00);			     // ADR [MSB]
	I2C1_Wr(0x02);			     // ADR [LSB]
	// Use page write method
	for(i=0; i<50; i++){
		I2C1_Wr((short)sequence[i]);    // DATA
	}
	I2C1_Stop();			     // Stop
	Delay_us(1);
}

void Load(){
	for(i=0; i<50; i++){
		I2C1_Start();			      // Start BIT
		I2C1_Wr(0xA0);			      // ControlCode[4](1010) - BlockSelect[1] - ChipSelect[2](A1A0) - R/W[1](1/0) --> ACK
		I2C1_Wr(0x00);			      // ADR [MSB]
		I2C1_Wr(i+START_ADR);	 	  // ADR [LSB]
		I2C1_Repeated_Start();		  // Wait for slave to ACK
		I2C1_Wr(0xA1);			      // ControlCode[4](1010) - BlockSelect[1] - ChipSelect[2](A1A0) - R/W[1](1/0) --> ACK
		sequence[i]=(char)I2C1_Rd(0u);// Read (no ACK)
		I2C1_Stop();			      // Stop Bit
		Delay_us(1);
	}
}

//**
//  Button Input
//**
int Input(int PORT, int pin){
	if (Button(&PORT, pin, 1, 1)) oldstate[pin] = 1;
	if (oldstate[pin] && Button(&PORT, pin, 1, 0)){
		oldstate[pin] = 0;
		return ON;
	}
	return OFF;
}

//**
//  Main Program
//**
void main() {
	//**
	//  Oscillator Configuration:
	//  o 0b0
	//  o 4 MHz (110)
	//  o Primary system clock (1)
	//  o Stable frequency (1)
	//  o System clock : Internal RC (10)
	//  o Total : 0b01101110
	//**
	OSCCON=0x6E;
	//**
	//  Comparator Configuration:
	//  o Disable all
	//  o Total : 0b00000111;
	//**
	CMCON=0x07;
	//**
	//  A/D Configuration:
	//  o 0b00
	//  o Select An0 (0000)
	//  o Convertor idle (0)
	//  o A/D Convertor enabled (1)
	//  o Total : 0b00000010;
	//**
	ADCON0=0x02;
	//**
	//  A/D Configuration:
	//  o 0b00
	//  o VRef - = VSS (0)
	//  o VRef + = VDD (0)
	//  o All Digital (1111)
	//  o Total : 0b00001111;
	//**
	ADCON1=0x0F;
	//**
	//  PORT Directions
	//  o Inputs
	//    PORTA
	//	 PORTD
	//  o Outputs
	//	 PORTB
	//**
	TRISA = 0x1F;
	TRISB = 0x00;
	TRISD = 0x0F;
	
	//**
	//  Initialization
	//**
	PORTA = TRISA & 0xFF;
	PORTB = 0x00;
	PORTD = TRISD & 0xFF;
	Sound_Init(&PORTA, 5);
	
	srand(Seed);
	I2C1_Init(100000);
	
	// Auto Load Saved Sequence
	LED1=LED2=LED3=LED4=LED5=ON;
	Sound_Play(SOUND6, DELAY);
	LED1=LED2=LED3=LED4=LED5=OFF;
	Load();
	Delay_ms(2*DELAY);
	LED1=LED2=LED3=LED4=LED5=ON;
	Sound_Play(SOUND6, DELAY);
	LED1=LED2=LED3=LED4=LED5=OFF;

	while(1){
		if(recording==0){
			if(Input(ACTIONS,PLAY)){
				//Play the sequence
				for(i=0; i<50; i++){
					switch(sequence[i]){
						case 0: LED1=ON; Sound_Play(SOUND1, DELAY); LED1=OFF; break;
						case 1: LED2=ON; Sound_Play(SOUND2, DELAY); LED2=OFF; break;
						case 2: LED3=ON; Sound_Play(SOUND3, DELAY); LED3=OFF; break;
						case 3: LED4=ON; Sound_Play(SOUND4, DELAY); LED4=OFF; break;
						case 4: LED5=ON; Sound_Play(SOUND5, DELAY); LED5=OFF; break;
						default : i=50;
					}
				}
				recording=0;
			}
			if(Input(ACTIONS,RANDOM)){
				// Play a random sequence
				for(i=0; i<RAND_L; i++){
					Seed=prand(4,0);
					switch(Seed){
						case 0: LED1=ON; Sound_Play(SOUND1, DELAY); LED1=OFF; break;
						case 1: LED2=ON; Sound_Play(SOUND2, DELAY); LED2=OFF; break;
						case 2: LED3=ON; Sound_Play(SOUND3, DELAY); LED3=OFF; break;
						case 3: LED4=ON; Sound_Play(SOUND4, DELAY); LED4=OFF; break;
						case 4: LED5=ON; Sound_Play(SOUND5, DELAY); LED5=OFF; break;
					}
				}
			}
			if(Input(ACTIONS,RECORD)){
				// Delete sequence
				for(i=0; i<50; i++){
					sequence[i]=8;
				}
				cursor=0;
				recording=1;
				LED1=LED2=LED3=LED4=LED5=ON;
				Sound_Play(SOUND6, DELAY);
				LED1=LED2=LED3=LED4=LED5=OFF;
			}
			//Simple Playing
			if(Input(BUTTONS,BTN1)){
				LED1=ON;
				Sound_Play(SOUND1, DELAY);
				LED1=OFF;
			}
			if(Input(BUTTONS,BTN2)){
				LED2=ON;
				Sound_Play(SOUND2, DELAY);
				LED2=OFF;
			}
			if(Input(BUTTONS,BTN3)){
				LED3=ON;
				Sound_Play(SOUND3, DELAY);
				LED3=OFF;
			}
			if(Input(BUTTONS,BTN4)){
				LED4=ON;
				Sound_Play(SOUND4, DELAY);
				LED4=OFF;
			}
			if(Input(BUTTONS,BTN5)){
				LED5=ON;
				Sound_Play(SOUND5, DELAY);
				LED5=OFF;
			}
		}
		else{
			// Auto Stop When we reach the limit
			if(cursor==49 || Input(ACTIONS,RECORD)){
				cursor=0;
				recording=0;
				LED1=LED2=LED3=LED4=LED5=ON;
				Sound_Play(SOUND6, DELAY);
				LED1=LED2=LED3=LED4=LED5=OFF;
				Save();
				Delay_ms(DELAY);
				LED1=LED2=LED3=LED4=LED5=ON;
				Sound_Play(SOUND6, DELAY);
				LED1=LED2=LED3=LED4=LED5=OFF;
			}
			//Capture all inputs
			if(Input(BUTTONS,BTN1)){
				sequence[cursor]=0;
				cursor++;
				LED1=ON;
				Sound_Play(SOUND1, DELAY);
				LED1=OFF;
			}
			if(Input(BUTTONS,BTN2)){
				sequence[cursor]=1;
				cursor++;
				LED2=ON;
				Sound_Play(SOUND2, DELAY);
				LED2=OFF;
			}
			if(Input(BUTTONS,BTN3)){
				sequence[cursor]=2;
				cursor++;
				LED3=ON;
				Sound_Play(SOUND3, DELAY);
				LED3=OFF;
			}
			if(Input(BUTTONS,BTN4)){
				sequence[cursor]=3;
				cursor++;
				LED4=ON;
				Sound_Play(SOUND4, DELAY);
				LED4=OFF;
			}
			if(Input(BUTTONS,BTN5)){
				sequence[cursor]=4;
				cursor++;
				LED5=ON;
				Sound_Play(SOUND5, DELAY);
				LED5=OFF;
			}
		}
	}
}