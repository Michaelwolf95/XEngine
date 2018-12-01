#include <fstream>
#include <iostream>
#include <string>
#include "Analytics.h"

CSVMaker::CSVMaker()
{
	fullFilePath = CSV_FILE_PATH + "csvFile.csv";

	try
	{
		csvFile.open(fullFilePath);
		csvFile << "Object Number" << ", " << "Frame Rate" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem opening file." << std::endl;
	}
}

CSVMaker::CSVMaker(std::string filename)
{
	fullFilePath = CSV_FILE_PATH + filename + ".csv";

	try
	{
		csvFile.open(fullFilePath);
		csvFile << "Object Number" << ", " << "Frame Rate" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem opening file." << std::endl;
	}
}

CSVMaker::CSVMaker(std::string filename, std::string columnOne, std::string columnTwo)
{
	fullFilePath = CSV_FILE_PATH + filename + ".csv";

	try
	{
		csvFile.open(fullFilePath);
		csvFile << columnOne << ", " << columnTwo << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem opening file." << std::endl;
	}
}

void CSVMaker::Write(float valueOne, float valueTwo)
{
	try
	{
		if (csvFile.is_open())
		{
			csvFile << valueOne << ", " << valueTwo << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem writing to file." << std::endl;
	}
}

void CSVMaker::Close()
{
	try
	{
		csvFile.close();
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Problem closing file." << std::endl;
	}
}