//
//  main.cpp
//  cs300-hw3
//
//  Created by gorkemg on 8.12.2020.
//

#include <iostream>
#include "Header.h"
#include <fstream>
#include <string>
#include <sstream>

int main(){
    
    MPQ my_queue;
    
    int building_label = 0;
    
    //    1. Read in the descriptions of all of the buildings from a file called input.txt
    string line;
    ifstream file_input;

   file_input.open("input3.txt");

    if(file_input.fail()){
        cout << "Failed to open cities file.." << endl;
        return 0;
    }

    int x_left,y,x_right , city_number;

    getline(file_input, line);
    {
        istringstream input(line);
        input >> city_number ;
    }
    
    while(getline(file_input, line))
    {
        istringstream str(line);
        str >> x_left >> y >> x_right ;

        node new_point;
        new_point.x_left_value = x_left;
        new_point.x_right_value = x_right;
        new_point.y_value = y;
        new_point.label = building_label;
        building_label++;
        
        my_queue.insert(new_point);
    }

    my_queue.skyline();
    
    return 0;
}

