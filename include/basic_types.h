#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned char BYTE;//8 bits
typedef unsigned short WORD;//16 bits
typedef unsigned int DWORD;//32 bits

class PixelColor{

 public:
  PixelColor(){}

  PixelColor(BYTE a, BYTE b, BYTE c)
    :r_(a), g_(b), b_(c){
  }

  BYTE GetR()const{return r_;}
  BYTE GetG()const{return g_;}
  BYTE GetB()const{return b_;}
  
 private:
  BYTE r_, g_, b_;
};

const double eps=1e-10;

template<class T>
int Sign(T a){
  if (a>-eps) return a>eps;
  return -1;
}

#endif
