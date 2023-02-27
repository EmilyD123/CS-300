//============================================================================
// Name        : CS300 Project Two.cpp
// Author      : Emily Domoracki
// Version     : 1.0
// Description : ABCU Course Planner utilizing a binary search tree 
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <conio.h>

using namespace std;

#define DELIMITER ','

/*
* credit for structs and binary search tree class and functions:
* BinarySearchTree.cpp by John Watson, SNHU
*/
//structure for course object
struct Course
{
    string courseId;
    string courseName;
    string prereqOne;
    string prereqTwo;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};
/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {

    delete root;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {

    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {

    //if root is null, create new node as root initialized with course
    if (root == nullptr) {
        root = new Node(course);
    }
    //else, invoke addNode with course
    else {
        this->addNode(root, course);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {

    //start at root
    Node* current = root;

    //traverse tree comparing courseId, return if found
    while (current != nullptr) {
        if (current->course.courseId.compare(courseId) == 0) {
            return current->course;
        }
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    //return empty course if bidId not found
    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {

    //if root courseId is larger than key courseId
    if (node->course.courseId.compare(course.courseId) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {

  //traverse tree left to right
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseId << ", "
             << node->course.courseName << endl;
        inOrder(node->right);
    }
    return;
}

/*
* load and parse csv files
* then insert data to bst
* 
* sources: https://www.geeksforgeeks.org/csv-file-management-using-c/
*          BinarySearchTree.cpp by John Watson, SNHU
*/
void LoadAndParse(string csvPath, BinarySearchTree* bst) {
    //create fstream in
    fstream fin;

    //open file
    fin.open(csvPath, ios::in);

    //define variables
    vector<vector<string>> file_content;
    vector<string> row;
    string line, delimited_string;

    if (fin.is_open()) {

        while (fin.good()) {

            while (getline(fin, line)) {

                row.clear();

                //create stringstream to parse line
                stringstream ss(line);

                while (getline(ss, delimited_string, DELIMITER)) {

                    //add delimited string to end of row vector
                    row.push_back(delimited_string);
                }

                //add row vector to file_content vector
                file_content.push_back(row);
            }

            //loop to read csv data in file_content vector
            for (int i = 0; i < file_content.size(); ++i) {

                //create course struct to hold course data
                Course course;
                course.courseId = file_content[i][0];
                course.courseName = file_content[i][1];

                //check for prerequisites
                if (file_content[i].size() == 3) {
                    course.prereqOne = file_content[i][2];
                }
                if (file_content[i].size() == 4) {
                    course.prereqOne = file_content[i][2];
                    course.prereqTwo = file_content[i][3];
                }
                //add instance of course to bst
                bst->Insert(course);
            }
            cout << "...Courses loaded.";
        }
    }

    else {
        cout << "failed to open file";
    }

    fin.close();
}

/**
 * Display the course information to the console
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {

    cout << "\n" << course.courseId << ", " << course.courseName << endl;

    //check for prerequisites
    if (course.prereqOne != "") {

        cout << "Prerequisites: " << course.prereqOne;

    }
    if (course.prereqTwo != "") {
        cout << ", " << course.prereqTwo;
   }
}

int main() {
   
    string csvPath, courseKey;

    cout << "::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << "::  Welcome to the ABCU Course Planner  ::" << endl;
    cout << "::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << "\n\n";
    
    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "     ***********" << endl;
        cout << "      Main Menu" << endl;
        cout << "     ***********" << "\n" << endl;
        cout << "  1. Load Course List File" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Find and Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "\n" << "Enter choice : ";
        cin >> choice;

        switch (choice) {

        case 1:

            if (csvPath == "") {
                cout << "\n\n";
                cout << "Alert! CSV File Path Needed." << endl
                    << "Example: C:/Users/Public/Documents/course_list.csv" << endl
                    << "\n" << "Please Enter CSV File Path: ";
                cin >> csvPath;
            }

            cout << "Loading Course List..." << endl;
            //method call to load the courses
            LoadAndParse(csvPath, bst);

            cout << "\n\n";
            break;

        case 2:

            cout << "\n\n";

            bst->InOrder();

            cout << "\n\n";
            break;

        case 3:

            cout << "\n\n";
            cout << "Course ID needed. Example: CSCI300 " << endl
                 << "Please enter course ID: ";
            cin >> courseKey;

            for (auto& c : courseKey) c = toupper(c);

            course = bst->Search(courseKey);

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course ID " << courseKey << " not found." << endl;
            }

            cout << "\n\n";
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
