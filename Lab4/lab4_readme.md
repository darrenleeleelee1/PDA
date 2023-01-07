# Lab4 - Channel Router
Implement a 2-layer detailed router to complete channel routing.

Use Greedy Channel Router[1] to solve it

Input and output formats please refer to [Lab4.pdf](./Lab4.pdf).

## Demonstration
* case1
    ![](./drawing/case1.png)
* case2
    ![](./drawing/case2.png)
* case3(Deutsch
difficult case)
    ![](./drawing/case3.png)
Download [case1.gds](./drawing/case1.gds), [case2.gds](./drawing/case2.gds) and [case3.gds](./drawing/case3.gds) to see the details.

## Makefile command
```
make -j4
```

## Execute command
```
./Lab4 <input> <output> [drawing output]
```
* example
```
./Lab4 ./case/case1.txt ./out/case1.txt ./drawing/case1
```
## Reference 
* \[1\][Ho T.-T., S. S. Iyengar, and S.-Q. Zheng, A General  Greedy Channel Routing Algorithm, IEEE Trans. on CAD of Integrated Circuits and Systems, vol. 10, pp. 204-211, 1991.](https://ieeexplore.ieee.org/document/1585533/citations?tabFilter=papers#citations)