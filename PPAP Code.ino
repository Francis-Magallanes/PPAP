#include <LiquidCrystal.h>

const int rs = 13, rw = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal LCD(rs, rw, en, d4, d5, d6, d7); 

int ButtonMain = 2;
int ButtonUp = 3;
int ButtonDown = 4;
int Current_Display = 0;
bool MainMenuDisplay = 0; // is main menu displaying
bool alphanumber = 0; // is alpha numeric mode on
bool NUM = 0;
int alnum = 32; //space / start value for alpha numeric mode
int num = 0; //for number operations
//Alpha Numeric Cycle
//All Caps -> Lower Caps -> Numbers -> Space and vice versa
int alnumposition = 0; //position for the lcd (column)
String temp = ""; //to be used later
int tempNum = 0;
char NewChar; // int to char variable
int hour = 00;
int minute = 00;
int second = 00;
bool idlestate = 1; 
String NameSet = ""; //Final variable
int weightset = 0; 
int freqset = 0;
int presetcycle = 0; //Cycling of feed options
bool feedsetting = 0; //If feed setting was chosen from menu
String TempName = ""; //Temp Holders
int Tempweight = 0;
int Tempfreq = 0;
bool CustomSet = 0;
int CustomSetting = 0; //Cycling for custom feed parameters



class Feed
{
    public:
      String Name;
      int weight;
      int frequency;
 
    
    Feed(String x, int y, int z){
      Name = x;
        weight = y;
        frequency = z;
    }
 
  
};

Feed Preset1("XDogs",1,6); 
Feed Preset2("YDogs",2,6); 
Feed Preset3("ZDogs",3,6); 
//Put above the presets for the dog breedds
//(String Name, int weight, int frequency)

void TimeDisplay(int TimeH, int TimeM, int TimeS)
{
  if (TimeH > 23){
      TimeH = 0;
        hour = 0; //Hour cannot go beyond 23, these are to reset the variables.
    }
    if (TimeM > 59){
      TimeM = 0;
        TimeH++;
        hour++;
        minute = 0; // Minute cannot go beyond 59, these are to reset the variables.
    }
    if (TimeS > 59){
      TimeS = 0;
        TimeM++;
        minute++;
        second = 0; //Second cannot go beyond 59, these are to reset the variables.
    }
    
  if (TimeH < 10){
      LCD.print("0");
        LCD.print(TimeH); //To show double digit values in the case of single digit hour/minute/second
    }
    else{
        LCD.print(TimeH);
    }
    LCD.print(":");
    if (TimeM < 10){
      LCD.print("0");
        LCD.print(TimeM);
    }
    else{
        LCD.print(TimeM);
    }
    LCD.print(":");
    if (TimeS < 10){
      LCD.print("0");
        LCD.print(TimeS);
    }
    else{
        LCD.print(TimeS);
    }
}

void ConfirmationDelay(int x){
 
  switch(x){
   case 0:
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Using Preset 1");
      LCD.setCursor(0, 1);
      LCD.print("Returning...");
      break;
    case 1:
        LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Using Preset 2");
      LCD.setCursor(0, 1);
      LCD.print("Returning...");
      break;
    case 2:
        LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Using Preset 3");
      LCD.setCursor(0, 1);
      LCD.print("Returning...");
      break;
    case 3:
        LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Using Custom Set");
      LCD.setCursor(0, 1);
      LCD.print("Returning...");
      break;
    case 4:
        LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Time Changed");
      LCD.setCursor(0, 1);
      LCD.print("Returning...");
      break;
  }
  
}

void MainDisplay() // This function will be for the main display
  //This will essentially be like the home display screen
{
  
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Current Time: ");
  LCD.setCursor(0, 1);
  TimeDisplay(hour, minute, second);
}

void Menu(int x) // Menu display
{

  switch(x) {
  case 0: //Feeding Settings
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Main Menu");
      LCD.setCursor(0, 1);
      LCD.print("Feeding Settings");
      break;
    case 1: //Clock(Time)
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Main Menu");
      LCD.setCursor(0, 1);
      LCD.print("Change Time");
      break;
    case 2: //Reset
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Main Menu");
      LCD.setCursor(0, 1);
      LCD.print("Reset");
      break;
    case 3: //Back
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Main Menu");
      LCD.setCursor(0, 1);
      LCD.print("Back");
      break;
  }
}

void FeedSettingMenu(int x){
  switch(x) {
    case 0: //Preset1
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print(Preset1.Name);
      break;
    case 1: //Preset2
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print(Preset2.Name);
      break;
    case 2: //Preset3
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print(Preset3.Name);
      break;
    case 3: //Custom
      LCD.clear();
      LCD.setCursor(0, 0);
        LCD.print("Select Option");
        LCD.setCursor(0, 1);
        LCD.print("Custom Set");
      break;
    case 4: //Back
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Select Option");
      LCD.setCursor(0, 1);
      LCD.print("Back");
      break;
  }
}


void setup()
{
  pinMode(ButtonMain, INPUT_PULLUP);
  pinMode(ButtonUp, INPUT_PULLUP);
  pinMode(ButtonDown, INPUT_PULLUP);
  LCD.begin(16,2);
  Serial.begin(9600);
  
  MainDisplay();
  
}

void loop()
{
  delay(1000);
  second++;
  
  if (idlestate == 1){
    MainDisplay();
  }
  
  if(CustomSet == 1){
      
      if(CustomSetting == 0){
        
        delay(1000);
        second++;
        LCD.setCursor(0, 0);
        LCD.print("Name: ");
        LCD.setCursor(alnumposition, 1);
        
      }
      
      if(CustomSetting == 1){
        
        delay(1000);
        second++;
        LCD.setCursor(0, 0);
        LCD.print("Amount(lbs): ");
        LCD.setCursor(alnumposition, 1);
        
      }
      
      if(CustomSetting == 2){
        
        delay(1000);
        second++;
        LCD.setCursor(0, 0);
        LCD.print("Frequency: ");
        LCD.setCursor(alnumposition, 1);
        
      }
      
    }
  
  bool b1 = digitalRead(ButtonMain); // Main/Confirm
  bool b2 = digitalRead(ButtonUp); //Up
  bool b3 = digitalRead(ButtonDown); //Down
  
  if (b1 == 0){ //Main/Confirm Button with if else to 
          //Differentiate each situation 
    
    if (NUM == 1){
      
     if (alnumposition < 1){
        
        delay(1000);
        second++;
        tempNum = num;
        alnumposition = alnumposition + 1;
        num = 0;
        
     }
      
     if (alnumposition == 1 && CustomSetting == 1){
        
      delay(1000);
        second++;
        weightset = num;
        num = 0;
        CustomSetting++;
        alnumposition = 0;
        LCD.clear();
       
     }
      
     if (alnumposition == 1 && CustomSetting == 2){
        
        freqset = num;
        num = 0;
        alnumposition = 0;
        LCD.clear();
        NUM = 0;
        ConfirmationDelay(3);
        delay(1000);
        second++;
        MainDisplay();
        idlestate = 1;
       
     }
      
    }
    
    if (alphanumber == 1){
      
      idlestate = 0;
      
      if (alnumposition == 15 && CustomSetting == 0){
       
    delay(1000);
        second++;
        NameSet = temp;
        CustomSetting++;
        alnum = 32;
        alnumposition = 0;
        alphanumber = 0;
        NUM = 1;
        LCD.clear();
        
      }
      
      else if (alnumposition < 15 && CustomSetting == 0){
        
        delay(1000);
        second++;
        NewChar = alnum;
        temp = temp + NewChar;
        alnumposition = alnumposition + 1;
        LCD.setCursor(alnumposition, 1);
        alnum = 32;
        
      }
      
    }
    
    if (feedsetting == 1){
      
        if(presetcycle == 0){
          
            NameSet = Preset1.Name;
            weightset = Preset1.weight;
            freqset = Preset1.frequency;
            ConfirmationDelay(0);
            delay(1000);
            second++;
            feedsetting = 0;
            MainDisplay();
            idlestate = 1;
          
        }
      
        if(presetcycle == 1){
          
            NameSet = Preset2.Name;
            weightset = Preset2.weight;
            freqset = Preset2.frequency;
            ConfirmationDelay(1);
            delay(1000);
            second++;
            feedsetting = 0;
            MainDisplay();
            idlestate = 1;
          
        }
      
        if(presetcycle == 2){
          
            NameSet = Preset3.Name;
            weightset = Preset3.weight;
            freqset = Preset3.frequency;
            ConfirmationDelay(2);
            delay(1000);
            second++;
            feedsetting = 0;
            MainDisplay();
            idlestate = 1;
          
        }
      
        if(presetcycle == 3){
      
            feedsetting = 0;
            alphanumber = 1;
            CustomSet = 1;
            delay(1000);
            second++;
            LCD.clear();
          
        }
      
        if(presetcycle == 4){
         
            feedsetting = 0;
            MainMenuDisplay = 1;
            delay(1000);
          second++;
            Menu(Current_Display);
          
        }
      
    }
    
    if (MainMenuDisplay == 0 && alphanumber == 0 && NUM == 0) { //While Menu is on 
                    //b1 = confirm button 
        idlestate = 0;
        LCD.clear();
      delay(1000);
        second++;
      Menu(Current_Display);
      MainMenuDisplay = 1;
      
    }
    
    else {
      
      if (MainMenuDisplay == 1 && Current_Display == 0){ //Feeding Settings
       
        delay(1000);
        second++;
        presetcycle = 0;
        MainMenuDisplay = 0;
        feedsetting = 1;
        FeedSettingMenu(presetcycle);
        
      }
      
      if (MainMenuDisplay == 1 && Current_Display == 1){ //Time Change Settings
        
        
        
      }
      
      if (MainMenuDisplay == 1 && Current_Display == 3){
       
        MainMenuDisplay = 0;
        delay(1000);
        second++;
        MainDisplay();
        idlestate = 1;
        
      }
      
    }
    
    
   
  }
  
   if (b2 == 0){
     
     if (MainMenuDisplay == 1){
       
       if (Current_Display == 3) {
         
         Current_Display = 0;
         delay(1000);
         second++;
         Menu(Current_Display);
         
       }

       else {
         
          Current_Display = Current_Display + 1; 
          delay(1000);
          second++;
          Menu(Current_Display);
         
       }
       
     }
     
     if (NUM == 1){
       
       if (num == 9){
        
          num = 0;
          delay(1000);
      second++;
          LCD.setCursor(alnumposition, 1);
          LCD.print(num);
         
       }
       
       else {
         
          num++;
          delay(1000);
      second++;
          LCD.setCursor(alnumposition, 1);
          LCD.print(num);
         
       }
       
     }
     
     if (alphanumber == 1){
      
       if (alnum == 90){
         
         alnum = 97;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
       else if (alnum == 122){
        
         alnum = 48;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
       else if (alnum == 57){
         
         alnum = 32;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
       else if (alnum == 32){
        
         alnum = 65;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
      
       else{
        
         alnum = alnum + 1;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
     }
     
     else {
       
       Serial.print("Test");
       
     }
     
     if (feedsetting == 1){
       
        if(presetcycle == 4){
          
          delay(1000);
          second++;
          presetcycle = 0; 
          FeedSettingMenu(presetcycle);
          
      }
       
      else{
          
          delay(1000);
          second++;
          presetcycle++; 
          FeedSettingMenu(presetcycle);
          
      }
       
     }
     
    
     
   }
  
   if (b3 == 0){
     
     if (MainMenuDisplay == 1){
     
       if (Current_Display == 0) {
         
          Current_Display = 3;
          delay(1000);
          second++;
          Menu(Current_Display);
         
       }

       else {
         
          Current_Display = Current_Display - 1; 
          delay(1000);
          second++;
          Menu(Current_Display);
         
       }

     }
     
     if (NUM == 1){
       
       if (num == 0){
        
          num = 9;
          delay(1000);
      second++;
          LCD.setCursor(alnumposition, 1);
          LCD.print(num);
         
       }
       
       else {
         
          num--;
          delay(1000);
      second++;
          LCD.setCursor(alnumposition, 1);
          LCD.print(num);
         
       }
       
     }
     
     if (alphanumber == 1){
      
       if (alnum == 97){
         
         alnum = 90;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
       else if (alnum == 65){
        
         alnum = 32;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
       else if (alnum == 48){
        
         alnum = 122;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
       else if (alnum == 32){
        
         alnum = 57;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
       else{
        
         alnum = alnum - 1;
         delay(1000);
         second++;
         LCD.setCursor(alnumposition, 1);
         NewChar = alnum;
         LCD.print(NewChar);
         
       }
       
     }
     
     if (feedsetting == 1){
       
        if(presetcycle == 0){
          
          delay(1000);
          second++;
          presetcycle = 4; 
          FeedSettingMenu(presetcycle);
          
      }
       
      else{
          
          delay(1000);
          second++;
          presetcycle--; 
          FeedSettingMenu(presetcycle);
          
      }
     }
     
   }
    
}
