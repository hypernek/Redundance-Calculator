
#include <vector>
#include <sstream>
#include <iostream>
#include "Brepresentation.h"
#include "Redundance.h"

void test()
{
	BRepresentation::init({ 5, 4, 3, 3 });
	BRepresentation::addElementsByIndex(10);
	BRepresentation::testSequence();
	BRepresentation::testNumbers();

	BRepresentation::reset();

	BRepresentation::init({ 2, 1 });
	BRepresentation::testSequence();
	BRepresentation::testNumbers();

	BRepresentation::reset();
	BRepresentation::init({ 4, 1 });
	BRepresentation::testSequence();

	digittype * digits = new digittype[8]{ 4,0,6,0,4,2,6,1 };
	BRepresentation w(8, digits);
	w.normalise();

	std::cout << w << " * 5 = " << w * 5 << std::endl;
}

bool check_Confluent(std::vector<digittype> coeffs)
{
	digittype a = coeffs[0];
	if (a < 1)
		return false;
	for (int i = 0; i < coeffs.size()-1; i++)
	{
		if (coeffs[i] != a)
			return false;
	}
	if (coeffs[coeffs.size() - 1] > a || coeffs[coeffs.size() - 1] < 1)
	{
		return false;
	}
	return true;
}

bool check_F(std::vector<digittype> coeffs)
{
	// check (F) property of basis, i. e.
	// check whether basis coefficients satisfy
	// t_1 >= t_2 >= ... >= t_m >= 1.
	if (coeffs[0] < 1)
		return false;
	for (int i = 0; i < coeffs.size() - 1; i++)
	{
		if (coeffs[i] < coeffs[i+1])
			return false;
	}
	if (coeffs[coeffs.size() - 1] < 1)
		return false;
	return true;
}

BRepresentation constructRepresentation(std::string input_word)
{
	// check if it is digits (0-9, A-Z, a-z allowed)
	for (int i = 0; i < input_word.length(); i++)
	{
		if (!isalnum(input_word[i]))
		{
			std::cout << "Invalid input, " << input_word[i] << " is not a digit, returning zero" << std::endl;
			return BRepresentation();
		}
	}
	// proceed with normalisation
	digittype *digits = new digittype[input_word.length()];
	for (int i = 0; i < input_word.length(); i++)
	{
		digits[i] = BRepresentation::charToDigit(input_word[i]);
		std::cout << "Next digit: " << input_word[i] << " = " << digits[i] << std::endl;
	}

	return BRepresentation(input_word.length(), digits);
}

void normaliseInput(std::string input_word)
{
	// check if it is digits (0-9, A-Z, a-z allowed)
	for (int i = 0; i < input_word.length(); i++)
	{
		if (!isalnum(input_word[i]))
		{
			std::cout << "Invalid input, " << input_word[i] << " is not a digit" << std::endl;
			return;
		}
	}
	digittype *digits = new digittype[input_word.length()];
	for (int i = 0; i < input_word.length(); i++)
	{
		digits[i] = BRepresentation::charToDigit(input_word[i]);
		std::cout << "Next digit: " << input_word[i] << " = " << digits[i] << std::endl;
	}
	// proceed with normalisation
	BRepresentation x(input_word.length(), digits);
	x.normalise();
}

void acceptInput()
{
	while (true)
	{
		bool Basis_ready = false;
		std::vector<digittype> Basis_coeffs(0);
		while (!Basis_ready)
		{
			std::cout << "\nPlease input coefficients of basis, separated by comma: " << std::endl;
			std::string coeffs;
			std::getline(std::cin, coeffs);
			std::stringstream ss(coeffs);

			while (ss.good())
			{
				std::string coef;
				std::getline(ss, coef, ','); // split off next coefficient
				bool coef_good = true;
				std::cout << "coef: " << coef << std::endl;
				// check if it is really a number
				for (int i = 0; i < coef.length(); i++)
				{
					if (!isdigit(coef[i]))
					{
						std::cout << "Invalid input, " << coef[i] << " is not a digit" << std::endl;
						coef_good = false;
						break;
					}
				}

				if (coef_good)
				{
					digittype coefficient = std::stoi(coef);
					Basis_coeffs.push_back(coefficient);
				}
				else
				{
					break;
				}
			}
			if (!ss.good() && !Basis_coeffs.empty())
			{
				Basis_ready = true;
				std::cout << "Proceeding with coefficients: " << Basis_coeffs[0];
				for (int i = 1; i < Basis_coeffs.size(); i++)
				{
					std::cout << ", " << Basis_coeffs[i];
				}
				std::cout << std::endl;
			}
		}

		bool is_F = check_F(Basis_coeffs);
		if (!is_F)
		{
			std::cout << "Basis is NOT (F). Please enter an (F) basis." << std::endl;
			continue;
		}
		std::cout << "Basis satisfies the (F) property. Proceeding with construction of basis..." << std::endl;

		BRepresentation::init(Basis_coeffs);

		while (true)
		{
			std::string input_w;
			std::cout << "\nEnter next word to normalise: (allowed digits: 0-9, A-Z, a-z), enter % to start over with a new basis" << std::endl;
			std::getline(std::cin, input_w);
			if (input_w == "%")
			{
				BRepresentation::reset();
				break;
			}
			else
			{
				normaliseInput(input_w);
			}
		}
	}	
}

void testIncrement()
{
	while (true)
	{
		bool Basis_ready = false;
		std::vector<digittype> Basis_coeffs(0);
		while (!Basis_ready)
		{
			std::cout << "\nPlease input coefficients of basis, separated by comma: " << std::endl;
			std::string coeffs;
			std::getline(std::cin, coeffs);
			std::stringstream ss(coeffs);

			while (ss.good())
			{
				std::string coef;
				std::getline(ss, coef, ','); // split off next coefficient
				bool coef_good = true;
				std::cout << "coef: " << coef << std::endl;
				// check if it is really a number
				for (int i = 0; i < coef.length(); i++)
				{
					if (!isdigit(coef[i]))
					{
						std::cout << "Invalid input, " << coef[i] << " is not a digit" << std::endl;
						coef_good = false;
						break;
					}
				}

				if (coef_good)
				{
					digittype coefficient = std::stoi(coef);
					Basis_coeffs.push_back(coefficient);
				}
				else
				{
					break;
				}
			}
			if (!ss.good() && !Basis_coeffs.empty())
			{
				Basis_ready = true;
				std::cout << "Proceeding with coefficients: " << Basis_coeffs[0];
				for (int i = 1; i < Basis_coeffs.size(); i++)
				{
					std::cout << ", " << Basis_coeffs[i];
				}
				std::cout << std::endl;
			}
		}

		//bool is_F = check_F(Basis_coeffs);
		//if (!is_F)
		//{
		//	std::cout << "Basis is NOT (F). Please enter an (F) basis." << std::endl;
		//	continue;
		//}
		//std::cout << "Basis satisfies the (F) property. Proceeding with construction of basis..." << std::endl;

		BRepresentation::init(Basis_coeffs);

		while (true)
		{
			std::string input_w;
			std::cout << "\nEnter next word to start incrementing: (allowed digits: 0-9, A-Z, a-z), enter % to start over with a new basis" << std::endl;
			std::getline(std::cin, input_w);
			if (input_w == "%")
			{
				BRepresentation::reset();
				break;
			}
			else
			{
				BRepresentation x = constructRepresentation(input_w);
				std::cout << "Press the ENTER key to increment, enter anything else to start incrementing some differnet number";
				while (true)
				{
					if (std::cin.get() == '\n')
					{
						x.incrementRepresentationIgnoreGreedy();
						std::cout << x;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

void countRedundancy()
{
	while (true)
	{
		bool Basis_ready = false;
		std::vector<digittype> Basis_coeffs(0);
		while (!Basis_ready)
		{
			std::cout << "R(n) TO DISC CALCULATOR" << std::endl;
			std::cout << "\nPlease input coefficients of basis, separated by comma: " << std::endl;
			std::string coeffs;
			std::getline(std::cin, coeffs);
			std::stringstream ss(coeffs);

			while (ss.good())
			{
				std::string coef;
				std::getline(ss, coef, ','); // split off next coefficient
				bool coef_good = true;
				std::cout << "coef: " << coef << std::endl;
				// check if it is really a number
				for (int i = 0; i < coef.length(); i++)
				{
					if (!isdigit(coef[i]))
					{
						std::cout << "Invalid input, " << coef[i] << " is not a digit" << std::endl;
						coef_good = false;
						break;
					}
				}

				if (coef_good)
				{
					digittype coefficient = std::stoi(coef);
					Basis_coeffs.push_back(coefficient);
				}
				else
				{
					break;
				}
			}
			if (!ss.good() && !Basis_coeffs.empty())
			{
				Basis_ready = true;
				std::cout << "Proceeding with coefficients: " << Basis_coeffs[0];
				for (int i = 1; i < Basis_coeffs.size(); i++)
				{
					std::cout << ", " << Basis_coeffs[i];
				}
				std::cout << std::endl;
			}
		}

		//bool is_F = check_F(Basis_coeffs);
		//if (!is_F)
		//{
		//	std::cout << "Basis is NOT (F). Please enter an (F) basis." << std::endl;
		//	continue;
		//}
		//std::cout << "Basis satisfies the (F) property. Proceeding with construction of basis..." << std::endl;

		BRepresentation::init(Basis_coeffs);

		while (true)
		{
			std::string input_w;
			std::cout << std::endl <<  "Enter limits to which you want to count R(n) in the given basis,"
				<< std::endl << "Alternatively, enter * if you want to count R(n) by length of representation,"
				<< std::endl << "Enter % to change to switch to a different basis" << std::endl;
			std::cout << "Enter bottom limit:" << std::endl;
			std::getline(std::cin, input_w);
			if (input_w == "%")
			{
				BRepresentation::reset();
				break;
			}
			if(input_w == "*")
			{
				input_w.clear();
				std::cout << "Enter minimal length:" << std::endl;
				std::getline(std::cin, input_w);
				if (input_w == "%")
				{
					BRepresentation::reset();
					break;
				}
				else
				{
					// parse input
					std::stringstream ss(input_w);
					int minLength, maxLength, maxNumberOfMaxima;
					ss >> minLength;

					input_w.clear();
					std::cout << "Enter maximal length:" << std::endl;
					std::getline(std::cin, input_w);
					if (input_w == "%")
					{
						BRepresentation::reset();
						break;
					}
					else
					{
						// parse input
						std::stringstream ss(input_w);
						ss >> maxLength;
						BRepresentation::testSequence();
						calculateR_nToDiscOnLengths(minLength, maxLength);
					}
				}
			}
			else
			{
				// parse input
				std::stringstream ss(input_w);
				long long botBound, topBound;
				ss >> botBound;

				input_w.clear();
				std::cout << "Enter top limit:" << std::endl;
				std::getline(std::cin, input_w);
				if (input_w == "%")
				{
					BRepresentation::reset();
					break;
				}
				else
				{
					// parse input
					std::stringstream ss(input_w);
					ss >> topBound;
				}

				BRepresentation::testSequence();
				calculateR_nToDiscOnBounds(botBound, topBound);
			}
		}
	}
}

void countMaxima()
{
	while (true)
	{
		bool Basis_ready = false;
		std::vector<digittype> Basis_coeffs(0);
		while (!Basis_ready)
		{
			std::cout << "MAXIMA OF R(n) CALCULATOR" << std::endl;
			std::cout << "\nPlease input coefficients of basis, separated by comma: " << std::endl;
			std::string coeffs;
			std::getline(std::cin, coeffs);
			std::stringstream ss(coeffs);

			while (ss.good())
			{
				std::string coef;
				std::getline(ss, coef, ','); // split off next coefficient
				bool coef_good = true;
				std::cout << "coef: " << coef << std::endl;
				// check if it is really a number
				for (int i = 0; i < coef.length(); i++)
				{
					if (!isdigit(coef[i]))
					{
						std::cout << "Invalid input, " << coef[i] << " is not a digit" << std::endl;
						coef_good = false;
						break;
					}
				}

				if (coef_good)
				{
					digittype coefficient = std::stoi(coef);
					Basis_coeffs.push_back(coefficient);
				}
				else
				{
					break;
				}
			}
			if (!ss.good() && !Basis_coeffs.empty())
			{
				Basis_ready = true;
				std::cout << "Proceeding with coefficients: " << Basis_coeffs[0];
				for (int i = 1; i < Basis_coeffs.size(); i++)
				{
					std::cout << ", " << Basis_coeffs[i];
				}
				std::cout << std::endl;
			}
		}

		BRepresentation::init(Basis_coeffs);

		while (true)
		{
			std::string input_w;
			std::cout << "\nEnter lengths on which you want to count maxima of R(n) in the given basis"
				<< std::endl << "Alternatively, enter % if you want to switch to a different basis" << std::endl;
			std::cout << "Enter minimal length:" << std::endl;
			std::getline(std::cin, input_w);
			if (input_w == "%")
			{
				BRepresentation::reset();
				break;
			}
			else
			{
				// parse input
				std::stringstream ss(input_w);
				int minLength, maxLength, maxNumberOfMaxima;
				ss >> minLength;

				input_w.clear();
				std::cout << "Enter maximal length:" << std::endl;
				std::getline(std::cin, input_w);
				if (input_w == "%")
				{
					BRepresentation::reset();
					break;
				}
				else
				{
					// parse input
					std::stringstream ss(input_w);
					ss >> maxLength;

					input_w.clear();
					std::cout << "Enter maximum number of maxima saved to output file (-1 = unlimited):" << std::endl;
					std::getline(std::cin, input_w);
					if (input_w == "%")
					{
						BRepresentation::reset();
						break;
					}
					else
					{
						// parse input
						std::stringstream ss(input_w);
						ss >> maxNumberOfMaxima;
						BRepresentation::testSequence();
						calculateR_nMaxima(minLength, maxLength, maxNumberOfMaxima);
						BRepresentation::testSequence();
					}
				}			
			}
		}
	}
}

int main()
{
	//test();	
	//testIncrement();
	countRedundancy();
	//countMaxima();
	
}


