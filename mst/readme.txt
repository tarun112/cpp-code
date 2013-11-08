
I have previous C++ experience, so this assignment and the course is a good 
refresher as well as a good way to pick up latest C++ concepts.  In this 
assignment I have used STL extensively.  I have used STL containers vector, 
map, list & STL algorithms related to sort, find, random, chrono, & others. 
Graph is stored using Edge-List, which is better performing in both processing 
& memory for sparse large graphs (20%, 40% edge densities).   Graph encapsulates 
all methods & variables by using only ids (int in this case) of nodes and edges.  All 
other processing and resolution are done within Graph itself.
      
STL is updated with new features so not only I get to refresh my STL skills but 
to learn some new ways to code with STL.  Some of the new features I have learned 
and used in this assignment includes:  
-convenient use of ‘auto’ instead of long declarations for iterators,  
-lambda expressions as comparison functions,  
-‘chrono’ for time related functions; in this case computing time taken by a process.


Naming conventions used to maximize readability:
-Data types (classes, typedefs) are as  ‘OneTwoThree’
-Local variables are in all small as  ‘onetwothree’
-Member variables are as  ‘m_oneTwoThree’
-Methods & functions are as  ‘oneTwoThree’

The code is edited, compiled & built on Visual Studio Express 2013

Output.png contains screenshot of result for sampledata.txt in png format.

The result for last run is:
-Minimum Spanning Tree total cost : 30
-Minimum Spanning Tree edges (with costs):
 0 - 2  (2)
 2 - 9  (1)
 8 - 9  (3)
 4 - 8  (1)
 4 - 7  (1)
 4 - 15  (2)
 7 - 10  (2)
 15 - 19  (2)
 9 - 12  (3)
 3 - 12  (1)
 11 - 12  (1)
 11 - 14  (1)
 12 - 17  (1)
 1 - 17  (1)
 1 - 6  (1)
 5 - 6  (1)
 5 - 18  (1)
 5 - 16  (2)
 9 - 13  (3)

Time taken : 1 milliseconds

-------------------------


