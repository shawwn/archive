#ifndef MACROS_H
#define MACROS_H

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef min
template<class T>
inline T min(T num1, T num2) { return (num1 < num2 ? num1 : num2); }
#endif

#ifndef max
template<class T>
inline T max(T num1, T num2) { return (num1 > num2 ? num1 : num2); }
#endif

#define NOMINMAX

template<class T>
inline T clamp(T High, T Low, T Clamping) { Clamping = (Clamping < Low ? Low : Clamping); Clamping = (Clamping > High ? High : Clamping); return Clamping; }

#endif