

/* Man-in-the-Middle */
int pinModoRepetidor = 7;
char default_buffer[14] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x82};

/* Variables Eststicas */
char repiter[1024];
char modem_buffer[512];
int rl, ml;

/* Funciones Auxiliares */
void hex_to_str(char* str, char* bytes, int length);

/**Main**********************************************/
void setup()
{
	/**MiM***************/
	pinMode(pinModoRepetidor, INPUT);
	/***************MiM**/

	/**Serial***************/
	Serial.begin(9600);
	Serial2.begin(1200);
	Serial3.begin(1200);

	/* Intial lengths */
	rl = 0;
	ml = 0;
	/****************Serial*/
}

void loop()
{
	if (digitalRead(pinModoRepetidor)){
		Serial.write("Yes");
		/*
		Serial2.write(default_buffer, 8);
		Serial.write("Paquete : ");
		if (Serial3.available()>0){
			Serial.write(Serial3.available());
			Serial3.readBytes(modem_buffer, ml);
			hex_to_str(repiter, modem_buffer, ml);
			Serial.write(repiter, ml*2);
		} else {
			delay(1000);
		}
		Serial.write("\n");
		*/
	} else {
		//                                                                          Serial.write("No");
		/*
		Serial.write("Paquete por defecto : ");
		hex_to_str(repiter, default_buffer, 8);
		Serial.write(repiter, 16);
		Serial.write("\n");
		delay(1000);
		*/
	}
	Serial.write("\n");
	delay(10);

}
/**********************************************Main**/

/**Funciones Auxiliares************************************/

void hex_to_str(char* str, char* bytes, int length)
/*!! str must have 2 times the size of bytes */
{
	char hx1, hx2;
	for (int i=length-1;i>=0;i--){
		hx1 = (bytes[i]>>4) & 0x0F;
		hx2 = bytes[i] & 0x0F;

		str[2*i]   = (hx1<0x0A)? hx1+48: hx1+65-0xA;
		str[2*i+1] = (hx2<0x0A)? hx2+48: hx2+65-0xA;
	}
}

/************************************Funciones Auxiliares**/