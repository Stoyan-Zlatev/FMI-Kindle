#include "Utils.h"
#include "GlobalConstants.h"

bool isPrefix(const MyString& text, const MyString& prefix)
{
	int i = 0;
	for (size_t i = 0; i < prefix.getSize(); i++)
	{
		if (i >= text.getSize() || text[i] != prefix[i])
			return false;
	}

	return true;
}

size_t getCommandLength(const MyString& text)
{
	size_t textLength = text.getSize();

	for (size_t i = 0; i < textLength; i++)
	{
		if (text[i] == ' ' || text[i] == '\t' || text[i] == '\r\n')
			return (i + 1);
	}
}

bool isDigit(char element)
{
	return element >= '0' && element <= '9';
}

void getField1Data(size_t startIndex, const MyString& text, MyString& field1)
{
	size_t textLength = text.getSize();
	size_t currentIndex;
	for (currentIndex = startIndex; currentIndex < textLength; currentIndex++)
	{
		if (isDigit(text[currentIndex]) || text[currentIndex] == '"')
		{
			currentIndex--;
			break;
		}
	}

	field1 = text.substring(startIndex, currentIndex);
}

void getField2Data(size_t startIndex, const MyString& text, MyString& title, MyString& data)
{
	size_t textLength = text.getSize();
	startIndex += title.getSize() + SpaceSize;
	size_t currentIndex;

	for (currentIndex = startIndex; currentIndex < textLength; currentIndex++)
	{
		if (text[currentIndex] == '"')
		{
			if (startIndex == currentIndex)
			{
				startIndex++;
				continue;
			}
			break;
		}
	}

	data = text.substring(startIndex, currentIndex);
}

int parseCharToInt(char element)
{
	return element - '0';
}

int parseStringToInt(const MyString& data)
{
	int number = 0;
	for (size_t i = 0; i < data.getSize(); i++)
	{
		number += parseCharToInt(data[i]);
		number *= 10;
	}
	number /= 10;

	return number;
}