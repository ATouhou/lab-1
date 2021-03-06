#include "includes.h"
#include "time.h"

// This is the function the students must fill out.
// They are given their location and the location of the goal.
// The default behavior is to return SOUTH.  This is nonsense.

Direction decide(Position& me, Position& theGoal, unsigned long North, unsigned long East, unsigned long South, unsigned long West) {
    unsigned long cur_x = me.getx();
    unsigned long cur_y = me.gety();
    unsigned long goal_x = theGoal.getx();
    unsigned long goal_y = theGoal.gety();
    unsigned long dir_x, dir_y;
    me.print();
    theGoal.print();
    int seed;
    double r;
    long int M;
    double x;
    int y;
    int z;
    int count;
    seed = (cur_x + cur_y + goal_x + goal_y + North + South + East + West) * 11;
    srand(time(NULL));
    
    M = North + East + South + West;
    r = ( (double)rand()/((double)(RAND_MAX)+(double)(1)) );
    x = (r*M);
    y = (int)x;
    z = y + 1;
    cout << z << endl;
    
    unsigned long north_sec = North;
    unsigned long east_sec = East + north_sec;
    unsigned long south_sec = South + east_sec;
    unsigned long west_sec = West + south_sec;
    cout << north_sec << " " << east_sec << " " << south_sec << " " << west_sec << endl;
    if ( z <= north_sec )
        return NORTH;
    else if ( z <= east_sec )
        return EAST;
    else if ( z <= south_sec )
        return SOUTH;
    else if ( z <= west_sec )
        return WEST;
    else
        cout << "Z is " << z << endl;
}

//
// Fills in the given variables with the terrain difficulty of the 
// locations around the given position.
//
void getSurroundings(Position p, unsigned int &n, unsigned int &e, unsigned int &s, unsigned int &w); 

vector< vector<unsigned long> > map;

int main(int argc, char** argv) {

	vector<unsigned long> row;
	unsigned long var;
	unsigned long x_temp = 0;
	unsigned long x_dimension = 0;
	unsigned long y_dimension = 0;
	cout << "Map Contents:\n";
	ifstream file;
	file.open(MAP_FILE);
	while(file >> var) {
		x_temp++;
		row.push_back(var);
		cout << "\t" << var;
		if(file.peek() == '\n') {
			if(x_temp > x_dimension) {
				x_dimension = x_temp;
			}
			x_temp = 0;
			y_dimension++;
			map.push_back(row);
			row.clear();
			cout << endl;
		}
	}
	file.close();
	row.clear();
	cout << "Map dimensions: " << x_dimension << "x" << y_dimension << endl;

	/*
	 * Initialize some variables for the main loop.
	 */
	unsigned long movecount = 0;
	unsigned int north, south, east, west;
	string name = "me!";
	Position start(STARTX, STARTY);
	Position end(ENDX, ENDY);
	Person me(start, name); 
	row.clear();

	/*
	 * Announce current position.
	 */
	Position current = me.getPosition();
	cout << "I am here: " << endl;
	current.print();
	cout << "The end is there: " << endl;
	end.print();
	cout << endl << "Moves: \n";

	/*
	 * Jump into the loop.
	 * Basically, keep going as long as we're alive and not there yet.
	 */
	while(!current.equals(end) && me.isAlive()) {
		string::size_type x = (string::size_type)current.getx();
		string::size_type y = (string::size_type)current.gety();

		if(y >= map.size() || y < 0) {
			cerr << "You failed!\nPlayer attempted to move out of bounds.\nX: " << x << "\nY: " << y << endl;
			return -1;
		}

		row = map.at(y);

		if(x >= row.size() || x < 0) {
			cerr << "You failed!\nPlayer attempted to move out of bounds.\nX: " << x << "\nY: " << y << endl;
			return -1;
		}

		movecount += row.at(x);
		cout << "\tRunning Total: " << movecount << endl;

		getSurroundings(current, north, east, south, west);
		Direction d = decide(current, end, north, east, south, west);
		me.move(d);
		current = me.getPosition();
	}
	cout << "Success! You reached the goal in " << movecount << " moves!" << endl;

	return 0;
}


void getSurroundings(Position p, unsigned int &n, unsigned int &e, unsigned int &s, unsigned int &w) {

	vector<unsigned long> row1;
	vector<unsigned long> row2;
	vector<unsigned long> row3;
	
	int x = p.getx();
	int y = p.gety();

	// Erase incoming values.
	n = 0;
	e = 0;
	s = 0;
	w = 0;

	// Make sure we don't access a point off the grid
	if(x < 0 || y < 0) {
		n = 0;
		s = 0;
		e = 0;
		w = 0;
	} else {

		//	  n
		//  	w * e
		// 	  s	
		if(y - 1 >= 0 && y < map.size()) {
			row1 = map.at(y - 1);
			if(x >= 0 && x < row1.size()) {
				n = row1.at(x);
			} else {
				n = 0;
			}
		}

		if(y >= 0 && y < map.size()) {
			row2 = map.at(y);
			if(x + 1 < row2.size()) {
				e = row2.at(x + 1);
			} else {
				e = 0;
			}
			if(x - 1 > 0) {
				w = row2.at(x - 1);
			} else {
				w = 0;
			}

		}

		if(y + 1 >= 0 && (y + 1) < map.size()) {
			row3 = map.at(y + 1);
			if(x >= 0 && x < row3.size()) {
				s = row3.at(x);
			} else {
				s = 0;
			}
		}
		if(n <=0 || n > 100) {
			n = 0;
		}
		if(s <=0 || s > 100) {
			s = 0;
		}
		if(e <=0 || e > 100) {
			e = 0;
		}
		if(w <=0 || w > 100) {
			w = 0;
		}

		// This is some really ugly code...
	}
}
