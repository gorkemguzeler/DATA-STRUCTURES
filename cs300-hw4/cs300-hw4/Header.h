//
//  Header.h
//  cs300-hw4
//
//  Created by gorkemg on 19.12.2020.
//

#ifndef Header_h
#define Header_h

#include <vector>
#include <iostream>

using namespace std;

struct node{
    
    int empty_seat;
    string block_name;
    int priority;
    
    node(){}
    
};

class MPQ
{
  public:
    MPQ(){
        node trivial;
        array.push_back(trivial);
    };
    
    ~MPQ(){};
    void remove(node );
    void insert( node );
    node GetMax();
    void percDown( vector<node> &a, int i, int n );
    void percUp( vector<node> &a, int i, int n );
    inline int leftChild( int i );
    void decrement_empty_seat(string );
    void increment_empty_seat(string );
    
  private:
    
    vector<node> array; // The heap array
    unordered_map<string, int> location; //locations of blocks in each row , block name to index of array
};

//******************************IMPLEMENTATIONS******************************
node MPQ::GetMax(){
    
    return array[1];
}

void MPQ::insert( node new_node )
{
    location[new_node.block_name] = array.size();
    array.push_back(new_node);
    
    percUp(array, array.size()-1 , array.size());
}

void MPQ::remove(node node){
      
    percDown(array, location[node.block_name] , array.size());
        
}

inline int MPQ::leftChild( int i )
{
    return 2*i;
}

// a is the array, i is the position to percolate up , n is the  size of the array
void MPQ::percUp( vector<node> &a, int i, int n )
{
      int parent;
      node tmp;
      node tmp2;
    
      for(tmp=a[i] ; i/2 > 0; i = parent )
      {
         parent = i/2;
         tmp2 = a[parent];
       
         if( a[parent].empty_seat < tmp.empty_seat )
           {
               a[parent] = a[i];
               location[a[parent].block_name] = parent;
               a[ i ] = tmp2;
               location[a[i].block_name] = i ;
              
           }
         else if(a[parent].empty_seat == tmp.empty_seat &&  tmp.priority < a[parent].priority)
         {
             a[parent] = a[i];
             location[a[parent].block_name] = parent;
             a[ i ] = tmp2;
             location[a[i].block_name] = i ;
         }
         else
             break;
     }
}

// a is the array, i is the position to percolate down , n is the  size of the array
void MPQ::percDown( vector<node> &a, int i, int n ) //swap with last element and percualte down
{
    int child;
    node tmp;

    int m = (array.size()-1);
    swap(array[i], array[m]);
    //LOCATION SWAPLE
    location[array[i].block_name] = i;
    location[array[m].block_name] = m;
    
    location.erase(array[m].block_name);
    array.erase(array.begin() + array.size()-1);
    n = n-1;
   
    for(tmp=a[i] ; leftChild(i) < n; i = child )
    {
       child = leftChild(i);
        
       if( child != n-1 && a[child].empty_seat < a[ child+1 ].empty_seat )
       {
           child++;
       }
       else if( child != n-1 && a[child].empty_seat == a[ child+1 ].empty_seat &&  a[child].priority > a[ child+1 ].priority)
       {
           child++;
       }
        
       if( a[child].empty_seat > tmp.empty_seat )
         {
             a[i] = a[child];
             location[a[i].block_name] = i;
             a[child] = tmp;
             location[a[child].block_name] = child ;
      
         }
       else if(a[child].empty_seat == tmp.empty_seat &&  tmp.priority > a[child].priority)
       {
           a[i] = a[child];
           location[a[i].block_name] = i;
           a[child] = tmp;
           location[a[child].block_name] = child ;
       }
       else
           break;
   }
}

void MPQ::decrement_empty_seat(string name){
      
    node tmp = array[location[name]];
    
    remove(array[location[name]]);
    
    tmp.empty_seat--;
    insert(tmp);
}

void MPQ::increment_empty_seat(string name){
  
    node tmp = array[location[name]];
    
    remove(array[location[name]]);
    
    tmp.empty_seat++;
    insert(tmp);
}

#endif /* Header_h */
