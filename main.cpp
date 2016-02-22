#include <iostream>

#include "ColParser.h"

#include "Graph.h"

#include "GraphColorer.h"

#include "string"

using namespace std;

int main()
{
    std::string filename1 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCP130.13.col";
    std::string filename2 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCP660.33.col";
    std::string filename3 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCP990.33.col";
    std::string filename4 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCPrnd83.9.col";
    std::string filename5 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCPrnd83.10.col";
    std::string filename6 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCPrnd127.13.col";
    std::string filename7 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCPrnd127.14.col";
    std::string filename8 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCPrnd157.9.col";
    std::string filename9 = "C:\\Users\\Alexey\\Documents\\AI_Lab1\\yuzGCPrnd157.10.col";

    ColParserResponse r;
    ColParser::ParseColFile(filename3, r);

    GraphColorer::ColorGraph(*(r.GetGraph()), 33, 10, 0.5);

    cout << "Hello World!" << endl;
    return 0;
}

