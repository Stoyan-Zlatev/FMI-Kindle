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

	int getBookIndexByName(const MyString& name) const;
	int getUserIndexByName(const MyString& name) const;
public:
	Kindle();
	
	void login(const MyString& userName, const MyString& password);
	void signup(const MyString& userName, const MyString& password);
	void logout(std::fstream& file);
	bool exit();

	void load(std::fstream& sourceFile);
	void saveToFile(std::fstream& file);
	
	void view() const;
	
	void addBook(const Book& book);
	void addBookComment(const MyString& bookTitle, const MyString& comment);
	void addBookPage(const MyString& bookTitle, const MyString& pageContent);
	
	void rateBookByName(const MyString& bookTitle, int rating);

	void readBook(const MyString& bookTitle);

	void printBookComments(const MyString& bookTitle) const;
	void printBookPage(const MyString& bookTitle, int pageNumber) const;
	void printBookRating(const MyString& bookTitle) const;
	
	void removeBookLastPage(const MyString& bookTitle);
	
	void editBookPage(const MyString& bookTitle, int page, const MyString& pageContent);
	void editBookRating(const MyString& bookTitle, int rate);
	
	bool containsBook(const MyString& bookTitle) const;

	const MyString getCurrentUserName() const;
	bool getIsUsed() const;
};