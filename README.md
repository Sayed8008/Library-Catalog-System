# Library Management System

## Team Members

- **MD. Abu Sayed** (ID: 20255103162)  
- **Majarul Islam Biplob** (ID: 20255103174)  
- **Mehedi Hasan Shuvro** (ID: 20255103188)  


## Overview

This project is a **console-based Library Management System** developed using **C++**. The system is designed to efficiently manage book searching, borrowing, returning, and user operations.

Unlike traditional systems that rely on databases, this project focuses on implementing **core data structures** to handle operations efficiently. It simulates real-world library behavior while demonstrating the practical application of data structures.


## Project Objective

The primary objective of this project is to demonstrate how multiple data structures can be integrated to solve real-world problems efficiently.

Key objectives include:

- Implement efficient book searching using Trie  
- Manage book and user data using Hash Maps  
- Handle dynamic data using Vector  
- Ensure fairness in waitlists using Queue  
- Implement undo functionality using Stack  


## Features

- Prefix-based book search  
- Add books to a temporary selection (bucket)  
- Borrow books with user authentication  
- Return borrowed books  
- Undo last selection (bucket undo)  
- Undo last borrow operation  
- Waiting list system for unavailable books  
- Add new books to the library  
- View user’s borrowed books  


## Data Structures Used

### Trie (Prefix Tree)
Used for efficient prefix-based searching of book names.

- Time Complexity: O(L)  
- Provides fast search without scanning all books  


### Hash Map (unordered_map)
Used for storing and accessing data efficiently.

- Book storage (Book ID → Name, Stock)  
- User data (Phone → Credentials)  
- Borrow records (User → Books)  

- Time Complexity: O(1) average  


### Vector
Used for dynamic storage.

- Bucket (temporary selected books)  
- Search results  

- Provides easy indexing and resizing  


### Queue
Used for managing waiting lists.

- First-In-First-Out (FIFO) ensures fairness  


### Stack
Used for undo operations.

- Last-In-First-Out (LIFO) allows reversing actions  


## Data Structure Summary

| Feature        | Data Structure | Reason                     |
|----------------|---------------|----------------------------|
| Search         | Trie          | Efficient prefix search    |
| Book Storage   | Hash Map      | Fast lookup                |
| User Data      | Hash Map      | Direct access              |
| Selection      | Vector        | Dynamic storage            |
| Waitlist       | Queue         | Fair processing (FIFO)     |
| Undo           | Stack         | Reversible actions (LIFO)  |


## System Workflow

### Search Flow
User input → Trie → Matching books → Stored in vector  

### Selection Flow
User selects book → Added to vector (bucket)  

### Undo Flow
Stack removes last selected book  

### Borrow Flow
Bucket → Authentication → Update stock → Assign books  
If unavailable → Added to queue (waitlist)  

### Return Flow
Book removed from user → Stock updated → Queue processed  


## Time Complexity Analysis

| Operation    | Complexity |
|--------------|-----------|
| Search       | O(L)      |
| Insert Book  | O(L)      |
| Borrow       | O(1)      |
| Return       | O(n)      |
| Undo         | O(1)      |
| User Lookup  | O(1)      |


## How to Run

### Compile
```bash
g++ main.cpp -o library



## File Information
- main.cpp → Main source code file
- Additional files → Supporting modules (if any)
## Conclusion

This project demonstrates how multiple data structures can be integrated to build an efficient and realistic system. Each structure is used based on its strength, ensuring optimized performance and clear logical design.

The system enhances understanding of data structures, algorithm efficiency, and real-world problem solving using C++.

## Report

 The full project report is included as:
Library_Management_System_Report.pdf
