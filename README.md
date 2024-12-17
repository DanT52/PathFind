# PathFind

This is a C++ implementation of the A* algorithm for pathfinding on a grid. The program reads a grid from standard input, where each cell has a specific cost associated with it, and finds the shortest path from a start point to an end point.

## Features

- Implements the A* algorithm for efficient pathfinding.
- Supports diagonal movement with increased cost.
- Handles different terrain costs.
- Outputs the grid with the path marked and the total cost of the path.

## Grid Representation

The grid is represented as a 2D array of characters. Each character represents a different type of terrain with an associated cost:

- `.`: Cost 1
- `,`: Cost 2
- `o`: Cost 3
- `=`: Cost 50
- `0`: Start point (Cost 0)
- `2`: End point (Cost 0)
- Any other character: Infinite cost (impassable)

## How to Use

1. Compile the program using a C++ compiler:
    ```sh
    g++ -o pathfind main.cpp
    ```

2. Run the program and provide the grid input through standard input (e.g., by redirecting a file or typing directly):
    ```sh
    ./pathfind < input.txt
    ```

3. The program will output the grid with the path marked by `*` and the total cost of the path.

## Example

### Input 
```
, , , , , , , , .
. . . . . . . . . 
. . . . = = = = ,
. 1 . . . 0 . = . 
. = = = = 1 . = , 
. . . . = = = = ,
, . . . , , , , , 
, . . . , , , , ,
, 2 3 3 , , , , ,
```
### Output
```
, , , , , , , , .
. . . . . . . . .
. * . . = = = = ,
* 1 * * * 0 . = .
* = = = = 1 . = ,
* . . . = = = = ,
, * . . , , , , ,
, * . . , , , , ,
, 2 3 3 , , , , ,
Total cost: 10.5
```
