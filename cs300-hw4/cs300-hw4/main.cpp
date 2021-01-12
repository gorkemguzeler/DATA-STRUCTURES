//
//  main.cpp
//  cs300-hw4
//
//  Created by gorkemg on 19.12.2020.
//

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Header.h"
#include <fstream>

using namespace std;

struct seat{
    string block_name; //vector
    string row_names; //vector
    int column_number;

    seat(string b, string r, int c){
        block_name = b;
        row_names = r;
        column_number = c;
    }

    bool operator==(const seat &other) const
     {
         return (block_name == other.block_name
               && row_names == other.row_names
               && column_number == other.column_number);
     }
    
    seat(){}

};

namespace std {
template <>
struct hash<seat>
  {
    std::size_t operator()(const seat& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      return ((hash<string>()(k.block_name)
               ^ (hash<string>()(k.row_names) << 1)) >> 1)
               ^ (hash<int>()(k.column_number) << 1);
    }
  };
}

int main(){
    
    ofstream out;
    out.open ("out.txt");
    
    string line;
    ifstream file_input;

   file_input.open("inputs9.txt");

    if(file_input.fail()){
        out << "Failed to open input file.." << endl;
        return 0;
    }

    string block_name, row_name;
    vector<string> my_blocks;
    vector<string> my_rows;
    int number_of_columns;

    
    getline(file_input, line);
    {
        istringstream input(line);
        while(input >> block_name){
            my_blocks.push_back(block_name);
        };
    }
    
    
    getline(file_input, line);
    {
        istringstream input(line);
        while(input >> row_name){
            my_rows.push_back(row_name);
        }
    }
    
    
    getline(file_input, line);
    {
        istringstream input(line);
        input >> number_of_columns ;
    }
    
    unordered_map<seat, string> stadium;
    
    unordered_map<string, seat> reserves;
    
    unordered_map<string, int> find_index; //name keeps the row name,int keeps the corresponding index of heap
    
    //heaps must be mapped by another unordered map: row name -> to heap
    
    vector<MPQ> my_queue(my_rows.size()); //create for each row, hold empty seat number with block name
    
    for(int k = 0; k < my_rows.size() ;k++){
        for(int i = 0; i < my_blocks.size() ;i++){
            node my_node;
            my_node.block_name = my_blocks[i];
            my_node.empty_seat = number_of_columns;
            my_node.priority = i; //lower number has high priority
            my_queue[k].insert(my_node);
            
        }
    }
    
    int q = 0;
    for(int k = 0; k < my_rows.size() ;k++){
        find_index[my_rows[k]] = q;
        q++;
    }
    
    string k , name , b_name, r_name;
    int c_num;
    
    while(getline(file_input, line))
    {

        istringstream str(line);
        str >> k;


        if (k == "print") {

            for(int i = 0; i< my_blocks.size()  ;i++){
                out << endl;
                out << my_blocks[i] << endl;
                out << "~~~~~~~" << endl;
                for(int y = 0; y < my_rows.size()  ;y++){
                    out << my_rows[y] << " : " ;
                    for(int z = 0; z< number_of_columns ;z++){
                       
                        if(stadium.find(seat(my_blocks[i],my_rows[y],z)) == stadium.end()){ //not reserved
                            out << "--- " ;
                        }
                           
                        else if(stadium.find(seat(my_blocks[i],my_rows[y],z)) != stadium.end()){ //already reserved
                            out << (stadium[seat(my_blocks[i],my_rows[y],z)]).substr(0,3) << " " ;
                        }
                    } out << endl;
                } out << "=======" << endl;
            }
            out << endl;
        }
        
        else if (k == "reserve_seat") {
            
            str >> name >> b_name >> r_name >> c_num;
            
            if(stadium.find(seat(b_name,r_name,c_num)) != stadium.end()){ //seat taken
                out << name << " could not reserve a seat!" << endl;
            }
            
            else if(reserves.find(name) != reserves.end() ){ //customer have another reservation
                
                out << name << " could not reserve a seat!" << endl;
            }
            
            else{ //seat is not taken and customer has no reservation before
                stadium[seat(b_name,r_name,c_num)] = name;
                reserves[name] = seat(b_name,r_name,c_num);
                my_queue[find_index[r_name]].decrement_empty_seat(reserves[name].block_name);
                out << name << " has reserved " << b_name << " " << r_name << "-" << c_num << endl;
            }
            
        }
        
        else if (k == "cancel_reservation") {
            str >> name;
            
            if(reserves.find(name) != reserves.end() ){ //name reserved a seat before
                stadium.erase(seat(reserves[name].block_name,reserves[name].row_names,reserves[name].column_number) );
                my_queue[find_index[reserves[name].row_names]].increment_empty_seat(reserves[name].block_name);
                reserves.erase(name);
                out << "Cancelled the reservation of " << name << endl;
            }
                   
            else{ //if name did not reserve any seat
                out << "Could not cancel the reservation for " << name << "; no reservation found!" << endl;
            }
            
        }
        
        else if (k == "get_seat") {
            str >> name;
          
            if( reserves.find(name) != reserves.end() ){ //name reserved a seat before
                string b_n = reserves[name].block_name;
                string r_n = reserves[name].row_names;
                int c_n = reserves[name].column_number;
                out << "Found that " << name << " has a reservation in " << b_n << " " << r_n << "-" << c_n << endl;
            }
                   
            else{ //if name did not reserve any seat
                out << "There is no reservation made for " << name << "!" << endl;
            }
            
        }
        
        else if (k == "reserve_seat_by_row") {
            str >> name >> r_name;
            
            if( reserves.find(name) != reserves.end() ){ //customer has reservation
                out <<  name << " could not reserve a seat!" << endl;
            }
            
            else if (my_queue[find_index[r_name]].GetMax().empty_seat == 0){ //empty seat is not found on the row
              out <<  name << " could not reserve a seat!" << endl;
            }
            
            else{ //customer made reservation
                
                string b_name = my_queue[find_index[r_name]].GetMax().block_name;
                
                for(int z = 0; z< number_of_columns ;z++){
                    if(stadium.find(seat(b_name,r_name,z)) == stadium.end()){ //not reserved 
                        stadium[seat(b_name,r_name,z)] = name;
                        reserves[name] = seat(b_name,r_name,z);
                        my_queue[find_index[r_name]].decrement_empty_seat(b_name);
                        break;
                    }
                }
                
                out << name  << " has reserved " << reserves[name].block_name << " " << reserves[name].row_names << "-" << reserves[name].column_number << " by emptiest block" << endl;
            }
        }
        
        k = "";
    }
    
    out.close();
    
    return 0;
}

