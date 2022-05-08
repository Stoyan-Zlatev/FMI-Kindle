#include "MyString.h"
#include "GlobalConstants.h"
#include <cstring>
#include <iostream>
#pragma warning(disable:4996)

void MyString::copyFrom(const MyString& other)
{
	str = new char[strlen(other.str) + 1];
	strcpy(str, other.str);
	size = other.size;
}
void MyString::free()
{
	delete[] str;
}

MyString::MyString()
{
	str = new char[1];
	str[0] = '\0';
	size = 0;
}

size_t getNumSize(size_t num)
{
	size_t res = 0;
	while (num)
	{
		res++;
		num /= 10;
	}
	return res;
}
MyString::MyString(size_t n)
{
	int numSize = getNumSize(n);
	str = new char[numSize + 1];

	str[numSize] = '\0';

	for (size_t i = 0; i < numSize; i++, n /= 10)
		str[numSize - 1 - i] = (n % 10) + '0';
	size = numSize;
}

MyString::MyString(const char* data)
{
	size = strlen(data);
	str = new char[size + 1];
	strcpy(str, data);
}

MyString::MyString(const MyString& other)
{
	copyFrom(other);
}
MyString& MyString::operator=(const MyString& other)
{
	//std::cout << "op=" << std::endl;

	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
MyString::~MyString()
{
	free();
}


size_t MyString::getSize() const
{
	return size;
}

void MyString::concat(const MyString& other)
{
	char* temp = new char[getSize() + other.getSize() + 1];
	strcpy(temp, str);
	strcat(temp, other.str);

	delete[] str;
	str = temp;
	size = size + other.getSize();
}

const char* MyString::c_str() const
{
	return str;
}

MyString& MyString::operator+=(const MyString& other)
{
	concat(other);
	return *this;
}


MyString operator+(const MyString& lhs, const MyString& rhs)
{
	MyString copyOfLeft(lhs);
	copyOfLeft += rhs;
	return copyOfLeft;
}

std::ostream& operator<<(std::ostream& stream, const MyString& str)
{
	stream << str.str;
	return stream;
}

std::istream& operator>>(std::istream& stream, MyString& str)
{
	delete[] str.str;
	char buff[MaxContentLength];
	stream >> buff;

	str.size = strlen(buff);
	str.str = new char[str.size + 1];
	strcpy(str.str, buff);

	return stream;
}

void MyString::getline(std::istream& stream)
{
	delete[] str;
	char buff[MaxContentLength];
	stream.getline(buff,MaxContentLength);

	size = strlen(buff);
	str = new char[size + 1];
	strcpy(str, buff);
}

bool operator==(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}
bool operator<=(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) <= 0;

}
bool operator<(const MyString& lhs, const MyString& rhs)
{
	return strcmp(lhs.c_str(), rhs.c_str()) < 0;

}

MyString::MyString(MyString&& otherString)
{
	str = otherString.str;
	size = otherString.size;
	otherString.str = nullptr;
}

MyString& MyString::operator=(MyString&& otherString)
{
	if (this != &otherString)
	{
		free();
		str = otherString.str;
		otherString.str = nullptr;
		size = otherString.size;
	}
	return *this;
}

char& MyString::operator[](int index) const
{
	if (size!=0 && (index < 0 || index >= size))
	{
		throw std::invalid_argument("Index out of range");
	}

	return str[index];
}

MyString MyString::substring(size_t startIndex, size_t endIndex) const
{
	char temp[MaxContentLength];
	for (size_t i = startIndex; i < endIndex; i++)
	{
		temp[i-startIndex] = str[i];
	}
	
	temp[endIndex - startIndex]='\0';
	MyString substr = temp;
	
	return substr;
}

