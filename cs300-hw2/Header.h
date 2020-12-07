//
//  Header.h
//  cs300-hw2
//
//  Created by gorkemg on 21.11.2020.
//

#ifndef Header_h
#define Header_h

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Node{
    
    int boundary_left;
    int boundary_top;
    int boundary_right;
    int boundary_bottom;
    
    int coordinate_x;
    int coordinate_y;
    
    string name;
    
    Node *north_west;
    Node *north_east;
    Node *south_west;
    Node *south_east;    
    
    Node(){
        north_west = nullptr;
        north_east = nullptr;
        south_west = nullptr;
        south_east = nullptr;
    }
};

// The main quadtree class
class Quad
{
    
public:
    Quad(){
        root = nullptr;
        
        most_left = 0;
        most_bottom = 0;
    }
    
    void display_cities();
    void pretty_print(Node*);
    void set_boundary(int x, int y);
    void insert(Node* );
    void search(int , int , int, Node* );
    Node* compare(Node* , Node*);
    Node* compare_search(int , int , int  , Node*  );
    Node* son(Node* , Node*);
    Node* get_root();
    
private:
    
    Node* root;
    
    int most_left;
    int most_top;
    int most_right;
    int most_bottom;
    
    vector<string> near_cities;
    vector<string> visited_cities;

};

//******************************IMPLEMENTATIONS******************************
Node* Quad::get_root(){
    return root;
}

void Quad::display_cities(){
    
    cout << endl;
    long int m = near_cities.size();
    
    for(int j = 0 ; j< m-1 ; j++){
        cout << near_cities[j] << ", "  ;
    }
    
    if(m > 0){
        cout << near_cities[m-1];
    }

    
    if(near_cities.size() == 0){
        cout << "<None>" ;
    }

    cout << endl;
    long int n = visited_cities.size();
    for(int i = 0 ; i< n-1 ; i++){
        cout <<  visited_cities[i] << ", "  ;
    }
    if(n > 0){
        cout << visited_cities[n-1];
    }
    
    if(visited_cities.size() == 0){
        cout << "None" ;
    }
    
    cout << endl;
    
    near_cities.clear();
    visited_cities.clear();
}

void Quad::pretty_print(Node* root){//print the quadtree rooted at root:
 
    if (root != NULL){ // if the tree is not empty
        cout << root->name << endl;
        pretty_print(root->south_east);
        pretty_print(root->south_west);
        pretty_print(root->north_east);
        pretty_print(root->north_west);
    }
}

void Quad::set_boundary(int x, int y){
     most_top = x ;
     most_right = y ;
}

Node* Quad::compare(Node* point1, Node* point2)//shows p1's position according to p2. upper right boundaries are open in case of equality.
{
                                                        
    if (point1->coordinate_x < point2->coordinate_x){
        
        if(point1->coordinate_y < point2->coordinate_y){
            return point2->south_west;
        }
        else{
            return point2->north_west;
            }
    }
    else if(point1->coordinate_y < point2->coordinate_y){
        return point2->south_east;
    }
    else{
        return point2->north_east;
    }
}

Node* Quad::son(Node* point1, Node* point2) // p1 = to be inserted, p2 = father
{
    if (point1->coordinate_x < point2->coordinate_x){

        if(point1->coordinate_y < point2->coordinate_y){
             return point2->south_west = point1;
        }
        else{
             return point2->north_west = point1;
            }
    }
    else if(point1->coordinate_y < point2->coordinate_y){
         return point2->south_east = point1;
            }
            else{
             return point2->north_east = point1;
            }
}
    
void Quad::insert(Node *p) //inserting node p to the quadtree 
{
    Node* f = nullptr;
    Node* t;
    Node* q;
    
    if(p->coordinate_x <= most_right &&  p->coordinate_y <= most_top && p->coordinate_x >= most_left &&  p->coordinate_y >= most_bottom ){
        
        if (root == NULL) //tree is empty
        {
            root = p;
        }
        else
        {
            t = root;

            while(t != nullptr && ((p->coordinate_x != t->coordinate_x) || (p->coordinate_y != t->coordinate_y))){ //go over each node till you reach out the same node or nullptr
            f = t;
            q = compare(p,t);
            t = q;
            }

            if(t == nullptr){ //if you reach the nullptr, add the node. else we already have it, duplicate.
                son(p,f);
            }
        }
    }

}

Node* Quad::compare_search(int x, int y, int radius , Node* point2 )
{
    //upper right boundaries are open in case of equality!!!!!
    //create a square with the boundaries of p1:
    int left_boundary = x - radius;
    int right_boundary = x + radius;
    int top_boundary = y + radius;
    int bottom_boundary  = y - radius;
    
    //CONDITIONS:
    
    // 13)distance of p2 and p1 < radius - all
    if (sqrt(pow((x - point2->coordinate_x), 2) + pow((y - point2->coordinate_y),2)) <= radius ){
        search(x, y , radius, point2->south_east);
        search(x, y , radius, point2->south_west);
        search(x, y , radius, point2->north_east);
        search(x, y , radius, point2->north_west);
    }
    
    // 1) lb < p2x and tb < p2y - se
    else if (left_boundary >= point2->coordinate_x && top_boundary < point2->coordinate_y){
        search(x, y , radius, point2->south_east);
    }
    // 2) p2x < rb and p2x > lb and p2y > tb  - se, sw
    else if (right_boundary >= point2->coordinate_x && left_boundary < point2->coordinate_x && top_boundary < point2->coordinate_y){
        search(x, y , radius, point2->south_east);
        search(x, y , radius, point2->south_west);
    }
    // 3) p2x > rb and p2y > tb   - sw
    else if (right_boundary < point2->coordinate_x  && top_boundary < point2->coordinate_y){
        search(x, y , radius, point2->south_west);
    }
    // 4) p2x < lb and p2y < top and p2y > bottom  - se, ne
    else if (left_boundary >= point2->coordinate_x && top_boundary >= point2->coordinate_y && bottom_boundary < point2->coordinate_y){
        search(x, y , radius, point2->south_east);
        search(x, y , radius, point2->north_east);
    }
    // 5) p2y < top and p2y > bottom and p2x > right -sw,nw
    else if (right_boundary < point2->coordinate_x && bottom_boundary < point2->coordinate_y && top_boundary >= point2->coordinate_y){
        search(x, y , radius, point2->south_west);
        search(x, y , radius, point2->north_west);
    }
    // 6) p2y < bottom and p2x < left -ne
    else if (left_boundary >= point2->coordinate_x && bottom_boundary >= point2->coordinate_y){
        search(x, y , radius, point2->north_east);
    }
    // 7) p2x < right and p2x > left and p2y < bottom - ne,nw
    else if (right_boundary >= point2->coordinate_x && left_boundary < point2->coordinate_x && bottom_boundary >= point2->coordinate_y){
        search(x, y , radius, point2->north_east);
        search(x, y , radius, point2->north_west);
    }
    // 8) p2x > right and p2y < bottom -nw
    else if (right_boundary <= point2->coordinate_x && bottom_boundary >= point2->coordinate_y){
        search(x, y ,radius, point2->north_west);
    }
    // 9) p2y < top and p2x > left and p2y > (top+bottom)/2 and p2x < (right+left )/2 and distance of p2 and p1 > radius  (not in the sphere) - all but nw
    else if (left_boundary <= point2->coordinate_x && top_boundary >= point2->coordinate_y && point2->coordinate_y > (top_boundary+bottom_boundary)/2 &&  point2->coordinate_x < (right_boundary+left_boundary)/2 && sqrt(pow((x - point2->coordinate_x), 2) + pow((y - point2->coordinate_y),2)) > radius ){
        search(x, y , radius, point2->south_east);
        search(x, y , radius, point2->south_west);
        search(x, y , radius, point2->north_east);
    }
    
    // 10)p2y < top and p2x < right and p2y > (top+bottom)/2 and p2x > (right+left )/2 and distance of p2 and p1 > radius  (not in the sphere) -all but ne
    else if (right_boundary >= point2->coordinate_x && point2->coordinate_x > (right_boundary+left_boundary)/2 && top_boundary >= point2->coordinate_y && point2->coordinate_y > (top_boundary+bottom_boundary)/2 && sqrt(pow((x - point2->coordinate_x), 2) + pow((y - point2->coordinate_y),2)) > radius ){
        search(x, y , radius, point2->south_east);
        search(x, y , radius, point2->south_west);
        search(x, y , radius, point2->north_west);
    }
    
    // 11)p2y > bottom and p2x > left and p2y < (top+bottom)/2 and p2x < (right+left )/2 and distance of p2 and p1 > radius  (not in the sphere) - all but sw
    else if (left_boundary <= point2->coordinate_x && point2->coordinate_x < (right_boundary+left_boundary)/2 && bottom_boundary <= point2->coordinate_y && point2->coordinate_y < (top_boundary+bottom_boundary)/2 && sqrt(pow((x - point2->coordinate_x), 2) + pow((y - point2->coordinate_y),2)) > radius ){
        search(x, y , radius, point2->south_east);
        search(x, y , radius, point2->north_east);
        search(x, y , radius, point2->north_west);
    }
    // 12)p2y > bottom and p2x < right and p2y < (top+bottom)/2 and p2x > (right+left )/2 and distance of p2 and p1 > radius  (not in the sphere) - all but se
    else if (right_boundary >= point2->coordinate_x && point2->coordinate_x > (right_boundary+left_boundary)/2 && bottom_boundary <= point2->coordinate_y && point2->coordinate_y < (top_boundary+bottom_boundary)/2 && sqrt(pow((x - point2->coordinate_x), 2) + pow((y - point2->coordinate_y),2)) > radius ){
        search(x, y , radius, point2->south_west);
        search(x, y , radius, point2->north_east);
        search(x, y , radius, point2->north_west);
    }
    
    return 0;
}


// Find a node in a quadtree
void Quad::search(int x , int y , int radius, Node* root)
{
   //SE , SW , NE , NW. check in this order

    if (root != NULL){ // if the tree is not empty
        
        visited_cities.push_back(root->name);
        
        if(sqrt(pow((root->coordinate_x - x), 2) + pow((root->coordinate_y - y),2)  ) <= radius)
        {
            near_cities.push_back(root->name);
        }
        
        compare_search(x, y, radius, root); //continue to subtrees
    }
    
}
   
#endif /* Header_h */
