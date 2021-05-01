#ifndef REDUNDANCE_H
#define REDUNDANCE_H

#include "Brepresentation.h"


// save space in RAM - R(n) is usually very small for systems with a=b (but only up to length 23 for m=2)
typedef unsigned char arrayTypeAEqualsB;
typedef unsigned short arrayTypeAGreaterThanB;
typedef unsigned int arrayTypeOther;
// we choose the type to use at runtime

// split work and reuse code
template<typename arrayNumberType>
arrayNumberType* calculateR_nArray(const BRepresentation& botBound, const BRepresentation& topBound);
template<typename arrayNumberType>
void writeR_nArrayToDisc(size_t arraySize, const BRepresentation& botBound,
	const arrayNumberType* RnArray, std::ofstream& RnArrayOutputFile);
template<typename arrayNumberType>
void calculateR_nMaximaOneRow(int L, int outputNumberOfMaxima, size_t arraySize, const BRepresentation& botBound,
	const arrayNumberType* RnArray, std::ofstream& MaximaOutputFile);

// calculate the value of R(n) on a chosen interval and output to csv file
void calculateR_nToDiscOnBounds(BRepresentation botBound, BRepresentation topBound);
void calculateR_nToDiscOnBounds(long long botBound, long long topBound);
void calculateR_nToDiscOnLengths(int representationLengthMin, int representationLengthMax);

// calculate maxima of R(n) straight away without saving R(n) to disc
void calculateR_nMaxima(int representationLengthMin, int representationLengthMax, int outputNumberOfMaxima);




#endif // REDUNDANCE_H