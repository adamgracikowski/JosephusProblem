#pragma once
#include "CircularList.h"

#include <exception>
#include <sstream>
#include <fstream>

//--------------------------------------------

class FileException : std::exception
{
public:
	explicit FileException(std::string msg) : std::exception(), msg{ msg } {}
	virtual const char* what() const noexcept { return msg.c_str(); }
private:
	std::string msg;
};

//--------------------------------------------

template<typename T>
class JosephusProblemSolver
{
public:
	JosephusProblemSolver() = default;

	CircularList<T> operator()(size_t numberOfSoldiers = 41,
		size_t numberOfSurvivors = 1,
		size_t executionStep = 2) const;
	CircularList<T> operator()(std::string filename,
		size_t numberOfSurvivors = 1,
		size_t executionStep = 2) const;
};

//--------------------------------------------

template<typename T>
CircularList<T> JosephusProblemSolver<T>::operator()(size_t numberOfSoldiers,
	size_t numberOfSurvivors,
	size_t executionStep) const
{
	CircularList<T> cl{};
	cl.generateN(numberOfSoldiers, [n = 1]() mutable { return n++; });
	cl.removeEveryNUntilMRemains(executionStep, numberOfSurvivors);
	return cl;
}

template<typename T>
CircularList<T> JosephusProblemSolver<T>::operator()(std::string filename,
	size_t numberOfSurvivors,
	size_t executionStep) const
{
	CircularList<T> cl{};
	size_t numberOfSoldiers{};
	if (cl.pushFromFile(filename)) {
		numberOfSoldiers = cl.size();
		cl.removeEveryNUntilMRemains(executionStep, numberOfSurvivors);
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
		<< "Execution step: " << executionStep << std::endl;
	<< "Survivors:" << std::endl;
	cl.print(headline, "\n");

	if (!(outputFile << headline.str())) {
		outputFile.close();
		std::stringstream msg;
		msg << "File Exception: Error while writing to " << outputFilename.str() << ".";
		throw FileException(msg.str());
	}
	else {
		outputFile.close();
	}
	return cl;
}
//--------------------------------------------
