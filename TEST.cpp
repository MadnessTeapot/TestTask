#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

static int nextID = 0;

struct Point // Structure representing a point in 2D space
{
  Point(); 
  Point(double x, double y); // Constructor for the Point structure
  double x; // x-coordinate of the point
  double y;// y-coordinate of the point
};

struct Color // Structure representing the RGB color values
{
  int red;
  int green;
  int blue;
};

class BaseObject // Base class for geometric objects
{
public:
  virtual ~BaseObject();
  virtual Point* getCenter();
  virtual bool isValid() = 0;
  virtual void read(std::ifstream& inputFile) = 0;
  virtual void write(std::ofstream& outputFile) const = 0;

  Color getColor()
  {
    return _color;

  }

  void setColor(int red, int green, int blue)
  {
    _color.red = red;
    _color.green = green;
    _color.blue = blue;
  }

protected:
  Point* _center{}; // Get the center point of the BaseObject
  std::vector<Point> _points;
  Color _color = { 0, 0, 0 };
  int _id = 0;
};

class Rect : public BaseObject // Class representing a rectangle, derived from BaseObject
{
private:
  int width;
  int height;

public:

  Rect();

  void set(const Point& p1, const Point& p2, const Point& p3, const Point& p4);  // Implementation of the virtual functions from the BaseObject class
  void set(const Point& p1, const Point& p3);
  virtual void read(std::ifstream& inputFile) override;
  virtual void write(std::ofstream& outputFile) const override;
  virtual bool isValid() override;

  Rect boundingBox() // Calculate the minimum and maximum x and y values of the points
  {
    double minX = _points[0].x;
    double minY = _points[0].y;
    double maxX = _points[0].x;
    double maxY = _points[0].y;

    for (const auto& point : _points)
    {
      if (point.x < minX)
        minX = point.x;
      if (point.y < minY)
        minY = point.y;
      if (point.x > maxX)
        maxX = point.x;
      if (point.y > maxY)
        maxY = point.y;
    }

    Rect box;
    box.set(Point(minX, minY), Point(maxX, maxY));

    return box;
  }


};

class Circle : public BaseObject // Class representing a circle, derived from BaseObject
{
public:
  Circle()
  {
    radius = 0; 
  }
  void read(std::istream& input) {
    input >> radius;
  }

  void write(std::ostream& output) const {
    output << "Circle: radius = " << radius << std::endl;
  }

private:
  double radius;

public:
  void set(const Point& center, double radius);
  virtual bool isValid() override;
  virtual void read(std::ifstream& inputFile) override;
  virtual void write(std::ofstream& outputFile) const override;
private:
  double _radius = 0.0;

  Rect boundingBox() // Calculate the minimum and maximum x and y values of the points
  {
    double minX = _center->x - _radius;
    double minY = _center->y - _radius;
    double maxX = _center->x + _radius;
    double maxY = _center->y + _radius;

    Rect box;
    box.set(Point(minX, minY), Point(maxX, maxY));

    return box;
  }


};

class Triangle : public BaseObject // Class representing a triangle, derived from BaseObject
{
private:
  double base;
  double height;
public:
  Triangle() : base(0), height(0)
  {
    base = 0.0;
    height = 0.0; 
  }
  void read(std::istream& input)
  {
    input >> base >> height;
  }

  void write(std::ostream& output) const
  {
    output << "Triangle: base = " << base << ", height = " << height << std::endl;
  }


public:
  void set(const Point& p1, const Point& p2, const Point& p3);
  virtual bool isValid() override;
  virtual void read(std::ifstream& inputFile) override;
  virtual void write(std::ofstream& outputFile) const override;

  Rect boundingBox() // Calculate the minimum and maximum x and y values of the points
  {
    double minX = _points[0].x;
    double minY = _points[0].y;
    double maxX = _points[0].x;
    double maxY = _points[0].y;

     for (const auto& point : _points)
    {
      if (point.x < minX)
        minX = point.x;
      if (point.y < minY)
        minY = point.y;
      if (point.x > maxX)
        maxX = point.x;
      if (point.y > maxY)
        maxY = point.y;
    }

    Rect box;
    box.set(Point(minX, minY), Point(maxX, maxY));

    return box;
  }
};

class Polyline : public BaseObject // Class representing a polyline, derived from BaseObject
{
public:

  Polyline();
  ~Polyline() {}
  Polyline(const std::vector<Point>& points);
  void set(const std::vector<Point>& points);
  virtual void read(std::ifstream& inputFile) override;
  virtual void write(std::ofstream& outputFile) const override;
  virtual bool isValid() override;

  Rect boundingBox() // Find the minimum and maximum x and y coordinates of the points
  {
    double minX = _points[0].x;
    double minY = _points[0].y;
    double maxX = _points[0].x;
    double maxY = _points[0].y;

    
    for (const auto& point : _points)
    {
      if (point.x < minX)
        minX = point.x;
      if (point.y < minY)
        minY = point.y;
      if (point.x > maxX)
        maxX = point.x;
      if (point.y > maxY)
        maxY = point.y;
    }

     
    Rect box;  // Create a rectangle using the min/max values as opposite corners
    box.set(Point(minX, minY), Point(maxX, maxY));

    return box;
  }

};

BaseObject* factory(int num) // Factory function to create objects based on the given number
{
  switch (num)
  {
  case 1:
    return new Rect();
  case 2:
    return new Circle();
  case 3:
    return new Triangle();
  case 4:
    return new Polyline();
  default:
    return nullptr;
  }
}

/*
num == 1 : create Rect
num == 2 : create Circle
num == 3 : create Triangle
num == 4 : create Polyline //additional geometric figure (Polyline)

*/

namespace
{
  void saveObjectsToFile(const std::vector<BaseObject*>& objects) // Function to save objects to a file
  {

    std::cout << "The data has been successfully saved to a file." << std::endl;

    std::ofstream outputFileStream("figures.txt");
    if (!outputFileStream.is_open())
    {
      std::cout << "Failed to open file for recording." << std::endl;
      return;
    }

    for (BaseObject* object : objects)
    {
      object->write(outputFileStream);
    }

    outputFileStream.close();
  }

  std::vector<BaseObject*> restoreObjectsFromFile(const std::string& filename) // Function to restore objects from file
  {
    std::vector<BaseObject*> objects;
    std::ifstream inputFile(filename);
    std::cout << "The data has been successfully restored from  file." << std::endl;

    if (!inputFile.is_open()) {
      std::cout << "Failed to open file for restoring." << std::endl;
      return objects;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
      std::istringstream iss(line);
      std::string typeOfObject;
      if (!(iss >> typeOfObject))
      {
        std::cout << "Failed to read object type from line: " << line << std::endl;
        continue;
      }
      // Create a new object based on the object type
      BaseObject* obj = nullptr;
      if (typeOfObject == "rectangle") {
        obj = new Rect();
        obj->read(inputFile);
      }
      else if (typeOfObject == "circle") {
        obj = new Circle();
        obj->read(inputFile);
      }
      else if (typeOfObject == "triangle") {
        obj = new Triangle();
        obj->read(inputFile);
      }
      else if (typeOfObject == "polyline") {
        obj = new Polyline();
        obj->read(inputFile);
      }
      else {
        std::cout << "Unknown object type: " << typeOfObject << std::endl;
        continue;
      }

      objects.push_back(obj);
    }

    inputFile.close();
    return objects;
  }

}

int main()
{



  BaseObject* pRect_1 = factory(1);
  Rect* pRect_2 = new Rect();

  BaseObject* pCircle_1 = factory(2);
  Circle* pCircle_2 = new Circle();

  BaseObject* pTriangle_1 = factory(3);
  Triangle* pTriangle_2 = new Triangle();

  BaseObject* pPolyline_1 = factory(4);
  Polyline* pPolyline_2 = new Polyline();

  std::vector<BaseObject*> objects;

  objects.push_back(pRect_1);
  objects.push_back(pRect_2);
  objects.push_back(pCircle_1);
  objects.push_back(pCircle_2);
  objects.push_back(pTriangle_1);
  objects.push_back(pTriangle_2);
  objects.push_back(pPolyline_1);
  objects.push_back(pPolyline_2);

  {//set rect
    Point p1(1.0, 1.0);
    Point p2(1.0, 5.0);
    Point p3(5.0, 5.0);
    Point p4(5.0, 1.0);
    ((Rect*)pRect_1)->set(p1, p2, p3, p4);
    pRect_2->set(p1, p3);
  }

  {//set triangle
    Point p1(1.0, 1.0);
    Point p2(3.0, 3.0);
    Point p3(4.0, 2.0);
    ((Triangle*)pTriangle_1)->set(p1, p2, p3);
    pTriangle_2->set(p1, p2, p3);
  }

  {//set Circle
    Point p1(1.0, 1.0);
    ((Circle*)pCircle_1)->set(p1, 5.0);
    pCircle_2->set(p1, 5.0);
  }

  {//set Polyline
    Point p1(1.0, 1.0);
    Point p2(3.0, 3.0);
    Point p3(4.0, 2.0);
    Point p4(5.0, 1.0);
    Point p5(3.0, 4.0);
    Point p6(4.0, 4.0);

    std::vector<Point> points1 = { p1, p2, p3, p4, p5, p6 };
    ((Polyline*)pPolyline_1)->set(points1);

    std::vector<Point> points2 = { p1, p2, p3, p4, p5, p6 };
    pPolyline_2->set(points2);
  }

  //Checking figures for centers

  if (pRect_1->getCenter()->x != 3.0 || pRect_1->getCenter()->y != 3.0)
    printf("FAIL 1\n");

  if (pRect_2->getCenter()->x != 3.0 || pRect_2->getCenter()->y != 3.0)
    printf("FAIL 2\n");

  if (pCircle_1->getCenter()->x != 1.0 || pCircle_1->getCenter()->y != 1.0)
    printf("FAIL 3\n");

  if (pCircle_2->getCenter()->x != 1.0 || pCircle_2->getCenter()->y != 1.0)
    printf("FAIL 4\n");

  if (std::abs(pTriangle_1->getCenter()->x - 2.6) > 0.1 || pTriangle_1->getCenter()->y != 2.0)
    printf("FAIL 5\n");

  if (std::abs(pTriangle_2->getCenter()->x - 2.6) > 0.1 || pTriangle_2->getCenter()->y != 2.0)
    printf("FAIL 6\n");

  std::string userInput;

  while (true) {
    std::cout << "Do you want to save the shapes to a file? (Save/Restore/Exit): ";
    std::cin >> userInput;

    std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

    if (userInput == "save") {
      saveObjectsToFile(objects);
      std::cout << "Objects saved to file." << std::endl;
    }
    else if (userInput == "restore") {
      objects = restoreObjectsFromFile("figures.txt");
      std::cout << "Objects restored from file." << std::endl;
      saveObjectsToFile(objects);
    }
    else if (userInput == "exit") {
      break;  // exit the loop
    }
    else {
      std::cout << "Invalid input. Please enter either 'Save', 'Restore', or 'Exit'." << std::endl;
    }
  }
  //release the memory allocated to objects through the operator "delete"
  delete pRect_1;
  delete pRect_2;
  delete pCircle_1;
  delete pCircle_2;
  delete pTriangle_1;
  delete pTriangle_2;
  delete pPolyline_1;
  delete pPolyline_2;

  return 0;
}

Rect::Rect()
{
  _points.clear(); // Clean up the list of points
  height = 0; // Initialize height with zero values
  width = 0; // Initialize width with zero values
  _center = nullptr;// Set the center to nullptr
  _color = { 0,0,0 };// Set the color of the object to black (RGB: 0,0,0)
  _id = nextID++; // Set the object ID and increment the counter for the next ID
}

void Rect::set(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
{
  if (_center == nullptr)
    // add  points to the point list
    _points.clear();
  _points.push_back(p1);
  _points.push_back(p2);
  _points.push_back(p3);
  _points.push_back(p4);
  // Calculate the center of the rectangle by averaging the coordinates of all four points
  double centerX = (p1.x + p2.x + p3.x + p4.x) / 4;
  double centerY = (p1.y + p2.y + p3.y + p4.y) / 4;
  _center = new Point(centerX, centerY);// create a new Point object for the center and set its coordinates
}

void Rect::set(const Point& p1, const Point& p3)
{
  // Use points p1 and p3 to calculate the remaining two points of the rectangle
  Point p2(p3.x, p1.y);
  Point p4(p1.x, p3.y);
  set(p1, p2, p3, p4); // Call the set method with the obtained points to set the rectangle
}

void Rect::write(std::ofstream& outputFile) const
{
  if (outputFile.is_open())
  {
    //Write information about the rectangle into the file
    outputFile << "rectangle\n" << _id << "," << _center->x << "," << _center->y << ",";
    // Write the coordinates of all points in the format "x,y,"
    for (const auto& point : _points)
    {

      if (point.x == _points[_points.size() - 1].x && point.y == _points[_points.size() - 1].y)
        outputFile << point.x << "," << point.y << "\n";
      else
        outputFile << point.x << "," << point.y << ",";
    }
  }
  else
  {
    std::cout << "Failed to open the file to save the rectangle." << std::endl;
  }
}

bool Rect::isValid()
{
  return (width > 0) && (height > 0); // A rectangle is valid if its width and height are positive values
}

void Rect::read(std::ifstream& inputFile)
{
  if (_center == nullptr)
    _center = new Point();

  std::string line;
  std::getline(inputFile, line);
  std::istringstream iss(line);

  std::string subString;
  int valuesCount = 0;

  while (std::getline(iss, subString, ','))
  {
    switch (valuesCount++)
    {
    case 0: // figure ID
      _id = std::stoi(subString);
      break;
    case 1: // X coordinate of the center
      _center->x = std::stod(subString);
      break;
    case 2: // Y coordinate of the center
      _center->y = std::stod(subString);
      break;
    default: // Vertex coordinates
      Point newPoint;
      newPoint.x = std::stod(subString);
      std::getline(iss, subString, ',');
      newPoint.y = std::stod(subString);

      _points.push_back(newPoint); // add a new point to the list of points
      break;
    }
  }
}

void Circle::set(const Point& center, double radius)
{
  if (_center == nullptr)
    _center = new Point();
  // Set the center and radius of the circle
  _center->x = center.x;
  _center->y = center.y;
  _radius = radius;
  _id = nextID++; // Set the object ID and increment the counter for the next ID
}

void Circle::write(std::ofstream& outputFile) const
{
  if (outputFile.is_open())
  {
    // Write information about the circle to a file
    outputFile << "circle\n" << _id << "," << _center->x << "," << _center->y << "," << _radius << "\n";
  }
  else
  {
    std::cout << "Failed to open the file to save the circle." << std::endl;
  }
}

void Circle::read(std::ifstream& inputFile)
{
  if (_center == nullptr)
    _center = new Point();

  std::string line;
  std::getline(inputFile, line);
  std::istringstream iss(line);

  std::string subString;
  int valuesCount = 0;
  while (std::getline(iss, subString, ',')) {
    switch (valuesCount++) {
    case 0: // figure ID
      _id = std::stoi(subString);
      break;
    case 1: // X coordinate of the center
      _center->x = std::stod(subString);
      break;
    case 2: // Y coordinate of the center
      _center->y = std::stod(subString);
      break;
    case 3: // Radius
      _radius = std::stod(subString);
      break;
    default:
      break;
    }
  }
}

bool Circle::isValid()
{
  return false;
}

void Triangle::set(const Point& p1, const Point& p2, const Point& p3)
{
  if (_center == nullptr)
    _center = new Point();
  // Calculate the center of the triangle using the average values of the coordinates
  _center->x = (p1.x + p2.x + p3.x) / 3.0;
  _center->y = (p1.y + p2.y + p3.y) / 3.0;
  // clear the list of points and add new points
  _points.clear();
  _points.push_back(p1);
  _points.push_back(p2);
  _points.push_back(p3);
  _id = nextID++;// Set the object ID and increment the counter for the next ID
}

void Triangle::read(std::ifstream& inputFile)
{
  if (_center == nullptr)
    _center = new Point();

  std::string line;
  std::getline(inputFile, line);
  std::istringstream iss(line);

  std::string subString;
  int valuesCount = 0;
  int pointIndex = 0;
  while (std::getline(iss, subString, ',')) {
    switch (valuesCount++) {
    case 0: // figure ID
      _id = std::stoi(subString);
      break;
    case 1: // X coordinate of the center
      _center->x = std::stod(subString);
      break;
    case 2: // Y coordinate of the center
      _center->y = std::stod(subString);
      break;
    default: // Point coordinates
      if (pointIndex < 3) {
        int coordinateIndex = (valuesCount - 3) % 2;
        if (coordinateIndex == 0)
          _points[pointIndex].x = std::stod(subString);
        else
          _points[pointIndex++].y = std::stod(subString);
      }
      break;
    }
  }

  set(_points[0], _points[1], _points[2]);
}

void Triangle::write(std::ofstream& outputFile) const
{
  if (outputFile.is_open())
  {
    // Write information about the triangle into the file
    outputFile << "triangle\n" << _id << "," << _center->x << "," << _center->y << ",";
    for (const auto& point : _points)
    {
      outputFile << point.x << "," << point.y << ",";
    }
    outputFile << base << "," << height << "\n";
  }
  else
  {
    std::cout << "Failed to open the file to save the triangle." << std::endl;
  }
}

bool Triangle::isValid()
{
  return false;
}

void Polyline::set(const std::vector<Point>& points)
{
  _points.clear();
  if (points.size() > 1)
  {
    int sumX = 0, sumY = 0;
    
    for (const auto& point : points)
    {
      _points.push_back(point);
      sumX += point.x;
      sumY += point.y;

    }
    delete _center;// delete the old center and create a new one with new coordinates
    _center = new Point(sumX / points.size(), sumY / points.size());
  }
}

void Polyline::write(std::ofstream& outputFile) const
{
  if (outputFile.is_open())
  {
    //Write information about the polyline into the file
    outputFile << "polyline\n" << _id << "," << _center->x << "," << _center->y << ",";
    for (const auto& point : _points)
    {

      if (point.x == _points[_points.size() - 1].x && point.y == _points[_points.size() - 1].y)
        outputFile << point.x << "," << point.y << "\n";
      else
        outputFile << point.x << "," << point.y << ",";
    }
  }
  else
  {
    std::cout << "Failed to open the file to save the polyline." << std::endl;
  }
}

void Polyline::read(std::ifstream& inputFile)
{
  if (_center == nullptr)
    _center = new Point();

  std::string line;
  std::getline(inputFile, line);
  std::istringstream iss(line);

  std::string subString;
  int valuesCount = 0;
  int pointIndex = 0;
  while (std::getline(iss, subString, ',')) {
    switch (valuesCount++) {

    case 0: // ID
      _id = std::stoi(subString);
      break;
    case 1: // X coordinate of the center
      _center->x = std::stod(subString);
      break;
    case 2: // Y coordinate of the center
      _center->y = std::stod(subString);
      break;
    default: // Point coordinates
      if (pointIndex < 6) {
        int coordinateIndex = (valuesCount - 2) % 2;
        if (coordinateIndex == 0)
          _points[pointIndex].x = std::stod(subString);
        else
          _points[pointIndex++].y = std::stod(subString);
      }
      break;
    }
  }
}

Polyline::Polyline()
{
  _points.clear(); // clear the list of points
  _center = nullptr; // Set the pointer to the center to nullptr
  _color = { 0,0,0 }; // Set the default color to black
  _id = nextID++; // Assign a unique identifier to the object
}

bool Polyline::isValid()
{
  // A polyline is valid if it has at least two points
  return _points.size() >= 2;
}

BaseObject::~BaseObject()
{
  delete _center; // Free the memory allocated to the center of the object
}

Point* BaseObject::getCenter()
{
  return _center; //return the pointer to the center of the object
}

Point::Point()
{
  x = 0; // Initialize x coordinate with a zero value
  y = 0; // initialize y coordinate with a zero value
}

Point::Point(double x, double y)
{
  this->x = x; // initialize x coordinate with the specified value
  this->y = y; // initialize y coordinate with the given value
}


