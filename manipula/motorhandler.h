


class MotorHandler{
public:
  int pwmtarget = 0;
  int pwmcurrent = 0;
  bool orientation = 1;

  int pwmchangefactor = 1;
  int pwmtargetchangefactor = 10;

  MotorHandler(int m_pwmpinforward,int m_pwmpinbackward)
  {
    pwmpinforward = m_pwmpinforward;
    pwmpinbackward = m_pwmpinbackward;
    pinMode(pwmpinforward,OUTPUT);
    pinMode(pwmpinbackward,OUTPUT);
  }

  void adjustrotation()//inloop iterational pwm adjusting
  { 
    if(pwmcurrent > pwmtarget)
    {
      pwmcurrent -= pwmchangefactor;
      if(pwmcurrent >= 0)
      {
        analogWrite(pwmpinforward,abs(pwmcurrent));
        analogWrite(pwmpinbackward,0);
      }
      else
      {
        analogWrite(pwmpinforward,0);
        analogWrite(pwmpinbackward,abs(pwmcurrent));        
      }      
    }  
    else if(pwmcurrent < pwmtarget)
    {
      pwmcurrent += pwmchangefactor;
      if(pwmcurrent >= 0)
      {
        analogWrite(pwmpinforward,abs(pwmcurrent));
        analogWrite(pwmpinbackward,0);
      }
      else
      {
        analogWrite(pwmpinforward,0);
        analogWrite(pwmpinbackward,abs(pwmcurrent));        
      }
    }
  }

  void increasePWM()
  {
    if(abs(pwmtarget) <= 240)
    {
      if(orientation)
        pwmtarget += pwmtargetchangefactor;
      else
        pwmtarget -= pwmtargetchangefactor;
    }
    
  }
  void decreasePWM()
  {
     if(abs(pwmtarget) > 0)
    {
      if(orientation)
        pwmtarget -= pwmtargetchangefactor;
      else
        pwmtarget += pwmtargetchangefactor;
    }
  }
  void switchPWM()
  {
    pwmtarget = (-1)*pwmtarget;
    orientation = !orientation;
  }


private:
  int pwmpinforward;
  int pwmpinbackward;

 
};