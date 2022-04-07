## Running
To run the program, simply run,
```bash
./scripts.sh
```
Or run the two commands separately,
```bash
g++ -o main main.cpp classes.cpp
./main
```

## Structure
+ The declarations of the classes and their functions, and some macros reside in [classes.hpp](./classes.hpp)
+ The definitions of those declared in [classes.hpp](./classes.hpp) are written in [classes.cpp](./classes.cpp)
+ [classes.hpp](./classes.hpp) has then been included in [main.cpp](./main.cpp) and a simple function has been written to deploy the program.

## Conventions
+ Each time the program is ran, the databases are initialized empty. The program does not write to nor reads from any file or csv.
+ Initially, only a librarian exists in the user database. The ID of this user is `lib` and the password is `books`.<br> No books exist in the database either, they need to be added by the librarian.
+ Since the file structure is so simple, a `Makefile` has been omitted.
+ All the inputs are taken from `stdin` into strings, so do not put spaces in the input, rather use underscores if absolutely necessary.
+ Classes `User_DB`, `Book_DB` and `Librarian` have a single object. No more copies of `Librarian` can be made from the command line. All these variables (`USER_DB`, `BOOK_DB`, `librarian`) are global, some other global variables/ MACROs include `lib_id`, the ID of the librarian, `close` to close the program completely, `logged_in` and `dummy` for tracing the state of the user currently logged in.
+ The librarian cannot be deleted at any point in the program.
+ To simulate a dating system, a global `int` variable `TODAY` has 
+ At any stage, the user can exit and close the program completely.
+ All the functions asked for in the document, have been implemented. More functions were needed to make them feasible and have thus been written.
+ `Login` and `Logout` have both been used as global functions, although `Logout` is a member function of `User` class.
+ Each user is equipped with a `Help` function which prints all the available options to the user. It was overloaded for the child classes by declaring it as `virtual` in the `User` class.
+ Comments have been written only in the [header](./classes.hpp).