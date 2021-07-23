
# Code Structure
- Overlapping Seeding Structure :- seeding code can be found in (OverlappingSeeding.h) which has a method NeighborMin_seed we assign vertices to communities. We will try multiple seeding strategies to assign initial communities.
- GenPermanence.cpp has the method runOverlappingPermanence which is like the main method for this code, we set up datastructures and then it first calls seeding which is defined above. After seeding we call optimize _Permanence method ( description about that method is defined below)
- Optimize_Permanence is the method  which can be found in GenPermanenceCalculation.h
  - GenPermanenceCaluclation.h has all the methods related to overlapping permanence calculation
  - In  Optimize_Permanence method we defined max iteration and we have a overall while loop which ensures that we keep track of iteration and it is less than max ietration, it also checks for overall permanence score from previous iteration and current iteration. The while loop terminates if it reaches max iteration or if overall permanence score is same as the previous iteration overall permanence.
  - compute_permanence_overlap method is being called in optimize_permanence which does the heavy lifting which computes  overlapping permanence score for each vertex.
  - The for lopp which is done in parallel calls the compute_permanence_overlap method.

- Compute_permanence_overlap Method 
  - This method is for each vertex, where for each of its neighbor look for the communities it belongs. The overall idea in this step is for a given vertex get its neighbors communities. We are figuring out which communities we should evaluate where vertex can move to get higher permanence score.
  - The next step is after identifying possible communities where vertex can go, next is the computation step of calculating the permanence score.
  - The formula for Genperm has many components so that all is done in this method, please refer to Gen perm formula in the original paper or HIPC draft
  - For each community we do need to calculate local clustering coefficient which calculates compute_CC_Numerator, where you check a vertex 2-hop neighbors. We made few optimizations here while calculating the local CC for vertex, this can be further improved as this is the most expensive stage in computation. Check compute_CC_Numerator
 
