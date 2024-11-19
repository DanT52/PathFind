#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <set>
#include <queue>
#include <vector>

// I did the A* algorithm

using namespace std;

#define MAX_STRINGS 50
#define MAX_LENGTH 100
#define INF 1000000

struct Point {
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // custom comperator overloads
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
    double distance; //real distance
    double h_dist;  // heuristic distance
    int cost;       // cost to enter tile
    Point parent;   // which tile we came from
    Point self;     // coords of self

    Node(double distance = 0, double h_dist = INF, int cost = 0, Point parent = Point(), Point self = Point()) 
        : distance(distance), h_dist(h_dist), cost(cost), parent(parent), self(self) {}
};


// get manhatten distance
double heuristic(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}


int main() {
    char strings[MAX_STRINGS][MAX_LENGTH + 1];     // preallocate a 2D array
    Node values[MAX_STRINGS][MAX_LENGTH / 2 + 1]; // length / 2 because initial has spaces between each char.

    int count = 0;      // number of strings
    int char_count = 0; // number of characters in the current string
    int node_count = 0; // current node we are on
    int c;              // Character read from input

    Node *start_point;
    Node *end_point;
    set<Point> visited;

    // custom compare function for q
    auto compare = [](const Node& a, const Node& b) {
        return a.h_dist > b.h_dist; // smallest h_dist has highest priority
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
            // reset counts for next string
            char_count = 0; 
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
            
            values[count][node_count] = Node(INF, INF, cost, Point(-1, -1), Point(count, node_count));
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

    int rows = count;
    int cols = char_count;

    start_point->distance = 0;
    start_point->h_dist = 0;

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
            Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1),    // Up, Down, Left, Right
            Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1)   // Diagonals
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
                    double newDist = n.distance + (isDiagonal ? (j.cost * 1.5) : j.cost);

                    // update if new distance smaller than current
                    if (newDist < j.distance) {
                        j.distance = newDist;
                        j.h_dist = heuristic(j.self, end_point->self) + newDist;
                        j.parent = n.self;
                        q.push(j);
                    }
                }
            }
        }



    }
    
    // go though the values from end point by parent
    // print the path onto the input table
    Point curr = end_point->parent;
    while (!(curr == start_point->self)){
        strings[curr.x][curr.y * 2] = '*';
        curr = values[curr.x][curr.y].parent;
    }

    //print input table with path on it
    for (int i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }
    printf("Total cost: %.1f\n", end_point->distance);

    return 0;
}