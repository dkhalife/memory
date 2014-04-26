#line 1 "H:/PIC Projects/Memory/memory.c"
#line 39 "H:/PIC Projects/Memory/memory.c"
unsigned int i = 0;
unsigned char Seed=1;
unsigned int cursor = 0;
unsigned int recording = 0;
unsigned char sequence[50];
unsigned int oldstate[8] = {0,0,0,0,0,0,0,0};




unsigned char prand(int Lim, int Start){
 return rand()%Lim+Start;
}




void Save(){
 I2C1_Start();
 I2C1_Wr(0xA0);
 I2C1_Wr(0x00);
 I2C1_Wr(0x02);

 for(i=0; i<50; i++){
 I2C1_Wr((short)sequence[i]);
 }
 I2C1_Stop();
 Delay_us(1);
}

void Load(){
 for(i=0; i<50; i++){
 I2C1_Start();
 I2C1_Wr(0xA0);
 I2C1_Wr(0x00);
 I2C1_Wr(i+ 2 );
 I2C1_Repeated_Start();
 I2C1_Wr(0xA1);
 sequence[i]=(char)I2C1_Rd(0u);
 I2C1_Stop();
 Delay_us(1);
 }
}




int Input(int PORT, int pin){
 if (Button(&PORT, pin, 1, 1)) oldstate[pin] = 1;
 if (oldstate[pin] && Button(&PORT, pin, 1, 0)){
 oldstate[pin] = 0;
 return  1 ;
 }
 return  0 ;
}




void main() {









 OSCCON=0x6E;





 CMCON=0x07;








 ADCON0=0x02;








 ADCON1=0x0F;








 TRISA = 0x1F;
 TRISB = 0x00;
 TRISD = 0x0F;




 PORTA = TRISA & 0xFF;
 PORTB = 0x00;
 PORTD = TRISD & 0xFF;
 Sound_Init(&PORTA, 5);

 srand(Seed);
 I2C1_Init(100000);


  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 1 ;
 Sound_Play( 500 ,  250 );
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 0 ;
 Load();
 Delay_ms(2* 250 );
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 1 ;
 Sound_Play( 500 ,  250 );
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 0 ;

 while(1){
 if(recording==0){
 if(Input( PORTD , 3 )){

 for(i=0; i<50; i++){
 switch(sequence[i]){
 case 0:  RB0_bit = 1 ; Sound_Play( 1200 ,  250 );  RB0_bit = 0 ; break;
 case 1:  RB1_bit = 1 ; Sound_Play( 1400 ,  250 );  RB1_bit = 0 ; break;
 case 2:  RB2_bit = 1 ; Sound_Play( 1600 ,  250 );  RB2_bit = 0 ; break;
 case 3:  RB3_bit = 1 ; Sound_Play( 1800 ,  250 );  RB3_bit = 0 ; break;
 case 4:  RB4_bit = 1 ; Sound_Play( 2000 ,  250 );  RB4_bit = 0 ; break;
 default : i=50;
 }
 }
 recording=0;
 }
 if(Input( PORTD , 1 )){

 for(i=0; i< 25 ; i++){
 Seed=prand(4,0);
 switch(Seed){
 case 0:  RB0_bit = 1 ; Sound_Play( 1200 ,  250 );  RB0_bit = 0 ; break;
 case 1:  RB1_bit = 1 ; Sound_Play( 1400 ,  250 );  RB1_bit = 0 ; break;
 case 2:  RB2_bit = 1 ; Sound_Play( 1600 ,  250 );  RB2_bit = 0 ; break;
 case 3:  RB3_bit = 1 ; Sound_Play( 1800 ,  250 );  RB3_bit = 0 ; break;
 case 4:  RB4_bit = 1 ; Sound_Play( 2000 ,  250 );  RB4_bit = 0 ; break;
 }
 }
 }
 if(Input( PORTD , 2 )){

 for(i=0; i<50; i++){
 sequence[i]=8;
 }
 cursor=0;
 recording=1;
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 1 ;
 Sound_Play( 500 ,  250 );
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 0 ;
 }

 if(Input( PORTA , 4 )){
  RB0_bit = 1 ;
 Sound_Play( 1200 ,  250 );
  RB0_bit = 0 ;
 }
 if(Input( PORTA , 3 )){
  RB1_bit = 1 ;
 Sound_Play( 1400 ,  250 );
  RB1_bit = 0 ;
 }
 if(Input( PORTA , 2 )){
  RB2_bit = 1 ;
 Sound_Play( 1600 ,  250 );
  RB2_bit = 0 ;
 }
 if(Input( PORTA , 1 )){
  RB3_bit = 1 ;
 Sound_Play( 1800 ,  250 );
  RB3_bit = 0 ;
 }
 if(Input( PORTA , 0 )){
  RB4_bit = 1 ;
 Sound_Play( 2000 ,  250 );
  RB4_bit = 0 ;
 }
 }
 else{

 if(cursor==49 || Input( PORTD , 2 )){
 cursor=0;
 recording=0;
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 1 ;
 Sound_Play( 500 ,  250 );
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 0 ;
 Save();
 Delay_ms( 250 );
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 1 ;
 Sound_Play( 500 ,  250 );
  RB0_bit = RB1_bit = RB2_bit = RB3_bit = RB4_bit = 0 ;
 }

 if(Input( PORTA , 4 )){
 sequence[cursor]=0;
 cursor++;
  RB0_bit = 1 ;
 Sound_Play( 1200 ,  250 );
  RB0_bit = 0 ;
 }
 if(Input( PORTA , 3 )){
 sequence[cursor]=1;
 cursor++;
  RB1_bit = 1 ;
 Sound_Play( 1400 ,  250 );
  RB1_bit = 0 ;
 }
 if(Input( PORTA , 2 )){
 sequence[cursor]=2;
 cursor++;
  RB2_bit = 1 ;
 Sound_Play( 1600 ,  250 );
  RB2_bit = 0 ;
 }
 if(Input( PORTA , 1 )){
 sequence[cursor]=3;
 cursor++;
  RB3_bit = 1 ;
 Sound_Play( 1800 ,  250 );
  RB3_bit = 0 ;
 }
 if(Input( PORTA , 0 )){
 sequence[cursor]=4;
 cursor++;
  RB4_bit = 1 ;
 Sound_Play( 2000 ,  250 );
  RB4_bit = 0 ;
 }
 }
 }
}
