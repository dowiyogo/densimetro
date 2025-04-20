#include <LiquidCrystal.h>
#define Select 0
#define Right 1
#define Left 2
#define Up 3
#define Down 4

LiquidCrystal lcd(8,13,9,4,5,6,7);

int adc_botton_value[5]={50, 250, 400, 600, 800}; //valores posibles
int x=0;
int nav =-1;
int enter=1;                     //entrar al menu una vez inicializado
int boton;                          // lectura de botones
int menuctrl=0;                      //control del menu
int punto =0;                    //control de cursor del menu
int subctrl =1;




void setup() {
lcd.begin(16,2); //inicializacion del lcd
lcd.clear();


}

void loop() {
  
   
 if (x==0){                  //inicio del lcd
  analogWrite(10,250);
  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.print("Inicia");
  
  analogWrite (10,200);
  
  
  delay (1000);
  x=1;
   boton = analogRead(0);
    
 }
 

 boton = analogRead(0);     //lectura de teclado
 
 nav=get_key(boton);
 movemenu();
 

   lcd.setCursor(15,punto);
   //lcd.print(punto, DEC);
   lcd.blink();
 
 
 
 
 
 if (enter) menu();
}

void menu(){     // menu en lcd
  
  enter=0;
    
  lcd.clear();
  
  switch (menuctrl){
   
 
    case 0 :
    //lcd.clear();
    if(subctrl==1){
    lcd.setCursor(0,0);
    lcd.print("-Lectura");
    lcd.setCursor(0,1);
    lcd.print("-Visualizar");
    lcd.setCursor(10,punto);
    }
    
    if(subctrl==2){
    lcd.setCursor(0,0);
    lcd.print("-Visualizar");
    lcd.setCursor(0,1);
    lcd.print("-Calibrar");
    lcd.setCursor(10,punto);
  }
  
   if(subctrl==3){
    lcd.setCursor(0,0);
    lcd.print("-Calibrar");
    lcd.setCursor(0,1);
    lcd.print("-Settings LCD");
    lcd.setCursor(10,punto);
  }
    lcd.blink();
    break;
    
    
    case 1 :
    lcd.print("Entro lectura");
    lcd.setCursor(0,1);
    lcd.print("-Back");
    lcd.blink();
    break;
    
    case 2 :
    lcd.print("Entro Visualzar");
    lcd.setCursor(0,1);
    lcd.print("-Back");
    lcd.blink();
    break;
    
    case 3 :
    lcd.print("Entro Calibrar");
    lcd.setCursor(0,1);
    lcd.print("-Back");
    lcd.blink();
    break;
    
    case 4 :
    lcd.print("Entro Settings");
    lcd.setCursor(0,1);
    lcd.print("-Back");
    lcd.blink();
    break;
    
    
  }
 
 
  
   
   
 /*  switch (nav){
     case 0 : 
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Right");
     lcd.setCursor(0,1);
     lcd.print(boton, DEC);
     lcd.blink();
     break;
     
      case 1 : 
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Up");
     lcd.setCursor(0,1);
     lcd.print(boton, DEC);
     lcd.blink();
     break;
  
    case 2 : 
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Down");
     lcd.setCursor(0,1);
     lcd.print(boton, DEC);
     lcd.blink();
     break;
     
       case 3 : 
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Left");
     lcd.setCursor(0,1);
     lcd.print(boton, DEC);
     lcd.blink();
     break;
     
       case 4 : 
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Select");
     lcd.setCursor(0,1);
     lcd.print(boton, DEC);
     lcd.blink();
     break;
     
     case -1:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("error ");
       lcd.print(boton, DEC);
       
       //lcd.setCursor(0,1);
       //lcd.print(x,DEC);
       delay(2000);    
       break;
       
}
*/
}
  
  
  void movemenu() {    // movimientos del menu lcd
    
    switch (nav){
    
     case 1:
    punto --;
    if (punto <0){ subctrl --;
    
       punto =0;
       
      if (subctrl<1) subctrl =1;
   
     
       menu();
    } 
    
    break;
    
    case 2:
    punto ++;
    if (punto>1)subctrl ++ ;
    
    punto =1;
         if (subctrl>3){ subctrl =3;
           
      menu();
    }
    
    break;
    
    case 4:
    if (menuctrl==0) {if (subctrl==1 & punto==0) {
 
      menuctrl=1;
      menu();
      
    }else if (subctrl==1 & punto==1){
     
      punto =0;
      menuctrl=2;      
      menu();
      
    }  else if(subctrl==2 & punto==0){

        menuctrl=2;
        menu();
    }       else if(subctrl==2 & punto==1){
      
              punto=0;    
              menuctrl=3;              
              menu();
            }     else if(subctrl==3 & punto==0){
              
              menuctrl=3;
              menu();
            }     else if(subctrl==3 & punto==1){
              
                            punto =0;
                            menuctrl=4;
                            menu();
                          }
    break;
    }
   /*if (menuctrl==1) if (punto>=1)  {
     
     menuctrl=0;
     subctrl=1;
     punto=0;
     menu();
     break;
   }
 
 
  if (menuctrl==2) if (punto>=1)  {
     
     menuctrl=0;
     subctrl=1;
     punto=0;
     menu();
     break;
   }
 
    if (menuctrl==3) if (punto>=1)  {
     
    menuctrl=0;
    subctrl=1;
    punto=0;
    menu();
    break;
   }
 
 
    if (menuctrl==4) if (punto>=1)  {
     
    menuctrl=0;
    subctrl=1;
    punto=0;
    menu();
    break;
   }
 
     
    */
    }
  }
  
  

int get_key(unsigned int input)  //Funcion para capturar el teclado....
    {
      int k;

      for (k = 0; k < 5; k++)
      {
        if (input < adc_botton_value[k])
        {
          return k;
        }
      }

      if (k >= 5)k = -1;  // Error en la lectura
      return k;
    }
  

