#include <LiquidCrystal.h>  
//#include <SoftwareSerial.h>

#include<EEPROM.h> 
#include<String.h>
#include<stdlib.h>                           
#include "PinChangeInterrupt.h"                       
const int Down=9,Up = 10,Entry = 11,Exit = 12;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
const int rs=48,en=46,d4=44,d5=42,d6=40,d7=38; /// display pin dsicription




//SoftwareSerial mySerial(19,18);

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int gprs_length=0;
int gprs_enable=0;

int gprs_event = 0;
int low_battery_value=0;char str_voltage[3];char str_voltage_sms[3]={'0','0','0'};

/////////////////// DCEM parameter///////////////////////
byte data[47];
int i;
long int kwh1=0,kwh2=0,kwh3=0,kwh4=0,voltage=0;
float voltage_1;float total_current; 
float i1,i2,i3,i4,temp_current;
long int temp1,temp2;
long int sum=0;
float display_sum;
String str_kwh1="00000000";
String str_kwh2="00000000";
String str_kwh3="00000000";
String str_kwh4="00000000";
////////////////////// GSM PARAMETER ////////////////////
char final_str[286] = " ";
char final_str_sms[160]=" ";
char ID_str[] = "#S118HAGT10463";
char ID_str1[]="475";
char ip_address[]="AT+CIPSTART=\"TCP\",\"125.19.233.85\",\"8189\"\r\n";
char str_temp[12]; char str_temp_gprs[12];
char alarm_string[7]="000000";
char status_string[7]="000000", status_string_sms[7]="000000";
int close_count=0;int ciicr_failed_flag=0;int close_count1=0;int close_count2=0;
String system_status="";
String dg_fault_status="";
float temp_val;
int ciicr_failcount = 0;
int auth_failcount=0;
////////////////////// AUCS RUN HOUR and DG status////////
 unsigned int timee=0;
 unsigned int pre_timee = 0;
 int minute_count = 0;
 int dg_flag = 0, batt_flag=0, mains_flag = 0;
 int dg_count = 0, batt_count = 0,mains_count = 0,dg_value_count=0,mains_value_count=0,batt_value_count=0;
 int timer3_count = 0, timer4_count = 0, timer5_count=0;

 String dg_run_hour="00000000";
 String mains_run_hour="00000000";
 String batt_run_hour="00000000";
//*************** AUCS PARAMETER  *******************/////
const int s0= 5, s1= 6, s2=7 , s3=8, ena=4;
int count=0;
int count1=0;
int Low_batt=0,low_batt_sum=0,low_batt_count=0;
int eeprom_value=0; // this value is saved in eeprom and fetched out when required. this will tell which alarm is occured.

//********************* functions for memory log ******************************/////
#include "page-update.h"
#include "logi-data.h"
void hold_page();void str_generate_gprs();
//****************************************************////

void gettimeResponse(int j) {
  
  
  if(j==1) // date and time
  { 
    for (int i=0;i<12;i++)
    {
      str_temp[i]=0;
    }
    int k=0;
     
    char response1[200];
    for(int i = 0 ; Serial1.available() > 0 && i<199 ; i++) {      
      response1[i]= Serial1.read();
  //  Serial.print(i);Serial.print(":");
    //  Serial.print(response1[i]);Serial.println();
      
      }
      str_temp[0]= response1[25]; str_temp[1]= response1[26]; str_temp[2]= response1[28]; str_temp[3]= response1[29];
      str_temp[4]= response1[31]; str_temp[5]= response1[32]; str_temp [6]= response1[34]; str_temp [7]= response1[35];
      str_temp[8]= response1[22]; str_temp[9]= response1[23]; str_temp[10]= response1[19]; str_temp[11]= response1[20];
   
      lcd.command(0x94);lcd.print("Time:");lcd.print(str_temp[2]);lcd.print(str_temp[3]);lcd.print(":");lcd.print(str_temp[4]);lcd.print(str_temp[5]);lcd.print(":");
      lcd.print(str_temp[6]);lcd.print(str_temp[7]);  /////////////////time is added/////////////////////////////////////

      lcd.command(0xd4);lcd.print("Date:");lcd.print(str_temp[0]);lcd.print(str_temp[1]);lcd.print("/");lcd.print(str_temp[8]);lcd.print(str_temp[9]);lcd.print("/");
      lcd.print(str_temp[10]);lcd.print(str_temp[11]);
      strcpy(str_temp_gprs,str_temp);

  } 
  
}


/*

 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919718605676\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

*/

void sms()
{
  if(strlen(final_str_sms)== 160)
  {
  lcd.clear();lcd.print("Sending SMS");delay(1000);
  Serial1.write("AT\r\n");
  delay(100);  
  Serial1.write("AT+CMGF=1\r\n");
  delay(100);
  Serial1.write("AT+CMGS=\"9718605676\"\r\n");   //5542555
  delay(100);
  
  //Serial1.write("hello");
  for(int i=0;i<strlen(final_str_sms);i++){Serial.print(final_str_sms[i]);Serial1.println(final_str_sms[i]);delay(50);}delay(300);
  Serial1.write((char)26);
  
  delay(1000);lcd.clear();lcd.print("SMS Sent");delay(1000);lcd.clear();
  for(int z=0;z<8;z++)
  {
    str_kwh1[z]='0';
    str_kwh2[z]='0';
    str_kwh3[z]='0';
    str_kwh4[z]='0';
    
  }
  }
 
  
}


void close_conn()
{
  int k=1;
  while(k==1)
  {
    
  char close_response[100]="";
  Serial1.write("AT+CIPSHUT\r\n");delay(2000);lcd.print("Breaking connection");
  do{
       for(int i = 0 ; Serial1.available() > 0 && i<200 ; i++) 
       {
         close_response[i] = Serial1.read();
       }
     delay(1000);lcd.clear();lcd.print("Closing..");close_count2 += 1; if(close_count2>100){close_count2=0;k=0;break;}
    }while(!(strstr(close_response,"SHUT OK")||(strstr(close_response,"ERROR"))));
    if(strstr(close_response,"ERROR")){close_count +=1; if(close_count>2){k=0;} }
    if(strstr(close_response,"SHUT OK")){close_count1 += 1; k=0;if(close_count1>3){close_count1=0/*;sms()*/;k=0;}}

  }
}
void getResponse(int i)
{
  if(i==0){
    char response[100]="";
    for(int i = 0 ; Serial1.available() > 0 && i<200 ; i++) 
      {
      response[i] = Serial1.read();
      }
      lcd.clear();
      delay(1000);
      
  }
}



char hexadeci(long int i)
{
  char dat;
  switch(i)
  {
      case 0:
            dat='0';
            break;
      case 1:
            dat='1';
            break;
      case 2:
            dat='2';
            break;
      case 3:
            dat='3';
            break;
      case 4:
            dat='4';
            break;
      case 5:
            dat='5';
            break;
      case 6:
            dat='6';
            break;
      case 7:
            dat='7';
            break;
      case 8:
            dat='8';
            break;
      case 9:
            dat='9';
            break;
      case 10:
            dat='A';
            break;
      case 11:
            dat='B';
            break;
      case 12:
            dat='C';
            break;
      case 13:
            dat='D';
            break;
      case 14:
            dat='E';
            break;
      case 15:
            dat='F';
            break;
      
  }
  return (dat);
}


void bintohex( int pos,int a, int b, int c, int d)
{
  long int binaryval=0,i=1, remainder;
  char hexval;
    binaryval = (a<<3)|(b<<2)|(c<<1)|(d);
    hexval = hexadeci(binaryval);
    alarm_string[pos] = hexval;     
}
void bintohex_status( int pos,int a, int b, int c, int d)
{
  long int binaryval=0,i=1, remainder;
  char hexval;
    binaryval = (a<<3)|(b<<2)|(c<<1)|(d<<0);
    hexval = hexadeci(binaryval);
   status_string[pos] = hexval;
   status_string_sms[pos] = hexval;
    
}
void tostring(char str[], long int num)
{
    int i;
    long int rem, len = 0;
    long n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}


void dcem_signal()
{
    digitalWrite(3,HIGH); //transmitting
  delay(20);
  Serial.write(0X01);Serial.write(0X04);Serial.write(0X00);Serial.write(0X00);Serial.write(0X00);
  Serial.write(0X15);  Serial.write(0X31);  Serial.write(0XC5);  delay(20);
  digitalWrite(3,LOW); // receiving
  delay(1000);
  for(i=0;i<47;i++)// reading dcem parameter
  {
   data[i]=Serial.read();
  }
  delay(500);
  for(i=0;i<47;i++)// reading dcem parameter
  {
   Serial.print(data[i]);
   //delay(1000);
  }
  //delay(1000);
}
void dcem_parameter()
{

  char str_kwh1_temp[8]={'0','0','0','0','0','0','0','0'};
  char str_kwh2_temp[8]={'0','0','0','0','0','0','0','0'};
  char str_kwh3_temp[8]={'0','0','0','0','0','0','0','0'};
  char str_kwh4_temp[8]={'0','0','0','0','0','0','0','0'};
  dcem_signal();
  sum=0;
  temp1=0;
  delay(300);
  lcd.clear();
  delay(500);
  temp1= data[6];sum=sum+temp1;
  temp1=data[5];sum = sum + (temp1*256);
  temp1=data[4];sum=sum+(temp1*65536);
  kwh1 = sum;
  display_sum = sum;
  //Serial.println(" ");
  //Serial.print("KWH1:");Serial.print(sum);
  
   itoa(kwh1,str_kwh1_temp,10);
  for(int i=0; i<strlen(str_kwh1_temp);i++){int x= (( 8-strlen(str_kwh1_temp))+i); str_kwh1[x]= str_kwh1_temp[i];}
  lcd.command(0x80);lcd.print("KWH1:");lcd.print(display_sum/10);
  //for(int i=0;i<8;i++){lcd.print(str_kwh1[i]);}
  Serial.println("\n");Serial.println("String KWH1: ");
  for(int i=0;i<8;i++){Serial.print(str_kwh1[i]);}
  delay(1000);
  
  sum=0;temp1=0;
  delay(300);
  
  temp_current = data[10]; sum = sum+ temp_current;
  temp_current = data[9]; sum = sum+ (temp_current*256);
  i1 = sum;
  Serial.println(" ");
  Serial.print("I1:");Serial.print(sum);
  sum=0;temp_current=0;
  lcd.command(0xc0);lcd.print("I1:");lcd.print(i1/10);
  
  delay(300);

  

  //lcd.clear();
  delay(200);
  temp1= data[14];sum=sum+temp1;
  temp1=data[13];sum = sum + (temp1*256);
  temp1=data[12];sum=sum+(temp1*65536);
  kwh2 = sum;  display_sum = sum;
  //Serial.println(" ");
  //Serial.print("KWH2:");Serial.print(sum);
  itoa(kwh2,str_kwh2_temp,10);
  for(int i=0; i<strlen(str_kwh2_temp);i++){int x= (( 8-strlen(str_kwh2_temp))+i); str_kwh2[x]= str_kwh2_temp[i];}
  lcd.command(0x94);lcd.print("KWH2:");lcd.print(display_sum/10);
  Serial.println("\n");Serial.println("String KWH1: ");
  for(int i=0;i<8;i++){Serial.print(str_kwh2[i]);}
  //for(int i=0;i<8;i++){lcd.print(str_kwh2[i]);}
  delay(1000);
  
  sum=0;temp1=0;
  delay(300);
  
  temp_current = data[18]; sum = sum+ temp_current;
  temp_current = data[17]; sum = sum+ (temp_current*256);
  i2 = sum;
  Serial.println(" ");
  Serial.print("I2:");Serial.print(sum); 
  sum=0;temp_current=0;
  lcd.command(0xd4);lcd.print("I2:");lcd.print(i2/10);
  delay(5000);

  lcd.clear(); 
  delay(200);
  temp1= data[22];sum=sum+temp1;
  temp1=data[21];sum = sum + (temp1*256);
  temp1=data[20];sum=sum+(temp1*65536);
  kwh3 = sum;display_sum = sum;
  //Serial.println(" ");
  //Serial.print("KWH3:");Serial.print(sum);
  itoa(kwh3,str_kwh3_temp,10);
  for(int i=0; i<strlen(str_kwh3_temp);i++){int x= (( 8-strlen(str_kwh3_temp))+i); str_kwh3[x]= str_kwh3_temp[i];}
  lcd.command(0x80);lcd.print("KWH3:");lcd.print(display_sum/10);
  Serial.println("\n");Serial.println("String KWH1: ");
  for(int i=0;i<8;i++){Serial.print(str_kwh3[i]);}
 // for(int i=0;i<8;i++){lcd.print(str_kwh3[i]);}
  delay(1000);
  
  sum=0;
  temp1=0;
  delay(300);
  
  temp_current = data[26]; sum = sum+ temp_current; 
  temp_current = data[25]; sum = sum+ (temp_current*256);
  i3 = sum;
  Serial.println(" ");
  Serial.print("I3:");Serial.print(sum);
  sum=0;temp_current=0;
  lcd.command(0xc0);lcd.print("I3:");lcd.print(i3/10);
  delay(300);

  //lcd.clear();
  delay(200);
  temp1= data[30];sum=sum+temp1;
  temp1=data[29];sum = sum + (temp1*256);
  temp1=data[28];sum=sum+(temp1*65536);
  kwh4 = sum;display_sum = sum;
  //Serial.println(" ");
  //Serial.print("KWH4:");Serial.print(sum);
 itoa(kwh4,str_kwh4_temp,10);
 for(int i=0; i<strlen(str_kwh4_temp);i++){int x= (( 8-strlen(str_kwh4_temp))+i); str_kwh4[x]= str_kwh4_temp[i];}
 lcd.command(0x94);lcd.print("KWH4:");  lcd.print(display_sum/10);
 Serial.println("\n");Serial.println("String KWH1: ");
 for(int i=0;i<8;i++){Serial.print(str_kwh4[i]);}
 //for(int i=0;i<8;i++){lcd.print(str_kwh4[i]);}
 delay(1000);

  
  sum=0;
  temp1=0;
  delay(300);
  
  temp_current = data[34]; sum = sum+ temp_current;
  temp_current = data[33]; sum = sum+ (temp_current*256);
  i4 = sum;
  Serial.println(" ");
  Serial.print("I4:");Serial.print(sum);
  sum=0;temp_current=0;
  lcd.command(0xd4);lcd.print("I4:");lcd.print(i4/10);
  delay(5000);

  lcd.clear();
  delay(200);
  temp1= data[36];sum=sum+temp1;
  temp1=data[35];sum = sum + (temp1*256);
  voltage = sum;
  voltage_1 = voltage;
  Serial.println(" ");
  Serial.print("Voltage:");Serial.print(voltage);  Serial.println(" ");
 
  tostring(str_voltage,voltage);delay(200);tostring(str_voltage_sms,voltage);
   
  sum=0;
  temp1=0;
  delay(300);
  total_current = i1+i2+i3+i4;
 

  
  
}


void timer_ini()
{
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=25535;// Load the timer with the starting value and it will go upto 65535 (maximum). 45535 for 10ms 64285 for 20ms
  TCCR1B=0X02; // prescaler is 256. fclk= 16mhz
  TIMSK1 |= (1<<TOIE1); // timer overflow intruppt is enabled. 
  
}

void timr3_ini()
{
  TCNT3=0;
  TCCR3A = 0x00;
  TCCR3B = 0x02;
  TIMSK3 |=(1<<TOIE3);  
  
}
void timr4_ini()
{
  TCNT4=0;
  TCCR4A = 0x00;
  TCCR4B = 0x02;  
  TIMSK4 |=(1<<TOIE4);  
  
}
void timr5_ini()
{
  TCNT5=0;
  TCCR5A = 0x00;
  TCCR5B = 0x02;
  TIMSK5 |=(1<<TOIE5);  
  
}


void setup() {
  lcd.begin(20,4);
  pinMode(3,OUTPUT); // enable pin for rs485 (dcem)
  Serial.begin(9600);
  Serial1.begin(9600);

//   mySerial.begin(9600);
  
  pinMode(2,INPUT);// signal from mux
  pinMode(ena,OUTPUT); // mux enable pin, make low to enable the mux
  pinMode(s0,OUTPUT); // s0 of mux
  pinMode(s1,OUTPUT); // s1 of mux
  pinMode(s2,OUTPUT); // s2 of mux
  pinMode(s3,OUTPUT); // s3 of mux
  pinMode(13,OUTPUT); // led pin for testing
  pinMode(9,INPUT); // SW1 for entry
  pinMode(10,INPUT); // SW2 for Up
  pinMode(11,INPUT); // SW3 for Down
  pinMode(12,INPUT); // SW4 for Exit
  digitalWrite(9,LOW);digitalWrite(10,LOW);digitalWrite(11,LOW);
  
  
  lcd.command(0x82 );
  lcd.print("RVS AUCS NMS");
  delay(2000);
  lcd.clear();
  interrupts(); 
  attachInterrupt(0,ISR1,RISING);// external interrupt is attached on pin 2.

  int retrieve_gprs_state= EEPROM.read(255);
  gprs_enable = retrieve_gprs_state;

  int retrieve_battry_value0 = EEPROM.read(254);
  int retrieve_battry_value1 = EEPROM.read(253);
  int retrieve_battry_value2 = EEPROM.read(252);
  int retrieve_value_total = (retrieve_battry_value2*100)+(retrieve_battry_value1*10)+(retrieve_battry_value0*1);
  low_battery_value =retrieve_value_total;
 }
 void hold_page()
{
  delay(1000);
  while(digitalRead(10)==HIGH)
  {
    
  }
}
ISR(TIMER1_OVF_vect)
{
  TCNT1=25535;
  digitalWrite(13,digitalRead(13) ^ 1);
  count1=count;
  count=0;
    
}

ISR(TIMER3_OVF_vect)
{
  timer3_count++;
  if(timer3_count>=1832)
  { dg_value_count++;
    if(dg_value_count>5)
   { dg_count++; timer3_count=0;dg_value_count=0; }
   
  }
}

ISR(TIMER4_OVF_vect)
{
  timer4_count++;
  if(timer4_count>=1832)  
  {
    batt_value_count++;
    if(batt_value_count>5)
    {batt_count++; timer4_count=0;batt_value_count=0; }
  }
}

ISR(TIMER5_OVF_vect)
{
  timer5_count++;
  if(timer5_count>=1832)
  { 
    mains_value_count++;
    if(mains_value_count>5)
    { mains_count++; timer5_count=0;mains_value_count=0; }
  }
}

void ISR1()
{
  count= count+1;
}
int count_signal()
{
    int val;
    timer_ini();
    delay(100);
    TCCR1A=0; TCCR1B=0;
    val = count1;
    count1=0;
    return val;    
}


void low_batt_test()
{
  lcd.clear();
  lcd.print(low_battery_value);delay(1000);lcd.clear();
  int sense=0;
  lcd.command(0x80);lcd.print("Site ID:");
  for(int i=2; i<strlen(ID_str); i++)
  {
    lcd.print(ID_str[i]);
  }
  lcd.command(0xc0);lcd.print("Power plant batt:");lcd.print(voltage_1/10);
  lcd.command(0x94);
  if(gprs_enable==1){lcd.print("GSM Mode: Enable");}
  if(gprs_enable==0){lcd.print("GSM Mode: Disable");}

  //int btry_vol=Low_Battery_Update();

  //lcd.print(btry_vol);

  if(voltage>=low_battery_value){sense=2;lcd.command(0xd4);lcd.print("BTS bat ok");}
  if(voltage<low_battery_value){sense=1;lcd.command(0xd4);lcd.print("BTS low bat");}
  delay(3000);
  if(digitalRead(Entry)== HIGH)
     {
      delay(50);
      if(digitalRead(Entry)== HIGH)
      {
        page_display(3);
      }
      
   }
  alarm_check_1st_byte(sense);
  
}

void alarm_check_1st_byte(int sense_receive)
{
  int mains,dgc,ebc,dgg,strt_fail,low_batt,llop, manual_value,return_val=0, i=1, j=0; 
  int door_alarm=0; int main_fail =1; int dg_on =0; int start_fail =0; //1st byte
  int site_on_battery=0, dg_fail_stop=0, rect_fail=0,dg_fuel_low=0;    // 2nd byte
  int battery_fuse_fail=0,load_fuse_fail=0,high_temp=0,fire_smoke =0;  // 3rd byte
  int low_batt_vol=0,ac_faulty=0, both_ac_faulty=0, dg_fault=0;       // 4th byte

  int llop_fault=0,hct=0,fuel=0,lcu=0;
  int emergency_fault=0,dg_overload=0;

  system_status = "00";
  dg_fault_status = "00";

  if(sense_receive==1) // low batt occured
  {
   low_batt_vol=1;
   if(i==1)
   {  lcd.clear();  
    digitalWrite(ena,LOW);digitalWrite(s3,HIGH);digitalWrite(s2,LOW);digitalWrite(s1,LOW);digitalWrite(s0,LOW);// EB mains led 
    mains = count_signal();//lcd.command(0x80);lcd.print("EBM    ");lcd.command(0x84);lcd.print(mains);
    i=i+1;
   
   }
   if(i==2)
   { 
    digitalWrite(ena,LOW);digitalWrite(s3,HIGH);digitalWrite(s2,LOW);digitalWrite(s1,LOW);digitalWrite(s0,HIGH);// EBC led 
    ebc = count_signal();//lcd.command(0x86);lcd.print(" ebc  ");lcd.command(0x8a);lcd.print(ebc);
    i=i+1;
   
   }
   if(i==3)
   { 
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,HIGH);digitalWrite(s1,LOW);digitalWrite(s0,LOW);// DG-GEN led 
    dgg = count_signal();//lcd.command(0xc0);lcd.print("dgg    ");lcd.command(0xc4);lcd.print(dgg);
    i=i+1;
   
   }
   if(i==4)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,HIGH);digitalWrite(s2,LOW);digitalWrite(s1,HIGH);digitalWrite(s0,LOW);// DG con led 
    dgc = count_signal();//lcd.command(0xc6);lcd.print(" dgc  ");lcd.command(0xca);lcd.print(dgc);delay(1000);
    i=i+1;
   }
   if(i==5)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,LOW);digitalWrite(s1,HIGH);digitalWrite(s0,HIGH);// start_fail led 
    strt_fail = count_signal();//lcd.command(0x8d);lcd.print("sf   ");lcd.command(0x90);lcd.print(strt_fail);delay(1000);
    i=i+1;
   }  
   if(i==6)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,HIGH);digitalWrite(s1,HIGH);digitalWrite(s0,LOW);// start_fail led 
    llop = count_signal();//lcd.command(0xcd);lcd.print("LL   ");lcd.command(0xd0);lcd.print(llop);delay(1000);
    i=i+1;
   }
   if(i==7)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,HIGH);digitalWrite(s1,HIGH);digitalWrite(s0,HIGH);// Manual led 
    manual_value = count_signal();
    lcd.command(0xd4);
    //lcd.print(manual_value);
    j=1;
   }
    if(j==1)
    {
      delay(5000);
      lcd.clear();
      lcd.command(0x80);lcd.print("Active Alarms");
      lcd.command(0xc0);
        
      if((10<=mains && mains<=12) &&  ( 12<=ebc && ebc<=13 ) && (dgg==10 && dgc ==10)) //(mains == 10 && ebc ==9)   || (mains == 11 && ebc== 13) )
      {
        //if( dgc<=8 )
        //{  
         lcd.command(0xc0);lcd.print("MAINS ON        ");eeprom_write(1);delay(1000);j=2;   // mains on
        //}
      }
      if((10<=mains && mains<=12) &&  ( 12<=ebc && ebc<=13 ) && (dgg==10 && dgc ==10))
        {
          //if (dgc <=8)
          //{
           lcd.command(0xc0);lcd.print("SITE ON MAINS ");eeprom_write(2);delay(1000);main_fail =0;   mains_flag=1;lcd.command(0xcf);/*lcd.print(mains_flag);*/lcd.clear(); dg_flag=0 ; batt_flag=0;system_status="05";dg_fault_status="94";  j=2;
          //} 
        }// site on mains

         //.......................................................................................................................................................
     
          else  if(  (12<=dgc  && dgc < 15)  && ( 11<dgg && dgg<15 ))
            {
              
                lcd.command(0xc0);lcd.print("SITE ON DG,MAINS FAIL");  delay(1000);eeprom_write(3);   mains_flag=0; dg_flag=1 ; batt_flag=0;    j=2;system_status="09";dg_fault_status="97";     // site on dg
           }
         else if(  (12<=dgc  && dgc < 15)  && ( 11<dgg && dgg<15 ))  {lcd.command(0xc0);lcd.print("DG START ");delay(1000);eeprom_write(4);dg_on =1;j=2;system_status="06";dg_fault_status="91";}

        //..............................................................................................................................................................
     
      else   if(( mains ==8 && ebc == 8 )  || ( mains ==6  && ebc == 8 && dgg==10 )) { lcd.command(0xc0);lcd.print("Mains fail,S.O.B");delay(1000);eeprom_write(5);delay(1000);site_on_battery=1;  mains_flag=0; dg_flag=0 ; batt_flag=1; /*lcd.print(batt_flag);*/ system_status="10";  j=2;}
 
      //.................................................................................................................................................................
      else if((mains == 10 && ebc == 12 &&  8<=dgc && dgc<=11 )) {lcd.command(0xc0);lcd.print("stop fail,DG-No load");eeprom_write(6);lcd.command(0x94);lcd.print("DG Fault");delay(1000);dg_fail_stop=1;j=2;system_status="03";}
    //................................................................................................................................................................................
     else if(llop == 6 || llop == 10)                                       {lcd.command(0xc0);lcd.print("LLOP            ");delay(1000);eeprom_write(7);llop_fault=1;j=2;dg_fault_status="93";}
   //............................................................................................................................................................................................
     else if((strt_fail==14 ) || (5<strt_fail && strt_fail<=8  ) )   {lcd.command(0xc0);lcd.print("Start fail,S.O.B");delay(1000);eeprom_write(8);lcd.command(0x94);lcd.print("DG Fault,Mains fail");delay(1000);start_fail =1;site_on_battery=1;j=2;system_status="08";}
    
     //........................................................................................................................................................................
     else if (mains==6 && ebc == 10 && dgg ==10 && dgc == 12)    {lcd.command(0xc0);lcd.print("FUEL LOW,S.O.DG ");eeprom_write(9);lcd.command(0x94);lcd.print("DG Fault");delay(1000);delay(1000);dg_fuel_low=1;j=2;dg_fault_status="9A";}
      if(manual_value>=4){eeprom_write(10);lcd.command(0xd4);lcd.print("Manual Mode  ");delay(500);j=2;}
     if(manual_value <=3){eeprom_write(11);lcd.command(0xd4);lcd.print("Auto Mode");delay(500);j=2;system_status="16";}    //changes done here
      //if(digitalRead(9)== LOW){lcd.command(0x80);lcd.print("Charging fail");lcd.command(0xc0);lcd.print("DG batt Low");}
      //if(digitalRead(10)== HIGH){lcd.command(0x94);lcd.print("Shelter door open");}
      //if(digitalRead(11)== HIGH){lcd.command(0xd4);lcd.print("DG door open");}
      
    }
    if(j==2)
    {
      lcd.command(0xdf);
      bintohex(0,door_alarm,main_fail,dg_on,start_fail);
      bintohex(1,dg_fail_stop,dg_fuel_low,rect_fail,site_on_battery);  
      bintohex(2,battery_fuse_fail,load_fuse_fail,high_temp,fire_smoke);
      bintohex(3,low_batt_vol,ac_faulty,both_ac_faulty, dg_fault);
      bintohex(4,0,0,0,0);
      bintohex(5,0,0,0,0);
      j=3;
    }
    if( j==3 ) // status string
    {
      bintohex_status(0,0,llop_fault,hct,0);
      bintohex_status(1,fuel,0,0,lcu);
      bintohex_status(2,0,0,0,0);
      bintohex_status(3,0,0,0,0);
      bintohex_status(4,0,0,0,0);
      bintohex_status(5,0,0,0,0);
      j=0;
      
    }
   
  }
  if(sense_receive==2) // bts battery ok
  {
    low_batt_vol=0;
    if(i==1)
   {  lcd.clear();
    digitalWrite(ena,LOW);digitalWrite(s3,HIGH);digitalWrite(s2,LOW);digitalWrite(s1,LOW);digitalWrite(s0,LOW);// EB mains led 
    mains = count_signal();//lcd.command(0x80);lcd.print("EBM   ");lcd.command(0x84);lcd.print(mains);
    i=i+1;
   
   }
   if(i==2)
   { 
    digitalWrite(ena,LOW);digitalWrite(s3,HIGH);digitalWrite(s2,LOW);digitalWrite(s1,LOW);digitalWrite(s0,HIGH);// EBC led 
    ebc = count_signal();//lcd.command(0x86);lcd.print(" ebc  ");lcd.command(0x8a);lcd.print(ebc);
    i=i+1;
   
   }
   if(i==3)
   { 
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,HIGH);digitalWrite(s1,LOW);digitalWrite(s0,LOW);// DG-GEN led 
    dgg = count_signal();//lcd.command(0xc0);lcd.print("dgg    ");lcd.command(0xc4);lcd.print(dgg);
    i=i+1;
   
   }
   if(i==4)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,HIGH);digitalWrite(s2,LOW);digitalWrite(s1,HIGH);digitalWrite(s0,LOW);// DG con led 
    dgc = count_signal();//lcd.command(0xc6);lcd.print(" dgc  ");lcd.command(0xca);lcd.print(dgc);delay(1000);
    i=i+1;
   }
   if(i==5)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,LOW);digitalWrite(s1,HIGH);digitalWrite(s0,HIGH);// start_fail led 
    strt_fail = count_signal(); //lcd.command(0x8d);lcd.print("sf   ");lcd.command(0x90);lcd.print(strt_fail);delay(1000);
    i=i+1;
    
   } 
   if(i==6)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,HIGH);digitalWrite(s1,HIGH);digitalWrite(s0,LOW);// start_fail led 
    llop = count_signal();//lcd.command(0xcd);lcd.print("LL   ");lcd.command(0xd0);lcd.print(llop);delay(1000);
    
   i=i+1;
   } 
    if(i==7)
   {
    digitalWrite(ena,LOW);digitalWrite(s3,LOW);digitalWrite(s2,HIGH);digitalWrite(s1,HIGH);digitalWrite(s0,HIGH);// Manual led 
    manual_value = count_signal();
      manual_value = count_signal();
    lcd.command(0xd4);
    //lcd.print(manual_value);
    j=1;
   } 
    if(j==1) 
    {
      delay(5000);
      lcd.clear();
      lcd.command(0x80);lcd.print("Active Alarms");
      lcd.command(0xc0);
      //..............................................................................................................................
    if((10<=mains && mains<=12) &&  ( 12<=ebc && ebc<=13 ) && (dgg==10 && dgc ==10))
    {
        lcd.command(0xc0);lcd.print("MAINS ON        "); delay(1000); eeprom_write(1);delay(1000);j=2;
      
      } 
       
     if((10<=mains && mains<=12) &&  ( 12<=ebc && ebc<=13 ) && (dgg==10 && dgc ==10)){
     
          lcd.command(0xc0);lcd.print("SITE ON MAINS "); delay(1000); eeprom_write(2);delay(1000);main_fail =0;   mains_flag=1; lcd.command(0xcf);/*lcd.print(mains_flag);*/ dg_flag=0 ; batt_flag=0;system_status="05";dg_fault_status="94";  j=2;
         
        }
        //................................................................................................................................
     
      else if(  (12<=dgc  && dgc < 15)  && ( 11<dgg && dgg<15 ))
          {
            lcd.command(0xc0);lcd.print("SITE ON DG,MAINS FAIL"); delay(1000); eeprom_write(3);delay(1000);  mains_flag=0; dg_flag=1 ; batt_flag=0;system_status="09";dg_fault_status="97"; j=2; // site on dg
          }
     else if(  (12<=dgc  && dgc < 15)  && ( 11<dgg && dgg<15 ))   {lcd.command(0xc0);lcd.print(" DG START            "); delay(1000);eeprom_write(4);delay(1000);dg_on =1;dg_fault_status="91";j=2;}
      //.....................................................................................................................................
    
   else   if(( mains ==8 && ebc == 8 )  || ( mains ==6  && ebc == 8 && dgg==10 )) { lcd.command(0xc0);lcd.print("Mains fail,S.O.B");delay(1000);eeprom_write(5);delay(1000);site_on_battery=1; mains_flag=0; dg_flag=0 ; batt_flag=1;/* lcd.print(batt_flag) ;*/system_status="10";  j=2;}
     
    //............................................................................................................................................................... 
     else if((mains == 10 && ebc == 12  &&  8<=dgc && dgc<=11 )) {lcd.command(0xc0);eeprom_write(6);lcd.print("stop fail,DG-No load");delay(1000);lcd.command(0x94);lcd.print("DG Fault");delay(1000);dg_fail_stop=1;system_status="03";j=2;}
      if(llop == 6 || llop == 10 )                                                          {lcd.command(0xc0);lcd.print("LLOP            ");eeprom_write(7);delay(1000);llop_fault=1;dg_fault_status="93";j=2;}
     else if((strt_fail==14 ) || (strt_fail>5 && strt_fail<=8  ) ) /*|| (strt_fail==15) ) */   {lcd.command(0xc0);lcd.print("Start fail,S.O.B");eeprom_write(8);delay(1000);lcd.command(0x94);lcd.print("DG Fault,Mains fail");delay(1000);start_fail =1;site_on_battery=1;system_status="08";j=2;}
      //if((mains==8||mains==6) && ebc == 10&& (dgg ==10||dgg ==12) && (dgc == 10||dgc==12)){lcd.command(0xc0);eeprom_write(9);lcd.print("S.O.DG,FUEL LOW");lcd.command(0x94);lcd.print("DG Fault,DG STOP");delay(1000);delay(1000);dg_fuel_low=1;dg_fault_status="9A";j=2;}
     if(manual_value >=4 ){eeprom_write(10);lcd.command(0xd4);lcd.print("Manual Mode  ");delay(500);j=2;}
     if(manual_value <=3){eeprom_write(11);lcd.command(0xd4);lcd.print("Auto Mode");delay(500);system_status="16";j=2;}
     //if(digitalRead(9)== LOW){lcd.command(0x80);lcd.print("Charging fail");lcd.command(0xc0);lcd.print("DG batt Low");}
      //if(digitalRead(10)== HIGH){lcd.command(0x94);lcd.print("Shelter door open");}.
      //if(digitalRead(11)== HIGH){lcd.command(0xd4);lcd.print("DG door open");}
     
    }

    if(j==2) // alarm_string
    {
      lcd.command(0xdf);
      bintohex(0,door_alarm,main_fail,dg_on,start_fail);
      bintohex(1,dg_fail_stop,dg_fuel_low,rect_fail,site_on_battery);
      bintohex(2,battery_fuse_fail,load_fuse_fail,high_temp,fire_smoke);
      bintohex(3,low_batt_vol,ac_faulty,both_ac_faulty, dg_fault);
      bintohex(4,0,0,0,0);
      bintohex(5,0,0,0,0);
      j=3;
    }
    if(j==3) // status string and comm string
    {
      bintohex_status(0,emergency_fault,llop_fault,hct,dg_overload);
      bintohex_status(1,fuel,0,0,lcu);
      bintohex_status(2,0,0,0,0);
      bintohex_status(3,0,0,0,0);
      bintohex_status(4,0,0,0,0);
      bintohex_status(5,0,0,0,0);
      j=0;
      
    }
  } 
}

void run_hour_data()
{
  
  char tempodg_run_hour[8];
  char tempobatt_run_hour[8];
  char tempomains_run_hour[8];
  
  itoa(dg_count,tempodg_run_hour,10);
   for(int i=0; i<strlen(tempodg_run_hour);i++){int x= (( 8-strlen(tempodg_run_hour))+i); dg_run_hour[x]= tempodg_run_hour[i];}
   Serial.println(dg_count);
   
  itoa(batt_count,tempobatt_run_hour,10);
   for(int i=0; i<strlen(tempobatt_run_hour);i++){int x= ((8-strlen(tempobatt_run_hour))+i); batt_run_hour[x]= tempobatt_run_hour[i];}
  Serial.println(batt_count);
  itoa(mains_count,tempomains_run_hour,10);
   for(int i=0; i<strlen(tempomains_run_hour);i++){int x= ((8-strlen(tempomains_run_hour))+i); mains_run_hour[x]= tempomains_run_hour[i];}
   Serial.println(mains_count);
}

void str_generate_gprs()
{
  
  char temp[4]="000";
  char fuel_level[4]="000";
  lcd.command(0x80);
  long int j=0,k=0;
  for (int i=0;i<strlen(ID_str);i++) // id is inserted
  {
    final_str[j] = ID_str[i];
    final_str_sms[j] = ID_str[i];
    j++;delay(50);

  // Serial.print(ID_str[i]);
   
  }
  //lcd.command(0xc0);
  for(int i=0 ; i<12 ; i++) // time and date is inserted
  {
    //lcd.print(str_temp[i]);
    final_str[j] = str_temp_gprs[i];
    final_str_sms[j] = str_temp_gprs[i];
    j++;delay(50);
   // Serial.print(str_temp_gprs[i]);
  }    
 for(int i=0 ; i<6 ; i++) // alarm string is inserted
  {
    final_str[j] = alarm_string[i];
    final_str_sms[j] = alarm_string[i];
    // testing purpose ......
   //  Serial.print(alarm_string[i]);
    j++;delay(50);
  }
 for(int i=0 ; i<6 ; i++) // status string is inserted
  {
    final_str[j] = status_string[i];
    final_str_sms[j] = alarm_string[i];
    j++;delay(50);
   // Serial.print(alarm_string[i]);
  }

  float val = analogRead(A2);
  if(val<10.0){temp[0]='0';}
  itoa(val,temp,10);
  Serial.println(val);
  if(val>300.00){lcd.command(0x94);lcd.print("Temp HIGH");delay(1000);}
  for(int i=0 ; i<3 ; i++) // temprature is inserted
  {
    final_str[j] = '0';
    final_str_sms[j] = '0';
    j++;delay(50);
   // Serial.print(final_str_sms[j]);
  }
  
  run_hour_data();  // run hour is calculated

   for(int i=0 ; i<3 ; i++) // fuellevel is inserted
  {
    final_str[j] = fuel_level[i];
    final_str_sms[j] = fuel_level[i];
    j++;delay(50);
   // Serial.print(final_str_sms[j]);
  }
  
  for(int i=0 ; i<3 ; i++) // site battery voltage is inserted
  {
    final_str[j] = str_voltage[i];
    final_str_sms[j] = fuel_level[i];
    j++;delay(50);
   // Serial.print(fuel_level[i]);
  }  
  k=j;
  for(int i=0;i<62;i++) //down counter dc,mains frequency
  {
    final_str[j]='0';
    j++;delay(50);
   // Serial.print(final_str[j]);
  }
   
 for(int i=0; i<8;i++)
  {
    final_str[j]=dg_run_hour[i];
    final_str_sms[k]=dg_run_hour[i];
    j++;k++;delay(50);
    //Serial.print(dg_run_hour[i]);
  }
   for(int i=0; i<8;i++)
  {
    final_str[j]=mains_run_hour[i];
    final_str_sms[k]=mains_run_hour[i];
    j++;k++;delay(50);
    ///Serial.print(mains_run_hour[i]);
  }
   for(int i=0; i<8;i++)
  {
    final_str[j]=batt_run_hour[i];
    final_str_sms[k]=batt_run_hour[i];
    j++;k++;delay(50);
    //Serial.print(batt_run_hour[i]);
  }
  
  
  for(int i=0;i<2;i++)
  {
    final_str[j]= system_status[i];
    final_str_sms[k]= system_status[i];
    j++;k++;delay(50);
   // Serial.print(system_status[i]);
  }
  for(int i=0;i<2;i++)
  {
    final_str[j]= dg_fault_status[i];
    final_str_sms[k]= dg_fault_status[i];
    j++;k++;delay(50);
   // Serial.print(dg_fault_status[i]);
  }
  
  for(int i=0;i<16;i++) // dg energy and mains energy is filled with zero value
  {
    final_str[j]='0';
    final_str_sms[k]='0';
    j++;k++;delay(50);
   // Serial.print(final_str_sms[k]);
  }
  
  
  for(int i=0;i<8;i++)  // dcem data kwh1 is filled
  {
    final_str[j]= str_kwh1[i];
    final_str_sms[k]= str_kwh1[i];
    j++;k++;delay(50);
    //Serial.print(str_kwh1[i]);
  }
  for(int i=0;i<24;i++)
  {
    final_str[j]='0';
    j++;
  //  Serial.print( final_str[j]);
  }

    for(int i=0;i<8;i++)  // dcem data kwh2 is filled
  {
    final_str[j]=str_kwh2[i];
    final_str_sms[k]=str_kwh2[i];
    j++;k++;delay(50);
  //  Serial.print(str_kwh2[i]);
  }
   for(int i=0;i<24;i++)
  {
    final_str[j]='0';
    j++;
   // Serial.print(final_str[j]);
  }

    for(int i=0;i<8;i++)  // dcem data kwh3 is filled
  {
    final_str[j]=str_kwh3[i];
    final_str_sms[k]=str_kwh3[i];
    j++;k++;delay(50);
   // Serial.print(str_kwh3[i]);
  }
   for(int i=0;i<24;i++)
  {
    final_str[j]='0';
    j++;
   // Serial.print(final_str[j]);
  }

    for(int i=0;i<8;i++)  // dcem data kwh4 is filled
  {
    Serial.print(str_kwh4[i]);
    final_str[j]=str_kwh4[i];
    final_str_sms[k]=str_kwh4[i];
    j++;k++;delay(50);
    //Serial.print(str_kwh4[i]);
  }
   for(int i=0;i<24;i++)
  {
    final_str[j]='0';
    j++;
    //Serial.print( final_str[j]);
  }

 for(int i=0;i<4;i++)
  {
    final_str[j]='0';
    j++;
   // Serial.print(final_str[j]);
  }
  final_str[j]= '.';j++;final_str[j]='\0';

  //Serial.print(final_str[j]);


  lcd.clear();lcd.command(0x80);lcd.print("S.L gprs:");
  lcd.print(strlen(final_str));delay(2000);
  gprs_length = strlen(final_str);
  
  for(int p=0;p<36;p++)
  {
    final_str_sms[k]='0';
    k++;
  }
 final_str_sms[k]= '.';   k++;    final_str_sms[k]='\0';
 lcd.command(0xc0);lcd.print("S.L SMS:");  
 lcd.print(strlen(final_str_sms));delay(3000);
   
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void noc()
{

/* for(int i=1;i<gprs_length;i++)
                {
                  Serial1.print(final_str[i]);delay(50);
                Serial.print(i);Serial.print(":");
                Serial.print(final_str[i]);Serial.println();
                    
                  lcd.print(final_str[i]);
                }
               */

  
    char updated_ip[42];
    delay(2000);lcd.clear();lcd.print("Starting noc");delay(1000);lcd.clear();
    Serial1.write("AT\r\n");delay(100);getResponse(0);delay(1000);lcd.clear();
    Serial1.write("AT+CPIN?\r\n");delay(100);getResponse(0);delay(1000);lcd.clear();
    Serial1.write("AT+CFUN=1\r\n");delay(100);getResponse(0);delay(1000);lcd.clear(); 
    Serial1.write("AT+CSTT=\"airtelgprs.com\"\r\n");delay(100);getResponse(0);delay(1000);lcd.clear();   ///airtelgprs.com//portalnmms
    Serial1.write("AT+CIICR\r\n");Serial1.write(13);Serial1.write(26);lcd.print("Activating GPRS");delay(3000);
    while(!(Serial1.available()>0));
    char ciicr_response[100]="";
    int ciicr_count=0,ipstart_count=0,terminating_count=0;
    
    do{
        for(int i = 0 ; Serial1.available() > 0 && i<200 ; i++) 
          {
          ciicr_response[i] = Serial1.read();
          }
     ciicr_count += 1;
     lcd.print(".");delay(500);
     if (ciicr_count>100){lcd.print("closing connection.");delay(1000);lcd.clear();close_conn();break;}
     
      }while(!(strstr(ciicr_response,"OK")||(strstr(ciicr_response,"ERROR"))));

      
    if(strstr(ciicr_response,"OK"))
      { 
        lcd.clear();lcd.print("GPRS activated");delay(1000);lcd.clear();
        Serial1.write("AT+CIFSR\r\n");delay(100);getResponse(0);delay(1000);lcd.clear();
        //for(i=0;i<strlen(ip_address);i++){updated_ip[i]=ip_address[i];}
        for(i=0;i<strlen(ip_address);i++){Serial1.write(ip_address[i]);delay(100);}lcd.clear();
        char ipstart_response[100]="";
          do{
                for(int i = 0 ; Serial1.available() > 0 && i<200 ; i++) 
                  {
                  ipstart_response[i] = Serial1.read();
                  }
                  ipstart_count += 1;lcd.command(0x80);lcd.print("Connecting to server");delay(500);
                  if (ipstart_count>150){ipstart_count=0;close_conn();break;}
                 
            }while(!(strstr(ipstart_response,"CONNECT OK")||strstr(ipstart_response,"ALREADY CONNECT")||strstr(ipstart_response,"CONNECT FAIL")||strstr(ipstart_response,"¬íur[B¬óTàxp@AUTH@")));
    
           if(strstr(ipstart_response,"CONNECT OK")||strstr(ipstart_response,"ALREADY CONNECT")||strstr(ipstart_response,"¬íur[B¬óTàxp@AUTH@"))
            {
              int auth_count=0;
              lcd.clear();lcd.print("NOC Connected"); delay(2000);
              Serial1.write("AT+CIPSEND=8\r\n");delay(100);delay(3000);lcd.clear();
              Serial1.write("acacme63\r\n");delay(100);lcd.clear();
              char auth_response[100]="";
              do{
                for(int i = 0 ; Serial1.available() > 0 && i<200 ; i++) 
                  {
                  auth_response[i] = Serial1.read();
                  }
                 
                 auth_count += 1;lcd.command(0x80);lcd.print("Authenticating..");delay(500);
                 if(auth_count>150){auth_count=0;close_conn();break;}
                }while(!(strstr(auth_response,"SEND OK")||(strstr(auth_response,"uq~\n@@AUTHOK@@"))));
            
            if((strstr(auth_response,"SEND OK"))||(strstr(auth_response,"uq~\n@@AUTHOK@@")))
            {
                delay(1500);lcd.clear();lcd.print("SENDING STRING"); Serial1.write("AT+CIPSEND=286\r\n");delay(100);delay(1000);lcd.clear();
                Serial1.print('#');delay(50);
                lcd.print('#');
                for(int i=1;i<gprs_length;i++)
                {
                  Serial1.print(final_str[i]);delay(50);
                Serial.print(i);Serial.print(":");
                Serial.print(final_str[i]);Serial.println();
                    
                  lcd.print(final_str[i]);
                }
                Serial1.write(13);Serial1.write(26);delay(3000);
                char str_response[100]="";
                do{
                    for(int i = 0 ; Serial1.available() > 0 && i<200 ; i++) 
                      {
                      str_response[i] = Serial1.read();
                      }
                      
                }while((strstr(str_response,"uq~##MSG##")));
                  
                 delay(3000); lcd.print("GPRS Pass");delay(3000);
                  char shut_response[100]="";
                  Serial1.write("AT+CIPSHUT\r\n");delay(1000);delay(1000);
                  do{
                        for(int i = 0 ; Serial1.available() > 0 && i<200 ; i++) 
                           {
                             shut_response[i] = Serial1.read();
                           }
                     lcd.print("Terminating connection");delay(500);lcd.clear();terminating_count += 1; if(terminating_count>150){terminating_count=0;break;}
                    }while(!(strstr(shut_response,"SHUT OK")||(strstr(shut_response,"ERROR"))));
                    if((strstr(shut_response,"ERROR"))){lcd.print("Connection end failed");}
                    else{lcd.print("Connection end");}

                
            }
            
            else
            {
              lcd.clear();lcd.print("AUTH FAILED");delay(2000);lcd.clear();close_conn();
              delay(1000);lcd.print("staring again noc"); delay(1000); noc();
            }
         }
         
      }
      else if(strstr(ciicr_response,"ERROR"))
      {
        ciicr_failed_flag += 1;lcd.clear();
        lcd.print("GPRS failed");delay(4000);lcd.clear();close_conn();
        delay(1000);
        if(ciicr_failed_flag<2){lcd.print("staring again noc"); delay(1000); noc();}
        else{ciicr_failed_flag=0;lcd.print("Sending sms"); delay(1000);}sms();}
        
      }
/*  */
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void loop() {

  
    attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);
    gprs_event += 1;
    dcem_parameter();
    
  if(digitalRead(Entry)== HIGH)
     {
          delay(50);
          if(digitalRead(Entry)== HIGH)
          {
            page_display(3);
          }
     }
    delay(1000);Serial1.write("AT\r\n");delay(200);delay(100);getResponse(0);
     lcd.clear();temp_val = analogRead(A2);lcd.command(0x80);lcd.print("Temp is:");temp_val=temp_val/10;lcd.print(temp_val);
     lcd.command(0xc0);lcd.print("Site voltage ");lcd.print(voltage_1/10);
     Serial1.write("AT+CCLK?\r\n");delay(500);gettimeResponse(1);delay(2000);lcd.clear();
          delay(50);
         if(digitalRead(Entry)== HIGH)
          {
            page_display(3);
          }
     
     
      low_batt_test();delay(3000);
      ////alarm srting print and status string print
    if(digitalRead(Entry)== HIGH)
      {
          delay(50);
          if(digitalRead(Entry)== HIGH)
          {
            page_display(3);
          }
      
       }
      
      if(dg_flag==1 || mains_flag==1 || batt_flag==1)
      {
        if(dg_flag==1){  timr3_ini(); TCCR4A=0; TCCR4B=0; TCCR5A=0; TCCR5B=0; }
        if(batt_flag==1){  timr4_ini(); TCCR3A=0; TCCR3B=0; TCCR5A=0; TCCR5B=0; }
        if(mains_flag==1){  timr5_ini(); TCCR4A=0; TCCR4B=0; TCCR3A=0; TCCR3B=0; }
      }
      delay(2000);
     if(digitalRead(Entry)== HIGH)
     {
          delay(50);
          if(digitalRead(Entry)== HIGH)
          {
            page_display(3);
          }
     }
     run_hour_data();
     lcd.clear();
     lcd.command(0xd4);lcd.print("Total Current:");lcd.print(total_current/10);
      lcd.command(0x80);lcd.print("Bat R.H:  ");for(int i=0;i<8;i++){lcd.print(batt_run_hour[i]);}
      lcd.command(0x94);lcd.print("D.g R.h:  ");for(int i=0;i<8;i++){lcd.print(dg_run_hour[i]);}
     // lcd.command(0xc0);lcd.print("Batt R.H: ");for(int i=0;i<8;i++){lcd.print(batt_run_hour[i]);}
      lcd.command(0xc0);lcd.print("EB  R.h:  ");for(int i=0;i<8;i++){lcd.print(mains_run_hour[i]);}
      //lcd.command(0xd4);lcd.print("Total Current:");lcd.print(total_current/10);
      delay(6000);
     if(digitalRead(Entry)== HIGH)
     {
          delay(50);
          if(digitalRead(Entry)== HIGH)
          {
            page_display(3);
          }
      
     }
     
     if((gprs_enable == 1)) //&& (gprs_event > 25))
     {
        gprs_event =0;
        delay(2000);lcd.clear();lcd.print("Calculating Length:");str_generate_gprs();delay(2000);lcd.command(0x94);lcd.print("GPRS length:");lcd.print(gprs_length); // sms();
        if((gprs_length == 286)||(strlen(final_str) == 286))
        {
       //sms();
        // Serial.println()
          noc();
        }
        
     }

     
     
 

}
  
