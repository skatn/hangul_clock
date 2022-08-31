#define CDS_PIN A0
#define CDS_MIN_DEFAULT 200
#define CDS_MAX_DEFAULT 800
#define CDS_MIN_LIMIT 0
#define CDS_MAX_LIMIT 999

#define ABS(x) (x > 0 ? x : -x)

unsigned int cdsMin = CDS_MIN_DEFAULT;
unsigned int cdsMax = CDS_MAX_DEFAULT;

int getCdsMin(){
  return cdsMin;
}

void setCdsMin(unsigned int value){
  if(value > CDS_MAX_LIMIT) value = CDS_MAX_LIMIT;
  else if(value < CDS_MIN_LIMIT) value = CDS_MIN_LIMIT;
  cdsMin = value;
}

int getCdsMax(){
  return cdsMax;
}

void setCdsMax(unsigned int value){
  if(value > CDS_MAX_LIMIT) value = CDS_MAX_LIMIT;
  else if(value < CDS_MIN_LIMIT) value = CDS_MIN_LIMIT;
  cdsMax = value;
}

unsigned int getCdsValue(){
  static unsigned int cdsValue = analogRead(CDS_PIN);
  static unsigned int tempValue = analogRead(CDS_PIN);
  static unsigned long prevMillisSampling = millis();
  static unsigned long prevMillisUpdate = millis();
  unsigned int currValue = analogRead(CDS_PIN);
  unsigned long currMillis = millis();

  if(currMillis - prevMillisSampling > 100){
    cdsValue = 0.9*cdsValue + 0.1*currValue;
    if(cdsValue < cdsMin) cdsValue = cdsMin;
    else if(cdsValue > cdsMax) cdsValue = cdsMax;
    prevMillisSampling = currMillis;
  }

  if(ABS((int)(tempValue - cdsValue)) < 5){
    prevMillisUpdate = currMillis;
  }
  else if(currMillis - prevMillisUpdate > 2000){
    tempValue = cdsValue;
  }
  
  return tempValue;
}
