#ifndef BREPRESENTATION_H
#define BREPRESENTATION_H

#include <string>
#include <ostream>
#include <vector>
#include <initializer_list>

typedef int digittype;
typedef size_t sequencetype;

class BRepresentation
{
private:
	// DATA
	unsigned size;
	digittype * digits;

	// STATIC VARIABLES
	static std::vector<digittype> coefficients; // recurrence coefficients
	static std::vector<sequencetype> sequence; // basis
	static int M; // basis/recurrence order
	static digittype a; // largest digit of alphabet
	static bool inited;

	// COPY, REALLOCATE, GREEDY REPRESENTATION, NORMALISATION
	void deepcopy(digittype* destdigits) const;  // copy representation
	void expand(digittype expanddigit = 0, bool forcedExpand = false); // reallocate to array 1 digit larger 
	void constructRepresentation(sequencetype num); // construct initial greedy representation	
	void carryNormalize(int i, int j);

public:
	// BASIS MANAGEMENT
	static void init(std::vector<digittype> coefs);
	static void init(std::initializer_list<digittype> coefs); 
	static void reset();
	static bool getInited() { return inited; };
	static digittype getAlphabetBiggestDigit() { return a; };
	static int getBasisOrder() { return M; };

	static const sequencetype getBasisElement(int index) { return sequence[index]; };
	static int addElementsByIndex(int maxElementIndex);
	static int addElementsByValue(sequencetype maxValue);
	
	// GET SOME INFO ABOUT COEFFICIENTS
	static bool checkConfluentAEqualB();
	static bool checkConfluentAGreaterThanB();
	static bool checkFProperty();

	// TESTS
	static void testSequence();
	static void testNumbers();

	// CONSTRUCTORS, DESTRUCTOR
	BRepresentation() : size(1), digits(new digittype(0)) {};
	BRepresentation(unsigned size, digittype* digits) : size(size), digits(digits) {};
	BRepresentation(sequencetype num);
	~BRepresentation() { size = 0; delete[] digits; };

	// COPY & MOVE
	BRepresentation(const BRepresentation& original) : size(0), digits(nullptr) { *this = original; };
	BRepresentation(BRepresentation&& original) noexcept : size(0), digits(nullptr)  { *this = std::move(original); };
	BRepresentation& operator=(const BRepresentation& original);
	BRepresentation& operator=(BRepresentation&& original) noexcept;
	
	// COMPARISON
	bool operator==(const BRepresentation &rhs) const;
	bool operator!=(const BRepresentation &rhs) const { return !(*this == rhs); };
	bool operator>(const BRepresentation &rhs) const;
	bool operator>=(const BRepresentation &rhs) const;
	bool operator<(const BRepresentation &rhs) const { return !(*this >= rhs); };
	bool operator<=(const BRepresentation &rhs) const { return !(*this > rhs); };

	// UNARY ARITHMETIC
	BRepresentation& operator+=(const BRepresentation& rhs);
	BRepresentation& operator++();
	BRepresentation& operator*=(const unsigned& p);

	// (NON)NORMALISATION
	void normalise();
	void incrementRepresentationIgnoreGreedy();

	// CONVERSION TO INTEGER TYPES
	int toInt() const;
	long long toLongLong() const;
	long long asStandardRepresentation() const;

	unsigned getSize() const { return size; };

	// PRINTING
	static char digitToChar(digittype digit);
	static digittype charToDigit(char c);
	std::string toString() const;
	friend std::ostream& operator<<(std::ostream& out, const BRepresentation& x);

	// CALCULATE R(n)
	friend void calculateR(BRepresentation topBound, BRepresentation botBound);
	friend void calculateR_nToDiscOnBounds(BRepresentation botBound, BRepresentation topBound);
	friend void calculateR_nMaxima(int representationLengthMin, int representationLengthMax, int outputNumberOfMaxima);
	friend void calculateR_nCPUTimes(int representationLengthMin, int representationLengthMax);
};

// BINARY ARITHMETIC
BRepresentation operator+(const BRepresentation& a, const BRepresentation& b);
//BRepresentation& operator-(const BRepresentation& a, const BRepresentation& b);
BRepresentation operator*(const BRepresentation& a, const unsigned& p);
//BRepresentation operator*(const unsigned& p, const BRepresentation& a) { return a * p; };

#endif // NSTDINT_H