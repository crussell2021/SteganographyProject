#include <iostream>
#include <fstream>
#include <string>

struct rgb {
	int r;
	int g;
	int b;
};

int main()
{
	//read in original
	std::string inputFileName = "stockTree.ppm";
	std::ifstream inFile;
	inFile.open(inputFileName.c_str(), std::ios::in);

	if (inFile.fail()) {
		std::cout << "Failed to open file: " << inputFileName << std::endl;
		return -1;
	}

	std::string	inputHeading;
	std::string	p3Heading = "P3";
	inFile >> inputHeading;
	if (inputHeading != p3Heading) {
		std::cout << "Heading: " << inputHeading << " not supported." << std::endl;
		return -1;
	}

	int width, height, pixelCount;
	inFile >> width >> height;
	pixelCount = width * height;
	std::cout << "Reading file with width: " << width << " and height: " << height << std::endl;

	rgb* pixelArray = new rgb[pixelCount];

	rgb pixel;
	for (int i = 0; i < pixelCount; i++) {
		inFile >> pixel.r >> pixel.g >> pixel.b;
		pixelArray[i] = pixel;
	}

	inFile.close();


	//output copy
	std::string outputFileName = "output.ppm";
	std::ofstream outFile;
	outFile.open(outputFileName.c_str(), std::ios::out);

	if (outFile.fail()) {
		std::cout << "Failed to create file: " << outputFileName << std::endl;
		return -1;
	}

	std::cout << "Writing...";
	outFile << p3Heading << std::endl;
	outFile << width << std::endl << height << std::endl;

	for (int i = 0; i < pixelCount; i++) {
		pixel = pixelArray[i];
		outFile << pixel.r << std::endl << pixel.g << std::endl << pixel.b << std::endl;
	}

	outFile.close();

	delete[] pixelArray;
}