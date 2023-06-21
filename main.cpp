#include <GLContext.h>
#include <random>

float PI = 3.14159265359f;
int armsN = 5;
int gen = 10;
float radius = 0.7f;
float pointSize = 1.0f;
bool randomColor = false;
vec4 pointColor = vec4(1, 1, 1, 1);
string message = " ";
char* filepath = "C:/Users/eliot/Desktop/test/yeah";
int counter = 0;
bool showInit = true;

using namespace GLContext;

int getRandomInt(int maxValue, int exclud = -1) {
    int result = exclud;
    while (result == exclud) {
        std::random_device rd;
        std::mt19937 gene(rd());
        std::uniform_int_distribution<> dist(0, maxValue - 1);
        result = dist(gene);
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

void Draw() {
    vec4 color;
    if (!randomColor) { color = pointColor; }
    else { color.x = getRandomInt(255);
           color.y = getRandomInt(255);
           color.z = getRandomInt(255);
           color.w = 255;
    }
    std::vector<vec2> points = distributePointsInCircle(armsN, radius);
    if (showInit) {
        for (vec2 p : points) {
            drawPoint(p);
        }
    }

    int startIndex = getRandomInt(armsN, -1);
    vec2 inPoint = points[startIndex];
    int index = getRandomInt(armsN, startIndex);
    vec2 sidePoint = points[index];
    
    for (int i = 0; i < gen; i++) {
        vec2 newPoint = getMidpoint(inPoint, sidePoint);
        drawPoint(newPoint, pointSize, color);
        inPoint = newPoint;
        index = getRandomInt(armsN, index);
        sidePoint = points[index];
    }
    
}



void Ui() {
    ImGui::Begin("Parameters");

    //PI
    //ImGui::Text("value of PI");
    ImGui::SliderFloat("value of PI", &PI, -10.0f, 10.0f);
    if (ImGui::Button("Reset")) { PI = 3.14159265359f; }

    //ArmsN
    //ImGui::Text("arms number");
    ImGui::InputInt("arms number", &armsN);

    //gen
    //ImGui::Text("generation between frame");
    ImGui::InputInt("generation/frame", &gen);

    //radius
    //ImGui::Text("radius");
    ImGui::SliderFloat("radius", &radius, 0.0f, 1.0f);

    //pointSize
    //ImGui::Text("point size");
    ImGui::SliderFloat("point size", &pointSize, 0.001f, 30.0f);

    //background
    ImGui::ColorEdit4("background", &background.x);

    //point color
    ImGui::ColorEdit4("point color", &pointColor.x);

    //alpha
    ImGui::Checkbox("alpha blending", &alpha);

    //showInit
    ImGui::Checkbox("show inits points", &showInit);

    //take screenshot
    if (ImGui::Button("take a screenshot")) {
        TakeScreenshot(filepath + to_string(counter) + ".png");
        message = "saved to : ";
        message.append(filepath + to_string(counter) + ".png");
        counter++;
    }

    ImGui::Text(message.c_str());


    

    ImGui::End();
}

void Input(int key) {
    if (key == 32) {
        TakeScreenshot("C:/Users/eliot/Desktop/test/yeah.png");
    }
}

int WinMain() {
	window_name = "snowflake generator";
	onDraw = Draw;
    onDrawUI = Ui;
    onInput = Input;
	init(1500, 1000);
}

//TODO : add paramteters GUI

