#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

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

const double epsilon = 1e-3;

bool inRange(double x, double a, double b) {
	return x >= (min(a, b) - epsilon) && x <= (max(a, b) + epsilon);
}

bool isEqualToZero(double x) {
	return abs(x) <= epsilon;
}

bool isGreaterThanZero(double x) {
	return x > epsilon;
}

bool isLessThanZero(double x) {
	return x < -epsilon;
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
		if (isGreaterThanZero(ccwStart * ccwEnd)) {
			debugLog(otherLine, "both ends of other line on one side: false");
			return false;
		}

		// Both ends of other line are at different sides?
		if (isLessThanZero(ccwStart * ccwEnd)) {

			// Both ends of this line are at different sides
			// or exactly one end of other line is on this line?
			// ("exactly one", because otherwise we would not be here)
			if (isLessThanOrEqualToZero(otherLine.ccw(s) * otherLine.ccw(e))) {
				debugLog(otherLine, "clearly intersect: true" );
				return true;
			}
			else {
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
	vector<Line> lines;
	double x1, y1, x2, y2;
	while (cin >> x1 >> y1 >> x2 >> y2) {
		//cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
		lines.push_back(Line(x1, y1, x2, y2));
	}

	int intersectCount = 0;
	for (vector<Line>::iterator it = lines.begin(); it != lines.end(); it++) {
		//cout << (*it)->sx << endl;
		vector<Line>::iterator jt = it;
		jt++;
		for (; jt != lines.end(); jt++)
		{
			if (it->intersect(*jt))
				intersectCount++;
		}
	}
	cout << intersectCount << endl;
	return 0;
}
