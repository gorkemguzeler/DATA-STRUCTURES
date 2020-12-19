//
//  Header.h
//  cs300-hw3
//
//  Created by gorkemg on 8.12.2020.
//

#ifndef Header_h
#define Header_h

#include <vector>
#include <iostream>

using namespace std;

struct node{
    
    int x_to_compare;
    int x_side;
    int x_left_value;
    int x_right_value;
    int y_value;
    int label;
    
    node(){
        x_to_compare= 0;
        x_side= 0; //0 for left , 1 for right
        x_left_value = 0;
        x_right_value = 0;
        y_value = 0;
        label = 0;
    }
};

class MPQ
{
  public:
    MPQ(){ //initiliaze private variables
        currentSize = 0;
        max_height= 0;
        x_value = 0;
        current_label= 0;
    };
    
    ~MPQ(){};
    
    void insert( node ); // This method inserts an item with the given value and label into the MPQ.
    void skyline(); //take array of nodes, sort according to the x values, display output of skyline
    int GetMax(vector<node>); // This method returns the maximum value that is currently stored. (Note that contrary to priority queues, we do not remove the value from the MPQ data structure!)
    void heapsort(vector<node> & );
    void percDown( vector<node> &a, int i, int n );
    inline int leftChild( int i );
    void check(); //check if the max height has changed, if so output the new one.
    void remove_from_skyline(int );
    
  private:
    
    int currentSize; // Number of elements in heap
    vector<node> array; // The heap array
    vector<int> location; //whose ith entry contains the position in the heap of the item having label i.
    vector<node> x_compare; //store x_right and x_left values of each node
    vector<node> skyline_mpq; //store visible buildings at x_value
    int max_height;
    int x_value;
    int current_label;
};

//******************************IMPLEMENTATIONS******************************


int MPQ::GetMax(vector<node> array){
    
    int max_value = 0;
    
    for(int i= 0 ; i < array.size() ;i++){
        if (array[i].y_value > max_value) {
            max_value = array[i].y_value;
        }
    }
    
    return max_value;
}

void MPQ::insert( node new_node )
{
    currentSize++;
    array.push_back(new_node);
    location.push_back(new_node.label);
}

inline int MPQ::leftChild( int i )
{
    return 2*i + 1;
}

  // a is the array, i is the position to percolate down , n is the  size of the array
void MPQ::percDown( vector<node> &a, int i, int n )
{
        int child;
        node tmp;

        for(tmp=a[i] ; leftChild(i) < n; i = child )
        {
           child = leftChild(i);
           if( child != n-1 && a[child].x_to_compare < a[ child+1 ].x_to_compare )
                child++;
           if( a[child].x_to_compare > tmp.x_to_compare )
               a[i] = a[child];
           else
               break;
       }
       a[ i ] = tmp;
}

void MPQ::heapsort(vector<node> &array)
{
    
    // buildHeap
    for(int i = array.size()/2; i >=0; i--){
        percDown(array, i, array.size());
    }
    
    // sort
    int m = (array.size()-1);
    for(int j = m ; j >0 ; j--){
        swap(array[0], array[j]);  // swap max to the last pos.
        percDown(array, 0, j); // re-form the heap
    }
}

void MPQ::remove_from_skyline(int label){
  
    for(int i = 0 ; i < skyline_mpq.size() ;i++){
        if(skyline_mpq[i].label == label){
            skyline_mpq.erase(skyline_mpq.begin()+i);
        }
    }
}

void MPQ::check(){ //check if max height has changed, if so output new value
    
    if(max_height < GetMax(skyline_mpq)){
        max_height = GetMax(skyline_mpq);
        cout << x_value << " " << max_height << endl;
    }
    
    else if(max_height > GetMax(skyline_mpq)){
        max_height = GetMax(skyline_mpq);
        cout << x_value << " " << max_height << endl;
    }
}

void MPQ::skyline(){ //take array of nodes, sort according to the x values, display output of skyline
    
    for(int i= 0 ; i < array.size() ;i++){
        node right;
        right.label = array[i].label;
        right.x_to_compare= array[i].x_right_value;
        right.y_value= array[i].y_value;
        right.x_side=1;
        
        node left;
        left.label = array[i].label;
        left.x_to_compare= array[i].x_left_value;
        left.y_value= array[i].y_value;
        left.x_side =0;
        
        x_compare.push_back(right);
        x_compare.push_back(left);
    }
    
    heapsort(x_compare);
    
    if(x_compare[0].x_to_compare > 0 ){
        cout << x_value << " " << max_height << endl;
    }

    for(int i= 0 ; i < x_compare.size() ;i++){   //eğer eşitlik varsa case'i kontrol et
        
        while(x_compare[i].x_to_compare == x_compare[i+1].x_to_compare){
            if(x_compare[i].x_side == 0){ //left , add to mpq
                skyline_mpq.push_back(x_compare[i]);
                x_value = x_compare[i].x_to_compare;
            }
            
            if(x_compare[i].x_side == 1){ //right , remove from mpq with a spesific label.
                remove_from_skyline(x_compare[i].label);
                x_value = x_compare[i].x_to_compare;
            }
            
            i++;
            
        }
        
        if(x_compare[i].x_side == 0){ //left , add to mpq
            skyline_mpq.push_back(x_compare[i]);
            x_value = x_compare[i].x_to_compare;

        }
        
        if(x_compare[i].x_side == 1){ //right , remove from mpq with a spesific label.
            remove_from_skyline(x_compare[i].label);
            x_value = x_compare[i].x_to_compare;
        }
        
        check();
    }
}

#endif /* Header_h */


