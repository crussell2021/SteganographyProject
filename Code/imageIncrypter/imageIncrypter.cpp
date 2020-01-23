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
int removeNBits(int value, int numBits);

int main()
{
	int n = 2;
	//read in original
	std::string originalInputFileName = "stockTree.ppm";
	std::ifstream originalFile;
	originalFile.open(originalInputFileName.c_str(), std::ios::in);

	if (originalFile.fail()) {
		std::cout << "Failed to open file: " << originalInputFileName << std::endl;
		return -1;
	}

	std::string	originalInputHeading;
	std::string	p3Heading = "P3";
	std::string	p2Heading = "P2";
	originalFile >> originalInputHeading;
	if (originalInputHeading != p3Heading && originalInputHeading != p2Heading) {
		std::cout << "Heading: " << originalInputHeading << " not supported." << std::endl;
		return -1;
	}

	int originalWidth, originalHeight, originalPixelCount, maxVal;
	originalFile >> originalWidth >> originalHeight >> maxVal;;
	originalPixelCount = originalWidth * originalHeight;
	std::cout << "Reading " + originalInputHeading + " file with width: " << originalWidth << " and originalHeight: " << originalHeight << std::endl;

	//read in hidden
	std::string hiddenInputFileName = "meme.ppm";
	std::ifstream hiddenFile;
	hiddenFile.open(hiddenInputFileName.c_str(), std::ios::in);

	if (hiddenFile.fail()) {
		std::cout << "Failed to open file: " << hiddenInputFileName << std::endl;
		return -1;
	}

	std::string	hiddenInputHeading;
	hiddenFile >> hiddenInputHeading;
	if (hiddenInputHeading != p3Heading && hiddenInputHeading != p2Heading) {
		std::cout << "Heading: " << hiddenInputHeading << " not supported." << std::endl;
		return -1;
	}

	int hiddenWidth, hiddenHeight, hiddenPixelCount;
	hiddenFile >> hiddenWidth >> hiddenHeight >> maxVal;;
	hiddenPixelCount = hiddenWidth * hiddenHeight;
	std::cout << "Reading " + hiddenInputHeading + " file with width: " << hiddenWidth << " and hiddenHeight: " << hiddenHeight << std::endl;

	maxVal = 255;

	if (originalInputHeading == p3Heading) {
		rgb* originalPixelArray = new rgb[originalPixelCount];

		rgb pixel;
		for (int i = 0; i < originalPixelCount; i++) {
			originalFile >> pixel.r >> pixel.g >> pixel.b;
			originalPixelArray[i] = pixel;
		}

		originalFile.close();

		rgb* hiddenPixelArray = new rgb[hiddenPixelCount];

		for (int i = 0; i < hiddenPixelCount; i++) {
			hiddenFile >> pixel.r >> pixel.g >> pixel.b;
			hiddenPixelArray[i] = pixel;
		}

		hiddenFile.close();

		int* hiddenBits = new int[hiddenPixelCount * 4 * 3];
		rgb tempPixel;
		int tempVal;
		int hiddenBitsIndex = 0;
		for (int i = 0; i < hiddenPixelCount; i++) {
			tempPixel = hiddenPixelArray[i];

			tempVal = tempPixel.r;
			for (int j = 0; j < 4; j++) {
				hiddenBits[hiddenBitsIndex++] = tempVal % 4;
				tempVal /= 4;
			}

			tempVal = tempPixel.g;
			for (int j = 0; j < 4; j++) {
				hiddenBits[hiddenBitsIndex++] = tempVal % 4;
				tempVal /= 4;
			}

			tempVal = tempPixel.b;
			for (int j = 0; j < 4; j++) {
				hiddenBits[hiddenBitsIndex++] = tempVal % 4;
				tempVal /= 4;
			}
		}

		rgb* newPixelArray = new rgb[originalPixelCount];
		//modify pixelarray
		int halfNibbleIndex = 0;
		for (int i = 0; i < originalPixelCount; i++) {
			if (halfNibbleIndex <= hiddenBitsIndex) {
				newPixelArray[i].r = originalPixelArray[i].r + hiddenBits[halfNibbleIndex++];
				newPixelArray[i].g = originalPixelArray[i].g + hiddenBits[halfNibbleIndex++];
				newPixelArray[i].b = originalPixelArray[i].b + hiddenBits[halfNibbleIndex++];
			}
			else {
				newPixelArray[i] = originalPixelArray[i];
			}
		}
		//output copy
		std::string outputFileName = originalInputFileName.substr(0, originalInputFileName.length() - 4) + "Plus" + hiddenInputFileName.substr(0, hiddenInputFileName.length() - 4) + ".ppm";
		std::ofstream outFile;
		outFile.open(outputFileName.c_str(), std::ios::out);

		if (outFile.fail()) {
			std::cout << "Failed to create file: " << outputFileName << std::endl;
			return -1;
		}

		std::cout << "Writing...";
		outFile << p3Heading << std::endl;
		outFile << originalWidth << "	" << originalHeight << std::endl << maxVal << std::endl;

		for (int i = 0; i < originalPixelCount; i++) {
			pixel = newPixelArray[i];
			outFile << (int)pixel.r << " " << (int)pixel.g << " " << (int)pixel.b << std::endl;
		}

		outFile.close();
		delete[] newPixelArray;

	}
	/*
	else if (originalInputHeading == p2Heading) {
		int* pixelArray = new int[originalPixelCount];

		for (int i = 0; i < originalPixelCount; i++) {
			inFile >> pixelArray[i];
		}

		inFile.close();

		for (int n = 1; n < 8; n++) {
			int* newPixelArray = new int[originalPixelCount];
			//modify pixelarray
			for (int i = 0; i < originalPixelCount; i++) {
				newPixelArray[i] = lastNBits(pixelArray[i], n);
			}
			//output copy
			std::string outputFileName = originalInputFileName.substr(0, originalInputFileName.length() - 4) + "Last" + std::to_string(n) + "Bits.pgm";
			std::ofstream outFile;
			outFile.open(outputFileName.c_str(), std::ios::out);

			if (outFile.fail()) {
				std::cout << "Failed to create file: " << outputFileName << std::endl;
				return -1;
			}

			std::cout << "Writing...";
			outFile << p2Heading << std::endl;
			outFile << width << "	" << originalHeight << std::endl << maxVal << std::endl;

			for (int i = 0; i < originalPixelCount; i++) {
				outFile << (int)newPixelArray[i] << std::endl;
			}

			outFile.close();
			delete[] newPixelArray;
		}
		delete[] pixelArray;
	}
	*/
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

int removeNBits(int value, int numBits) {
	return value - (value % powerOf(2, numBits));
}