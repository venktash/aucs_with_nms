#ifndef logi-data_h
#define logi-data_h
#include "Arduino.h"
void hold_page();

int write_address = 0;int retrive_main_address = 0;
int data_value1 = 0;
char time_data[12];

void eeprom_write(int data_value)
 {
  if(data_value1 != data_value)
  {
    data_value1 = data_value;
    
    if(write_address>255){write_address = 0;}
   
    EEPROM.update(write_address,data_value);
    write_address += 1;
    for (int i=0;i<12;i++)
    {
      EEPROM.update(write_address,str_temp[i]);
      write_address +=1;
      if(write_address==250)
      {
        break;
      }
    }
  }
  
  
 }
 
void LOG_display(int retrive_val, int count_alarm)
{
  lcd.command(0x94);lcd.print("Time:");lcd.print(time_data[2]);lcd.print(time_data[3]);lcd.print(":");lcd.print(time_data[4]);lcd.print(time_data[5]);lcd.print(":");
  lcd.print(time_data[6]);lcd.print(time_data[7]);
  lcd.command(0xd4);lcd.print("Date:");lcd.print(time_data[0]);lcd.print(time_data[1]);lcd.print("/");lcd.print(time_data[8]);lcd.print(time_data[9]);lcd.print("/");
  lcd.print(time_data[10]);lcd.print(time_data[11]);
  
  if(retrive_val==1){  lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Mains on");  }
  if(retrive_val==2){  lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Site on mains");}
  if(retrive_val==3){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Site on dg"); }
  if(retrive_val==4){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" DG start"); }
  if(retrive_val==5){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Site on battery"); }
  if(retrive_val==6){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Stop fail"); }
  if(retrive_val==7){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" LLOP"); }
  if(retrive_val==8){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Start fail"); }
  if(retrive_val==9){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Fuel Low"); }
  if(retrive_val==10){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Auto Mode"); }
  if(retrive_val==11){lcd.command(0xc0);lcd.print(count_alarm);lcd.print(" Manual Mode");}
  
}

void LOG_data()
{
  
  int retrive_address = 0;
  int retrive_value = 0;
  int alarm_count = 0;
  int p=1;
  lcd.clear();lcd.print("Press UP for alarm");delay(1000);
  while(p==1)
  {
   while(digitalRead(Up)==HIGH)
     {
      alarm_count += 1;
      lcd.clear();lcd.print(" Alarms are ");
      retrive_value = EEPROM.read(retrive_address);
      retrive_main_address =  retrive_address;
     
      retrive_address += 1;
      for(int i=0;i<12;i++)
      {
        time_data[i]= EEPROM.read(retrive_address);
        retrive_address += 1;
      }
      LOG_display(retrive_value,alarm_count);
       
      while(digitalRead(Up)==HIGH);
     }
  while(digitalRead(Down)==HIGH)
     {
      retrive_main_address = retrive_main_address - 13;
      retrive_address = retrive_main_address;
      alarm_count -= 1;
      lcd.clear();lcd.print(" Alarms are ");

             
      retrive_value = EEPROM.read(retrive_address);
      
      retrive_address += 1;
      for(int i=0;i<12;i++)
      {
        time_data[i]= EEPROM.read(retrive_address);
        retrive_address += 1;
      }
      LOG_display(retrive_value,alarm_count);
     
      delay(200);
      while(digitalRead(Up)==HIGH);
     }
   while((digitalRead(Exit)==HIGH))  // hold to erase the data from memory log
   {
      p++; delay(10);attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);
      if(p>200)
      { 
       lcd.clear();lcd.print("Erase log data?");delay(1000);
        while(digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW );
        while(digitalRead(Exit)==HIGH){ p=0;break;}
        while(digitalRead(Entry)==HIGH){ p=0;for(int i=0;i<=255;i++){EEPROM.write(i,0);}} 
      }
    
    }
  }

}
#endif
