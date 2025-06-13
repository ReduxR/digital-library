# Digital Library

A simple console-based library system written in C for managing a collection of books. The program uses a local text file (`books.db`) as its database.

## 🚀 Features

- Load and save book records from/to a `books.db` file
- Add new books with details like ISBN, title, authors, year, and genre
- Mark books as borrowed or returned (with date)
- Search for books by ISBN (exact match) or title (partial match)
- If the database file is missing, it will be created automatically

## ▶️ How to Run

```bash
gcc main.c -o library
./library
```

The program will attempt to load `books.db`. If the file doesn't exist, it will be created when you save data.

## 🧑‍💻 How to Use

When you run the program, a simple text menu appears:

- `[1]` Find the book
- `[2]` Add the book
- `[3]` Return the book
- `[4]` End the program

### 🔍 Finding a Book

You can search in two ways:

- **By ISBN** — must match exactly  
- **By Title** — partial match works (e.g., typing `"The"` will return all books with `"The"` in the title)

If multiple books match your input, you’ll be shown a list to pick from:

```
Books found:
----------------------
1. The C Programming Language
2. The Hobbit

Choose a book by index (0 to cancel): __
```

### **➕ Adding a Book**

Choose option `[2]` to add a new book to the database. You’ll be asked to enter:

- ISBN
- Title
- Author(s)
- Year
- Genre

After successful entry, the book will be saved in the database for further interactions.

### **🔁 Returning a Book**

Option `[3]` lets you return a previously borrowed book. You’ll need to:

- Search for the book by the ISBN-13
- If it’s marked as borrowed, the status will be updated to false and the return date will be reset.

### **❌ Ending the Program**

Option `[4]` exits the program. If you’ve made changes (added or returned books), they will be saved automatically to books.db.

## 📁 Database File Format

The `books.db` file stores each book on a separate line, with fields separated by `|`:

```
ISBN|Title|Authors|Year|Genre|Borrowed (true/false)|Date (DD-MM-YYYY or "-")
```

Example:

```
9780131101630|The C Programming Language|Kernighan, Ritchie|1978|Programming|false|-
```

## 🔧 Future Plans

- Improve whole logic and code structure
- Break code into modules (`db.c`, `ui.c`, etc.)
- Improve input validation
- Add more search filters (e.g., by year or genre)
- Enhance menu usability

## 📄 License

MIT License — whole project is free to use and modify.