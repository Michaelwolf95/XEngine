#pragma once

#include <fstream>
#include <string>

class CSVMaker
{
private:
	std::string CSV_FILE_PATH = "../Analytics/";
	std::string fullFilePath;
	std::ofstream csvFile;

public:
	CSVMaker();
	CSVMaker(std::string);
	CSVMaker(std::string, std::string, std::string);
	void Write(float, float);
	void Close();
};
