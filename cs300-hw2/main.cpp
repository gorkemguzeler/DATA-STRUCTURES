//
//  main.cpp
//  cs300-hw2
//
//  Created by gorkemg on 21.11.2020.
//

#include <iostream>
#include "Header.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(){

    Quad QuadTree;
    
    string line;
    ifstream file_input;

   file_input.open("cities1.txt");

    if(file_input.fail()){
        cout << "Failed to open cities file.." << endl;
        return 0;
    }

    int x,y;

    getline(file_input, line);
    {
        istringstream input(line);
        input >> x  >>  y  ;
    }

    QuadTree.set_boundary(x, y);

    string city_name ;
    int city_x = 0, city_y = 0;

    while(getline(file_input, line))
    {
        istringstream str(line);
        str >> city_name >> city_x >> city_y ;

        Node* subtree = new Node;
        subtree->name = city_name;
        subtree->coordinate_x = city_x ;
        subtree->coordinate_y =city_y;

        QuadTree.insert(subtree); 
    }

    QuadTree.pretty_print(QuadTree.get_root()); // pretty print the quadtree rooted at root

    string line1;
    ifstream file_input1;
    
    file_input1.open("queries9.txt");
    
    if(file_input1.fail()){
        cout << "Failed to open queries file.." << endl;
        return 0;
    }
        
    int query_x, query_y , query_radius;
    
    char comma;
    
    while(getline(file_input1, line1))
    {
        istringstream str1(line1);
        str1 >> query_x >> comma >> query_y >> comma >> query_radius ;

        QuadTree.search(query_x, query_y, query_radius,QuadTree.get_root());
        
        QuadTree.display_cities();
    }

    return 0;
}
