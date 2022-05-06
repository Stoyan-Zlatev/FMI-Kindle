#pragma once
#include "User.h"
#include "Book.h"
#include "Collection.hpp"

class Kindle
{
	Collection<User> users;
	Collection<Book> booksToRead;
	User currentUser;
	bool isUsed;
	
	bool containsUser(const MyString& username, const MyString& password) const;
	bool containsUser(const MyString& username) const;
	bool conatinsBook(const MyString& bookTitle) const;
	size_t getBookIndexByName(const MyString& name) const;
public:
	Kindle();
	void saveToFile(std::fstream& file);
	void load(std::fstream& sourceFile);
	void login(const MyString& userName, const MyString& password);
	void signup(const MyString& userName, const MyString& password);
	void logout(std::fstream& file);
	void view() const;
	void rateBookByName(const MyString& title, const MyString& username, int rating);
	void addBook(const Book& book);
	void printBookComments(const MyString& bookTitle) const;
	void addBookComment(const MyString& bookTitle, const MyString& comment);
	void printBookPage(const MyString& bookTitle, int pageNumber) const;
	void printBookRating(const MyString& bookTitle) const;
	void addBookPage(const MyString& bookTitle, const MyString& pageContent);
	void removeBookLastPage(const MyString& bookTitle);
	void readBook(const MyString& bookTitle);
	size_t getUserIndexByName(const MyString& name) const;


	const MyString getCurrentUserName() const;
};