#ifndef priority_queue_h_
#define priority_queue_h_

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
using namespace std;

// A custom templated priority queue.  The priority queue is a min
// heap (smallest values on top).  The heap is stored in a vector.
// The integer location for an element within the vector can be
// quickly looked up using a map.

// find out the current index of the element within the heap.

// ASSIGNMENT: The class implementation is incomplete.  Finish the
//   implementation of this class, and add any functions you need.


// =========================================================================

template <class T>
class PriorityQueue {
 public:
  // CONSTRUCTOR
  PriorityQueue() {}

  // =========================
  // ACCESSORS
  unsigned int size() const { return m_heap.size(); }
  T top() const;

  // =========================
  // MODIFIERS
  // add an element
  void push(T element);
  // remove the top element, or an element elsewhere in the structure
  void pop();
  void remove(T element);
  // update the priority of an element somewhere in the structure
  void update_position(T element);

  // =========================
  // for debugging
  void print_heap(std::ostream & ostr) const;
  bool check_heap() const;
  
 private:
  // =========================
  // PRIVATE ACCESSOR FUNCTIONS
  int last_non_leaf() const { return ((int)size()-1) / 2; }
  int get_parent(int i) const { assert (i > 0 && i < (int)size()); return (i-1) / 2; }
  bool has_left_child(int i) const { return (2*i)+1 < (int)size(); }
  bool has_right_child(int i) const { return (2*i)+2 < (int)size(); }
  int get_left_child(int i) const { assert (i >= 0 && has_left_child(i)); return 2*i + 1; }
  int get_right_child(int i) const { assert (i >= 0 && has_right_child(i)); return 2*i + 2; }
  // PRIVATE MODIFIER FUNCTIONS
  void percolate_up(int i);
  void percolate_down(int i);

  // =========================
  // REPRESENTATION
  // the heap is stored in a vector representation 
  // (the binary tree structure "unrolled" one row at a time)
  std::vector<T> m_heap;
  // the map stores a correpondence between elements & indices in the heap
  std::map<T,int> locations;

};

// =========================================================================


// access the top element (minimum value) in the heap
template <class T>
T PriorityQueue<T>::top() const  {
  assert(!m_heap.empty());
  return m_heap[0]; 
}


// add a new element to the heap
template <class T>
void PriorityQueue<T>::push(T element) {
  // first, verify that the element isn't already in the heap
  typename std::map<T,int>::iterator itr = locations.find(element);
  if (itr != locations.end()) {
    std::cout << "ERROR!  priority queue aleady contains " << element << std::endl;
    assert (element == itr->first);
  }
  assert (itr == locations.end());
  // add the element at the edge of heap vector and percolate up
  m_heap.push_back(element);
  locations[element] = m_heap.size()-1;
  this->percolate_up(int(m_heap.size()-1));
                                                              this->check_heap();
}


// remove the top element (minimum value) from the heap
template <class T>
void PriorityQueue<T>::pop() {
  assert(!m_heap.empty());
  int success = locations.erase(m_heap[0]);
  assert (success == 1);
  // place the last element temporarily at the top of the heap, but
  // push it down to a proper position using percolate down
  m_heap[0] = m_heap.back();
  m_heap.pop_back();
  this->percolate_down(0);
}


// remove a specific element that could be anywhere in the heap ASSIGNMENT
//locations is <T,int>
template <class T>
void PriorityQueue<T>::remove(T element) {
  //swap the element with the last element
  //remove the last element 
  //percolate down with where the element was
  assert(!m_heap.empty());
  typename map<T,int>::iterator itr = locations.find(element);
  if(itr == locations.end()) return;
  int position = itr->second;

//deal with the map
  locations.erase(itr);
  assert(locations.find(element) == locations.end());
//switch the last element with the element we want to remove
  Edge* temp = m_heap[position];
  m_heap[position] = m_heap[m_heap.size()-1];
  m_heap[m_heap.size()-1] = temp;
//update the map
  //dont need to update the one we want to remove because its already gone
  locations[m_heap[position]] = position;
//erase the element we want to remove that is now at the end of the heap
  int temp_size = m_heap.size();
  m_heap.erase(m_heap.begin()+m_heap.size()-1);
  assert(m_heap.size() == temp_size-1);

//still need to mess with percolations
  //the last element in the list needs to percolate down now
  this->percolate_down(position);

}


// this element may have had its value change, so we should adjust the
// position of the element within the heap (it might need to move up
// or down)
template <class T>
void PriorityQueue<T>::update_position(T element) {
  typename std::map<T,int>::iterator itr = locations.find(element);/*
  assert (itr != locations.end());
  this->percolate_up(itr->second);
  this->percolate_down(itr->second);*/

  assert (itr != locations.end());
  this->percolate_up(itr->second);
  this->percolate_down(itr->second);
}

// =========================================================================
// =========================================================================

// for debugging, print all of the data in the heap
template <class T>
void PriorityQueue<T>::print_heap(std::ostream & ostr) const {
  
  for (int i=0; i<(int)m_heap.size(); ++i)
    ostr << "[" << std::setw(4) << i << "] : " 
         << std::setw(6) << m_heap[i]->getPriorityValue() 
         << " " << *m_heap[i] << std::endl;
}


// for debugging, walk over the heap and check all of the elements &
// their values
template <class T>
bool PriorityQueue<T>::check_heap() const {
  if (m_heap.size() <= 1) return true;
  bool error_found = false;
  
  // first check to see if the element values are up-to-date
  for (int i=0;i< m_heap.size(); ++i) {
    if (m_heap[i]->CheckValue()) {
      
      error_found = true;
    }

  }
  
  // next loop over all nodes that have at least one child
  for (int i=0; has_left_child(i); ++i) {
    assert (has_left_child(i));
    int left_child = 2*i+1;
    // verify that this is a min heap
    // (parent is smaller value than both children
    //cout<<m_heap[left_child]->getPriorityValue()<<endl;
    if (m_heap[i]->getPriorityValue() > m_heap[left_child]->getPriorityValue()) {
      error_found = true;
      std::cout << "Error: at heap location " << i 
                << ", the value is greater than the value at the left child.\n";
      std::cout << "value@" << i<< "=" <<  m_heap[i]->getPriorityValue() 
                << "value@" << left_child << "=" << m_heap[left_child]->getPriorityValue() << std::endl;
      
    }
    int right_child = 2*i+2;
    if (right_child < (int)m_heap.size() && 
        m_heap[i]->getPriorityValue() > m_heap[right_child]->getPriorityValue()) {
      error_found = true;
      std::cout << "Error: at heap location " << i 
                << ", the value is greater than the value at the right child.\n";
      std::cout << "value@" << i<< "=" <<  m_heap[i]->getPriorityValue() 
                << "value@" <<right_child << "=" << m_heap[right_child]->getPriorityValue() << std::endl;
    }
  }

  if (error_found) {
    print_heap(std::cout);
  }
  return !error_found;
}


// =========================================================================
//ASSIGNMENT - update the map too
// =========================================================================

/*

  int last_non_leaf() return ((int)size()-1) / 2;
  int get_parent(int i) return (i-1) / 2; 
  bool has_left_child(int i) return (2*i)+1 < (int)size(); 
  bool has_right_child(int i) return (2*i)+2 < (int)size(); 
  int get_left_child(int i) return 2*i + 1; 
  int get_right_child(int i) return 2*i + 2; 

percolate_up(TreeNode<T> * p) {
while (p->parent)
if (p->value < p->parent->value) {
swap(p, parent);
p = p->parent;
}
else
break;
}

*/

// allow the element at this location to move up
template <class T>
void PriorityQueue<T>::percolate_up(int i) {
  if(m_heap.size() <2 || i==0) return;
  //cout<<"getting parent for: "<<i<<endl;
  while(i>0 && get_parent(i)>=0){
    //cout<<"THIS: "<<m_heap[i]<<"  HAS PARENT: "<< m_heap[get_parent(i)]<< endl;
    //cout<<"if("<<m_heap[i]<<"<"<<m_heap[get_parent(i)]<<")"<<endl;
    if(m_heap[i]->getPriorityValue()<m_heap[get_parent(i)]->getPriorityValue()){
      Edge* temp = m_heap[i];
      m_heap[i] = m_heap[get_parent(i)];
      m_heap[get_parent(i)]=temp;
      //deal with the map
      locations[m_heap[i]] = i;
      locations[m_heap[get_parent(i)]]=get_parent(i);

      i=get_parent(i);
    }
    else{
      break;
    }
  }
  

}
/*

  int last_non_leaf() return ((int)size()-1) / 2;
  int get_parent(int i) return (i-1) / 2; 
  bool has_left_child(int i) return (2*i)+1 < (int)size(); 
  bool has_right_child(int i) return (2*i)+2 < (int)size(); 
  int get_left_child(int i) return 2*i + 1; 
  int get_right_child(int i) return 2*i + 2; 


  percolate_down(TreeNode<T> * p) {
while (p->left) {
TreeNode<T>* child;
if (p->right && p->right->value < p->left->value)
child = p->right;
else
child = p->left;
if (child->value < p->value) {
swap(child, p);
p = child;
}
else
break;
}
}


*/

// allow the element at this location to move down
template <class T>
void PriorityQueue<T>::percolate_down(int i) {
  while(has_left_child(i)){
    int first_switch;
    if(has_right_child(i) && m_heap[get_right_child(i)]->getPriorityValue()<m_heap[get_left_child(i)]->getPriorityValue() ){
      first_switch = get_right_child(i);
    }
    else{
      first_switch = get_left_child(i);
    }
    if(m_heap[first_switch]->getPriorityValue() < m_heap[i]->getPriorityValue()){
      Edge* first_switch_edge = m_heap[first_switch];
      m_heap[first_switch] = m_heap[i];
      m_heap[i] = first_switch_edge;
      //deal with the map
      locations[m_heap[i]] = i;
      locations[m_heap[first_switch]] = first_switch;

      i=first_switch;
    }
    else{
      break;
    }
  }
}

// =========================================================================
// =========================================================================
#endif
