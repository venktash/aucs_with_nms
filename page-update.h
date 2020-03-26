#ifndef page-update_h
#define page-update_h
#include "Arduino.h"
#include "logi-data.h"

#include<String.h>
#include<stdlib.h> 

void hold_page();
void  Low_Battery_Update();
void IPaddress_update()
{
  int pos = 0;
  int update_pos = 0;
  int update_value = 0,k=1,j=0;
  lcd.clear();
 
    for(int i=19;i<=38;i++)
    {
    lcd.print(ip_address[i]);
    delay(100);
    }
    lcd.command(0xc0|pos);lcd.print("-");
  while(digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW && digitalRead(Entry)==LOW );
  while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting..");attachPCINT(digitalPinToPCINT(10),hold_page,HIGH); delay(400);lcd.clear();break;}
  while(digitalRead(Entry)==HIGH){j=1;}
  while(j==1)
  {
    if(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW)
    {
      delay(100);
      lcd.command(0xc0|pos);lcd.print(" ");pos= pos+1; lcd.command(0xc0|pos);lcd.print("-"); update_pos = pos; if(pos>19){lcd.command(0xc0|(pos-1));lcd.print(" ");pos = 0;}
      while((digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW));
    }
    if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW)
    {
      delay(100);
      lcd.command(0xc0|pos);lcd.print(" ");pos= pos-1; lcd.command(0xc0|pos);lcd.print("-"); update_pos = pos; if(pos < 0){lcd.command(0xc0|(pos+1));lcd.print(" ");pos = 0;}
      while((digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW));
    }
    if(digitalRead(Entry)==HIGH && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW )
    {
      lcd.command(0x94);lcd.print("updating"); k=1;
      while((digitalRead(Entry)==HIGH &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW ));
      while(k==1)
      {
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW)
        {
          update_value += 1;lcd.command(0x80|update_pos); lcd.print(update_value);ip_address[update_pos]=update_value; if(update_value>9 || update_value < 0){update_value=0;}
          while(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW);
        }
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW)
        {
          update_value -= 1;lcd.command(0x80|update_pos); lcd.print(update_value);ip_address[update_pos]=update_value; if(update_value>9 || update_value < 0){update_value=0;}
          while(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW);
        }
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH)
        {
         k=0;lcd.command(0x94);lcd.print("updated ");delay(500);lcd.command(0x94);lcd.print("        ");update_value=0;update_pos=0;attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);
         while(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH);
        }
      }
      
    }

    if(digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH )
    {
      lcd.clear();lcd.print("Exiting.."); delay(400);lcd.clear();j=0;attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);break;
      while((digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH ));
    }
  }

}





void siteid_upate()
{
  int pos = 0;
  int update_pos = 0;
  int update_value = 0,k=1,j=0;
  char list_data[37]="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  lcd.clear();
 
    for(int i=5;i<14;i++)
    {
    lcd.print(ID_str[i]);
    delay(100);
    }
  lcd.command(0xc0|pos);lcd.print("-");
  while(digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW && digitalRead(Entry)==LOW );
  while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting.."); attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);delay(400);lcd.clear();break;}
  while(digitalRead(Entry)==HIGH){j=1;}
  while(j==1)
  {
    if(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW)
    {
      delay(100);
      lcd.command(0xc0|pos);lcd.print(" ");pos= pos+1; lcd.command(0xc0|pos);lcd.print("-"); update_pos = pos; if(pos>8){lcd.command(0xc0|(pos-1));lcd.print(" ");pos = 0;}
      while((digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW));
    }
    
    if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW)
    {
      delay(100);
      lcd.command(0xc0|pos);lcd.print(" ");pos= pos-1; lcd.command(0xc0|pos);lcd.print("-"); update_pos = pos; if(pos < 0){lcd.command(0xc0|(pos+1));lcd.print(" ");pos = 0;}
      while((digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW));
    }
    
    if(digitalRead(Entry)==HIGH && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW )
    {
      lcd.command(0x94);lcd.print("updating"); k=1;
      while((digitalRead(Entry)==HIGH &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW ));
      while(k==1)
      {
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW)
        {
         lcd.command(0x80|update_pos); lcd.print(list_data[update_value]);ID_str[update_pos+5]=list_data[update_value];  update_value += 1;
          while(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW);
        }
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW)
        {
          update_value -= 1; lcd.command(0x80|update_pos); lcd.print(list_data[update_value]);ID_str[update_pos+5]=list_data[update_value]; 
          while(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW);
        }
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH)
        {
         lcd.command(0x94);lcd.print("updated ");delay(500);lcd.command(0x94);lcd.print("        ");update_value=0;update_pos=0;k=0;
         while(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH);
        }
      }
      
    }                   

    if(digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH )
    {
      lcd.clear();lcd.print("Exiting.."); delay(400);lcd.clear();j=0;attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);break;
      while((digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH ));
    }
  }
}

void enablegprs()
{
  int k=1;
  lcd.clear();
  lcd.command(0x80);lcd.print("Pr UP to  Enable");
  lcd.command(0xc0);lcd.print("Pr DN to Disable");
  while(digitalRead(Entry)== LOW && digitalRead(Up)== LOW && digitalRead(Down)== LOW && digitalRead(Exit)== LOW);
  while(k==1)
  { 
    
    if(digitalRead(Entry)== LOW && digitalRead(Up)== HIGH && digitalRead(Down)== LOW && digitalRead(Exit)== LOW)
    {
      lcd.clear();gprs_enable=1; EEPROM.write(255,gprs_enable); lcd.command(0xc0);lcd.print("GPRS Enabled");k=0;
      while(digitalRead(Entry)== LOW && digitalRead(Up)== HIGH && digitalRead(Down)== LOW && digitalRead(Exit)== LOW);
    }
    
    if(digitalRead(Entry)== LOW && digitalRead(Up)== LOW && digitalRead(Down)== HIGH && digitalRead(Exit)== LOW)
    {
      lcd.clear();gprs_enable=0;EEPROM.write(255,gprs_enable);lcd.command(0xc0);lcd.print("GPRS Disabled");k=0;
      while(digitalRead(Entry)== LOW && digitalRead(Up)== LOW && digitalRead(Down)== HIGH && digitalRead(Exit)== LOW);
    }
  }
}

void settings_update()
{
  lcd.clear();
  lcd.command(0x80);lcd.print("Choose + to Update");
  lcd.command(0x94);lcd.print("Choose - to view LOG");
  while(digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW);
  while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting.."); delay(400);lcd.clear();attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);break;}
  while(digitalRead(Down)==HIGH){lcd.clear(); LOG_data();}
  while(digitalRead(Up)==HIGH)
  {
    int m=0;
    lcd.command(0x80);lcd.print("Press Enter to Update");
    lcd.command(0xc0);lcd.print(" Update Site ID");
    lcd.command(0x94);lcd.print(" Update IP address");
    lcd.command(0xd4);lcd.print(" Enable/Disable GPRS");
    while(digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW && digitalRead(Entry)==LOW );
    while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting..");attachPCINT(digitalPinToPCINT(10),hold_page,HIGH); delay(400);lcd.clear();break;}
    while(digitalRead(Entry)==HIGH){m=1;}
    while(m==1)
    {
      if(digitalRead(Down)== HIGH && digitalRead(Up)== LOW)
      {
        lcd.clear();
        lcd.command(0x80);lcd.print("  Update Site ID");
        lcd.command(0xc0);lcd.print("> Update IP address");
        lcd.command(0x94);lcd.print("  Enable/Disable GPRS"); 
        while(digitalRead(Entry)== LOW && digitalRead(Up)== LOW && digitalRead(Down)== LOW && digitalRead(Exit)== LOW);
        while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting..");attachPCINT(digitalPinToPCINT(10),hold_page,HIGH); delay(400);lcd.clear();break;}
        if(digitalRead(Entry)==HIGH)
        {
          lcd.clear();lcd.print("updating IP");delay(1000);lcd.clear();
          IPaddress_update();
        }
        if(digitalRead(Down)== HIGH && digitalRead(Up)== LOW)
        {
              lcd.clear();
              lcd.command(0x80);lcd.print("  Update Site ID");
              lcd.command(0xc0);lcd.print("  Update IP address");
              lcd.command(0x94);lcd.print(">Enable/Disable GPRS");
              while(digitalRead(Entry)== LOW && digitalRead(Up)== LOW && digitalRead(Down)== LOW && digitalRead(Exit)== LOW);
              while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting..");attachPCINT(digitalPinToPCINT(10),hold_page,HIGH); delay(400);lcd.clear();break;}
              if(digitalRead(Entry)==HIGH)
                {
                  enablegprs();
                }
         
        }
  
      }
      if(digitalRead(Down)== LOW && digitalRead(Up)== HIGH)
      {
        lcd.clear();
        lcd.command(0x80);lcd.print("-> Update Site ID");
        lcd.command(0xc0);lcd.print("  Update IP address");
        while(digitalRead(Entry)== LOW && digitalRead(Up)== LOW && digitalRead(Down)== LOW && digitalRead(Exit)== LOW);
        while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting..");attachPCINT(digitalPinToPCINT(10),hold_page,HIGH); delay(400);lcd.clear();break;}
        if(digitalRead(Entry) == HIGH && digitalRead(Down)== LOW && digitalRead(Up)== LOW)
        {
          lcd.clear();lcd.print("updating SITE id");delay(1000);lcd.clear();
          siteid_upate();
        }
      }

        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>added new function>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> added new function
         if(digitalRead(Down)== HIGH && digitalRead(Up)== LOW)
      {
        lcd.clear();
        
        lcd.command(0x80);lcd.print("-> Update batt Voltage");
        lcd.command(0x94);lcd.print("  Update IP address");
        while(digitalRead(Entry)== LOW && digitalRead(Up)== LOW && digitalRead(Down)== LOW && digitalRead(Exit)== LOW);
        while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting..");attachPCINT(digitalPinToPCINT(10),hold_page,HIGH); delay(400);lcd.clear();break;}
        if(digitalRead(Entry) == HIGH && digitalRead(Down)== LOW && digitalRead(Up)== LOW)
        {
          lcd.clear();lcd.print("updating low battery");delay(1000);lcd.clear();
        Low_Battery_Update();
//int farzi=Low_Battery_Update();
       // lcd.print(farzi);
        }
      }
   //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  
      
      if(digitalRead(Entry) == LOW && digitalRead(Down)== LOW && digitalRead(Up)== LOW && digitalRead(Exit)== HIGH)
      {
        lcd.clear();lcd.print("Exiting..");delay(1000);lcd.clear();attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);m=0;
        
      }
      
    }
  }
}

void eeprom_save_battery(int x)
{
  int j=0;
  j= x%10;
  EEPROM.update(254,j);
  x = x/10;
  j= x%10;
  EEPROM.update(253,j);
  x = x/10;
  j= x%10;
  EEPROM.update(252,j);
  
  
}
void  Low_Battery_Update()
{
  int bttry_value=0;
 int pos = 0;
  int update_pos = 0;
  int update_value = 0,k=1,j=0;
  char list_data[]="0123456789";
  lcd.clear();
 
    for(int i=0;i<3;i++)
    {
    lcd.print(ID_str1[i]);
    delay(100);
    }
  lcd.command(0xc0|pos);lcd.print("-");
  while(digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW && digitalRead(Entry)==LOW );
  while(digitalRead(Exit)==HIGH){lcd.clear();lcd.print("Exiting.."); attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);delay(400);lcd.clear();break;}
  while(digitalRead(Entry)==HIGH){j=1;}
  while(j==1)
  {
    if(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW)
    {
      delay(100);
      lcd.command(0xc0|pos);lcd.print(" ");pos= pos+1; lcd.command(0xc0|pos);lcd.print("-"); update_pos = pos; if(pos>2){lcd.command(0xc0|(pos-1));lcd.print(" ");pos = 0;}
      while((digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW));
    }
    
    if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW)
    {
      delay(100);
      lcd.command(0xc0|pos);lcd.print(" ");pos= pos-1; lcd.command(0xc0|pos);lcd.print("-"); update_pos = pos; if(pos < 0){lcd.command(0xc0|(pos+1));lcd.print(" ");pos = 0;}
      while((digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW));
    }
    
    if(digitalRead(Entry)==HIGH && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW )
    {
      lcd.command(0x94);lcd.print("updating"); k=1;
      while((digitalRead(Entry)==HIGH &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW ));
      while(k==1)
      {
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW)
        {
         lcd.command(0x80|update_pos); lcd.print(list_data[update_value]);ID_str1[update_pos+0]=list_data[update_value];  update_value += 1;// bttry_value=atoi(ID_str1);
          while(digitalRead(Entry)==LOW &&  digitalRead(Up)==HIGH && digitalRead(Down)==LOW && digitalRead(Exit)==LOW);
        }
       // bttry_value=atoi(ID_str1);
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW)
        {
          update_value -= 1; lcd.command(0x80|update_pos); lcd.print(list_data[update_value]);ID_str1[update_pos+0]=list_data[update_value];//bttry_value=atoi(ID_str1); 
          while(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==HIGH && digitalRead(Exit)==LOW);
        }
        //bttry_value=atoi(ID_str1);
        if(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH)
        {
         lcd.command(0x94);lcd.print("updated ");delay(500);lcd.command(0x94);lcd.print("        ");update_value=0;update_pos=0;k=0;
         while(digitalRead(Entry)==LOW &&  digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH);
        }
      }
     // return bttry_value;
    }                   

    if(digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH )
    {
      lcd.clear();bttry_value=atoi(ID_str1);lcd.print(bttry_value);eeprom_save_battery(bttry_value);low_battery_value=bttry_value;lcd.command(0xc0);lcd.print("Exiting.."); delay(400);lcd.clear();j=0;attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);break;
      while((digitalRead(Entry)==LOW && digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==HIGH ));
    }
  } 
}


void page_display(int pass_value)
{
  disablePCINT(digitalPinToPCINT(10));
  int click_count = 0;
  int n=1;
  lcd.clear();
  lcd.command(0x80);
  lcd.print("  CONFIG SETTINGS  ");
  lcd.command(0xc0);
  lcd.print("Enter Password");
  lcd.command(0x94);
  lcd.print(".....");
  while(digitalRead(Up)==LOW && digitalRead(Down)==LOW && digitalRead(Exit)==LOW);
  while(digitalRead(Exit)==HIGH)
  {
    lcd.clear();
    lcd.print("Exiting..");
    n=0; 
    delay(400);
    lcd.clear();
    attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);
    break;
    }
  while(digitalRead(Entry)== HIGH){n=1;}
  while(n==1)
  {
      if(digitalRead(Up)==HIGH && digitalRead(Down)==LOW)
      {
        lcd.command(0x94|click_count);
        delay(200);
        pass_value = pass_value+1;
        click_count++;
        lcd.print("*");
        while((digitalRead(Up)==HIGH && digitalRead(Down)==LOW));
        }
        
      if(digitalRead(Up)==LOW && digitalRead(Down)== HIGH)
      {
        lcd.command(0x94|click_count);
      delay(200);
      pass_value = pass_value-1;
      click_count++;
      lcd.print("*");
      while((digitalRead(Up)==LOW && digitalRead(Down)== HIGH));
      }
      if(click_count == 5)
      {
          if(pass_value == 2)
          {
            delay(500);lcd.clear();lcd.print("     welcome to      ");delay(200);
            lcd.command(0xc0);lcd.print("    Control Page      "); delay(1000); lcd.clear();
            pass_value=0;click_count=0;
            settings_update();
              
          }
          else{delay(500);lcd.clear();lcd.print("Wrong pasword  ");delay(200);lcd.clear(); pass_value=0;click_count=0;}
          attachPCINT(digitalPinToPCINT(10),hold_page,HIGH);
          break;
      }
  }
}






#endif
