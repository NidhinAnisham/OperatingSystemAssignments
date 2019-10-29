Using SelectionSort.java

1. Compile & Run!
2. For changing the input data, edit variable "array" in main()


Logic for swapping min with current:
1. Swap next values of both current and min
2. Swap next values of nodes previous to current and min

There are four cases:
1. Swap elements that are not next to each other
--Use logic above

2. Swap between head and an element that is not next to the head
--make element as head and make head.next as the old head and old head.next as element.next

3. Swap between head and element next to it
--make head.next as element.next and make element.next as old head. Make element as head

4. Swap elements next to each other
--make current1.next as min.next and make min.next as current1. Make element previous to current1 point to min 
