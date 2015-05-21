#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <unistd.h>

using namespace std;

// Vorschläge für Optimierung:
// inRange(const double& x, ...)
//  same in Point constructor
//  same in Line constructor
//  ...
// g++ mit bester Optimierungsstufe aufrufen
// run this: <s_100_1.dat time parallel --pipe --block <default=1M> intersect | awk '{s+=$0} END {print s}'
// $(($(stat -c%s s_100_1.dat)/2))
// mist, nein, das geht nicht, weil wenn input file aufgeteilt wird, haben wir ja nicht mehr alle kombinationen!
// run this: <s_100_1.dat time parallel --pipe intersect | awk '{s+=$0} END {print s}'

const double epsilon = numeric_limits<double>::epsilon();

bool inRange(double x, double a, double b) {
	return x >= min(a, b) && x <= max(a, b);
}

bool isEqualToZero(double x) {
	return abs(x) <= epsilon;
}

bool isLessThanOrEqualToZero(double x) {
	return x <= epsilon;
}

struct Point {
	double x;
	double y;
	Point(double x, double y) : x(x), y(y) { }
};

struct Line {
	Point s; // start point
	Point e; // end point
	Line(double sx, double sy, double ex, double ey) : s(sx, sy), e(ex, ey) { }
	void debugLog(const Line& otherLine, const string& text) const {
		 cerr << s.x << " " << s.y << " "
		      << e.x << " " << e.y << " "
		      << otherLine.s.x << " " << otherLine.s.y << " "
		      << otherLine.e.x << " " << otherLine.e.y << " "
		      << text << endl;
	}
	double ccw(const Point& p) const {
		return (s.x*e.y - s.y*e.x) + (e.x*p.y - e.y*p.x) + (s.y*p.x - s.x*p.y);
	}
	bool intersect(const Line& otherLine) const {
		const double ccwStart = ccw(otherLine.s);
		const double ccwEnd   = ccw(otherLine.e);

		// Both ends of other line at one side?
		if (ccwStart * ccwEnd > 0) {
			debugLog(otherLine, "both ends of other line on one side: false");
			return false;
		}

		// Both ends of other line are at different sides?
		if (ccwStart * ccwEnd < 0) {

			// Both ends of this line are at different sides
			// or exactly one end of other line is on this line?
			// ("exactly one", because otherwise we would not be here)
			if (isLessThanOrEqualToZero(otherLine.ccw(s) * otherLine.ccw(e))) {
				debugLog(otherLine, "clearly intersect: true" );
				return true;
			} else {
				debugLog(otherLine, "not parallel, no intersect: false");
				return false;
			}
		}

		if (isEqualToZero(ccwStart)
			&& inRange(otherLine.s.x, s.x, e.x)
			&& inRange(otherLine.s.y, s.y, e.y)) {
			debugLog(otherLine, "startpoint of other on this line: true");
			return true;
		}

		if (isEqualToZero(ccwEnd)
			&& inRange(otherLine.e.x, s.x, e.x)
			&& inRange(otherLine.e.y, s.y, e.y)) {
			debugLog(otherLine, "endpoint of other on this line: true");
			return true;
		}

		debugLog(otherLine, "otherwise: false");
		return false;
	}
};
	
int main(int argc, char *argv[]) {
	unsigned int outerLoopLimit;
	int c;
	while ((c = getopt(argc, argv, "hl:")) != -1) {
		switch (c) {
			case 'l':
				outerLoopLimit = atoi(optarg);
				break;
			case 'h':
				cout << "usage: intersect [ -h | -l <limit_for_outer_loop> ]" << endl;
			        return 0;
		}
	}

	vector<Line> lines;
	double x1, y1, x2, y2;
	while (cin >> x1 >> y1 >> x2 >> y2) {
		//cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
		lines.push_back(Line(x1, y1, x2, y2));
	}

	if (outerLoopLimit == 0) {
		outerLoopLimit = lines.max_size();
	}
	unsigned int intersectCount = 0;
	unsigned int outerLoopCounter = 0;
	for (vector<Line>::iterator it = lines.begin();
			outerLoopCounter < outerLoopLimit
			&& it != lines.end(); it++) {
		vector<Line>::iterator jt = it;
		jt++;
		for (; jt != lines.end(); jt++)
		{
			if (it->intersect(*jt))
				intersectCount++;
		}
		outerLoopCounter++;
	}
	cout << intersectCount << endl;
	return 0;
}
