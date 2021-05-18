
#include <vector>
#include <sstream>
#include <iostream>
#include "Brepresentation.h"
#include "Redundance.h"

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

		BRepresentation::init(Basis_coeffs);

		while (true)
		{
			std::string input_w;
			std::cout << std::endl <<  "Enter limits to which you want to count R(n) in the given basis,"
				<< std::endl << "Alternatively, enter * if you want to count R(n) by length of representation,"
				<< std::endl << "or enter % to reset and enter coefficients of a different basis" << std::endl;
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
				<< std::endl << "Alternatively, enter % if you want to reset and switch to a different basis" << std::endl;
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
					std::cout << "Enter highest number of maxima of R(n) to be saved to output file (-1 = unlimited):" << std::endl;
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
					}
				}			
			}
		}
	}
}

void countTimes()
{
	while (true)
	{
		bool Basis_ready = false;
		std::vector<digittype> Basis_coeffs(0);
		while (!Basis_ready)
		{
			std::cout << "R(n) CPU Time Benchmark" << std::endl;
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
			std::cout << "\nEnter lengths on which you want to count R(n) in the given basis"
				<< std::endl << "Alternatively, enter % if you want to reset and switch to a different basis" << std::endl;
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
					calculateR_nCPUTimes(minLength, maxLength);
				}
			}
		}
	}
}

int main()
{
	//test();	
	//testIncrement();
	//countRedundancy();
	//countMaxima();

	countTimes();
	
}


