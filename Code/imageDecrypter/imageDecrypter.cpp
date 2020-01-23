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

int lastNBits(int value, int nBits=2);
int removeNBits(int value, int numBits);

int main()
{
	//int n = 2;
	//read in steganogram
	std::string stegInputFileName = "steganogram.ppm";
	std::ifstream stegFile;
	stegFile.open(stegInputFileName.c_str(), std::ios::in);

	if (stegFile.fail()) {
		std::cout << "Failed to open file: " << stegInputFileName << std::endl;
		return -1;
	}

	std::string	stegInputHeading;
	std::string	p3Heading = "P3";
	stegFile >> stegInputHeading;
	if (stegInputHeading != p3Heading) {
		std::cout << "Heading: " << stegInputHeading << " not supported." << std::endl;
		return -1;
	}

	int stegWidth, stegHeight, stegPixelCount, maxVal;
	stegFile >> stegWidth >> stegHeight >> maxVal;
	stegPixelCount = stegWidth * stegHeight;
	std::cout << "Reading " + stegInputHeading + " file with width: " << stegWidth << " and originalHeight: " << stegHeight << std::endl;

	int* stegPixelArray = new int[stegPixelCount*3];

	for (int i = 0; i < stegPixelCount*3; i++) {
		stegFile >> stegPixelArray[i];
	}

	stegFile.close();

	int* hiddenBits = new int[stegPixelCount*3];
	for (int i = 0; i < stegPixelCount*3; i++) {
		hiddenBits[i] = stegPixelArray[i] % 4;
	}

	int* newPixelArray = new int[stegPixelCount*3/4];
	//modify pixelarray
	int index = 0;
	for (int i = 0; i < stegPixelCount*3/4; i++) {
		newPixelArray[i] = hiddenBits[index++] + hiddenBits[index++]*4 + hiddenBits[index++]*16 + hiddenBits[index++]*64;
	}

	//output 
	std::string outputFileName = stegInputFileName.substr(0, stegInputFileName.length() - 4) + "Decrypted" + ".ppm";
	std::ofstream outFile;
	outFile.open(outputFileName.c_str(), std::ios::out);

	if (outFile.fail()) {
		std::cout << "Failed to create file: " << outputFileName << std::endl;
		return -1;
	}

	std::cout << "Writing...";
	outFile << p3Heading << std::endl;
	int finalWidth, finalHeight;
	std::cout << "Please enter hidden file width.";
	std::cin >> finalWidth;
	std::cout << "Please enter hidden file height.";
	std::cin >> finalHeight;

	outFile << finalWidth << "	" << finalHeight << std::endl << maxVal << std::endl;

	for (int i = 0; i < stegPixelCount/4; i++) {
		outFile << newPixelArray[i] << " " << newPixelArray[i+1] << " " << newPixelArray[i+2] << std::endl;
	}

	outFile.close();
	delete[] newPixelArray;

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