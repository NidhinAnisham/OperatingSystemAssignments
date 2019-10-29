/* This program performs Selection Sort on a Linked List by swapping nodes */

public class SelectionSort {

	//Class for each individual Linked List Node
	static class LinkedListNode {
		public int data;
        public LinkedListNode next;

        public LinkedListNode(int nodeData) {
            this.data = nodeData;
            this.next = null;
        }
    }
	
	//Class for the Linked List Data Structure
    static class LinkedList {
        public LinkedListNode head;
        public LinkedListNode tail; //using tail so that insert is easier
		
        public LinkedList() {
            this.head = null;
            this.tail = null;
        }
		
		//function to insert new node at the end of the Linked List
        public void insert(int nodeData) {
            LinkedListNode node = new LinkedListNode(nodeData);
            if (this.head == null) {
                this.head = node; //assigning head if Linked List is new
            } 
            else {
                this.tail.next = node; //adding new node to the tail
            }
            this.tail = node; //updating the tail
        }
        
		//function to traverse through the Linked List where the head is passed as parameter
        public void traverse(LinkedListNode tmp) {
        	if(tmp == null) {
        		System.out.println();
        		return;
        	}
        	else {
        		System.out.print(tmp.data+"\t");
        		traverse(tmp.next);
        	}       	
        }
        
		//function to perform selection sort om the Linked List
        public void selection_sort() {
        	LinkedListNode current1 = this.head; //this is used as the first pointer for the list
        	LinkedListNode prev1 = null; //this is used to point to the node previous to current
        	while(current1!=null) {
        		this.traverse(this.head); //printing the traversal of the array as it is being sorted
        		LinkedListNode current2 = current1.next; //this is used as second pointer for the list that traverses through unsorted region
        		LinkedListNode min = current1; //this is used to point to the minimum value in the unsorted region
        		LinkedListNode prev2 = current1; //this is used to store the node previous to current2
        		LinkedListNode prev_min = prev1; //this is used to store the node previous to min
        		while(current2!=null) {
        			if(min.data>current2.data) { //change min to current2 if it is lesser than min
        				min = current2;
        				prev_min = prev2;
        			}
        			current2 = current2.next;
        			prev2 = prev2.next;
        		}
				
				//logic to swap min with current1 explained in Readme file
        		if(min != current1) { //swap only if min has changed
        			if(current1.next != min) {
        				LinkedListNode tmp = current1.next;
            			current1.next = min.next;
            			min.next = tmp;
	        			if(prev1 == null) { //head is to be swapped
	        				LinkedListNode tmp2 = this.head;
	        				this.head = prev_min.next;
	        				prev_min.next = tmp2;
	        			}
	        			else {
	        				LinkedListNode tmp2 = prev1.next;
	        				prev1.next = prev_min.next;
	        				prev_min.next = tmp2;
	        			}
        			}
        			else { //elements are next to each other
            			current1.next = min.next;
            			min.next = current1;
        				if(prev1 == null) {
	        				this.head = min;
	        			}
	        			else {
	        				prev1.next = min;
	        			}
        			}
        		}
        		prev1 = min;
        		current1 = min.next;        		
        		prev2 = current1;
        	}
        }
        
    }
	
	//main function for testing the sort
	public static void main(String[] args){
		LinkedList to_sort = new LinkedList();
		int[] array = new int[]{6,14,5,15,10,1,3,7,2,9,11,12,4,13,8};   //linked list data is passed as an array
		for(int i = 0;i < array.length; i++) {
			to_sort.insert(array[i]);
		}
		System.out.println("Before Sort:");
		to_sort.traverse(to_sort.head); //traversal before sort
		System.out.println();
		to_sort.selection_sort();
		System.out.println();
		System.out.println("After Sort:");
		to_sort.traverse(to_sort.head); //traversal after sort
	}
}
