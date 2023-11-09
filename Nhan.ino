

#include <avr/interrupt.h>
#include <avr/io.h>
#define STEP_1   2 
#define DIR_1    3 
#define STEP_2   4 
#define DIR_2    5
#define STEP_3   6
#define DIR_3    7
#define EN_3     8

char a = '0' ; 
String m  ;
int index =   0   ;  
int indexOfInt = 0 ; 
int step_M1 = 0  ; 
int step_M2 = 0  ;
int step_M3 = 0 ; 
bool run_Pr = false ; 


String aaaa   = "hiell " ; 
void run (int step1  , const int step_Pin  , const int dir_Pin    , int direc = 0 )  ; 
void run_M1 (int step1  , const int step_Pin  , const int dir_Pin    , int direc = 0 ) ;
void setup () 
{
  pinMode (STEP_1 , OUTPUT) ;
  pinMode (DIR_1, OUTPUT ) ;
  pinMode (STEP_2 , OUTPUT) ;
  pinMode (DIR_2, OUTPUT ) ;
  pinMode (STEP_3 , OUTPUT) ;
  pinMode (DIR_3, OUTPUT ) ;
  pinMode (EN_3, OUTPUT ) ;
  
  digitalWrite (EN_3 , HIGH   ) ; 
  UCSR0B |=   (1<<RXCIE0 ) | (0<<TXCIE0) | (1 << RXEN0 ) | (1 << TXEN0) ;
  UCSR0C |=   (1<<UCSZ01) | (1<<UCSZ00) | (1<<USBS0) ;
  UBRR0L = 103 ;
  UDR0= 'a'  ;
sei();

}
void loop () 
{
  if (run_Pr){
    run_M1 (step_M1 , STEP_1 , DIR_1 ) ; 
    delay (100) ;
    digitalWrite (EN_3 , 1 )  ; 
    run (step_M2 , STEP_2 ,DIR_2 ,1   );
    delay (100);
     digitalWrite (EN_3 , 0 )  ; 
    run (step_M3 , STEP_3 , DIR_3 ,1 ) ; 
    delay(100) ;

    run_Pr = false ; 
    send_String ("done \n"); 
  }

}


ISR (USART_RX_vect ) {
 
a = UDR0 ;  
 while (( UCSR0A & 1 <<UDRIE0)  == 0 ) ;
if (a != 'e')
{  
 m = m + a ; 
}else if (a == 'e'){
  int space_1  = m. indexOf (' ') ;
  int space_2 = m. lastIndexOf (' ') ; 
  String step1 = m.substring(0 , space_1) ;
  String step2 = m.substring(space_1 + 1 , space_2) ;
  String step3 =  m.substring(space_2 + 1 ) ;

  step_M1 = step1.toInt () ; 
  step_M2 = step2.toInt () ;
  step_M3 = step3.toInt () ;
  m = "" ;
  run_Pr  = true ; 

  send_String (step1) ;
  send_String (step2) ;
  send_String (" 2 " ) ;
  send_String (step3) ;
  send_String (" " ) ;
  send_String ("3 \n") ;
}
 
}


void  USART_send(char c ) 
{
// wait until UDRE flag is set to logic 1
while ((UCSR0A & (1<<UDRE0)) == 0x00){;}

UDR0 = c; // Write character to UDR for transmission
}

void send_String (String c ) 
{
  for (int i = 0 ; i < c.length() ; i++ ){
    USART_send (c[i]) ; 
  }
}


void run (int step1  , const int step_Pin  , const int dir_Pin    , int direc = 0 ) 
{
  if (step1 > 0 ) {
    digitalWrite (dir_Pin , direc )  ;
    for (int i = 0 ; i < step1 ; i ++  ) {
      digitalWrite (step_Pin , 0 ) ;
      delay (15 ) ;
      digitalWrite (step_Pin , 1 ) ;
      delay (15 ) ;
    } 
  }else {
    digitalWrite (dir_Pin , !direc ) ; 
    for (int i = 0 ; i < -step1 ; i ++  ) {
      digitalWrite (step_Pin , 0 ) ;
      delay (15 ) ;
      digitalWrite (step_Pin , 1 ) ;
      delay (15 ) ; 
    } 
    
  }
}
void run_M1 (int step1  , const int step_Pin  , const int dir_Pin    , int direc = 0 ) 
{
  if (step1 > 0 ) {
    digitalWrite (dir_Pin , direc )  ;
    for (int i = 0 ; i < step1 ; i ++  ) {
      digitalWrite (step_Pin , 0 ) ;
      delay (1 ) ;
      digitalWrite (step_Pin , 1 ) ;
      delay (1 ) ;
    } 
  }else {
    digitalWrite (dir_Pin , !direc ) ; 
    for (int i = 0 ; i < -step1 ; i ++  ) {
      digitalWrite (step_Pin , 0 ) ;
      delay (5 ) ;
      digitalWrite (step_Pin , 1 ) ;
      delay (5 ) ; 
    } 
    
  }
}

