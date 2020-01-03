/**
 * Created by yubraj rai on 03/01/2020.
 **/

#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include <vector>
#include <iomanip> 
using namespace std;

void GSM7BitDecoder(std::string strEncodedMessage, std::string& strDecodedMessage)
{
	std::stringstream hexStream;
	std::stringstream hexStreamOutput;
	std::vector<int> vecHexaNumbers;
	std::vector<int> vecTempNumbers;
	for(int i = 0 ;i < strEncodedMessage.length();)
	{
		int HexNumber ;
		hexStream.str(strEncodedMessage.substr(i,2));
		hexStream >> std::hex >> HexNumber;
		hexStream.clear();
		vecHexaNumbers.push_back(HexNumber);
		i = i+2;
	}
	
	int i = 0; 
	std::bitset<8> bCurrentBit;
	std::bitset<8> bPrevBit;
	while(i < vecHexaNumbers.size())
	{
		bCurrentBit = vecHexaNumbers[i];
		bCurrentBit <<= i;
		bCurrentBit[7] = 0;
		
		for(int j = 0; j < i; j++)
			bCurrentBit[j] = bPrevBit[8-i+j];

		bPrevBit = vecHexaNumbers[i];
		hexStreamOutput << (char)bCurrentBit.to_ulong();
		if(i == vecHexaNumbers.size()-1 || i == 6)
		{
			bPrevBit >>= 7-i;
			hexStreamOutput << (char)bPrevBit.to_ulong();
		}

		i++;
		if(i >= 7)
		{
			for(int j = i; j < vecHexaNumbers.size() ; j++)
				vecTempNumbers.push_back(vecHexaNumbers[j]);
			i = 0; 
			vecHexaNumbers.clear();
			vecHexaNumbers = vecTempNumbers;
		}
	}
	strDecodedMessage = hexStreamOutput.str();
	
}

void GSM7BitEncoder(std::string& strEncodedMessage, std::string strRawMessage)
{
	   strEncodedMessage.clear();
	   std::vector<long> vecEncodedMessage;
	   int iBitLength = (int)strRawMessage.length();
	   int i = 0 ;
	   while(i < iBitLength)
	   {
			 if(i >= 7)
			 {
				    i = 0;
				    iBitLength = (int)strRawMessage.length() - 8;
				    strRawMessage = strRawMessage.substr(8);
			 }

			 std::bitset<8> bPrev(strRawMessage[i]);
			 bPrev >>= i;
			 std::bitset<8> bNext(strRawMessage[i+1]);
			 for(int j = i , n=0; j >= 0 ; j--)
				    bPrev[7-j] = bNext[n++];
			 vecEncodedMessage.push_back(bPrev.to_ulong());
			 i++;
	   }

	   std::stringstream hexStream;
	   hexStream << std::hex << std::setw(2) << std::setfill('0');
	   for (int i = 0; i < vecEncodedMessage.size(); ++i)
			 hexStream << std::hex << vecEncodedMessage[i];

	   strEncodedMessage = hexStream.str();
}

int main()
{
	   std::string strEncoderString;
	   GSM7BitEncoder(strEncoderString,"Google");
	   std::cout<<strEncoderString<<endl;

	   GSM7BitDecoder(strEncoderString,strEncoderString);
	   std::cout<<strEncoderString<<endl;
	   return 0;
}
