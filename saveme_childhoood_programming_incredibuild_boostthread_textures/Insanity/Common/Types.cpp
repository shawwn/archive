#include "Types.h"
#include <math.h>

static const double ShortConvert = 65536 / (2 * 3.141592653);
static const double ShortInvert = (2 * 3.141592653) / 65536;
/*
void CUVECTOR::Compress(const NxVec3 &V)
{
	theta = (short)(atan2(V.z,V.x) * ShortConvert);
	phi = (short)(acos(V.y) * ShortConvert);
}

NxVec3 CUVECTOR::Decompress() const
{
	double thetachanged = theta * ShortInvert;
	return NxVec3((NxReal)cos(thetachanged),(NxReal)cos(phi * ShortInvert),(NxReal)sin(thetachanged));
}*/