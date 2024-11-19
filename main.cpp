#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <set>
#include <queue>
#include <vector>


// . = 1
// , = 2
// o = 3
// = = 50
// 1 = inf
// 3 = inf
// 0 = 0
// 2 = 0

// 0 is the start_point
// 2 is the end_point


using namespace std;

#define MAX_STRINGS 50
#define MAX_LENGTH 100
//#define INF numeric_limits<int>::max() 
#define INF 1000000

struct Point {
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // custom compre for the set
    bool operator<(const Point& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Node {
    double distance; // Change from int to double
    double h_dist; // New h_dist property
    int cost;
    Point parent;
    Point self;

    Node(double distance = 0, double h_dist = INF, int cost = 0, Point parent = Point(), Point self = Point()) 
        : distance(distance), h_dist(h_dist), cost(cost), parent(parent), self(self) {}
};


// get manhatten distance
double heuristic(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}


int main() {
    char strings[MAX_STRINGS][MAX_LENGTH + 1]; // Preallocate a 2D array

    Node values[MAX_STRINGS][MAX_LENGTH / 2 + 1];

    int count = 0; // Number of strings
    int char_count = 0; // Number of characters in the current string
    int node_count = 0;
    int c; // Character read from input

    Node *start_point;
    Node *end_point;

    set<Point> visited;

    // custom compare function for q
    auto compare = [](const Node& a, const Node& b) {
        return a.distance > b.distance; // smallest distance has highest priority
    };
    priority_queue<Node, std::vector<Node>, decltype(compare)> q(compare);

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            // get rid of trailing spaces
            while (char_count > 0 && strings[count][char_count - 1] == ' ') {
                char_count--;
            }
            strings[count][char_count] = '\0'; // terminate the current string
            count++;
            char_count = 0; // reset character count for the next string
            node_count = 0;
            continue;
        
        } else {
            strings[count][char_count] = c; 
            char_count++;
        } 
        if (c != ' ') {

            int cost;
            switch (c) {
                case '.':
                    cost = 1;
                    break;
                case ',':
                    cost = 2;
                    break;
                case 'o':
                    cost = 3;
                    break;
                case '=':
                    cost = 50;
                    break;
                case '0':
                    cost = 0;
                    start_point = &values[count][node_count];
                    break;
                case '2':
                    cost = 0;
                    end_point = &values[count][node_count];
                    break;
                default:
                    cost = INF; //else
                    break;
            }
            
            values[count][node_count] = Node(INF, INF, cost, Point(-1, -1), Point(count, node_count)); // Update constructor call
            node_count++;
        }
        
    }
    // put in the last null terminator
    if (char_count > 0 && count < MAX_STRINGS) {
        while (char_count > 0 && strings[count][char_count - 1] == ' ') {
                char_count--;
        }
        strings[count][char_count] = '\0';
        count++;
    }



    //debuggin print stuff
    // // print what we read in
    // for (int i = 0; i < count; i++) {
    //     printf("%s\n", strings[i]); // Print each string
    // }
    // // print the start and end points
    // printf("Start Point: (%d, %d)\n", start_point->self.x, start_point->self.y);
    // printf("End Point: (%d, %d)\n", end_point->self.x, end_point->self.y);

    // // print the grid size
    // printf("Grid Size: %d x %d\n", count, char_count);


    int rows = count;
    int cols = char_count;

    start_point->distance = 0;
    start_point->h_dist = heuristic(start_point->self, end_point->self); // Initialize h_dist for start_point
    q.push(*start_point);

    while (!q.empty()) {
        // first get the top node from the queue
        Node n = q.top();
        q.pop();

        // we got to the destination!
        if (n.self == end_point->self){
            break;
        }

        //check if it is already visited
        if (visited.find(n.self) != visited.end()) {
            continue;    // if visited the move on
        }

        // add n to the visited set
        visited.insert(n.self);

        // possible ways can move
        vector<Point> moves = {
            Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1), // Up, Down, Left, Right
            Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1) // Diagonals
        };

        // loop though possible moves
        for (const auto& move : moves) { 
            int x = n.self.x + move.x;
            int y = n.self.y + move.y;

            // now only for moves that are within the board
            if (x >= 0 && x < rows && y >= 0 && y < cols) {
                Node& j = values[x][y]; // get the neighbor
                // make sure its not visited
                if (visited.find(j.self) == visited.end()) { 
                    bool isDiagonal = (move.x != 0 && move.y != 0);
                    double newDist = n.distance + (isDiagonal ? (j.cost * 1.5) : j.cost); // Ensure newDist is calculated as double
                    // update if new distance smaller than current
                    if (newDist < j.distance) {
                        j.distance = newDist;
                        j.h_dist = heuristic(j.self, end_point->self); // Update h_dist for neighbor
                        j.parent = n.self;
                        q.push(j);
                    }
                }
            }
        }



    }
    
    //go though the values, and print the path onto the input table

    Point curr = end_point->parent;

    while (!(curr == start_point->self)){
        strings[curr.x][curr.y * 2] = '*';
        curr = values[curr.x][curr.y].parent;
    }

        // print what we read in
    for (int i = 0; i < count; i++) {
        printf("%s\n", strings[i]); // Print each string
    }
    printf("Total cost: %.1f\n", end_point->distance);

    return 0;
}