NAME:  Henry Grover


ALGORITHM ANALYSIS: 
Assume v_0 vertices, e_0 edges, and t_0 triangles in the initial mesh,
reduced to t_final triangles using the -shortest criteria.  What is
the overall order notation for the program?  Analyze the separate
compoenents of the program to justify your answer.

ALGORITHM ANALYSIS -- LINEAR:

Building the sets of edges and triangles initially with add_edge() is O(v_0 + e_0 + t_0)
With the FindEdge() function, which goes through the edges in O(e_0) time.
Then for each of the edges selected to delete, we go through connected vertices and triangles and create 
		new ones connected to the invalid vertex, and update their length.
This is relatively inexpensive and is practically constant in the grand scheme of things. 

The overall time of the linear method is O(v_0 + e_0^2 + t_0)

ALGORITHM ANALYSIS -- PRIORITY QUEUE:

Adding triangles and verteces are similar in the priority queue, O(v_0 + e_0 + t_0), but adding each edge is an additional log(e_0)
		because we add it to our heap. in return though, getting each edge in the FindEdge() function is O(loge_0) because we 
		still have to update the other edges positions.
The overall time of the priority queue method is O(v_0 + e_0*loge_0 + t_0)




Issues:

I run into an assertion error that the update position cannot find the edge inside the edges map. This could be because of two things: one is that in my collapse I accidentally keep an edge that was actually removed from the priority queue class, and two is that somewhere in priority queue I update the map incorrectly to make it think that the edge is no longer there. I also get an error that a few lengths are incorrect inside the prioroty queue. I believe that these two errors are connected, and that if the priority queue could in fact find the missing edge that these length errors would dissapear.







