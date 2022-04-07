#include "classes.hpp"

Book_DB BOOK_DB;
User_DB USER_DB;
Librarian librarian;
int TODAY = 0;
bool close = false;
User dummy;
User* logged_in = (&dummy);


void Book::Change_available(string issuer, int day){
    issued_to = issuer;
    due_date = day;
}
bool Book::Check_availability(){
    if(issued_to == lib_id){
        cout<<"This book is currently available for issuing!\n";
    }else{
        cout<<"Sorry, this book is not currently available.\n";
    }
    return (issued_to == lib_id);
}
Book::Book(string t, string a, string i, string p){
    Title = t, Author = a, ISBN = i, Publication = p;
}
bool Book::Book_Request(string (&requester), int day = 0){
    if(requester == lib_id){
        issued_to = lib_id, due_date = 0;
        return true;
    }
    if(Check_availability()){
        cout<<"Book has been issued to you!\n";
        issued_to = requester, due_date = day;
        return true;
    }else{
        cout<<"Sorry, the book is currently issued to someone else.\n";
        return false;
    }
}
void Book::Show_duedate(){
    cout<<"The due date for this book is:\t"<<due_date<<'\n';
}
bool Book::operator == (const Book& b) const{
    return (this->Title == b.Title) && (this->Author == b.Author) && (this->ISBN == b.ISBN) && (this->Publication == b.Publication);
}
void Book::Display(){
    cout<<"Title of this Book:\t"<<Title<<'\n';
    cout<<"Author of this Book:\t"<<Author<<'\n';
    cout<<"ISBN:\t"<<ISBN<<'\n';
    cout<<"Publication:\t"<<Publication<<"\n\n";
}
bool Book::operator < (const Book& b) const{
    return ((this->ISBN) < b.ISBN);
}
void Book::Change_name(){
    cout<<"Enter the new name of the book: ";
    string n; cin>>n;
    Title = n;
}
void Book::Issued(){
    if(issued_to == lib_id){
        cout<<"This book is not issued to anyone currently.";
    }else{
        cout<<"This book is issued to the user with id: "<<issued_to;
    }
    cout<<"\n";
}

void Book_DB::Add(Book b){
    database[b.ISBN] = b;
}
void Book_DB::Delete(Book b){
    if(Contains(b.ISBN)){
        database.erase(b.ISBN);
    }
}
void Book_DB::Delete(string isbn){
    if(Contains(isbn)){
        database.erase(isbn);
    }
}
void Book_DB::Update(Book b, string issuer, int new_date){
    if(!Contains(b.ISBN)){
        return;
    }
    b.issued_to = issuer;
    b.due_date = new_date;
    database[b.ISBN] = b;
}
void Book_DB::Update(Book b){
    database[b.ISBN] = b;
}
Book Book_DB::Search(string isbn){
    return database[isbn];
}
bool Book_DB::Contains(string isbn){
    return (database.find(isbn) != database.end());
}
bool Book_DB::Request_Book(string (&req), string isbn, int today){
    cout<<"\n";
    bool ret;
    if(Contains(isbn)){
        Book b = database[isbn];
        ret = b.Book_Request(req, today);
        Update(b);
    }else{
        cout<<"Book does not exist in the Catalogue.\n";
        ret = false;
    }
    cout<<"\n"; 
    return ret;
}
void Book_DB::Display(){
    cout<<"\n\n";
    if(database.size() == 0){
        cout<<"The catalogue is currently empty!\n";
        return;
    }
    cout<<"The current catalogue is:\n\n";
    for(auto i : database){
        Book b = i.second;
        b.Display();
    }
    cout<<"\n";
}
void Book_DB::Check_available(){
    cout<<"\nEnter the ISBN of the book you want to check availability of: ";
    string i; cin>>i;
    if(Contains(i)){
        Book b = database[i];
        b.Check_availability();
    }else{
        cout<<"Book does not exist in the catalogue.\n";
    }
    cout<<"\n";
}

int User::Calculate_fine(int today=TODAY){
    if(this->ID == lib_id){
        cout<<"\nLibrarian cannot have any fines.\n";
        return 0;
    }
    int fine = 0;
    for(auto b : issued_books){
        fine += fineamt * max(today - b.due_date, 0);
    }
    if(fine){
        cout<<"Your current outstanding fines are: "<<fine<<".\n";
    }else{
        cout<<"You currently have no pending fines.\n";
    }
    return fine;
}
void User::Return_books(int today=TODAY){
    if(this->ID == lib_id){
        cout<<"\nLibrarian cannot return books.\n";
        return;
    }
    int fine = Calculate_fine(today);
    cout<<"\nClearing a fine of:\t"<<fine<<'\n';
    cout<<"Returning all books that you currently have.\n...";
    for(auto b : issued_books){
        BOOK_DB.Update(b, lib_id, 0);
    }
    issued_books.clear();
    cout<<"Returned successfully.\n";
}
bool User::operator == (const User& u){
    return (this->ID == u.ID) && (this->password == u.password);
}
bool User::Check_password(string pwd){
    return password == pwd;
}
void User::Show_books(){
    if(issued_books.size() == 0){
        cout<<"No books issued yet.\n\n"; 
        return;
    }
    cout<<"The following books have been issued:\n\n";
    for(auto b : issued_books){
        b.Display();
    }
}
void User::Request_Book(){
    if(max_books && issued_books.size() == max_books){
        cout<<"Sorry, you cannot issue more books!\n";
        return;
    }
    cout<<"Please enter the ISBN of the book you want to issue: ";
    string isbn; cin>>isbn;
    if(BOOK_DB.Request_Book(this->ID, isbn, TODAY+issue_days)){
        issued_books.insert(BOOK_DB.Search(isbn));
    }
}
void User::Clear_fine_amount(int today=TODAY){
    cout<<"\nReturning the books which are due...\n\n";

    set<Book> to_return;
    for(auto b : issued_books){
        if(b.due_date >= today){
            to_return.insert(b);
        }
    }
    for(auto b : to_return){
        issued_books.erase(b);
    }
}
void User::Change_name(){
    if(ID == lib_id){
        cout<<"\nCannot change name of Librarian.\n";
        return;
    }
    cout<<"Enter the new name you wish to have: ";
    string n; cin>>n;
    USER_DB.Update(n, this->ID);
    cout<<"\n";
}
void User::Logout(){
    logged_in = &dummy;
    TODAY++;
    cout<<"\nLogged out successfully!\n";
}
void User::Help(){
    cout<<"\n\nEnter:";
    cout<<"\t1:\t To log-in.\n";
    cout<<"\t2:\t To exit and close the program.\n";
    cout<<"Option: \t";
    int instruction; cin>>instruction;
    switch (instruction){
        case 1:
            Login();
            break;
        case 2:
            close = true;
            break;
        default:
            cout<<"Invalid input.\n";
            break;
    }
    cout<<"\n\n";
}
void User::Change_password(){
    if(ID == lib_id){
        cout<<"\nCannot change the password of the Librarian.\n";
        return;
    }

    string pwd;
    cout<<"\nEnter the current password: "; cin>>pwd;
    if(Check_password(pwd)){
        cout<<"Enter the new password: "; cin>>pwd;
        this->password = pwd;
        cout<<"Password changed successfully!\n";
    }else{
        cout<<"Password does not match.\nPermission denied!\n";
    }
}
void User::Display(){
    cout<<"\n";
    cout<<"Name of the user:\t"<<name;
    cout<<"\nID of the user:\t\t"<<ID;
    cout<<"\n";
}
void User::Show_dues(){
    bool due = false;

    for(auto b : issued_books){
        due = true;
        b.Show_duedate();
    }
    if(!due){
        cout<<"\nNo issued books are due right now.\n";
    }
}

Professor::Professor(string n, string i, string p){
    name = n, ID = i, password = p;
    fineamt = 5; type = "p"; issue_days = 60;
}
void Professor::Help(){
    cout<<"\nEnter:";
    cout<<"\t1:\t Display all the books in catalogue.\n";
    cout<<"\t2:\t Display all the books issued to you.\n";
    cout<<"\t3:\t Show the books which are due.\n";
    cout<<"\t4:\t Check availability of a book.\n";
    cout<<"\t5:\t Request a book.\n";
    cout<<"\t6:\t See your outstanding fines.\n";
    cout<<"\t7:\t Clear your fines and return the books which are due.\n";
    cout<<"\t8:\t Return all your books.\n";
    cout<<"\t9:\t Change your name.\n";
    cout<<"\t10:\t Change your password.\n";
    cout<<"\t11:\t Display the day today.\n";
    cout<<"\t12:\t Log-out.\n";
    cout<<"\t13:\t Exit and close the program.\n";
    cout<<"Option: \t";

    int instruction; cin>>instruction;
    switch (instruction){
        case 1:
            BOOK_DB.Display();
            break;
        case 2:
            Show_books();
            break;
        case 3:
            Show_dues();
            break;
        case 4:
            BOOK_DB.Check_available();
            break;
        case 5:
            Request_Book();
            break;
        case 6:
            Calculate_fine();
            break;
        case 7:
            Clear_fine_amount();
            break;
        case 8:
            Return_books();
            break;
        case 9:
            Change_name();
            break;
        case 10:
            Change_password();
            break;
        case 11:
            Show_today();
            break;
        case 12:
            (*logged_in).Logout();
            break;
        case 13:
            close = true;
            break;
        default:
            cout<<"Invalid input.\n";
            break;
    }
    cout<<"\n";
}

Student::Student(string n, string i, string p){
    name = n, ID = i, password = p;
    fineamt = 2, max_books = 5; type = "s"; issue_days = 30;
}
void Student::Help(){
    cout<<"\nEnter:";
    cout<<"\t1:\t Display all the books in catalogue.\n";
    cout<<"\t2:\t Display all the books issued to you.\n";
    cout<<"\t3:\t Show the books which are due.\n";
    cout<<"\t4:\t Check availability of a book.\n";
    cout<<"\t5:\t Request a book.\n";
    cout<<"\t6:\t See your outstanding fines.\n";
    cout<<"\t7:\t Clear your fines and return the books which are due.\n";
    cout<<"\t8:\t Return all your books.\n";
    cout<<"\t9:\t Change your name.\n";
    cout<<"\t10:\t Change your password.\n";
    cout<<"\t11:\t Display the day today.\n";
    cout<<"\t12:\t Log-out.\n";
    cout<<"\t13:\t Exit and close the program.\n";
    cout<<"Option: \t";

    int instruction; cin>>instruction;
    switch (instruction){
        case 1:
            BOOK_DB.Display();
            break;
        case 2:
            Show_books();
            break;
        case 3:
            Show_dues();
            break;
        case 4:
            BOOK_DB.Check_available();
            break;
        case 5:
            Request_Book();
            break;
        case 6:
            Calculate_fine();
            break;
        case 7:
            Clear_fine_amount();
            break;
        case 8:
            Return_books();
            break;
        case 9:
            Change_name();
            break;
        case 10:
            Change_password();
            break;
        case 11:
            Show_today();
            break;
        case 12:
            (*logged_in).Logout();
            break;
        case 13:
            close = true;
            break;
        default:
            cout<<"Invalid input.\n";
            break;
    }
    cout<<"\n";
}

Librarian::Librarian(){
    name = "Librarian", ID = "lib", password = "books"; type = "l";
    USER_DB.Add(this);
}
void Librarian::Add_Book(){
    string t, a, isbn, pub;
    cout<<"\n\nPlease enter the ISBN number of the book: "; cin>>isbn;
    if(BOOK_DB.Contains(isbn)){
        cout<<"Book with this ISBN number already exists! Cannot add another book.\n";
        return;
    }
    cout<<"Please enter the Title of the book: "; cin>>t;
    cout<<"Please enter the Author of the book: "; cin>>a;
    cout<<"Please enter the Publication of the book: "; cin>>pub;
    BOOK_DB.Add(Book(t, a, isbn, pub));
    cout<<"Book added successfully!\n";
}
void Librarian::Add_User(){
    string n, i, p, t;
    cout<<"\n\nEnter the ID of the user: "; cin>>i;
    if(USER_DB.Contains(i)){
        cout<<"User with this ID already exists! Cannot add another User.\n";
        return;
    }
    cout<<"Enter the name of the user: "; cin>>n;
    cout<<"Enter the password of the user: "; cin>>p;
    cout<<"Enter the type of user (p/s): "; cin>>t;

    User* u;
    if(t[0] == 'p' || t[0] == 'P' || t == "Professor" || t == "professor" || t == "prof" || t == "Prof"){
        u = new Professor(n, i, p);
        USER_DB.Add(u);
    }else if(t[0] == 's' || t[0] == 'S' || t == "student" || t == "Student"){
        User* u = new Student(n, i, p);
        USER_DB.Add(u);
    }else{
        cout<<"Type error. User not added.\n";
        return;
    }
    cout<<"User added successfully!\n";
}
void Librarian::Delete_Book(){
    string isbn;
    cout<<"\nEnter the ISBN number of the book to delete: "; cin>>isbn;
    cout<<"\n";
    if(BOOK_DB.Contains(isbn)){
        BOOK_DB.Delete(isbn);
        cout<<"Book deleted successfully!\n";
    }else{
        cout<<"Book is not present in the database!\n";
    }
    cout<<"\n";
}
void Librarian::Delete_User(){
    string id;
    cout<<"\nEnter the ID of the user to delete: "; cin>>id;
    cout<<"\n";
    if(id == lib_id){
        cout<<"Cannot delete the Librarian!\n";
        return;
    }
    if(USER_DB.Contains(id)){
        User* u = USER_DB.Search(id);
        u->Clear_fine_amount();
        u->Return_books();
        USER_DB.Delete(id);
        cout<<"User deleted successfully.\n";
    }else{
        cout<<"User is not present in the database!\n";
    }
    cout<<"\n";
}
void Librarian::Update_Book(){
    cout<<"\nEnter the ISBN of the book: ";
    string isbn; cin>>isbn;
    if(BOOK_DB.Contains(isbn)){
        Book book = BOOK_DB.Search(isbn);
        if(book.issued_to == lib_id){
            book.Change_name();
            BOOK_DB.Update(book);
            cout<<"Book Title updated successfully!\n";
        }else{
            cout<<"Cannot change a book which is currently issued to someone.\n";
        }
    }else{
        cout<<"Book with this ISBN does not exist.\n";
    }
}
void Librarian::Update_User(){
    cout<<"\nEnter the ID of the user: ";
    string id; cin>>id;
    if(USER_DB.Contains(id)){
        auto user = USER_DB.Search(id);
        user->Change_name();
    }else{
        cout<<"This user does not exist.\n";
    }
}
void Librarian::Check_issue(){
    cout<<"\nEnter the ISBN of the book: ";
    string isbn; cin>>isbn;
    if(BOOK_DB.Contains(isbn)){
        Book book = BOOK_DB.Search(isbn);
        book.Issued();
    }else{
        cout<<"Book with this ISBN does not exist.\n";
    }
}
void Librarian::Change_day(){
    cout<<"\nEnter the new day (integer): ";
    int t; cin>>t;
    if(t < TODAY){
        cout<<"New day cannot be smaller than today!\n";
    }else if(t == TODAY){
        cout<<"This is already the day today.\n";
    }else{        
        cout<<"Day has been changed from "<<TODAY<<" to "<<t<<".\n";
        TODAY = t;
    }
}
void Librarian::User_Books(){
    cout<<"\nEnter the ID of the user: ";
    string id; cin>>id;
    cout<<"\n";
    if(USER_DB.Contains(id)){
        auto user = USER_DB.Search(id);
        user->Show_books();
    }else{
        cout<<"User with this ID does not exist.\n";
    }
}
void Librarian::Help(){
    cout<<"\nEnter:";
    cout<<"\t1:\t Display all the books.\n";
    cout<<"\t2:\t Display all the current users.\n";
    cout<<"\t3:\t Show books issued to a particular user.\n";
    cout<<"\t4:\t Check availability of a book.\n";
    cout<<"\t5:\t Add a book.\n";
    cout<<"\t6:\t Add a user.\n";
    cout<<"\t7:\t Delete a book.\n";
    cout<<"\t8:\t Delete a user.\n";
    cout<<"\t9:\t Change title of a book.\n";
    cout<<"\t10:\t Change name of a user.\n";
    cout<<"\t11:\t Check issuer of a book.\n";
    cout<<"\t12:\t Show the day today.\n";
    cout<<"\t13:\t Change the day today.\n";
    cout<<"\t14:\t Log-out.\n";
    cout<<"\t15:\t Exit and close the program.\n";
    cout<<"Option: \t";
    int instruction; cin>>instruction;
    switch (instruction){
        case 1:
            BOOK_DB.Display();
            break;
        case 2:
            USER_DB.Display();
            break;
        case 3:
            User_Books();
            break;
        case 4:
            BOOK_DB.Check_available();
            break;
        case 5:
            Add_Book();
            break;
        case 6:
            Add_User();
            break;
        case 7:
            Delete_Book();
            break;
        case 8:
            Delete_User();
            break;
        case 9:
            Update_Book();
            break;
        case 10:
            Update_User();
            break;
        case 11:
            Check_issue();
            break;
        case 12:
            Show_today();
            break;
        case 13:
            Change_day();
            break;
        case 14:
            (*logged_in).Logout();
            break;
        case 15:
            close = true;
            break;
        default:
            cout<<"Invalid input.\n";
            break;
    }
    cout<<"\n";
}

void User_DB::Add(User* u){
    database[u->ID] = u;
}
bool User_DB::Delete(User* u){
    if(u->ID == lib_id){
        cout<<"Cannot remove the librarian!\n";
        return false;
    }else{
        if(database.find(u->ID) != database.end()){
            cout<<"User not in database.\n";
            return false;
        }else{
            database.erase(u->ID);
            delete u;
            return true;
        }
    }
}
bool User_DB::Delete(string ID){
    if(ID == lib_id){
        cout<<"Cannot remove the librarian!\n";
        return false;
    }else{
        if(Contains(ID)){
            auto u = database[ID];
            database.erase(ID);
            delete u;
            return true;
        }else{
            cout<<"User not in database.\n";
            return false;
        }
    }
}
User* User_DB::Search(string ID){
    return database[ID];
}
bool User_DB::Contains(string ID){
    return database.find(ID) != database.end();
}
void User_DB::Update(string n, string i){
    if(Contains(i)){
        User* u = database[i];
        u->name = n;
        cout<<"Name changed successfully.\n";
    }else{
        cout<<"User with this ID does not exist.\n";
    }
}
void User_DB::Display(){
    cout<<"\n";
    for(auto u : database){
        auto user = *(u.second);
        user.Display();
    }
    cout<<"\n";
}

void Login(){
    cout<<"\n\n";
    string id, pwd;
    cout<<"Enter your ID: ";  cin>>id;
    if(USER_DB.Contains(id)){
        cout<<"Enter your password: "; cin>>pwd;
        auto u = USER_DB.Search(id);
        if(u->Check_password(pwd)){
            logged_in = USER_DB.Search(id);
        }else{
            cout<<"Incorrect password!\n";
        }
    }else{
        cout<<"\nUser with this ID does not exist.\n";
        cout<<"Please ask the librarian to add the ID to the database.\n";
    }
}

void Show_today(){
    cout<<"\nToday is "<<TODAY<<".\n";
}