// Junji Bressan
// 301 Section 1
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "person.cpp"
#include "book.cpp"

using namespace std;

// Primary functions
void printMenu();
void readBooks(vector<Book *> &myBooks);
int readPersons(vector<Person *> &myCardholders);
void readRentals(vector<Book *> &myBooks, vector<Person *> myCardholders);
void bookCheckout(vector<Book *> &myBooks, vector<Person *> myCardholders);
void bookReturn(vector<Book *> &myBooks);
void availableBooks(vector<Book *> myBooks);
void outstandingRentals(vector<Book *> myBooks);
void cardholderRentals(vector<Book *> myBooks, vector <Person *> myCardholders);
void openCard(vector<Person *> &myCardholders, int &newID);
void closeCard(vector<Person *> &myCardholders);
void updateFiles(vector<Book *> myBooks, vector<Person *> myCardholders);
void handleMemory(vector<Book *> myBooks, vector<Person *> myCardholders);
void searchByCategory(vector<Book *> myBooks, vector<Person *> myCardholders);
void addBook(vector<Book *> &myBooks);

// Helper functions
int checkCardholder(vector<Person *> myCardholders, int id);
int checkBookID(vector<Book *> myBooks, int bookID);
void readRentalsFile(vector<int> &rentedBooks, vector<int> &cardIDs);
int findBookIndex(vector<Book *> myBooks, int bookID);
int findCardholderIndex(vector<Person *> myCardholders, int cardID);
int generateBookID(vector<Book *> myBooks);

int main()
{
    vector<Book *> books;
    vector<Person *> cardholders;
    int newID = 0;

    readBooks(books);
    newID = readPersons(cardholders);
    readRentals(books, cardholders);

    int choice;
    do
    {
        printMenu();
        cin >> choice;
        cout << endl;
        switch(choice)
        {
            case 1:
                // Book checkout
                bookCheckout(books, cardholders);
                break;

            case 2:
                // Book return
                bookReturn(books);
                break;

            case 3:
                // View all available books
                availableBooks(books);
                break;

            case 4:
                // View all outstanding rentals
                outstandingRentals(books);
                break;

            case 5:
                // View outstanding rentals for a cardholder
                cardholderRentals(books, cardholders);
                break;

            case 6:
                // Open new library card
                openCard(cardholders, newID);
                break;

            case 7:
                // Close library card
                closeCard(cardholders);
                break;

            case 8:
                // Search by category
                searchByCategory(books, cardholders);
                break;

            case 9:
                // Add a book to the Inventory
                addBook(books);
                break;

            case 10:
                // Update records in files before exiting the program
                updateFiles(books, cardholders);
                handleMemory(books, cardholders);
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 10);
      return 0;
}

// ============================================================================
// This function displays a menu for the library's book rental system.
// It does not return any thing.
void printMenu()
{
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Search by Category" << endl;
    cout << "9.  Add Book to Inventory" << endl;
    cout << "10.  Exit system" << endl;
    cout << "Please enter a choice: ";
}
// ============================================================================

// ============================================================================
// This function reads from the books.txt file and stores the information
// in books vector. It takes the books vector as an argument by
// reference. It does not return anything.
void readBooks(vector<Book *> &myBooks)
{
  int id;
  string title, author, category, discard;
  ifstream readData;
  Book *bookPtr = nullptr;

  readData.open("books.txt");

  while(readData >> id)
  {
    getline(readData, discard);
    getline(readData, title);
    getline(readData, author);
    getline(readData, category);
    bookPtr = new Book(id, title, author, category);
    myBooks.push_back(bookPtr);
  }
  bookPtr = nullptr;

  readData.close();
}
// ============================================================================

// ============================================================================
// This function reads from the persons.txt file and stores the information
// in the cardholders vector. It takes the cardholders vector as an argument
// by reference. It returns an integer value which is the highest ID number
// plus 1.
int readPersons(vector<Person *> &myCardholders)
{
  int cardID;
  bool active;
  string firstName, lastName;
  ifstream readData;
  Person *personPtr = nullptr;

  readData.open("persons.txt");

  while(readData >> cardID)
  {
    readData >> active;
    readData >> firstName;
    readData >> lastName;
    personPtr = new Person(cardID, active, firstName, lastName);
    myCardholders.push_back(personPtr);
  }
  personPtr = nullptr;
  readData.close();

  return (cardID + 1);
}
// ============================================================================

// ============================================================================
// This function checks if the book ID the user entered is valid.
// If the book exists then it returns a zero. If not it returns a negative one.
int checkBookID(vector<Book *> myBooks, int bookID)
{
  int isValid = -1;

  for(int i = 0; i < myBooks.size(); i++)
  {
    if(bookID == myBooks[i]->getId())
    {
      isValid = 0;
      return isValid;
    }
  }
  return isValid;
}
// ============================================================================

// ============================================================================
// This function checks if the cardholder ID the user entered is valid.
// It checks to see if the cardholder exists and their card is active.
// If both these conditions are met it returns the index number of the
// cardholder. If not it returns a -1.
int checkCardholder(vector<Person *> myCardholders, int id)
{
  int isValid = -1, cardHolder = 0, cardID = id;

  for(int i = 0; i < myCardholders.size(); i++)
  {
    if(cardID == myCardholders[i]->getId() && myCardholders[i]->isActive() == 1)
    {
      return cardHolder = i;
    }
  }
  return isValid;
}
// ============================================================================

// ============================================================================
// This function will use the person pointer in the book class and point to
// the person object that has checked out the book based on rentals.txt. It
// does not return anything.
void readRentals(vector<Book *> &myBooks, vector<Person *> myCardholders)
{
  vector<int> rentedBooks;
  vector<int> cardIDs;
  Person *personPtr = nullptr;
  int bookID, bookIndex, userID, cardIndex;

  readRentalsFile(rentedBooks, cardIDs);

  for(int i = 0; i < rentedBooks.size(); i++)
  {
    bookID = rentedBooks[i];
    userID = cardIDs[i];
    bookIndex = findBookIndex(myBooks, bookID);
    cardIndex = findCardholderIndex(myCardholders, userID);
    personPtr = myCardholders[cardIndex];
    myBooks[bookIndex]->setPersonPtr(personPtr);
  }
  personPtr = nullptr;
}
// ============================================================================

// ============================================================================
// This function reads the information from rentals.txt and stores the book ID
// and the card ID in their respective vectors. It does not return anything.
void readRentalsFile(vector<int> &rentedBooks, vector<int> &cardIDs)
{
  ifstream readData;
  int bookID, cardID;

  readData.open("rentals.txt");

  while(readData >> bookID)
  {
    readData >> cardID;
    rentedBooks.push_back(bookID);
    cardIDs.push_back(cardID);
  }
  readData.close();
}
// ============================================================================

// ============================================================================
// This function finds the index in the myBooks vector containing the book
// object corresponding to the book ID stored in the bookID argument. It returns
// the index number of the book object based on matching book IDs.
int findBookIndex(vector<Book *> myBooks, int bookID)
{
  for(int i = 0; i < myBooks.size(); i++)
  {
    if(bookID == myBooks[i]->getId())
    {
      return i;
    }
  }
}
// ============================================================================

// ============================================================================
// This function finds the index in the myCardholders vector containing the
// person object that is currently renting a specific book. It returns the
// the index number of the person object based on matching card IDs.
int findCardholderIndex(vector<Person *> myCardholders, int cardID)
{
  for(int i = 0; i < myCardholders.size(); i++)
  {
    if(cardID == myCardholders[i]->getId())
    {
      return i;
    }
  }
}
// ============================================================================

// ============================================================================
// This function allows the user to checkout a book provided that: their ID is
// valid, the book ID is valid, and the book has not been checked out yet.
// If all these conditions are met the user will be able to checkout the book
// they requested.
void bookCheckout(vector<Book *> &myBooks, vector<Person *> myCardholders)
{
  int cardID, cardIndex, bookID, bookIndex, isValid = 0, cardHolder = 0;
  string name;
  Person *personPtr = nullptr;

// Card ID validation.
  cout << "Please enter the card ID: ";
  cin >> cardID;
  cardHolder = checkCardholder(myCardholders, cardID);
  if(cardHolder != -1)
  {
    cout << "Cardholder: " << myCardholders[cardHolder]->fullName() << endl;
  }
  else
  {
    cout << "Card ID not found\n";
    return;
  }

// Book ID validation, rental status, and completion of rental.
  cout << "Please enter the book ID: ";
  cin >> bookID;
  isValid = checkBookID(myBooks, bookID);
  if(isValid == -1)
  {
    cout << "Book ID not found\n";
    return;
  }
  else
  {
    for(int i = 0; i < myBooks.size(); i++)
    {
      if(bookID == myBooks[i]->getId() && myBooks[i]->getPersonPtr() != nullptr)
      {
        cout << "Book already checked out\n";
        return;
      }
    }
    for(int j = 0; j < myBooks.size(); j++)
    {
      if(bookID == myBooks[j]->getId())
      {
        cout << "Title: " << myBooks[j]->getTitle() << endl;
        cout << "Rental Completed\n";
        cardIndex = findCardholderIndex(myCardholders, cardID);
        bookIndex = findBookIndex(myBooks, bookID);
        personPtr = myCardholders[cardIndex];
        myBooks[bookIndex]->setPersonPtr(personPtr);
      }
    }
  }
}
// ============================================================================

// ============================================================================
// This function allows the user to return a book provided that the book ID is
// valid and the book being returned is currently rented.
void bookReturn(vector<Book *> &myBooks)
{
  int bookID, bookIndex, isValid = 0;
  Person *personPtr = nullptr;

  cout << "Please enter the book ID to return: ";
  cin >> bookID;
  isValid = checkBookID(myBooks, bookID);
  bookIndex = findBookIndex(myBooks, bookID);
  if(isValid == -1 || myBooks[bookIndex]->getPersonPtr() == nullptr)
  {
    cout << "Book ID not found\n";
    return;
  }
  else
  {
    for(int i = 0; i < myBooks.size(); i++)
    {
      if(bookID == myBooks[i]->getId())
      cout << "Title: " << myBooks[i]->getTitle() << endl;
      myBooks[bookIndex]->setPersonPtr(personPtr);
    }
  }
  cout << "Return Completed\n";
}
// ============================================================================

// ============================================================================
// This function displays all of the books that are still available.
void availableBooks(vector<Book *> myBooks)
{
  int bookCounter = 0;

  for(int i = 0; i < myBooks.size(); i++)
  {
    if(myBooks[i]->getPersonPtr() == nullptr)
    {
      cout << "Book ID: " << myBooks[i]->getId() << endl;
      cout << "Title: " << myBooks[i]->getTitle() << endl;
      cout << "Author: " << myBooks[i]->getAuthor() << endl;
      cout << "Category: " << myBooks[i]->getCategory() << endl << endl;
      bookCounter++;
    }
  }
  if(bookCounter == 0)
  {
    cout << "No available books\n";
  }
}
// ============================================================================

// ============================================================================
// This function displays the books that currently rented out.
void outstandingRentals(vector<Book *> myBooks)
{
  int rentalCounter = 0;

  for(int i = 0; i < myBooks.size(); i++)
  {
    if(myBooks[i]->getPersonPtr() != nullptr)
    {
      cout << "Book ID: " << myBooks[i]->getId() << endl;
      cout << "Title: " << myBooks[i]->getTitle() << endl;
      cout << "Author: " << myBooks[i]->getAuthor() << endl;
      cout << "Cardholder: " << myBooks[i]->getPersonPtr()->fullName() << endl;
      cout << "Card ID: " << myBooks[i]->getPersonPtr()->getId() << endl << endl;
      rentalCounter++;
    }
  }
  if(rentalCounter == 0)
  {
    cout << "No outstanding rentals\n";
  }
}
// ============================================================================

// ============================================================================
// This function asks the user to enter a card ID and then displays the books
// currently rented out by that cardholder. So long the card ID entered is valid.
void cardholderRentals(vector<Book *> myBooks, vector <Person *> myCardholders)
{
  int cardID, cardHolder, cardholderRentalsCntr = 0;

  cout << "Please enter the card ID: ";
  cin >> cardID;
  cardHolder = checkCardholder(myCardholders, cardID);
  if(cardHolder == -1)
  {
    cout << "Card ID not found\n";
    return;
  }
  else
  {
    cout << "Cardholder: " << myCardholders[cardHolder]->fullName() << endl;
    for(int i = 0; i < myBooks.size(); i++)
    {
      if(myBooks[i]->getPersonPtr() != nullptr && cardID == myBooks[i]->getPersonPtr()->getId())
      {
        cout << endl;
        cout << "Book ID: " << myBooks[i]->getId() << endl;
        cout << "Title: " << myBooks[i]->getTitle() << endl;
        cout << "Author: " << myBooks[i]->getAuthor() << endl;
        cardholderRentalsCntr++;
      }
    }
    if(cardholderRentalsCntr == 0)
    {
      cout << "No books currently checked out\n";
    }
  }
}
// ============================================================================

// ============================================================================
// This function will ask the user to enter a book category and then search the
// books vector and display the books based on that category it will also
// display the rental status of the book.
void searchByCategory(vector<Book *> myBooks, vector<Person *> myCardholders)
{
  string cat;

  cout << "Enter the category: ";
  cin.ignore();
  getline(cin, cat);
  cout << endl;

  for(int i = 0; i < myBooks.size(); i++)
  {
    if(cat == myBooks[i]->getCategory())
    {
      cout << "Book ID: " << myBooks[i]->getId() << endl;
      cout << "Title: " << myBooks[i]->getTitle() << endl;
      cout << "Author: " << myBooks[i]->getAuthor() << endl;
      if(myBooks[i]->getPersonPtr() != nullptr)
      {
        cout << "Book checked out\n";
      }
      else
      {
        cout << "Available\n";
      }
      cout << endl;
    }
  }
}
// ============================================================================

// ============================================================================
// This function will add a book to the books vector. It will first generate a
// random book ID by calling the generateBookID function. It will then ask the
// user to enter a title, author, and category. It will check the title to
// ensure that there are no duplicates. If everything checks out the book will
// be added to the vector.
void addBook(vector<Book *> &myBooks)
{
  int bookID = generateBookID(myBooks);
  string title, author, category;

  cout << "Enter the title of the book: ";
  cin.ignore();
  getline(cin, title);
  cout << "Enter the Author of the book: ";
  getline(cin, author);
  cout << "Enter the category of the book: ";
  getline(cin, category);
  cout << endl;

  //Check for duplicates
  for(int i = 0; i < myBooks.size(); i++)
  {
    if(title == myBooks[i]->getTitle())
    {
      cout << "Book already in inventory\n";
      return;
    }
  }

  // Add to myBooks vector
  Book *bookPtr = new Book(bookID, title, author, category);
  myBooks.push_back(bookPtr);
  cout << "Book ID: " << bookID << endl;
  cout << "Title: " << title << endl;
  cout << "Author: " << author << endl;
  cout << "Category: " << category << endl;
  bookPtr = nullptr;
}
// ============================================================================

// ============================================================================
// This function is called from the addBook function and generates a random
// ID. The randomly generated ID will be compared with the other IDs in the
// books vector to ensure no duplicates are present. The ID will be returned if
// it is not a duplicate.
int generateBookID(vector<Book *> myBooks)
{
  int bookID = 0;
  bool checkCondition = true, checkDuplicate = false;
  unsigned seed = time(0);
  do
  {
    srand(seed);
    bookID = (rand() % (99999 - 10000 + 1) + 10000);

    for(int i = 0; i < myBooks.size(); i++)
    {
      if(bookID == myBooks[i]->getId())
      {
        cout << "Book ID exists\n";
        checkDuplicate = true;
        break;
      }
    }
    if(checkDuplicate == false)
    {
      return bookID;
    }
  } while(checkCondition == true);
}
// ============================================================================

// ============================================================================
// This function creates a new card for the user. If the first name and last
// name do not match with any of the existing names in the system then a new
// card will be created. If the first and last names do match then that card
// will be reactivated.
void openCard(vector<Person *> &myCardholders, int &newID)
{
  string firstName, lastName;
  bool active = 1;
  Person *personPtr = nullptr;

  cout << "Please enter the first name: ";
  cin >> firstName;
  cout << "Please enter the last name: ";
  cin >> lastName;

  for(int i = 0; i < myCardholders.size(); i++)
  {
    if(firstName == myCardholders[i]->getFirstName() && lastName == myCardholders[i]->getLastName())
    {
      myCardholders[i]->setActive(active);
      cout << "Card ID " << myCardholders[i]->getId() << " active\n";
      cout << "Cardholder: " << myCardholders[i]->fullName() << endl;
      return;
    }
  }

    personPtr = new Person(newID, active, firstName, lastName);
    myCardholders.push_back(personPtr);
    cout << "Card ID " << newID << " active\n";
    cout << "Cardholder: " << myCardholders.back()->fullName() << endl;
    personPtr = nullptr;
    newID++;
}
// ============================================================================

// ============================================================================
// This function deactivates a card. If the ID entered is matched to a card that
// is currently active then confirmation will be asked to deactivate the card.
// If the ID is matched to a card that is already deactivated then a message
// will be displayed stating that.
void closeCard(vector<Person *> &myCardholders)
{
  int cardID, cardHolder = 0, cardIndex;
  char confirmation;
  bool active = 0;

  cout << "Please enter the card ID: ";
  cin >> cardID;

  for(int i = 0; i < myCardholders.size(); i++)
  {
    if(cardID != myCardholders[i]->getId())
    {
      cardHolder++;
    }
  }
  cardIndex = findCardholderIndex(myCardholders, cardID);
  if(cardHolder == myCardholders.size())
  {
    cout << "Card ID not found\n";
    return;
  }
  if(active == myCardholders[cardIndex]->isActive())
  {
    cout << "Cardholder: " << myCardholders[cardIndex]->fullName() << endl;
    cout << "Card ID is already inactive\n";
    return;
  }
  else
  {
    cout << "Are you sure you want to deactivate card\n";
    cout << "(y/n)? ";
    cin >> confirmation;

    if(confirmation == 'y')
    {
      myCardholders[cardIndex]->setActive(active);
      cout << "Card ID deactivated\n";
      return;
    }
    else
    {
      return;
    }
  }
}
// ============================================================================

// ============================================================================
// This function updates the persons.txt, books.txt, and rentals.txt files based
// on the options the user selected.
void updateFiles(vector<Book *> myBooks, vector<Person *> myCardholders)
{
  ofstream writeData;
  int bookID, cardID;
  bool active;
  string firstName, lastName, title, author, category;

  // Updating persons.txt
  writeData.open("persons.txt");

  for(int i = 0; i < myCardholders.size(); i++)
  {
    cardID = myCardholders[i]->getId();
    active = myCardholders[i]->isActive();
    firstName = myCardholders[i]->getFirstName();
    lastName = myCardholders[i]->getLastName();
    writeData << cardID << " " << active << " " << firstName << " " << lastName << endl;
  }
  writeData.close();

  // Updating rentals.txt
  writeData.open("rentals.txt");

  for(int j = 0; j < myBooks.size(); j++)
  {
    if(myBooks[j]->getPersonPtr() != nullptr)
    {
      bookID = myBooks[j]->getId();
      cardID = myBooks[j]->getPersonPtr()->getId();
      writeData << bookID << " " << cardID << endl;
     }
  }
  writeData.close();

  // Updating books.txt
  writeData.open("books.txt");

  for(int k = 0; k < myBooks.size(); k++)
  {
    bookID = myBooks[k]->getId();
    title = myBooks[k]->getTitle();
    author = myBooks[k]->getAuthor();
    category = myBooks[k]->getCategory();
    writeData << bookID << endl;
    writeData << title << endl;
    writeData << author << endl;
    writeData << category << endl << endl;
  }
  writeData.close();
}
// ============================================================================

// ============================================================================
// This function will delete the dynamic memory pointed to by the pointers in
// the books and cardholders vectors.
void handleMemory(vector<Book *> myBooks, vector<Person *> myCardholders)
{
  for(int i = 0; i < myBooks.size(); i++)
  {
    delete myBooks[i];
    myBooks[i] = nullptr;
  }

  for(int j = 0; j < myCardholders.size(); j++)
  {
    delete myCardholders[j];
    myCardholders[j] = nullptr;
  }
}
// ============================================================================
