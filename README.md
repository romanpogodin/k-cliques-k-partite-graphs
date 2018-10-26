# k-cliques-k-partite-graphs 
## Description
The algorithm finds all cliques of size k in a k-partite binary undirected graph.

This is an implementation of
[this paper](http://www.sciencedirect.com/science/article/pii/S0305054800000538) [@Grunert2002].
## Installation
```commandline
cmake CMakeLists.txt
make
```
## Usage
Run as
```
./k_cliques_k_partite_graphs --graph_filename path-to-file --max_cliques integer
```
The parameter ```--max_cliques d``` regulates early stopping of the algorithm.
When ```d``` is non-negative, the algorithm automatically stops when ```d``` cliques have been found.
Negative values mean no upper bound (default).

If you want to save (and print) all the cliques, add ```--save_solutions```.
#### Input
The graph file is a simple .txt (specified by ```--graph_filename```) of the following form:
```
n
p1 p2 ... pn
w11 w12 ... w1n 
...............
wn1 wn2 ... wnn
```
Here 

1. ```n``` is the total number of vertices
2. ```pi``` is the partition of ```i```-th vertex (a number from ```1``` to ```k```)
3. ```wij``` is the binary weight between vertices ```i``` and ```j```.

Note that only the weights for ```i>j``` will be used.
A connection between two vertices from the same partition will results in an error.

#### Output
Prints the number of k-cliques. If ```--save_solutions``` is specified,
the following lines contain each clique in the format
```
x1 x2 ... xk
```
with ```xi``` being the global index of the vertex in partition ```i```.

## Note on other algorithms
I've found only one more algorithm that solves exactly this problem.
It is presented in [this paper](https://link.springer.com/article/10.1007/s11590-012-0536-y) 
[@Mirghorbani2013]

## References
[@Grunert2002]:
"Finding all k-cliques in k-partite graphs, an application in textile engineering",
Tore Grünert, Stefan Irnich, Hans-Jürgen Zimmermann, Markus Schneider, Burkhard Wulfhorst,
Computers & Operations Research (2002), 29: 13.
https://doi.org/10.1016/S0305-0548(00)00053-8

[@Mirghorbani2013]:
"On finding k-cliques in k-partite graphs"
Mirghorbani, M. & Krokhmal, P. Optim Lett (2013) 7: 1155.
https://doi.org/10.1007/s11590-012-0536-y
