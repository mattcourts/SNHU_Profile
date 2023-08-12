/*
 * Program Name: Course Planner
 * Author: Matthew Courts
 * Version : 1.0
 * Description: viewing of course information and requirements
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

/******************* Globals *******************/

struct Course
{
    string name;
    string courseId;
    vector<string> prerequisites;
};

vector<Course> courseList;

/******************* Main Program - Supporting Methods *******************/

string courseDataFile = "classes.csv";

/**
 * Split a string by delimiter returning list of the contents
 *
 * @param str string to be split
 * @param del delimiter used for splitting string
 * @return vector list of line separated
 *
 */
vector<string> splitString(string str, char del)
{
    vector<string> contents;
    stringstream ss(str);
    string token;

    while (getline(ss, token, del))
    {
        contents.push_back(token);
    }
    return contents;
}

/**
 * Load data file
 *
 * @param contents pointer to source datalist for contents to be loaded
 * @return status if file loading was successful or not
 *
 */
bool ParseFileContents(vector<Course> &contents)
{
    bool result = false;
    fstream sourceFile;
    sourceFile.open(courseDataFile, fstream::in);
    string lineBuffer;

    if (sourceFile.is_open())
    {
        while (getline(sourceFile, lineBuffer))
        {
            if (lineBuffer != "")
            {
                vector<string> line = splitString(lineBuffer, ',');
                Course course;

                course.courseId = line.front();
                line.erase(line.begin());
                course.name = line.front();

                if (line.size() > 0)
                {
                    line.erase(line.begin());
                    course.prerequisites = line;
                }
                contents.push_back(course);
            }
        }
        result = true;
    }
    else
    {
        return result;
    }

    sourceFile.close();
    return result;
}

/**
 * Searches a vector list for a course by id
 *
 * @param contents source list to be searched
 * @param ID id of the course being searched for
 * @return course if found
 *
 */
Course SearchCourseListByID(vector<Course> &contents, string ID)
{
    auto it = find_if(contents.begin(), contents.end(), [ID](Course course)
                      { return course.courseId == ID; });

    if (it != contents.end())
    {
        return *it;
    }
    else
    {
        return Course();
    }
}

/**
 * pupulates courselist and checks that all prerequisite courses exist
 *
 * @param contents pointer to source datalist for contents to be loaded
 * @return status if all courses exist
 *
 */
bool loadcourselist(vector<Course> &contents)
{
    bool status = true;
    ParseFileContents(contents);

    // validate prereqs are included.
    if (contents.size() > 0)
    {
        for (Course course : contents)
        {
            for (string prereqID : course.prerequisites)
            {
                Course prereq = SearchCourseListByID(contents, prereqID);
                if (prereq.name == "")
                {
                    cout << prereqID << " Prequisite course missing" << endl;
                    status = false;
                }
            }
        }
    }
    else
    {
        status = false;
    }

    return status;
}

/**
 * prints all prerequisite courses
 *
 * @param course the course data
 *
 */
void printPrerequisite(Course course)
{
    cout << "Prerequisites: ";
    if (course.prerequisites.size() != 0)
    {

        for (string prepreq : course.prerequisites)
        {
            cout << prepreq << ", ";
        }
    }
    else
    {
        cout << "None";
    }
    cout << endl;
}

/**
 * Prints course information
 *
 * @param course the course data
 * @param prereq if the prerequisite list is to be printed
 *
 */

void printCourse(Course course, bool prereq = false)
{
    cout << course.courseId << ", " << course.name << endl;
    if (prereq)
    {
        printPrerequisite(course);
    }
}

/**
 * wrapper procedure for printing all courses
 *
 * @param courseList list of all courses
 *
 */
void printCourseList(vector<Course> &courseList)
{
    cout << "Course List:" << endl;
    for (Course course : courseList)
    {
        printCourse(course);
    }
}

/**
 * Partition the vector of course into two parts, low and high
 *
 * @param course Address of the vector<Course> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 * @return high value
 *
 */
int partition(vector<Course> &course, int begin, int end)
{
    int low = begin;
    int high = end;
    int pivot = low + (high - low) / 2;
    string pivotVal = course[pivot].name;
    bool done = false;

    while (!done)
    {
        // shift low iterator
        while (course[low].name < pivotVal)
        {
            low += 1;
        }

        // shift high iterator
        while (pivotVal < course[high].name)
        {
            high -= 1;
        }

        // if iterators meet exit or swap values
        if (low >= high)
        {
            done = true;
        }
        else
        {
            swap(course[low], course[high]);
            low += 1;
            high -= 1;
        }
    }

    return high;
}

/**
 * Perform a quick sort on course id
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param course address of the vector<Course> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 *
 */
void quickSort(vector<Course> &course, int begin, int end)
{
    // base statement
    if (begin >= end)
    {
        return;
    }

    // set low bounds for splitting sort
    int low = partition(course, begin, end);
    // recursive lower bounds
    quickSort(course, begin, low);
    // recuirsive upper bounds
    quickSort(course, low + 1, end);
}

/******************* Main program *******************/

/**
 * Program entry point and menu interaction
 *
 * @param argc count of arguments passed by cli
 * @param argv array pointer of all passed arguments
 * @return status of exit
 */
int main(int argc, char *argv[])
{
    int choice = 0;
    vector<Course> courseList;
    string searchCourseID;
    Course searchResult;

    while (choice != 9)
    {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do ? ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (loadcourselist(courseList))
            {
                cout << "Course data loaded successfuly." << endl;
            }
            else
            {
                cout << "There was a problem loaded the course data, validate data file and prerequisite course2 are included" << endl;
            }
            break;
        case 2:
            quickSort(courseList, 0, courseList.size() - 1);
            printCourseList(courseList);
            break;
        case 3:
            cout << "What course do you want to know about? ";
            cin >> searchCourseID;

            searchResult = SearchCourseListByID(courseList, searchCourseID);

            if (searchResult.name == "")
            {
                cout << "Course not found" << endl;
            }
            else
            {
                printCourse(searchResult, true);
            }
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option" << endl;
            // case 9 : caught by while loop
        }
    }

    return 0;
}
