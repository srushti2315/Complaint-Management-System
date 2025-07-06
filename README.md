# Complaint Management System

**A C++ console application developed for a Data Structures course at AASTMT-Aswan (Spring 2024). Manages customer complaints using manually implemented data structures: linked list, queue, stack, and priority queue. Supports Customer, Employee, and Admin roles with file-based data persistence.**

## Features

#### Customer Role:
- Submit complaints (name, phone, email, content).
- View complaint details by ID.
- Delete complaints by ID.
- Search complaint history by email.

#### Employee Role:
- View and reply to unreplied complaints.
- Add problem summaries (stored in stack).
- Search summaries by content.
- View urgent complaints (priority queue).

#### Admin Role:
- Manage employees (add, delete, view).
- View unreplied complaints and total count.
- Prioritize urgent complaints.

#### Data Structures:
##### Linked List: Employee management.
##### Queue (Linked List): Complaint storage.
##### Stack: Problem summaries.
##### Priority Queue (Sorted Linked List): Urgent complaints.
##### Persistence: Complaints saved to `complaint_data.txt.`

## Installation

#### 1. Clone the repository:
`git clone https://github.com/MideoHub/Complaint-Management-System.git`

####  2. Navigate to the project directory:
`cd Complaint-Management-System`

####  3. Compile the code

- Linux/macOS:
  ```g++ src/main.cpp -o complaint_system```

- Windows (MinGW):
  ```g++ src/main.cpp -o complaint_system.exe```

####  4. Run the program
- Linux/macOS:
  `./complaint_system`

- Windows:
  `complaint_system.exe`

## Usage
- Launch the program and select a role (Customer, Employee, Admin).
- Follow menu prompts to perform actions.
- Complaints are saved to `complaint_data.txt` when exiting Customer menu.

## Project Structure
- `src/main.cpp`: Main source code.
- `complaint_data.txt`: Stores complaint data.
- `.gitignore`: Excludes compiled binaries.

## Limitations
- Plain-text passwords (educational use only).
- Basic input validation (e.g., email format).
- Single-file implementation.

## Future Improvements
- Implement password hashing.
- Modularize code into headers.
- Add GUI.

-------------
#### Feel free to reach out with feedback or collaboration ideas!
