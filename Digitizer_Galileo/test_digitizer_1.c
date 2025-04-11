#include <stdio.h>
#include "CAENDigitizer.h"
#include "./gpio/galileo_gpio.h"
#include "./gpio/display_arduino.h"
#include <unistd.h>
#include <sys/time.h>


int adc_botton_value[5]={500, 1550, 2250, 2850, 8050};
int menuctrl=0,subctrl=1;
int loop=1, nav=-1,punto=1,inicia=1;

void menu(){     // menu en lcd
  
  	//loop=0;
	clear();
  
  switch (menuctrl){
   
 
    case 0 :
    if(subctrl==1){
    firstline(0);
    lcd_print("-Lectura");
    secondline(0);
    lcd_print("-Version equipo");
    if (punto==1)firstline(15);
    if (punto==2)secondline(15);
    }
    
    if(subctrl==2){
    firstline(0);
    lcd_print("-Version equipo");
    secondline(0);
    lcd_print("-Calibrar");
    if (punto==1)firstline(15);
    if (punto==2)secondline(15);
  }
  
   if(subctrl==3){
    home();
    lcd_print("-Calibrar");
    secondline(0);
    lcd_print("-Settings LCD");
    if (punto==1)firstline(15);
    if (punto==2)secondline(15);		
  }
    onoff_display(0,1);
    break;
    
    
    case 1 :
    lcd_print("Leyendo ...");
    secondline(0);
    lcd_print("den=");
    onoff_display(1,1);
    break;
    
    case 2 :
firstline(0);
    lcd_print("Densimetro Nuclear");
secondline(0);    
    lcd_print("Version 1");
    onoff_display(0,1);
    break;
    
    case 3 :
    lcd_print("Entro Calibrar");
    firstline(0);
    //lcd_print("-Back");
    onoff_display(0,1);
    break;
    
    case 4 :
    lcd_print("Entro Settings");
    secondline(0);
    
    onoff_display(0,1);
    break;
    
    
  }

}

void movemenu() {    // movimientos del menu lcd
    
	//int see;
    switch (nav){
    
	
	case 1://down
    punto ++;
	
    if (punto > 2){subctrl ++ ;
    	punto =1;
	if (subctrl>3) subctrl =3;
        menu();
  
         
	 } else {if (punto==1)firstline(15);
    		if (punto==2)secondline(15);}    
    	 break;

	case 2:
	
	if(menuctrl!=0) menuctrl=0;
	menu();
	break;

	case 3://Select
    if (menuctrl==0) {if (subctrl==1 & punto==1) {
 
      menuctrl=1;
      menu();
      
    }else if (subctrl==1 & punto==2){
     
      punto =0;
      menuctrl=2;      
      menu();
      
    }  else if(subctrl==2 & punto==1){

        menuctrl=2;
        menu();
    }       else if(subctrl==2 & punto==2){
      
              punto=0;    
              menuctrl=3;              
              menu();
            }     else if(subctrl==3 & punto==1){
              
              menuctrl=3;
              menu();
            }     else if(subctrl==3 & punto==2){
              
                            punto =0;
                            menuctrl=4;
                            menu();
                          }
				}
	
    break;

	
     case 4:	//up
    punto --;
	 if (punto <1){ subctrl --;
    
       punto =2;
       
      if (subctrl<1) subctrl =1;
   
     menu();
     
    } else {if (punto==1)firstline(15);
    if (punto==2)secondline(15);
}
      
    break;
    
   
   
    }


}





int get_key(int input)  //Funcion para capturar el teclado....
    {
      int k;
	if (input<3500 && input>3300) return -1;

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

uint32_t th2int(int pol,float value){
	uint32_t ret = 0;
	switch(pol){
	case -1:	ret = (uint32_t) ((2.25+value)*0x4000/2.25);
			break;
	case 0:		ret = (uint32_t) ((1.25+value)*0x4000/2.25);
			break;
	case 1:		ret = (uint32_t) (value*0x4000/2.25);
			break;
	default:	ret=0;
	}
	return ret;
}


int execute()
{
	timeval ti,tf;
	char densidad[10];
	CAEN_DGTZ_ErrorCode ret;
	int	handle;
	int i,x,j;
	int c = 0,conta=0;
	uint32_t habilitados=0xf; 
	
	onoff_display(0,0);
		

	CAEN_DGTZ_BoardInfo_t BoardInfo;
	CAEN_DGTZ_EventInfo_t eventInfo;
	CAEN_DGTZ_UINT16_EVENT_t *Evt = NULL;
	CAEN_DGTZ_TriggerMode_t triggermode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;	//para uso con el proyecto
	CAEN_DGTZ_AcqMode_t acqmode = CAEN_DGTZ_SW_CONTROLLED;			//control por software
	char *buffer = NULL;
	char *evtptr = NULL;
	int size,bsize;
	int numEvents;
	i = sizeof(CAEN_DGTZ_TriggerMode_t);
	int trigthresh = 6550;//-0.5v

	ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB,0,0,0,&handle);		 //configuracion para uso con USB
	ret = CAEN_DGTZ_Reset(handle);// resetting Digitizer	
	ret = CAEN_DGTZ_GetInfo(handle, &BoardInfo);			//guarda en BoardInfo informacion sobre el modulo
	ret = CAEN_DGTZ_SetRecordLength(handle,40);		// samples a grabar por acquisition windows CAMBIAR es decir tamaño de buffer
	ret = CAEN_DGTZ_SetChannelEnableMask(handle,habilitados); 		// no disponible en el DT5740

	//ret = CAEN_DGTZ_SetSWTriggerMode(handle, triggermode);			//modo trigger 
	//ret = CAEN_DGTZ_SetExtTriggerInputMode(handle, triggermode); 		//mod de trigger
	ret = CAEN_DGTZ_SetChannelSelfTrigger(handle,CAEN_DGTZ_TRGMODE_ACQ_ONLY,0x9);

	ret = CAEN_DGTZ_SetPostTriggerSize(handle, 2); 	//% a grabar por cada trigger del record length
	ret = CAEN_DGTZ_SetAcquisitionMode(handle, acqmode);			// modo de adquisicion
	//ret = CAEN_DGTZ_SetIOLevel(handle, CAEN_DGTZ_IOLevel_TTL); 		// entrada salida TTL puede ser CAEN_DGTZ_IOLevel_NIM
	
	//ret = CAEN_DGTZ_SetChannelDCOffset(handle, -1, 0x8000);			//Fijar offset, (rango excursion)
  //utilizado para el ejemplo, con trigger automatico
	ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle,0,trigthresh+308);		// 14 bits, distribuidos según el rango. [-1.125 , 1.125], [0 , 2.25]
	ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle,1,trigthresh+48);		
	ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle,2,trigthresh+161);	
	ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle,3,trigthresh+75);	
	 	
	//ret = CAEN_DGTZ_SetChannelPulsePolarity(handle, 0, 0);          //no me compila, nose por que

	ret = CAEN_DGTZ_SetTriggerPolarity(handle, 0, CAEN_DGTZ_TriggerOnFallingEdge);
	ret = CAEN_DGTZ_SetTriggerPolarity(handle, 1, CAEN_DGTZ_TriggerOnRisingEdge);
	ret = CAEN_DGTZ_SetTriggerPolarity(handle, 2, CAEN_DGTZ_TriggerOnRisingEdge);
	ret = CAEN_DGTZ_SetTriggerPolarity(handle, 3, CAEN_DGTZ_TriggerOnRisingEdge);
// Set trigger on channel 0 to be ACQ_ONLY
	ret = CAEN_DGTZ_SetMaxNumEventsBLT(handle,1);			//numero maximo de eventos por transferencia
	uint32_t registros;
	ret= CAEN_DGTZ_ReadRegister(handle, 0x810C, &registros);
	//printf("Originals Settings:\t%x\n",registros);
	//registros |= (0x8<<24); //seteando el level
	//registros |= (0x1<<24);
	registros &= ~((unsigned int)0x1<<31);
	registros &= ~(0x1<<30);
	//registros &= ~(0x0);
	ret= CAEN_DGTZ_WriteRegister(handle, 0x810C, registros);
	ret= CAEN_DGTZ_ReadRegister(handle, 0x810C, &registros);
	//printf("New Settings:\t%x\n",registros);
	
	
    	ret = CAEN_DGTZ_MallocReadoutBuffer(handle,&buffer,(uint32_t *)&size);	//localiza buffer pointer en la memoria
										//the *buffer MUST be initialized to NULL
										//the size in byte of the buffer allocated
	printf("tamaño buffer : %d\n",size);
	ret = CAEN_DGTZ_SWStartAcquisition(handle);	
	x = 0;
	//FILE *data;
	//data = fopen("datos_prueba.txt","w");
	//fprintf(data,"#line:event\tch0\tch1\n");
	int line=0, whi=1;
	int state=0;
	int n0, n0p, n1p, aux0, aux1;
	n1p=0;
	n0p=0;
	n0=0;
	aux0=0;
	aux1=0;
	float densc;
	gettimeofday(&ti,NULL);
while(whi) {
		ret = CAEN_DGTZ_ReadData(handle,CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,buffer,(uint32_t *)&bsize);
		//entrega en numEvents el numero de eventos capturados
		ret = CAEN_DGTZ_GetNumEvents(handle,buffer,bsize,(uint32_t *)&numEvents);	
		conta +=numEvents;
		printf("\revents: %d",conta);
		fflush(stdout);
		  
		for (i=0;i<numEvents;i++) {
		
			ret = CAEN_DGTZ_GetEventInfo(handle,buffer,bsize,i,&eventInfo,(char **)&evtptr);
			ret = CAEN_DGTZ_DecodeEvent(handle,evtptr,(void **)&Evt);	
			
			//for (j=0;j<Evt->ChSize[0];j++)
			for (j=10;j<30;j++)
			{
				//fprintf(data,"%d\t%d\t%d\t%d\n", line++,conta,*(Evt->DataChannel[0]+j),*(Evt->DataChannel[1]+j));
					
						aux0+= *(Evt->DataChannel[0]+j);
						aux1+= *(Evt->DataChannel[1]+j);
		
			}
			ret = CAEN_DGTZ_FreeEvent(handle,(void **)&Evt);
			if(142000<aux0 && aux0<147500)n0p++;
			if(171200<aux1 && aux1<171850 && 142000<aux0 && aux0<147500)n1p++;
			aux0=0;
			aux1=0;
		}

		//if(conta >= 1000000) break;			// CANTIDAD MÁXIMA DE EVENTOS
		if(n1p >= 1000) break;
		//c = checkCommand();
		nav=get_key(read_analog_gpio(0));
		if (nav==2) whi=0;
	}
	//gettimeofday(&tf,NULL);
	//putchar('\n');
	printf("holi");
	//fclose(data);
	ret = CAEN_DGTZ_FreeReadoutBuffer(&buffer);	//Frees memory allocated by the CAEN_DGTZ_MallocReadoutBuffer
	ret = CAEN_DGTZ_CloseDigitizer(handle);
	double time_elapsed = tf.tv_sec-ti.tv_sec + tf.tv_usec/1e6-ti.tv_usec/1e6;
	printf("time elapsed: %5.2f\n<rate>: %3.2f\n",time_elapsed,conta/time_elapsed);
	printf("Eventos totales: %d \n",conta);
	printf("Eventos 511 tagger: %d \n",n0p);	
	printf("Eventos 511 detector 2: %d \n",n1p);
	n0=conta;
	densc= ((float)(n0p*n0p)/(float)(n0*n1p));
	
	
	printf("Densisdad: %f \n",densc);
	snprintf(densidad,10,"%f",densc);
	secondline(5);
	lcd_print(densidad);
	
	return 0;
}

int main(int argc, char* argv[]){
	int i=1,aux,loopin=1;
	
	set_analog_gpio(0);
	
	//init_lcd(8,9,7,6,5,4);
	init_lcd(8,9,7,6,5,4);
	
	delay(1000); 
	firstline(1); 

	lcd_print("Densimetro USM"); 

	secondline(4);
	 
	lcd_print("*SiLab*");

	onoff_display(0,0);


	 while(loopin){
	
	nav=get_key(read_analog_gpio(0));
	if(nav!=-1) loopin=0;
}
	
	
 while(loop){

	if (inicia==1){menu();
	inicia=0;
	nav=-1;
}

	while(aux){
	nav=get_key(read_analog_gpio(0));
	//printf("valor en adc %d\n",nav);
	//printf("valor en punto %d\n",punto);
	delay(50);
	if (nav!=-1) aux=0;
	}
	aux=1;
	movemenu();

	if(menuctrl==1) execute();
	if(menuctrl==1 && nav==0) execute();
}





	

}






