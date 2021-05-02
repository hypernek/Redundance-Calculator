
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "Redundance.h"


template<typename arrayNumberType>
arrayNumberType* calculateR_nArray(const BRepresentation& botBound, const BRepresentation& topBound)
{
	size_t maxWantedValue = topBound.toLongLong();
	size_t minWantedValue = botBound.toLongLong();
	size_t arraySize = maxWantedValue - minWantedValue + size_t(1);
	// estimate array size in RAM
	double arraySizeMB = arraySize * sizeof(arrayNumberType) * 0.000001;

	// watch out for minimum bound, so that we don't miss any representation of botBound
	// this needed because for general botBound there can exist representations 
	// that are radix smaller but larger in value than botBound
	// thus, starting at the representation botBound we would omit them
	// however if botbound is equal to B_k - 1 for some k, any radix smaller representation is also smaller in value
	// (see Lemma 3.4)
	size_t startValue;
	BRepresentation actualMinBound;
	if (minWantedValue != BRepresentation::getSequenceElement(botBound.getSize()) - 1)
	{
		startValue = BRepresentation::getSequenceElement(botBound.getSize() - 1) - 1;
		actualMinBound = BRepresentation(startValue);
	}
	else
	{
		startValue = minWantedValue;
		actualMinBound = botBound;
	}

	

	std::cout << "R(n) will be calculated on these values:" << std::endl;
	std::cout << "bottom bound B-representation: " << botBound << std::endl;
	std::cout << "top bound B-representation: " << topBound << std::endl;
	std::cout << "minimum wanted n (DEC): " << minWantedValue << std::endl;
	std::cout << "maximum wanted n (DEC): " << maxWantedValue << std::endl;
	std::cout << "minimum n we have to check (DEC): " << startValue << std::endl;
	std::cout << "B-representation of this minimum: " << actualMinBound << std::endl;
	std::cout << "array size: " << arraySize << " values ~ " << arraySizeMB << " MB of memory" << std::endl;

	// allocate and initialize counting array for R(n)
	arrayNumberType* RnArray = new arrayNumberType[arraySize];
	std::fill_n(RnArray, arraySize, 0);

	// initialize
	BRepresentation workRepresentation = actualMinBound;
	BRepresentation topBRepresentation = topBound;
	// count R(n) and report progress
	size_t counter = 0;
	int promile = 1;
	int percent = 0;
	// calculate period for reporting progress
	// note: # of representations != value of topBound - value of botBound
	/*std::cout << "topBound and botBound as " << BRepresentation::getAlphabetBiggestDigit() + 1 << "-ary representation: " << std::endl;
	std::cout << topBound.asStandardRepresentation() << ", " << botBound.asStandardRepresentation() << std::endl;*/
	size_t counter_period = (topBound.asStandardRepresentation() - botBound.asStandardRepresentation()) / 1000;
	//std::cout << "counter_period: " << counter_period << std::endl;

	long long value;
	while (workRepresentation <= topBRepresentation) // radix ordering here, not by value!!!
	{
		long long value = workRepresentation.toLongLong();
		// ignore representations with value outside our interval
		if (value >= minWantedValue && value <= maxWantedValue)
		{
			RnArray[value - minWantedValue] += 1;
		}
		workRepresentation.incrementRepresentationIgnoreGreedy();
		if (counter == counter_period)
		{
			promile += 1;
			if (promile == 10)
			{
				percent += 1;
				promile = 0;
			}
			std::cout << "\r" << "Progress: " << workRepresentation << " < " << topBRepresentation
				<< "     " << percent << "." << promile << " %         ";
			std::cout.flush();
			counter = 0;
		}
		else
			counter++;
	}

	std::cout << std::endl << "last representation reached: " << workRepresentation << std::endl;

	return RnArray;
}

template<typename arrayNumberType>
void writeR_nArrayToDisc(size_t arraySize, const BRepresentation& botBound, const arrayNumberType* RnArray, std::ofstream& RnArrayOutputFile)
{
	std::cout << "Starting write to output file" << std::endl;
	double percent = 0.0;
	long long counter = 0;
	long long counter_period = arraySize / 1000;
	long long minReachableValue = botBound.toLongLong();

	BRepresentation iBrepresentation = botBound;
	bool basisisF = BRepresentation::checkFProperty();

	// write to file with representations
	for (size_t i = 0; i < arraySize; i++) // this range is enough!
	{
		RnArrayOutputFile << i + minReachableValue << "," << iBrepresentation << "," << int(RnArray[i]) << std::endl;
		if (basisisF)
			// increment with normalisation (so that we don't construct a new representation every time)
			++iBrepresentation;
		else
			iBrepresentation = BRepresentation(i + minReachableValue + 1);
		if (counter == counter_period)
		{
			percent += 0.1;
			std::cout << "\r" << "writing to disc..." << percent << " %";
			std::cout.flush();
			counter = 0;
		}
		else
			counter++;
	}
	percent += 0.1;
	std::cout << "\r" << "writing to disc...100 %";
	std::cout.flush();

	std::cout << std::endl << "Done" << std::endl;
}

template<typename arrayNumberType>
void calculateR_nMaximaOneRow(int L, int outputNumberOfMaxima, size_t arraySize, const BRepresentation& botBound,
	const arrayNumberType* RnArray, std::ofstream& MaximaOutputFile)
{
	std::cout << "Starting counting maxima" << std::endl;
	double percent = 0.0;
	size_t counter_period = arraySize / 1000;
	size_t arrayStart = botBound.toLongLong();
	double arraySizeMB = arraySize * sizeof(arrayNumberType) * 0.000001;
	// prepare 
	int maximum = 1;
	int numberOfMaximaFound = 0;
	std::vector<BRepresentation> maximalRepresentations = {};

	// start counting maxima
	for (size_t i = 0; i < arraySize; i++)
	{
		arrayNumberType R_n = RnArray[i];
		if (R_n > maximum)
		{
			maximum = R_n;
			numberOfMaximaFound = 1;
			maximalRepresentations.clear();
			std::cout << std::endl << "New maximum found!!!: R(" << i + arrayStart << ") = " << maximum << std::endl;
			BRepresentation maximumRepresentation = BRepresentation(i + arrayStart);
			std::cout << "                        First representation with R(w) = " << maximum << " found : w = " << maximumRepresentation << std::endl;
			maximalRepresentations.push_back(maximumRepresentation);
		}
		else
		{
			if (R_n == maximum)
			{
				numberOfMaximaFound += 1;
				BRepresentation maximumRepresentation = BRepresentation(i + arrayStart);
				std::cout << std::endl << "                        Last representation with R(w) = " << maximum << " found : w = " << maximumRepresentation;
				// save only the first few maxima
				if (outputNumberOfMaxima > -1 && numberOfMaximaFound < outputNumberOfMaxima)
				{
					maximalRepresentations.push_back(maximumRepresentation);
				}
				else
				{
					if (outputNumberOfMaxima == -1)
						maximalRepresentations.push_back(maximumRepresentation);
				}
			}
		}
		if (counter_period == 0 || i % counter_period == 0)
		{
			percent += 0.1;
			std::cout << "\r" << "Progress: " << percent << " %   ";
			std::cout.flush();
		}
	}

	std::cout << std::endl << "Maxima of length " << L << " done" << std::endl;

	MaximaOutputFile << L << "," << maximum << "," << numberOfMaximaFound;
	for (int i = 0; i < maximalRepresentations.size(); i++)
	{
		MaximaOutputFile << "," << maximalRepresentations[i];
	}
	MaximaOutputFile << std::endl;

	BRepresentation::testSequence();
}

void calculateR_nToDiscOnBounds(BRepresentation botBound, BRepresentation topBound)
{
	if (!BRepresentation::getInited())
	{
		std::cout << "Cannot calculate redundancies, BRepresentation was not initialised!" << std::endl;
		return;
	}
	int L_MAX_CONFLUENT_A_EQUALS_B;
	if (BRepresentation::getBasisOrder() == 2)
		L_MAX_CONFLUENT_A_EQUALS_B = 24;
	else
		L_MAX_CONFLUENT_A_EQUALS_B = 16 * (BRepresentation::getBasisOrder() + 1);
	// for confl. systems with a > b however, the maxima are max R(l) = 2^(ceil(l/m) - 1) where l is length of representation
	// unsigned short fits 65536 = 2^16 - 1, so it is applicable only for L such that \ceil(L/m)-1 < 16
	int L_MAX_CONFLUENT_A_GREATER_THAN_B = 16 * BRepresentation::getBasisOrder();
	bool CONFLUENT_A_EQUALS_B = BRepresentation::checkConfluentAEqualB();
	bool CONFLUENT_A_GREATER_THAN_B = BRepresentation::checkConfluentAGreaterThanB();
	if (CONFLUENT_A_EQUALS_B)
	{
		std::cout << "INFO: Basis is confluent with a = b and order " << BRepresentation::getBasisOrder() << std::endl;
	}
	else if (CONFLUENT_A_GREATER_THAN_B)
	{
		std::cout << "INFO: Basis is confluent with a > b and order " << BRepresentation::getBasisOrder() << std::endl;
	}
	else
	{
		std::cout << "INFO: Basis is not confluent and has order " << BRepresentation::getBasisOrder() << std::endl;
	}
	int L = topBound.getSize();

	// allow even non-(F) bases
	bool basisisF = BRepresentation::checkFProperty();
	if (!basisisF)
	{
		BRepresentation::a = *std::max_element(BRepresentation::coefficients.begin(), BRepresentation::coefficients.end());
		std::cout << "INFO: Basis is not (F) " << std::endl;
	}
	// for non-(F) bases though the maximum coefficient won't be the maximal digit of the canonical alphabet, 
	// but usually larger

	// prepare output filename
	std::stringstream dataFilename;
	dataFilename << "R(n)_";
	for (int i = 0; i < BRepresentation::coefficients.size() - 1; i++)
		dataFilename << BRepresentation::coefficients[i] << ",";
	dataFilename << BRepresentation::coefficients[BRepresentation::coefficients.size() - 1];
	dataFilename << "_LENGTH_" << L;
	dataFilename << "_limits_" << botBound << "-" << topBound;
	dataFilename << ".csv";
	// create new file
	std::ofstream RedundanceData(dataFilename.str());
	RedundanceData << "n,n_B,R(n)" << std::endl; // write column titles

	// start clock
	auto start = std::chrono::steady_clock::now();
	auto end = std::chrono::steady_clock::now();
	auto R_n_time = std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
	size_t arraySize = topBound.toLongLong() - botBound.toLongLong() + size_t(1);
	// start counting R(n)
	// save RAM space based on coefficients of basis
	if (CONFLUENT_A_EQUALS_B && L < L_MAX_CONFLUENT_A_EQUALS_B)
	{
		arrayTypeAEqualsB* RnArray = calculateR_nArray<arrayTypeAEqualsB>(botBound, topBound);
		end = std::chrono::steady_clock::now();
		R_n_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Time needed for calculating R(n): " << R_n_time << " ms" << std::endl;

		start = std::chrono::steady_clock::now();
		writeR_nArrayToDisc(arraySize, botBound, RnArray, RedundanceData);
		delete[] RnArray;
	}
	else if (CONFLUENT_A_GREATER_THAN_B && L < L_MAX_CONFLUENT_A_GREATER_THAN_B)
	{
		arrayTypeAGreaterThanB* RnArray = calculateR_nArray<arrayTypeAGreaterThanB>(botBound, topBound);
		end = std::chrono::steady_clock::now();
		R_n_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Time needed for calculating R(n): " << R_n_time << " ms" << std::endl;

		start = std::chrono::steady_clock::now();
		writeR_nArrayToDisc(arraySize, botBound, RnArray, RedundanceData);
		delete[] RnArray;
	}
	else
	{
		arrayTypeOther* RnArray = calculateR_nArray<arrayTypeOther>(botBound, topBound);
		end = std::chrono::steady_clock::now();
		R_n_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Time needed for calculating R(n): " << R_n_time << " ms" << std::endl;

		start = std::chrono::steady_clock::now();
		writeR_nArrayToDisc(arraySize, botBound, RnArray, RedundanceData);
		delete[] RnArray;
	}
	end = std::chrono::steady_clock::now();
	auto discTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Time needed for writing to disc : " << discTime << " ms" << std::endl;
	RedundanceData << "R(n) time:, " << R_n_time << "ms, Disc Time:, " << discTime << "ms " << std::endl;
	// cleanup
	std::cout << "Saved data to " << dataFilename.str() << std::endl;
	RedundanceData.close();
}

void calculateR_nToDiscOnBounds(long long botBound, long long topBound)
{
	BRepresentation bot(botBound);
	BRepresentation top(topBound);
	calculateR_nToDiscOnBounds(bot, top);
}

void calculateR_nToDiscOnLengths(int representationLengthMin, int representationLengthMax)
{
	// calculate R(n) only for representations of a given length
	// pregenerate needed sequence elements
	BRepresentation::addElementsByIndex(representationLengthMax + 1);
	// prepare bounds 
	// (largest number of desired length & largest number of desired length-1)
	std::cout << std::endl << std::endl << "STARTING CALCULATION OF R(n) ON LENGTHS " << representationLengthMin << "-" << representationLengthMax << std::endl;
	BRepresentation botBound;
	if (representationLengthMin > 1)
		botBound = BRepresentation(BRepresentation::getSequenceElement(representationLengthMin - 1));
	else
		botBound = BRepresentation(0); // we count zero as representation of length 1
	BRepresentation topBound = BRepresentation(BRepresentation::getSequenceElement(representationLengthMax) - 1);
	calculateR_nToDiscOnBounds(botBound, topBound);
}


void calculateR_nMaxima(int representationLengthMin, int representationLengthMax, int outputNumberOfMaxima)
{
	if (!BRepresentation::getInited())
	{
		std::cout << "Cannot calculate maxima of R(n), BRepresentation was not initialised!" << std::endl;
		return;
	}

	int L_MAX_CONFLUENT_A_EQUALS_B;
	if (BRepresentation::getBasisOrder() == 2)
		L_MAX_CONFLUENT_A_EQUALS_B = 24;
	else
		L_MAX_CONFLUENT_A_EQUALS_B = 16 * (BRepresentation::getBasisOrder() + 1);
	// for confl. systems with a > b however, the maxima are max R(l) = 2^(ceil(l/m) - 1) where l is length of representation
	// unsigned short fits 65536 = 2^16 - 1, so it is applicable only for L such that \ceil(L/m)-1 < 16
	int L_MAX_CONFLUENT_A_GREATER_THAN_B = 16 * BRepresentation::getBasisOrder();
	bool CONFLUENT_A_EQUALS_B = BRepresentation::checkConfluentAEqualB();
	bool CONFLUENT_A_GREATER_THAN_B = BRepresentation::checkConfluentAGreaterThanB();
	if (CONFLUENT_A_EQUALS_B)
	{
		std::cout << "INFO: Basis is confluent with a = b and order " << BRepresentation::getBasisOrder() << std::endl;
	}
	else if (CONFLUENT_A_GREATER_THAN_B)
	{
		std::cout << "INFO: Basis is confluent with a > b and order " << BRepresentation::getBasisOrder() << std::endl;
	}
	else
	{
		std::cout << "INFO: Basis is not confluent and has order " << BRepresentation::getBasisOrder() << std::endl;
	}

	// pregenerate needed sequence elements
	BRepresentation::addElementsByIndex(representationLengthMax + 1);

	// prepare output files
	std::stringstream dataFilename;
	std::stringstream infoFilename;
	for (int i = 0; i < BRepresentation::coefficients.size() - 1; i++)
		dataFilename << BRepresentation::coefficients[i] << ",";
	dataFilename << BRepresentation::coefficients[BRepresentation::coefficients.size() - 1];
	dataFilename << "_maxima_L-" << representationLengthMin << "-" << representationLengthMax << ".csv";

	for (int i = 0; i < BRepresentation::coefficients.size() - 1; i++)
		infoFilename << BRepresentation::coefficients[i] << ",";
	infoFilename << BRepresentation::coefficients[BRepresentation::coefficients.size() - 1];
	infoFilename << "_maxima_L-" << representationLengthMin << "-" << representationLengthMax << "_info.csv";
	std::ofstream MaximaOutputFile(dataFilename.str());
	std::ofstream TimesOutputFile(infoFilename.str());

	// write column titles
	MaximaOutputFile << "length,max R(n),#argmax R(n),argmax R(n)" << std::endl; 
	TimesOutputFile << "length,array size,array size (MB),R(n) time (ms),Maxima time (ms)" << std::endl;

	for (int L = representationLengthMin; L <= representationLengthMax; L++)
	{
		// prepare bounds 
		// (largest number of desired length & largest number of desired length-1)
		std::cout << std::endl << std::endl << "STARTING R(n) ON LENGTH " << L <<
			" (TOTAL RANGE: "<< representationLengthMin << "-" << representationLengthMax << ")" << std::endl;
		BRepresentation topBound = BRepresentation(BRepresentation::getSequenceElement(L) - 1);
		BRepresentation botBound;
		if (L > 1)
			botBound = BRepresentation(BRepresentation::getSequenceElement(L - 1));
		else
			botBound = BRepresentation(0);
		size_t arraySize = topBound.toLongLong() - botBound.toLongLong() + size_t(1);

		// start clock
		auto start = std::chrono::steady_clock::now();

		// cheating to save RAM space based on coefficients of basis
		if (CONFLUENT_A_EQUALS_B && L < L_MAX_CONFLUENT_A_EQUALS_B)
		{
			arrayTypeAEqualsB * RnArray = calculateR_nArray<arrayTypeAEqualsB>(botBound, topBound);
			std::cout << RnArray[0];
			auto end = std::chrono::steady_clock::now();
			auto R_n_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "Time needed for calculating R(n): " << R_n_time << " ms" << std::endl;
			
			// restart clock
			start = std::chrono::steady_clock::now();
			calculateR_nMaximaOneRow(L, outputNumberOfMaxima, arraySize, botBound, RnArray, MaximaOutputFile);
			end = std::chrono::steady_clock::now();
			auto maximaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "Time needed for counting maxima : " << maximaTime << " ms" << std::endl;
			TimesOutputFile << L << "," << arraySize << "," << arraySize * sizeof(arrayTypeAEqualsB) * 0.000001 
				<< "," << R_n_time << "," << maximaTime << std::endl;
			delete[] RnArray;
		}
		else if (CONFLUENT_A_GREATER_THAN_B && L < L_MAX_CONFLUENT_A_GREATER_THAN_B)
		{
			arrayTypeAGreaterThanB* RnArray = calculateR_nArray<arrayTypeAGreaterThanB>(botBound, topBound);
			auto end = std::chrono::steady_clock::now();
			auto R_n_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "Time needed for calculating R(n): " << R_n_time << " ms" << std::endl;

			// restart clock
			start = std::chrono::steady_clock::now();
			calculateR_nMaximaOneRow(L, outputNumberOfMaxima, arraySize, botBound, RnArray, MaximaOutputFile);
			end = std::chrono::steady_clock::now();
			auto maximaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "Time needed for counting maxima : " << maximaTime << " ms" << std::endl;
			TimesOutputFile << L << "," << arraySize << "," << arraySize * sizeof(arrayTypeAGreaterThanB) * 0.000001
				<< "," << R_n_time << "," << maximaTime << std::endl;
			delete[] RnArray;
		}
		else
		{
			arrayTypeOther* RnArray = calculateR_nArray<arrayTypeOther>(botBound, topBound);
			auto end = std::chrono::steady_clock::now();
			auto R_n_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "Time needed for calculating R(n): " << R_n_time << " ms" << std::endl;

			// restart clock
			start = std::chrono::steady_clock::now();
			calculateR_nMaximaOneRow(L, outputNumberOfMaxima, arraySize, botBound, RnArray, MaximaOutputFile);
			end = std::chrono::steady_clock::now();
			auto maximaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "Time needed for counting maxima : " << maximaTime << " ms" << std::endl;
			TimesOutputFile << L << "," << arraySize << "," << arraySize * sizeof(arrayTypeOther) * 0.000001
				<< "," << R_n_time << "," << maximaTime << std::endl;
			delete[] RnArray;
		}
		
	}
	// clean up
	std::cout << "Data saved to " << dataFilename.str() << std::endl;
	std::cout << "Calculation information saved to " << infoFilename.str() << std::endl;
	MaximaOutputFile.close();
	TimesOutputFile.close();
	
}

