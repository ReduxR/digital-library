#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//====== DATABASE STRUCTURE DEFINITION ======
/*
    Database structure:
    - Represents a single book with properties like ISBN, title, authors, year, genre, borrowed status, and borrow date.
    - Used to store book information in a library management system.
*/
typedef struct
{
   char isbn[15];     // ISBN of the book (13 digits)
   char nameBook[51]; // Title of the book (up to 50 characters)
   char authors[201]; // Authors of the book (comma-separated, up to 200 characters)
   int year;          // Publication year
   char genre[101];   // Genre(s) of the book (comma-separated, up to 100 characters)
   char date[11];     // Borrow date in DD-MM-YYYY format or "-" if not borrowed
   char borrowed[6];  // Borrow status ("true" or "false")
} Database;

//====== LOAD DATABASE FUNCTION ======
/*
    loadDatabase function:
    - Loads book records from "books.db" into a dynamically allocated array.
    - Dynamically resizes the array if needed.
    - Returns 1 on success, 0 on failure (file or memory issues).
*/
int loadDatabase(Database **database, int *currentSize)
{
   // Open file for reading
   FILE *file = fopen("books.db", "r");
   if (!file) 
   {
      fprintf(stderr, "Missing books.db. Creating a new one...\n");
      file = fopen("books.db", "w+");
   }

   // Initialize database array
   int maxSize = 10;
   *database = malloc(maxSize * sizeof(Database));
   if (!*database)
   {
      fprintf(stderr, "Error: Memory allocation failed.\n");
      fclose(file);
      return 0;
   }

   char line[256];
   *currentSize = 0;

   // Read and parse each line
   while (fgets(line, sizeof(line), file))
   {
      line[strcspn(line, "\n")] = '\0';

      // Resize array if needed
      if (*currentSize >= maxSize)
      {
         maxSize += 10;
         *database = realloc(*database, maxSize * sizeof(Database));
         if (!*database)
         {
            fprintf(stderr, "Error: Memory reallocation failed.\n");
            fclose(file);
            return 0;
         }
      }

      Database *book = &(*database)[*currentSize];
      char *token = strtok(line, "|");

      // Parse ISBN
      if (token)
      {
         strncpy(book->isbn, token, sizeof(book->isbn) - 1);
         book->isbn[sizeof(book->isbn) - 1] = '\0';
      }
      else
      {
         fprintf(stderr, "Error: Missing ISBN in line: %s\n", line);
         continue;
      }

      // Parse title
      token = strtok(NULL, "|");
      if (token)
      {
         strncpy(book->nameBook, token, sizeof(book->nameBook) - 1);
         book->nameBook[sizeof(book->nameBook) - 1] = '\0';
      }
      else
      {
         fprintf(stderr, "Error: Missing book name in line: %s\n", line);
         continue;
      }

      // Parse authors
      token = strtok(NULL, "|");
      if (token)
      {
         strncpy(book->authors, token, sizeof(book->authors) - 1);
         book->authors[sizeof(book->authors) - 1] = '\0';
      }
      else
      {
         fprintf(stderr, "Error: Missing authors in line: %s\n", line);
         continue;
      }

      // Parse year
      token = strtok(NULL, "|");
      if (token)
      {
         book->year = atoi(token);
      }
      else
      {
         fprintf(stderr, "Error: Missing year in line: %s\n", line);
         continue;
      }

      // Parse genre
      token = strtok(NULL, "|");
      if (token)
      {
         strncpy(book->genre, token, sizeof(book->genre) - 1);
         book->genre[sizeof(book->genre) - 1] = '\0';
      }
      else
      {
         fprintf(stderr, "Error: Missing genre in line: %s\n", line);
         continue;
      }

      // Parse borrowed status
      token = strtok(NULL, "|");
      if (token)
      {
         strncpy(book->borrowed, token, sizeof(book->borrowed) - 1);
         book->borrowed[sizeof(book->borrowed) - 1] = '\0';
      }
      else
      {
         fprintf(stderr, "Error: Missing borrowed status in line: %s\n", line);
         continue;
      }

      // Parse borrow date
      token = strtok(NULL, "|");
      if (token)
      {
         strncpy(book->date, token, sizeof(book->date) - 1);
         book->date[sizeof(book->date) - 1] = '\0';
      }
      else
      {
         fprintf(stderr, "Error: Missing date in line: %s\n", line);
         continue;
      }

      (*currentSize)++;
   }

   fclose(file);
   return 1;
}

//====== ADD BOOK FUNCTION ======
/*
    addBook function:
    - Adds a new book to the database.
    - Validates ISBN (13 digits), title (≤50 characters), and year (≤2025).
    - Saves the updated database to "books.db".
*/
void addBook(Database **database, int *currentSize, int *maxSize)
{
   // Resize database if needed
   if (*currentSize >= *maxSize)
   {
      *maxSize += 10;
      *database = realloc(*database, *maxSize * sizeof(Database));
      if (!*database)
      {
         fprintf(stderr, "Error! Memory reallocation failed.\n");
         return;
      }
   }

   Database *newBook = &(*database)[*currentSize];
   getchar();

   // Input and validate ISBN
   while (1)
   {
      printf("----------------------\n");
      printf("Adding a new book:\n");
      printf("Enter ISBN (13 digits): ");
      fgets(newBook->isbn, sizeof(newBook->isbn), stdin);
      newBook->isbn[strcspn(newBook->isbn, "\n")] = '\0';
      if (strlen(newBook->isbn) == 13)
      {
         break;
      }
      else if (strlen(newBook->isbn) < 13)
      {
         printf("\nError: ISBN must contain exactly 13 digits! Too few digits.\n");
      }
      else
      {
         printf("\nError: ISBN must contain exactly 13 digits! Too many digits.\n");
         int ch;
         while ((ch = getchar()) != '\n' && ch != EOF)
            ;
      }
   }

   // Input and validate title
   while (1)
   {
      printf("Enter book title: ");
      fgets(newBook->nameBook, sizeof(newBook->nameBook), stdin);
      newBook->nameBook[strcspn(newBook->nameBook, "\n")] = '\0';
      if (strlen(newBook->nameBook) <= 50)
      {
         break;
      }
      else
      {
         printf("\nError: Title of the book is bigger then maximum of allowed letters!(50)\n");
         int ch;
         while ((ch = getchar()) != '\n' && ch != EOF)
            ;
      }
   }

   // Input authors
   printf("Enter author(s) (separated by commas): ");
   fgets(newBook->authors, sizeof(newBook->authors), stdin);
   newBook->authors[strcspn(newBook->authors, "\n")] = '\0';

   // Input and validate year
   while (1)
   {
      printf("Enter year of publication: ");
      scanf("%d", &newBook->year);
      if (newBook->year <= 2025)
      {
         getchar();
         break;
      }
      else
      {
         printf("A year you've entered cant be bigger then current year! Try again.\n");
      }
   }

   // Input genre
   printf("Enter genre(s) (also separated by commas): ");
   fgets(newBook->genre, sizeof(newBook->genre), stdin);
   newBook->genre[strcspn(newBook->genre, "\n")] = '\0';

   // Set default values for borrowed status and date
   strcpy(newBook->borrowed, "false");
   strcpy(newBook->date, "-");

   (*currentSize)++;
   printf("Book added successfully!\n");

   // Save database to file
   FILE *file = fopen("books.db", "w");
   if (!file)
   {
      printf("Not possible to open a file!\n");
      return;
   }

   for (int i = 0; i < *currentSize; i++)
   {
      fprintf(file, "%s|%s|%s|%d|%s|%s|%s\n",
              (*database)[i].isbn, (*database)[i].nameBook, (*database)[i].authors,
              (*database)[i].year, (*database)[i].genre, (*database)[i].borrowed,
              (*database)[i].date);
   }
   fclose(file);
}

//====== DELETE BOOK FUNCTION ======
/*
    deleteBook function:
    - Removes a book from the database by index.
    - Shifts remaining books to fill the gap.
    - Saves the updated database to "books.db".
*/
void deleteBook(Database *database, int *currentSize, int index)
{
   // Validate index
   if (index < 0 || index >= *currentSize)
   {
      printf("Invalid book index.\n");
      return;
   }

   // Shift books to remove the selected one
   for (int i = index; i < *currentSize - 1; i++)
   {
      database[i] = database[i + 1];
   }

   (*currentSize)--;

   printf("Book deleted successfully!\n");

   // Save updated database to file
   FILE *file = fopen("books.db", "w");
   if (!file)
   {
      printf("Error: Unable to open 'books.db' for writing.\n");
      return;
   }
   for (int i = 0; i < *currentSize; i++)
   {
      fprintf(file, "%s|%s|%s|%d|%s|%s|%s\n",
              database[i].isbn, database[i].nameBook, database[i].authors,
              database[i].year, database[i].genre, database[i].borrowed,
              database[i].date);
   }
   fclose(file);
}

//====== BORROW BOOK FUNCTION ======
/*
    borrowBook function:
    - Marks a book as borrowed by ISBN.
    - Sets borrow date to the current date.
    - Checks if the book exists and is not already borrowed.
*/
void borrowBook(Database *database, int *currentSize, const char *isbn)
{
   for (int i = 0; i < *currentSize; i++)
   {
      if (strcmp(database[i].isbn, isbn) == 0)
      {
         if (strcmp(database[i].borrowed, "false") == 0)
         {
            strcpy(database[i].borrowed, "true");
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(database[i].date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
            printf("Book '%s' has been borrowed successfully!\n", database[i].nameBook);
         }
         else
         {
            printf("This book is already borrowed.\n");
         }
         return;
      }
   }
   printf("Book with ISBN %s not found.\n", isbn);
}

//====== SHOW BORROWED BOOKS FUNCTION ======
/*
    showBorrowedBooks function:
    - Displays all books currently marked as borrowed.
    - Shows ISBN, title, authors, and borrow date.
*/
void showBorrowedBooks(Database *database, int currentSize)
{
   int found = 0;
   printf("\nBooks currently borrowed:\n");

   for (int i = 0; i < currentSize; i++)
   {
      if (strcmp(database[i].borrowed, "true") == 0)
      {
         found = 1;
         printf("ISBN: %s\n", database[i].isbn);
         printf("Title: %s\n", database[i].nameBook);
         printf("Author(s): %s\n", database[i].authors);
         printf("Borrowed on: %s\n\n", database[i].date);
      }
   }
   if (!found)
   {
      printf("No books are currently borrowed.\n");
   }
}

//====== SAVE DATABASE FUNCTION ======
/*
    saveDatabase function:
    - Saves all books in the database to the specified file.
    - Overwrites the file with the current state of the database.
*/
void saveDatabase(const char *filename, Database *database, int currentSize)
{
   FILE *file = fopen(filename, "w");
   if (!file)
   {
      printf("Error! Unable to open file for writing.\n");
      return;
   }
   for (int i = 0; i < currentSize; i++)
   {
      fprintf(file, "%s|%s|%s|%d|%s|%s|%s\n",
              database[i].isbn, database[i].nameBook, database[i].authors,
              database[i].year, database[i].genre, database[i].borrowed,
              database[i].date);
   }

   fclose(file);
}

//====== TO LOWERCASE FUNCTION ======
/*
    toLowerCase function:
    - Converts a string to lowercase for case-insensitive comparisons.
    - Skips newline and carriage return characters.
*/
void toLowerCase(char *str)
{
   for (int i = 0; str[i]; i++)
   {
      if (str[i] != '\n' && str[i] != '\r')
      {
         str[i] = tolower(str[i]);
      }
   }
}

//====== FIND BOOK BY ISBN FUNCTION ======
/*
    findBookByISBN function:
    - Searches for a book by ISBN and displays its details.
    - Offers options to borrow or delete the book, or return to the search/main menu.
    - Sets exitToMain flag to 1 if returning to the main menu.
*/
void findBookByISBN(Database *database, int currentSize, const char *isbn, int *exitToMain)
{
   int bookFound = 0;
   for (int i = 0; i < currentSize; i++)
   {
      if (strcmp(database[i].isbn, isbn) == 0)
      {
         Database *selectedBook = &database[i];

         // Display book details
         printf("\nBook found:\n");
         printf("ISBN: %s\n", selectedBook->isbn);
         printf("Title: %s\n", selectedBook->nameBook);
         printf("Authors: %s\n", selectedBook->authors);
         printf("Year: %d\n", selectedBook->year);
         printf("Genre: %s\n", selectedBook->genre);
         printf("Borrowed: %s\n", selectedBook->borrowed);
         printf("Date: %s\n", selectedBook->date);
         printf("-----------------------\n");
         printf("What would you like to do with this book?\n");
         printf("1. Borrow the book\n");
         printf("2. Delete the book\n");
         printf("3. Back to search menu\n");
         printf("4. Back to main menu\n");

         int action;
         scanf("%d", &action);
         getchar();

         // Handle user action
         switch (action)
         {
         case 1:
            borrowBook(database, &currentSize, selectedBook->isbn);
            saveDatabase("books.db", database, currentSize);
            break;
         case 2:
            deleteBook(database, &currentSize, i);
            saveDatabase("books.db", database, currentSize);
            break;
         case 3:
            printf("Returning to search menu...\n");
            return;
         case 4:
            printf("Returning to main menu...\n");
            *exitToMain = 1;
            return;
         default:
            printf("Invalid action. Returning to search menu...\n");
            return;
         }

         bookFound = 1;
         break;
      }
   }
   if (!bookFound)
   {
      printf("Book with ISBN %s not found.\n", isbn);
   }
}

//====== FIND BOOK BY TITLE FUNCTION ======
/*
    findBookByTitle function:
    - Searches for books by title (case-insensitive, partial match).
    - Displays matching books and allows the user to select one for actions.
    - Offers options to borrow, delete, or return to the search/main menu.
    - Sets exitToMain flag to 1 if returning to the main menu.
*/
void findBookByTitle(Database *database, int currentSize, const char *title, int *exitToMain)
{
   int foundIndexes[100];
   int foundCount = 0;
   char lowerTitle[50];
   strcpy(lowerTitle, title);
   toLowerCase(lowerTitle);

   // Find matching books
   for (int i = 0; i < currentSize; i++)
   {
      char lowerName[50];
      strcpy(lowerName, database[i].nameBook);
      toLowerCase(lowerName);

      if (strstr(lowerName, lowerTitle) != NULL)
      {
         printf("%d. %s (ISBN: %s)\n", foundCount + 1, database[i].nameBook, database[i].isbn);
         foundIndexes[foundCount++] = i;
      }
   }

   // Handle no matches
   if (foundCount == 0)
   {
      printf("No books found with title containing: %s\n", title);
      return;
   }

   // Prompt for book selection
   printf("\nChoose a book by index (0 to cancel): ");
   int choice;
   scanf("%d", &choice);
   getchar();

   if (choice < 1 || choice > foundCount)
   {
      printf("Returning to search menu...\n");
      return;
   }

   int selectedBookIndex = foundIndexes[choice - 1];
   Database *selectedBook = &database[selectedBookIndex];

   // Display selected book and options
   printf("\nYou selected: %s (ISBN: %s)\n", selectedBook->nameBook, selectedBook->isbn);
   printf("----------------------\n");
   printf("Choice menu:\n");
   printf("1. Borrow a book\n");
   printf("2. Delete a book\n");
   printf("3. Back to search menu\n");
   printf("4. Back to main menu\n");
   printf("Choose what you want to do next: ");

   int action;
   scanf("%d", &action);
   getchar();

   // Handle user action
   switch (action)
   {
   case 1:
      borrowBook(database, &currentSize, selectedBook->isbn);
      saveDatabase("books.db", database, currentSize);
      break;
   case 2:
      deleteBook(database, &currentSize, selectedBookIndex);
      saveDatabase("books.db", database, currentSize);
      break;
   case 3:
      printf("Returning to search menu...\n");
      break;
   case 4:
      printf("Returning to main menu...\n");
      *exitToMain = 1;
      break;
   default:
      printf("Invalid action. Returning to search menu...\n");
   }
}

//====== RETURN BOOK FUNCTION ======
/*
    returnBook function:
    - Marks a book as returned by ISBN.
    - Resets borrow status to "false" and date to "-".
    - Checks if the book exists and is borrowed.
*/
void returnBook(Database *database, int currentSize, const char *isbn)
{
   for (int i = 0; i < currentSize; i++)
   {
      if (strcmp(database[i].isbn, isbn) == 0)
      {
         if (strcmp(database[i].borrowed, "true") == 0)
         {
            strcpy(database[i].borrowed, "false");
            strcpy(database[i].date, "-");
            printf("Book '%s' has been returned successfully!\n", database[i].nameBook);
         }
         else
         {
            printf("This book was not borrowed.\n");
         }
         return;
      }
   }
   printf("Book with ISBN %s not found.\n", isbn);
}

//====== MAIN FUNCTION ======
/*
    main function:
    - Entry point of the program.
    - Loads the database, displays a menu, and handles user interactions.
    - Supports adding, searching, returning books, and exiting the program.
*/
int main(void)
{
   Database *database = NULL;
   int currentSize = 0;
   int maxSize = 50;

   // Load database
   printf("Loading database...\n");
   if (!loadDatabase(&database, &currentSize))
   {
      printf("Error: Could not load the database. Exiting...\n");
      free(database);
      return 1;
   }

   int exitToMain = 0;
   printf("Database loaded successfully. Total books: %d\n", currentSize);
   printf("Hello! Please, choose what you want to do: \n");

   // Main menu loop
   while (1)
   {
      exitToMain = 0;
      printf("----------------------\n");
      printf("Main menu:\n");
      printf("1. Find a book\n");
      printf("2. Add a book\n");
      printf("3. Return a book\n");
      printf("4. End the program\n");
      printf("----------------------\n");
      printf("Make your choice: ");

      int choice;
      scanf("%d", &choice);

      // Validate menu choice
      if (choice < 1 || choice > 4)
      {
         printf("You can't enter a number greater than 4 or less than 1! Try again!\n");
         continue;
      }

      switch (choice)
      {
      case 1:
      {
         // Search menu loop
         while (1)
         {
            printf("----------------------\n");
            printf("Search menu:\n");
            printf("1. Find the book by its title\n");
            printf("2. Find by the ISBN-13\n");
            printf("3. Show the borrowed books\n");
            printf("4. Return back to main menu\n");
            printf("----------------------\n");
            printf("Make your choice: ");
            int subChoice;
            scanf("%d", &subChoice);
            getchar();

            if (subChoice == 1)
            {
               char title[50];
               printf("----------------------\n");
               printf("Please enter a title of the book: ");
               fgets(title, sizeof(title), stdin);
               printf("Books found:\n");
               printf("----------------------\n");
               title[strcspn(title, "\n")] = '\0';
               findBookByTitle(database, currentSize, title, &exitToMain);
            }
            else if (subChoice == 2)
            {
               char isbn[15];
               printf("----------------------\n");
               printf("Enter ISBN to search: ");
               fgets(isbn, sizeof(isbn), stdin);
               isbn[strcspn(isbn, "\n")] = '\0';
               findBookByISBN(database, currentSize, isbn, &exitToMain);
            }
            else if (subChoice == 3)
            {
               printf("----------------------\n");
               printf("Showing borrowed books...\n");
               showBorrowedBooks(database, currentSize);
            }
            else if (subChoice == 4)
            {
               printf("Going back to the main menu...\n");
               break;
            }
            else
            {
               printf("Invalid choice! Try again.\n");
            }
            if (exitToMain)
            {
               break;
            }
         }
         break;
      }
      case 2:
         addBook(&database, &currentSize, &maxSize);
         saveDatabase("books.db", database, currentSize);
         break;
      case 3:
      {
         char isbn[15];
         printf("----------------------\n");
         printf("Enter the ISBN of the book to return: ");
         getchar();
         fgets(isbn, sizeof(isbn), stdin);
         isbn[strcspn(isbn, "\n")] = '\0';
         returnBook(database, currentSize, isbn);
         saveDatabase("books.db", database, currentSize);
         break;
      }
      case 4:
         printf("----------------------\n");
         printf("Program was ended. Have a nice day!\n");
         free(database);
         return 0;
      }
   }
}