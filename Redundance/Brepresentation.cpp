#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "Brepresentation.h"

// STATIC VARIABLES' INITIALISATION
std::vector<digittype> BRepresentation::coefficients = {};
std::vector<sequencetype> BRepresentation::sequence = { (sequencetype)1 };
int BRepresentation::M = 0;
digittype BRepresentation::a = 0;
bool BRepresentation::inited = false;

// COPY, REALLOCATE, GREEDY REPRESENTATION
void BRepresentation::deepcopy(digittype * destdigits) const
{
	for (int i = 0; i < size; i++)
		destdigits[i] = digits[i];
}

void BRepresentation::expand(digittype expanddigit, bool forcedExpand)
{
	// expand only if needed
	if (digits[size - 1] == 0 && !forcedExpand)
		return;
	digittype* newdigits = new digittype[size + 1];
	deepcopy(newdigits);
	newdigits[size] = expanddigit; 
	delete[] digits;
	this->digits = newdigits;
	size += 1; 
}

void BRepresentation::constructRepresentation(sequencetype num)
{
	// add required elements for our representations
	addElementsByValue(num);
	// find maximum needed element
	// position of the first element not strictly smaller than num
	std::vector<sequencetype>::iterator it = std::lower_bound(sequence.begin(), sequence.end(), num);  
	int index = it - sequence.begin();
	// due to how std::lower_bound works we need the following condition
	// Case (#): it points to the first element not strictly smaller than num
	// example: Bn = 3Bn-1 + Bn-2, num = 42. Then
	//				index = 3
	//					\/
	//		1	4	13	43	142
	//		B0	B1	B2	B3	B4
	// however, to represent num = 42, only 3 spaces suffice, thus we allocate basistype[3]
	// and then lower the index for the construction of the representation: index--; => index = 2
	// Case (##): for num = 43 it will point to the same sequence element, because !(43 < 43)
	//				index = 3
	//					\/
	//		1	4	13	43	142
	//		B0	B1	B2	B3	B4
	// however, to represent 43 we need to allocate 4 spaces, i.e. 4 = index + 1 
	// and for the greedy algorithm we leave index = 3.
	if ((it != sequence.end() && num == *it) || num == 0)
	{
		// case (##) - i.e. when we exactly hit a sequence element
		// num = B_index
		digits = new digittype[index + 1];
		size = index + 1;
	}
	else
	{
		// case num < B_index (#) - num isn't equal to any sequence element
		digits = new digittype[index];
		size = index;
		index--; // shift
	}
	// construct greedy representation
	for (; num > 0 || index >= 0; --index)
	{
		// a_i = floor( A / B_i )
		digits[index] = num / sequence[index];
		// A = A - a_iB_i
		num = num % sequence[index];
	}
}

// BASIS MANAGEMENT
void BRepresentation::init(std::vector<digittype> coefs)
{
	if (!inited)
	{
		coefficients.insert(coefficients.end(), coefs.begin(), coefs.end()); 
		M = coefficients.size(); // basis order
		a = coefficients[0]; // largest alphabet digit ( a = t_1 )
		addElementsByIndex(M); // generate initial conditions
		inited = true;
	}
	else
	{
		std::cout << "cannot init: sequence was already initialized " << std::endl;
	}
}


void BRepresentation::init(std::initializer_list<digittype> coefs)
{
	std::vector<digittype> coefs_vect(0);
	coefs_vect.insert(coefs_vect.end(), coefs.begin(), coefs.end());
	BRepresentation::init(coefs_vect);
}

void BRepresentation::reset()
{
	if (inited)
	{
		coefficients.clear(); 
		sequence = { (sequencetype)1 };
		M = 0; 
		a = 0; 
		inited = false;
	}
	else
	{
		std::cout << "cannot reset: sequence wasn't initialized " << std::endl;
	}
}

int BRepresentation::addElementsByIndex(int maxElementIndex)
{
	int maxindex = sequence.size() - 1;
	for (; maxindex < maxElementIndex; maxindex++)
	{
		sequencetype novy = 0;
		// add 1 to initial conditions
		if (maxindex < M - 1)
			novy += 1;

		// generate from recurrence
		for (int i = 0; i < M && i < sequence.size(); i++)
			novy += coefficients[i] * sequence[sequence.size() - 1 - i];

		sequence.push_back(novy);
	}
	return maxindex;
}

int BRepresentation::addElementsByValue(sequencetype maxValue)
{
	// adds new elements if requested value is bigger than current highest sequence element
	// the actual threshold is a bit lower, but it's there to have some extra space
	// otherwise does nothing
	int maxindex = sequence.size() - 1;
	while (sequence[maxindex] <= maxValue)
	{
		maxindex = addElementsByIndex(maxindex + 1);
	}
	return maxindex;
}



bool BRepresentation::checkConfluentAEqualB()
{
	// check if basis is confluent with coefficients a,a,...,a,b and a==b
	for (int i = 0; i < coefficients.size() - 1; i++)
	{
		if (coefficients[i] != a)
			return false;
	}
	digittype b = coefficients[coefficients.size() - 1];
	if (b > a || b < 1)
	{
		return false;
	}
	else
	{
		if (b == a)
		{
			return true;
		}
	}
	return false;
}

bool BRepresentation::checkConfluentAGreaterThanB()
{
	// check if basis is confluent with coefficients a,a,...,a,b and a>b
	for (int i = 0; i < coefficients.size() - 1; i++)
	{
		if (coefficients[i] != a)
			return false;
	}
	digittype b = coefficients[coefficients.size() - 1];
	if (b > a || b < 1)
	{
		return false;
	}
	else
	{
		if (b == a)
		{
			return false;
		}
	}
	return true;
}

bool BRepresentation::checkFProperty()
{
	// check if basis satisfies (F) property, 
	// i.e. if coefficients satisfy t_1 >= t_2 >= ... >= t_m >= 1
	if (coefficients[0] < 1)
		return false;
	for (int i = 0; i < coefficients.size() - 1; i++)
	{
		if (coefficients[i] < coefficients[i + 1])
			return false;
	}
	if (coefficients[coefficients.size() - 1] < 1)
		return false;
	return true;
}

// TESTY
void BRepresentation::testSequence()
{
	using namespace std;
	if (!inited)
	{
		cout << "Basis wasn't initialized" << endl;
		return;
	}
	
	cout << "SEQUENCE TEST" << endl;
	cout << "Recurrence coefficients: " << endl;
	for (auto t : coefficients)
		cout << t << ", ";
	cout << endl << endl;
	cout << "Generated sequence elements: " << endl;
	for (auto b : sequence)
		cout << b << ", ";
	cout << endl << endl;
}

void BRepresentation::testNumbers()
{
	using namespace std;
	if (inited)
	{
		cout << "B-REPRESENTATION TEST" << endl;
		int test_values[] = { 1, 10, 1632, 44050 };
		std::vector<BRepresentation> cisla;
		for (int i : test_values)
		{
			BRepresentation cislo = BRepresentation(i);
			cisla.push_back(cislo);
			cout << i << "DEC = " << cislo << endl;
		}

		cout << cisla[0] << " + " << cisla[0] << " = " << cisla[0] + cisla[0] << endl;

		cout << cisla[1] << " + " << cisla[0] << " = " << cisla[1] + cisla[0] << endl;
		cout << cisla[2] << " + " << cisla[1] << " = " << cisla[2] + cisla[1] << endl;
		BRepresentation x = cisla[2] + cisla[2];
		cout << cisla[2] << " + " << cisla[2] << " = " << x << endl;
		BRepresentation y = x + cisla[2];
		cout << x << " + " << cisla[2] << " = " << y << endl;
		BRepresentation z = x + y;
		cout << x << " + " << y << " = " << z << endl;

		cout << "Enter a number: " << endl;
		int GGG = 10;
		int* cifry = new int[GGG];
		for (int i = 0; i < GGG; i++)
		{
			cifry[i] = (i * 7) % 10;
		}
			
		BRepresentation arbitrary = BRepresentation(GGG, cifry);
		arbitrary.normalise();
	}
	else
	{
		cout << "Cannot test B-representations, basis wasn't initialized" << endl;
	}
}

// CONSTRUCTORS
BRepresentation::BRepresentation(sequencetype num)
{
	constructRepresentation(num);
}

// COPY & MOVE
BRepresentation & BRepresentation::operator=(const BRepresentation & original)
{
	if (this != &original)
	{
		// free memory
		if (digits != nullptr)
			delete[] digits;
		size = original.size;
		digits = new digittype[size];
		original.deepcopy(digits);
	}
	return *this;
}

BRepresentation & BRepresentation::operator=(BRepresentation && original) noexcept
{
	if (this != &original)
	{
		// free memory
		if (digits != nullptr)
			delete[] digits;
		//size = 0;
		// assume ownership
		size = original.size;
		digits = original.digits;
		// deassign (free) RHS
		original.size = 0;
		original.digits = nullptr;
	}
	return *this;
}

// COMPARISON OPERATORS
bool BRepresentation::operator==(const BRepresentation & rhs) const
{
	if (size == rhs.size)
	{
		for (int i = 0; i < size; i++)
		{
			if (digits[i] != rhs.digits[i])
				return false;
		}
		return true;
	}
	return false;
}

bool BRepresentation::operator>(const BRepresentation & rhs) const
{
	// radix order
	if (size > rhs.size)
		return true;
	else
	{
		if (size == rhs.size)
		{
			for (int i = size - 1; i >= 0; i--) // from most significant to least significant
			{
				if (digits[i] > rhs.digits[i])
					return true;
				else
					if (digits[i] < rhs.digits[i])
						return false;
			}
			return false; // we reached the end, equality
		}
		return false; // size < rhs.size
	}
}

bool BRepresentation::operator>=(const BRepresentation & rhs) const
{
	// radix order
	if (size > rhs.size)
		return true;
	else
	{
		if (size == rhs.size)
		{
			for (int i = size - 1; i >= 0; i--) // from most significant to least significant
			{
				if (digits[i] > rhs.digits[i])
					return true;
				else
					if (digits[i] < rhs.digits[i])
						return false;
			}
			return true; // we reached the end, equality
		}
		return false; // size < rhs.size
	}
}

// UNARY ARITHMETIC OPERATIONS
BRepresentation & BRepresentation::operator+=(const BRepresentation & rhs)
{
	// add digit by digit and normalise
	int newsize = std::max(size, rhs.size);
	if (newsize > size)
	{
		digittype* newdigits = new digittype[newsize];
		int i = 0;
		for (; i < size; ++i)
			newdigits[i] = digits[i] + rhs.digits[i];
		for (; i < newsize; ++i)
			newdigits[i] = rhs.digits[i];
		delete[] digits;
		digits = newdigits;
		size = newsize;
	}
	else
	{
		int i = 0;
		for (; i < rhs.size; ++i)
			digits[i] = digits[i] + rhs.digits[i];
	}
	normalise();
	return *this;
}

BRepresentation & BRepresentation::operator++()
{
	digits[0] += 1;
	//if (digits[0] > a) 
		normalise(); 
	return *this;
}

BRepresentation & BRepresentation::operator*=(const unsigned & p)
{
	// multiply digit by digit and then normalise
	for (int i = 0; i < size; i++)
	{
		digits[i] = digits[i] * p;
	}
	normalise();
	return *this;
}

// NORMALISATION
void BRepresentation::normalise()
{
	// brute force normalisation algorithm (works in every (F) basis)
	using namespace std;
	int i = size - 1;
	bool normal = false;
	while (!normal)
	{
		for (; i >= 0; i--)
		{
			if (digits[i] >= a) // a = t_0 = coefficients[0]
			{
				// we found a potential start of a factor larger than the maximal one,
				// thus we check max. M - 1 more coefficient to see if they should be replaced
				int j = i;
				bool localOK = false;
				for (; i - j <= M - 1 && j >= 0; j--)
				{
					if (digits[j] < coefficients[i - j])
					{
						// representation is OK
						localOK = true;
						i = j; // catch up
						break;
					}
					else
					{
						if (digits[j] > coefficients[i - j])
						{
							// we found an index r = j - i, r \e {0,1,2,...,M-1}
							// such that a_(i+r) > t_r, not a greedy representation
							break;
						}
						else
						{
							if (i - j == M - 1 && digits[j] == coefficients[i - j])
								break;
							if (j == 0)
								localOK = true; // end
							
						}
					}
				}
				if (localOK == false)
				{
					carryNormalize(i, j);
					i = size;
				}
			}
		}
		normal = true;
	}
}

void BRepresentation::carryNormalize(int i, int j)
{
	using namespace std;
	if (i - j == M - 1)
	{
		// 1)
		// we checked M digits in total, the string satisfies d_i d_i-1 ... d_i-M+1 >= t_1 t_2 ... t_K, thus we can subtract
		if (i == size - 1) 
			expand(1);
		else
			digits[i + 1] += 1;
		for (int r = 0; r < M; r++)
			digits[i - r] -= coefficients[r];
	}
	else
	{
		if (i == size - 1) 
			expand(1);
		else
			digits[i + 1] += 1;
		int r = 0;
		// between i & j we can subtract straight away
		for (; r < i - j; r++)
		{
			digits[i - r] -= coefficients[r];
		}
		// after j we have to be more careful
		for (; r <= M - 1; r++)
		{
			if (i - r > 0 || r == M -1)
			{
				if (digits[i - r] >= coefficients[r])
				{
					// digit larger than coefficient, we subtract
					// if strictly larger, we move j for future use
					// so that we can borrow from the least significant digit possible
					if (digits[i - r] > coefficients[r])
					{
						j = i - r; 	
					}
					digits[i - r] -= coefficients[r];
				}
				else
				{
					// digit smaller than coefficient, we have to borrow from a more significant digit
					// we borrow from the closest digit that was strictly larger than a basis coefficient
					// (i.e. d_j > t_p + 1 for some p < r)
					digits[j] -= 1;
					int s = 0;
					// continue until r is reached
					for (; j - 1 - s < i - r; s++)
					{
						// add to already visited digits (between j & r)
						digits[j - 1 - s] += coefficients[s];
					}
					for (; r <= M - 1; s++, r++)
					{
						// from r further on add and subtract
						digits[j - 1 - s] = digits[j - 1 - s] - coefficients[r] + coefficients[s];
						if (j - 1 - s == 0)
						{
							// - 1 + 1 = 0, initial conditions cancel out, 
							// thus we don't add anything extra, except for the case r == M - 1
							// this condition is sufficient, the case s == r cannot occur, s < r always
							if (r == M - 1) 
								digits[0] += 1;
							return;
						}
					}
					for (; s <= M - 1; s++)
					{
						// here we "exhausted" r: r == M - 1, 
						// so we only add
						digits[j - 1 - s] += coefficients[s];
						if (j - 1 - s == 0 && s < M - 1)
						{
							// if we reached the digit at B_0 with s < M - 1, we need to add 1 due to initial conditions
							digits[0] += 1;
							return;
						}
					}
					return;
				}
			}
			else
			{
				// we know: i - r == 0 && r < M - 1
				if (digits[i - r] >= coefficients[r] + 1)
				{
					digits[i - r] = digits[i - r] - coefficients[r] - 1;
					return;
				}
				else
				{
					// we reached the digit at B_0 but it is smaller than t_r + 1, we need to borrow
					// we borrow from the closest digit that was strictly larger than a basis coefficient
					// (i.e. d_j > t_p + 1 for some p < r)
					digits[j] -= 1;
					int s = 0;
					// continue until r is reached
					for (; j - 1 - s < i - r; s++)
					{
						// add to already visited digits (between j & r)
						digits[j - 1 - s] += coefficients[s];
					}
					// subtract and add
					digits[j - 1 - s] = digits[j - 1 - s] - coefficients[r] + coefficients[s];
					if (j - 1 - s == 0)
					{
						// - 1 + 1 = 0, initial conditions cancel out
						return;
					}
					return;
				}
			}
			
		}
	}
}

void BRepresentation::incrementRepresentationIgnoreGreedy()
{
	// increment this representation to the closest radix greater one 
	// (ignoring greediness/ as if it was a standard b-ary representation)
	for (unsigned i = 0; i < size; i++)
	{
		if (digits[i] < a)
		{
			digits[i] += 1;
			return;
		}
		else
		{
			// carry over to bigger digit
			digits[i] -= a;
		}
	}
	// if we get here we just went over a string of all ds and substracted a, we have to expand
	// dd...dd -> 00...001
	//std::cout << "expanding representation";
	expand(1, true);
}


// KONVERZE
int BRepresentation::toInt() const
{
	int result = 0;
	for (int i = 0; i < size; i++)
	{
		result += digits[i] * sequence[i];
	}
	return result;
}

long long BRepresentation::toLongLong() const
{
	long long result = 0;
	for (int i = 0; i < size; i++)
	{
		result += digits[i] * sequence[i];
	}
	return result;
}

long long BRepresentation::asStandardRepresentation() const
{
	// return a number as if this was a standard representation in base a
	long long result = 0;
	// (F) system has the same digits as the a+1-ary system
	long long base = a + 1; 
	int i = size - 1;
	result += digits[i];
	for (; i > 0; i--)
	{
		result = result * base;
		result += digits[i];
	}
	return result;
}

// PRINTING
char BRepresentation::digitToChar(digittype digit)
{
	// conversion for readability (for digits < 62)
	// so that they take up only one space (character)
	if (digit < 10)
		return '0' + digit;
	else if (digit < 36)
		return 'A' - 10 + digit;
	else
		return 'a' - 10 - 26 + digit;
}

digittype BRepresentation::charToDigit(char c)
{
	// conversion for input
	if (isdigit(c))
		return digittype(c - '0');
	else if (isupper(c))
		return digittype(c - 'A' + 10);
	else if (islower(c))
		return digittype(c - 'a' + 10 + 26);
}

std::string BRepresentation::toString() const
{
	// print in correct order (most significant digit first)
	std::stringstream sstream;
	for (int i = size - 1 ; i >= 0; i--) 
		sstream << digitToChar(digits[i]);
	return sstream.str();
}

std::ostream & operator<<(std::ostream & out, const BRepresentation& x)
{
	out << x.toString();
	return out;
}

// BINARY ARITHMETIC
BRepresentation operator+(const BRepresentation & a, const BRepresentation & b)
{
	BRepresentation c = a;
	c += b;
	return c;
}

BRepresentation operator*(const BRepresentation & a, const unsigned & p)
{
	BRepresentation c = a;
	c *= p;
	return c;
}




