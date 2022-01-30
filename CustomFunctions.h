//AAS CanSat | Custom functions if needed. -- Coded by Eduritez
//Latest revision: 15/11/2021

double Round(double a) { //Basically keeps the number as accurate as possible.
  double output=floor(a*100+0.1)/100;
  return output;
}

class Vector {
  public:
    float X=0;
    float Y=0;
    float Z=0;
    void Update(int x, int y, int z) {
      X=x; Y=y; Z=z;
    }
};

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
