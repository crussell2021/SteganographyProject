#include <iostream>
#include <fstream>
#include <string>

struct rgb {
	int r;
	int g;
	int b;
};

bool* decToBin(int value);
int binToDecimal(bool*);
int powerOf(int base, int exponent);

int lastNBits(int value, int nBits);

int main()
{
	//read in original
	std::string inputFileName = "test.pgm";
	std::ifstream inFile;
	inFile.open(inputFileName.c_str(), std::ios::in);

	if (inFile.fail()) {
		std::cout << "Failed to open file: " << inputFileName << std::endl;
		return -1;
	}

	std::string	inputHeading;
	std::string	p3Heading = "P3";
	std::string	p2Heading = "P2";
	inFile >> inputHeading;
	if (inputHeading != p3Heading && inputHeading != p2Heading) {
		std::cout << "Heading: " << inputHeading << " not supported." << std::endl;
		return -1;
	}

	int width, height, pixelCount;
	inFile >> width >> height;
	pixelCount = width * height;
	std::cout << "Reading " + inputHeading + " file with width: " << width << " and height: " << height << std::endl;

	if (inputHeading == p3Heading) {
		rgb* pixelArray = new rgb[pixelCount];

		rgb pixel;
		for (int i = 0; i < pixelCount; i++) {
			inFile >> pixel.r >> pixel.g >> pixel.b;
			pixelArray[i] = pixel;
		}

		inFile.close();

		for (int n = 1; n < 2; n++) {
			rgb* newPixelArray = new rgb[pixelCount];
			//modify pixelarray
			for (int i = 0; i < pixelCount; i++) {
				newPixelArray[i].r = lastNBits(pixelArray[i].r, n);
				newPixelArray[i].g = lastNBits(pixelArray[i].g, n);
				newPixelArray[i].b = lastNBits(pixelArray[i].b, n);
			}
			//output copy
			std::string outputFileName = inputFileName.substr(0, inputFileName.length() - 4) + "Last" + std::to_string(n) + "Bits.ppm";
			std::ofstream outFile;
			outFile.open(outputFileName.c_str(), std::ios::out);

			if (outFile.fail()) {
				std::cout << "Failed to create file: " << outputFileName << std::endl;
				return -1;
			}

			std::cout << "Writing...";
			outFile << p3Heading << std::endl;
			outFile << width << "	" << height << std::endl;

			for (int i = 0; i < pixelCount; i++) {
				pixel = newPixelArray[i];
				outFile << pixel.r << std::endl << pixel.g << std::endl << pixel.b << std::endl;
			}

			outFile.close();
		}
		delete[] pixelArray;

	} else if (inputHeading == p2Heading) {
		int* pixelArray = new int[pixelCount];

		for (int i = 0; i < pixelCount; i++) {
			inFile >> pixelArray[i];
		}

		inFile.close();

		for (int n = 1; n < 8; n++) {
			int* newPixelArray = new int[pixelCount];
			//modify pixelarray
			for (int i = 0; i < pixelCount; i++) {
				newPixelArray[i] = lastNBits(pixelArray[i], n);
			}
			//output copy
			std::string outputFileName = inputFileName.substr(0, inputFileName.length() - 4) + "Last" + std::to_string(n) + "Bits.pgm";
			std::ofstream outFile;
			outFile.open(outputFileName.c_str(), std::ios::out);

			if (outFile.fail()) {
				std::cout << "Failed to create file: " << outputFileName << std::endl;
				return -1;
			}

			std::cout << "Writing...";
			outFile << p2Heading << std::endl;
			outFile << width << "	" << height << std::endl;

			for (int i = 0; i < pixelCount; i++) {
				outFile << newPixelArray[i] << std::endl;
			}

			outFile.close();
		}
		delete[] pixelArray;
	}
}


bool* decToBin(int value)
{
	bool* binaryDigits = new bool[32];

	int n = value;
	for (int i = 0; i < 32; i++) {
		binaryDigits[i] = n % 2;
		n = n / 2;
	}
	return binaryDigits;
}

int binToDecimal(bool* binaryArray) {
	int value = 0;
	for (int i = 0; i < 32; i++) {
		if (binaryArray[i]) {
			value += powerOf(2, i);
		}
	}
	return value;
}

int powerOf(int base, int exponent) {
	int value = 1;
	for (int i = 0; i < exponent; i++) {
		value *= base;
	}
	return value;
}

int lastNBits(int value, int nBitIndex) {
	bool* bin = new bool[32];
	bin = decToBin(value);
	for (int i = nBitIndex; i < 32; i++) {
		bin[i] = 0;
	}
	return binToDecimal(bin);
}