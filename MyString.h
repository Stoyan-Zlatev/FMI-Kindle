#pragma once

#include <fstream>
class MyString
{
	char* str;
	size_t size;

	void copyFrom(const MyString& other);
	void free();

public:
	MyString();
	MyString(const char*);
	MyString(size_t n);

	MyString(const MyString&);

	MyString(MyString&&);

	MyString& operator=(const MyString& other);
	MyString& operator=(MyString&& other);

	~MyString();

	size_t getSize() const;
	void concat(const MyString& other);

	const char* c_str() const;

	MyString& operator+=(const MyString& other);
	//MyString& substring(size_t startIndex, size_t endIndex);

	friend std::ostream& operator<<(std::ostream& stream, const MyString& str);
	friend std::istream& operator>>(std::istream& stream, MyString& str);

};

MyString operator+(const MyString& lhs, const MyString& rhs);

bool operator==(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);
bool operator<(const MyString& lhs, const MyString& rhs);