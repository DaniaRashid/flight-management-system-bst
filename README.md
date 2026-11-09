# Flight Management System (BST)

A C++ console program that stores flight records in a **Binary Search Tree (BST) ordered by Flight ID**. Flight IDs are unique, so a duplicate ID is rejected and no second node is created.

## BST rule

- Flight IDs smaller than a node go to its **left** subtree.
- Flight IDs greater than a node go to its **right** subtree.
- Inorder traversal (left, node, right) shows the flights in ascending Flight ID order.
- Deletion uses `transplant` and handles a leaf, a node with one child, and a node with two children (the inorder successor replaces it).

## Menu

```
========== FLIGHT MANAGEMENT SYSTEM ==========
1. Add New Flight
2. Search Flight by ID
3. Delete Flight by ID
4. Display All Flights (Ascending Flight ID)
5. Calculate Flight Duration and ETA
6. Surprise Feature (Update Flight)
7. Save Flights to File
8. Load Flights from File
0. Exit
```

| Option | What it does |
|---|---|
| 1 | Adds a flight. Rejects a duplicate Flight ID. |
| 2 | Finds a flight by ID, or shows a clear "not found" message. |
| 3 | Deletes a flight by ID. |
| 4 | Shows all flights in ascending Flight ID order. |
| 5 | ETA = Departure Time + Flight Duration, marked **On-Time**, **Early** or **Delayed** compared with the scheduled arrival. |
| 6 | **Surprise feature:** update a flight's ticket price, departure and arrival time, or status without deleting it. |
| 7 | Saves all flights to `flight.txt`. |
| 8 | Loads flights from `flight.txt` (flights are also loaded automatically when the program starts). |

## Input rules

- **Times:** type them as numbers, for example `930` for 09:30 or `1115` for 11:15. They are shown and saved as `HH:MM`.
- **Status when adding a flight:** type `1` (Scheduled), `2` (Delayed) or `3` (Cancelled).
- **Duration:** type the hours as a decimal, for example `1.75` for 1 hour 45 minutes.
- Do not use commas inside any field.

## File format

`flight.txt` has one flight per line, fields separated by commas:

```
101,Lahore,Karachi,PIA,09:30,11:15,1:45/1.75,18000,Scheduled
```

Order: `ID, Source, Destination, Airline, Departure, Arrival, Duration (h:mm/hours), Price, Status`.

The file is written in preorder, so loading it rebuilds the same tree shape.

## How to run

**Visual Studio:** create a C++ Console App, paste `main.cpp` into it, and put `flight.txt` in the project folder (next to the `.cpp` file). Press Ctrl+F5.

**Command line:** `g++ -std=c++17 -o flights main.cpp`, then `./flights` (run it from the folder that contains `flight.txt`).

## Test checklist

1. Insert at least five flights with unique IDs.
2. Search for an existing ID and a missing ID.
3. Delete a leaf, a node with one child, and a node with two children (reload `flight.txt` before each test).
4. Display all flights and check the IDs are ascending.
5. Save, close, run again and confirm the flights load.
6. Try a duplicate Flight ID and confirm it is rejected.
