#include <GLContext.h>
#include <random>

float PI = 3.14159265359f;
int armsN = 5;
int gen = 10;
float radius = 0.7f;
float pointSize = 1.0f;

int getRandomInt(int maxValue, int exclud) {
    int result = exclud;
    while (result == exclud) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, maxValue - 1);
        result = dist(gen);
    }
    return result;
}

std::vector<vec2> distributePointsInCircle(int numPoints, double radius) {
    std::vector<vec2> points;

    // Calculate the angle between each point
    float angle = 2 * PI / numPoints;

    // Distribute the points evenly in the circle
    for (int i = 0; i < numPoints; i++) {
        float theta = i * angle;

        // Calculate the coordinates using polar to Cartesian conversion
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        // Create a point and add it to the vector
        vec2 point;
        point.x = x;
        point.y = y;
        points.push_back(point);
    }

    return points;
}

vec2 getMidpoint(const vec2& p1, const vec2& p2) {
    vec2 midpoint;
    midpoint.x = (p1.x + p2.x) / 2.0;
    midpoint.y = (p1.y + p2.y) / 2.0;
    return midpoint;
}

void Draw(GLContext& context) {
    std::vector<vec2> points = distributePointsInCircle(armsN, radius);
    for (vec2 p : points) {
        context.drawPoint(p);
    }

    int startIndex = getRandomInt(armsN, -1);
    vec2 inPoint = points[startIndex];
    int index = getRandomInt(armsN, startIndex);
    vec2 sidePoint = points[index];
    
    for (int i = 0; i < gen; i++) {
        vec2 newPoint = getMidpoint(inPoint, sidePoint);
        context.drawPoint(newPoint, pointSize);
        inPoint = newPoint;
        index = getRandomInt(armsN, index);
        sidePoint = points[index];
    }
    
}

void Input(GLContext& context, int key) {

}

void initialize(GLContext& context) {

}

int main() {
	GLContext context;
	context.window_name = "test";
	context.background = vec4(0, 0, 0, 0);
	context.onDraw = Draw;
	context.onInput = Input;
    context.initialize = initialize;
    context.alpha = true;
	context.init(1000, 1000);
}

