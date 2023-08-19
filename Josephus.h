#pragma once
#include <exception>
#include <sstream>
#include <fstream>

#include "CircularList.h"
#include "Soldier.h"

class FileException : std::exception {
public:
	explicit FileException(std::string msg) : std::exception(), msg{ msg } {}
	virtual const char* what() const noexcept { return msg.c_str(); }
private:
	std::string msg;
};

CircularList solveJosephusProblem(size_t numberOfSoldiers = 41, size_t numberOfSurvivors = 1, size_t executionStep = 2)
{
	CircularList soldiers{};
	soldiers.generateN(numberOfSoldiers, [n = 1]() mutable { return n++; });
	soldiers.removeEveryNUntilMRemains(executionStep, numberOfSurvivors);
	return soldiers;
}

void solveJosephusProblem(std::string filename, size_t numberOfSurvivors = 1, size_t executionStep = 2)
{
	CircularList soldiers{};
	size_t numberOfSoldiers{};
	if (soldiers.pushFromFile(filename)) {
		numberOfSoldiers = soldiers.size();
		soldiers = solveJosephusProblem(numberOfSoldiers, numberOfSurvivors, executionStep);
	}
	else {
		std::stringstream msg;
		msg << "File Exception: " << filename << " does not exist or invalid format.";
		throw FileException(msg.str());
	}

	std::stringstream outputFilename{};
	outputFilename << "solution_" << filename;
	std::ofstream outputFile(outputFilename.str(), std::ios::out | std::ios::trunc);

	if (!outputFile) {
		std::stringstream msg;
		msg << "File Exception: Unable to create " << outputFilename.str() << ".";
		throw FileException(msg.str());
	}

	std::stringstream headline;
	headline << "The Josephus Problem:" << std::endl
		<< "Initial parameters:" << std::endl
		<< "Number of soldiers: " << numberOfSoldiers << std::endl
		<< "Number of survivors: " << numberOfSurvivors << std::endl
		<< "Survivors:" << std::endl
		<< soldiers;

	if (!(outputFile << headline.str())) {
		outputFile.close();
		std::stringstream msg;
		msg << "File Exception: Error while writing to " << outputFilename.str() << ".";
		throw FileException(msg.str());
	}
	else {
		outputFile.close();
	}
}