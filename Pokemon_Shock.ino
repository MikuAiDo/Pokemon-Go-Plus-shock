#define BLINKER_WIFI
#include <Blinker.h>
#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

#define FREQ 2000     // 频率
#define RESOLUTION 8  // 分辨率


char auth[] = "c3d2e3af440e";

WiFiManager wifiManager;

int shock = 4;
int plus = 1;
int onoff = 0;
int emergencyExit = 0;
int gosleepmsg = 0;
int sleeptime = 0;
int havesleep = 0;
int tolongdeep = 0;
int buttononoff = 0;
int sleepstate = 0;
int hourcount = 0;
int mintcount = 0;
int sliderhour = 0;
int slidermint = 0;
volatile int count = 0;
volatile int mint = 0;
volatile int tens = 0;
hw_timer_t *tim = NULL;
// 新建组件对象
BlinkerButton ButtonSleep("btn-abc");
BlinkerButton ButtonSafely("btn-v74");
BlinkerButton ButtonShallow("btn-wq7");
BlinkerButton ButtonDeeply("btn-4kc");
BlinkerButton ButtonExit("btn-0e6");
BlinkerNumber NumberSleep("num-abc");
BlinkerSlider SliderHour("ran-d83");
BlinkerSlider SliderMint("ran-rkj");
BlinkerButton ButtonIcon("btn-d32");

void ButtonSleep_callback(const String &state) {
  BLINKER_LOG("get ButtonSleep state: ", state);
  if(sleepstate != 0){
    buttononoff = 1;
    ButtonIcon.icon("fas fa-snooze");
    ButtonIcon.color("#FAFAD2");
    ButtonIcon.text("正在睡觉");
    ButtonIcon.print();
  }
  
}

void ButtonExit_callback(const String &state) {
  BLINKER_LOG("get ButtonExit state: ", state);
  emergencyExit = 1;
}

void ButtonSafely_callback(const String &state) {
  BLINKER_LOG("get ButtonSafely state: ", state);
  if (state == "on") {
    sleepstate = 2;
    ButtonSafely.print("on");
    ButtonShallow.print("off");
    ButtonDeeply.print("off");
  }

  if (state == "off") {
    sleepstate = 0;
    ButtonSafely.print("off");
    ButtonShallow.print("off");
    ButtonDeeply.print("off");
  }
}

void ButtonShallow_callback(const String &state) {
  BLINKER_LOG("get ButtonShallow state: ", state);
  if (state == "on") {
    sleepstate = 1;
    ButtonSafely.print("off");
    ButtonShallow.print("on");
    ButtonDeeply.print("off");
  }

  if (state == "off") {
    sleepstate = 0;
    ButtonSafely.print("off");
    ButtonShallow.print("off");
    ButtonDeeply.print("off");
  }
}

void ButtonDeeply_callback(const String &state) {
  BLINKER_LOG("get ButtonDeeply state: ", state);
  if (state == "on") {
    sleepstate = 3;
    ButtonSafely.print("off");
    ButtonShallow.print("off");
    ButtonDeeply.print("on");
  }

  if (state == "off") {
    sleepstate = 0;
    ButtonSafely.print("off");
    ButtonShallow.print("off");
    ButtonDeeply.print("off");
  }
}

void SliderHour_callback(int32_t value) {
  BLINKER_LOG("get SliderHour value: ", value);
  sliderhour = value;
  hourcount = value * 60;
}

void SliderMint_callback(int32_t value) {
  BLINKER_LOG("get SliderMint value: ", value);
  slidermint = value;
  mintcount = value;
}

// 如果未绑定的组件被触发，则会执行其中内容
//void dataRead(const String & data)
//{
//    BLINKER_LOG("Blinker readString: ", data);
//    counter++;
//    NumberSleep.print(counter);
//}


void PokemonplusOn(){

  digitalWrite(plus,HIGH);
  Blinker.delay(1600);
  digitalWrite(plus,LOW);
}

void Emergency_Exit(){

  if(emergencyExit == 1){
    Serial.println("Emergency Exit.");
  }
}

void GotoSleep() {

  Serial.println("GotoSleep");
  while (1) {
    Blinker.run();
    Emergency_Exit();
    if(emergencyExit == 1){
      break;
    }
    if (mint == 10) {
      //mint = 0;
      gosleepmsg = 0;
      break;
    }
  }
}
void Shallow(int st) {
  int s = st / 4 + 1;
  Serial.println("Shallow");
  for (int i = 0; i < s; i++) {
    Serial.println("Star1");
    while (1) {
      Blinker.run();
      Emergency_Exit();
      if(emergencyExit == 1){
      break;
    }
      if (mint >= 4) {
        onoff = 1;
        Serial.println("Star3");
        ledcWrite(shock, 45);
        Serial.println("StarShock");
        while (1) {
          Blinker.run();
          Emergency_Exit();
          if(emergencyExit == 1){
            break;
          }
          if (tens >= 1) {
            tens = 0;
            onoff = 0;
            mint = 0;
            ledcWrite(shock, 0);
            break;
          }
        }
        if (onoff == 0) {
          break;
        }
      }
    }
    Serial.println("Out");
  }
}

void Safely(int st) {
  int s = st / 10 + 1;
  Serial.println("Safely");
  for (int i = 0; i < s; i++) {
    Serial.println("Star1");
    while (1) {
      Blinker.run();
      Emergency_Exit();
      if(emergencyExit == 1){
      break;
    }
      if (mint >= 10) {
        onoff = 1;
        Serial.println("Star3");
        ledcWrite(shock, 45);
        Serial.println("StarShock");
        while (1) {
          Blinker.run();
          Emergency_Exit();
          if(emergencyExit == 1){
            break;
          }
          if (tens >= 1) {
            tens = 0;
            onoff = 0;
            mint = 0;
            ledcWrite(shock, 0);
            break;
          }
        }
        if (onoff == 0) {
          break;
        }
      }
    }
    Serial.println("Out");
  }
}

void Deeply(int st) {

  int halfs = st / 2;
  if(st >= 350){
    tolongdeep = 1;
  }
  Serial.println("Deeply");
  for (int i = 0; i < 2; i++) {
    Serial.println("Star1");
    while (1) {
      Blinker.run();
      Emergency_Exit();
      if(emergencyExit == 1){
        break;
      }
      if (mint >= halfs) {
        if(st >= 350){
           onoff = 1;
           Serial.println("Star3");
           ledcWrite(shock, 45);
           Serial.println("StarShock");
           while (1) {
             Blinker.run();
             Emergency_Exit();
             if(emergencyExit == 1){
               break;
             }
             if (tens >= 1) {
              tens = 0;
              onoff = 0;
              mint = 0;
              ledcWrite(shock, 0);
              break;
            }
        }
        }
        Serial.println("Star3");
        onoff = 0;
        mint = 0;
        break;
        }
      }
    }
    Serial.println("Out");
}

void service_timer0() {
  count++;
  // Serial.print(count);
  // Serial.println(" Secon");
  if (count % 10 == 0) {
    if (onoff == 1) {
      tens++;
      // Serial.print(tens);
      // Serial.println(" TenSecon");
    }
  }
  if (count >= 60) {
    mint++;
    if(gosleepmsg == 0){
        havesleep++;
    }
    NumberSleep.print(havesleep);
    count = 0;
  }
}

void clearCount() {
  havesleep=0;
  emergencyExit = 0;
  count = 0;
  tens = 0;
  mint = 0;
  onoff = 0;
}

void Init_wifi(){

  // 自动连接WiFi。
  wifiManager.autoConnect("睡饱饱宝可睡");
  // 如果您希望该WiFi添加密码，可以使用以下语句：
  // wifiManager.autoConnect("AutoConnectAP", "12345678");
  // 以上语句中的12345678是连接AutoConnectAP的密码
  // WiFi连接成功后将通过串口监视器输出连接成功信息
  Serial.println("");
  Serial.print("ESP32 Connected to ");
  Serial.println(WiFi.SSID());  // WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  // IP
}

void Init_Blinker(){

  String ssid = WiFi.SSID();
  const char* ssid_char = ssid.c_str();
  String pswd = WiFi.psk();
  const char* pswd_char = pswd.c_str();

  Blinker.begin(auth, ssid_char, pswd_char);

  Blinker.attachHeartbeat(heartbeat);
  ButtonSleep.attach(ButtonSleep_callback);
  ButtonExit.attach(ButtonExit_callback);
  ButtonSafely.attach(ButtonSafely_callback);
  ButtonShallow.attach(ButtonShallow_callback);
  ButtonDeeply.attach(ButtonDeeply_callback);
  SliderHour.attach(SliderHour_callback);
  SliderMint.attach(SliderMint_callback);
}

void Init_timer() {

  tim = timerBegin(1000000);
  timerAttachInterrupt(tim, service_timer0);
  timerAlarm(tim, 1000000, true, 0);
}

void Update_BlinkerUI(){

  NumberSleep.print(havesleep);
  SliderHour.print(sliderhour);
  SliderMint.print(slidermint);
  switch(sleepstate){
    case 0 :
        ButtonSafely.print("off");
        ButtonShallow.print("off");
        ButtonDeeply.print("off");
        break;
    case 1 :
        ButtonSafely.print("off");
        ButtonShallow.print("on");
        ButtonDeeply.print("off");
        break;
    case 2 :
        ButtonSafely.print("on");
        ButtonShallow.print("off");
        ButtonDeeply.print("off");
        break;
    case 3 :
        ButtonSafely.print("off");
        ButtonShallow.print("off");
        ButtonDeeply.print("on");
        break;
  }

  switch(buttononoff){
    case 0 :
        ButtonIcon.icon("far fa-sun");
        ButtonIcon.color("#F4A460");
        ButtonIcon.text("等待睡觉");
        ButtonIcon.print();
        break;
    case 1 :
        ButtonIcon.icon("fas fa-snooze");
        ButtonIcon.color("#FAFAD2");
        ButtonIcon.text("正在睡觉");
        ButtonIcon.print();
        break;
  }

}

void heartbeat()
{
  
  Update_BlinkerUI();
}



void setup() {
  Serial.begin(115200);
  // BLINKER_DEBUG.stream(Serial);
  // BLINKER_DEBUG.debugAll();
  pinMode(shock, OUTPUT);
  pinMode(plus,OUTPUT);
  digitalWrite(shock, LOW);
  digitalWrite(plus, LOW);
  Init_timer();
  timerStop(tim);   //Stop之后ReStart不生效
  ledcAttach(shock, FREQ, RESOLUTION);
  ledcWrite(shock, 0);
  digitalWrite(shock, HIGH);
  Init_wifi();
  Init_Blinker();

}

void loop() {
  Blinker.run();
    if (buttononoff == 1) {
      sleeptime = hourcount + mintcount - 10;
      if (sleeptime <= 0) {
        Serial.println("erro time");
        buttononoff=0;
      } 
      if(sleepstate == 0){
        Serial.println("erro state");
        buttononoff=0;
      }

      if(sleeptime > 0 && sleepstate != 0) {
        if (sleepstate == 1) {
          //sleeptime = 280;
          timerStart(tim);
          PokemonplusOn();
          clearCount();
          timerRestart(tim);
          gosleepmsg = 1;
          GotoSleep();
          Shallow(sleeptime);
          GotoSleep();
          PokemonplusOn();
          timerStop(tim);
          Serial.println("Shock End.");
          clearCount();
          ButtonIcon.icon("far fa-sun");
          ButtonIcon.color("#F4A460");
          ButtonIcon.text("等待睡觉");
          ButtonIcon.print();
        }
        if (sleepstate == 2) {
          //sleeptime = 280;
          timerStart(tim);
          PokemonplusOn();
          clearCount();
          timerRestart(tim);
          gosleepmsg = 1;
          GotoSleep();
          Safely(sleeptime);
          GotoSleep();
          PokemonplusOn();
          timerStop(tim);
          Serial.println("Shock End.");
          clearCount();
          ButtonIcon.icon("far fa-sun");
          ButtonIcon.color("#F4A460");
          ButtonIcon.text("等待睡觉");
          ButtonIcon.print();
        }
        if (sleepstate == 3) {
          //sleeptime = 280;
          timerStart(tim);
          PokemonplusOn();
          clearCount();
          timerRestart(tim);
          gosleepmsg = 1;
          GotoSleep();
          Deeply(sleeptime);
          GotoSleep();
          PokemonplusOn();
          timerStop(tim);
          Serial.println("Shock End.");
          clearCount();
          ButtonIcon.icon("far fa-sun");
          ButtonIcon.color("#F4A460");
          ButtonIcon.text("等待睡觉");
          ButtonIcon.print();
        }

        buttononoff = 0;
      }
    }

}
