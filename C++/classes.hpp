#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <bits/stdc++.h>
using namespace std;
#define lib_id "lib"

class Book;
class Book_DB;
class User;
class Librarian;
class Professor;
class Student;
class User_DB;

extern Book_DB BOOK_DB;
extern User_DB USER_DB;
extern Librarian librarian;
extern int TODAY;
extern bool close;
extern User dummy;
extern User* logged_in;


class Book{
    private:
        // *** As mentioned in the document ***
        string Title = "", Author = "", ISBN = "", Publication = "", issued_to = lib_id;
        void Change_available(string, int);         // Changes the id the book is issued to, also takes due date as an argument
    public:
        int due_date = 0;                           // Due date of a book
        bool Check_availability();                  // Used when a user checks for availability
        Book(){;}
        Book(string, string, string, string);       // The used constructor
        bool Book_Request(string & , int);          // Used when a user wants to request a book
        void Show_duedate();                        // *** As mentioned in the document *** prints due date of a book
        bool operator ==(const Book& b) const;      
        bool operator <(const Book& b) const;       // For making a set of books
        void Change_name();                         // Changes the title of the book
        void Issued();                              // Displays the id of the user, the book has been issued to
        void Display();                             // Used in printing a book
        friend Book_DB;
        friend Librarian;
};

class Book_DB{ // Only one object of this class exists in the whole program
    protected:
        map<string, Book> database;
        void Add(Book);                             // *** As mentioned in the document *** adds a book to the database
        void Delete(Book);                          // *** As mentioned in the document *** deletes the book from the database
        void Delete(string);                        // *** As mentioned in the document *** deletes the book with a given isbn from the database
        bool Contains(string);                      // Checks if a book with a particular isbn exists in the database
    public:
        Book_DB(){;}
        bool Request_Book(string & , string, int);  // Used while requesting a book
        void Update(Book, string, int);             // Updates the book
        void Update(Book b);                        // *** As mentioned in the document *** updates the book in the database
        Book Search(string);                        // *** As mentioned in the document *** returns a book with a given isbn from the database, assumes existence in the DB
        void Display();                             // *** As mentioned in the document *** prints all the books currently in the database
        void Check_available();                     // Useful in checking availability
        friend Librarian;
};

class User{
    protected: // So that they may be accessed by the derived classes
        string ID = "", password = "";              // *** As mentioned in the document ***
        int fineamt = 0, max_books = 0;             // Required for calculating fine for prof and student. Caps max books issued by a student.
        set<Book> issued_books;                     // *** As mentioned in the document *** (list of books)
    public:
        string type = "", name = "";                // *** As mentioned in the document ***
        int issue_days = 0;                         // * 30 for students, 60 for professors
        User(){;}
        int Calculate_fine(int);                    // *** As mentioned in the document *** calculates fine for users
        void Clear_fine_amount(int);                // *** As mentioned in the document *** clears and returns the books which are due for a user
        void Return_books(int);                     // Returns all the books currently issued to the user
        bool operator == (const User& u);
        void Show_books();                          // *** As mentioned in the document *** (Displays books in the catalogue)
        bool Check_password(string);                // Matches password
        void Show_dues();                           // *** As mentioned in the document ***
        void Change_name();                         // Changes name of the user
        void Change_password();                     // Changes password of the user
        void Request_Book();                        // Request a book
        void Logout();
        void Display();                             // Prints user details
        virtual void Help();
        friend User_DB;
        friend Librarian;
        friend void Login();
};

class Professor : public User{
    public:
        Professor(string, string, string);
        void Help();
        friend void Login();
};

class Student : public User{
    public:
        Student(string, string, string);
        void Help();
        friend void Login();
};

class Librarian : public User{ // Only one object of this class exists in the whole program
    private:
    public:
        Librarian();
        void Add_Book();                            // *** As mentioned in the document ***
        void Delete_Book();                         // *** As mentioned in the document ***
        void Update_Book();                         // *** As mentioned in the document ***
        void Add_User();                            // *** As mentioned in the document ***
        void Delete_User();                         // *** As mentioned in the document ***
        void Update_User();                         // *** As mentioned in the document ***
        void Change_day();
        void Help();
        void Check_issue();                         // *** As mentioned in the document ***
        void User_Books();                          // *** As mentioned in the document ***
        friend void Login();
};

class User_DB{ // Only one object of this class exists in the whole program
    public:
        map<string, User*> database;
        User_DB(){;}
        void Add(User*);                            // *** As mentioned in the document *** adds a user from db
        bool Delete(User*);                         // *** As mentioned in the document *** deletes a user from db
        bool Delete(string);                        // *** As mentioned in the document *** deletes a user from db
        User* Search(string);                       // *** As mentioned in the document *** returns user with id "id" in db, assumes it to be present in db
        bool Contains(string);                      // Checks whether a user with id is present in the database 
        void Update(string, string);                // *** As mentioned in the document *** updates name of the user
        void Display();                             // Prints all the users in the database
        friend Librarian;
};

void Login();
void Show_today();                                  // Shows the date today

#endif