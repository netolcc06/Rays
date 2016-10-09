# Rays

This code reads a polygon with n vertices (vx, vy) / (-1 <= vx <= 1) and (-1 <= vy <= 1) and generates 64 lines that may or not cross the polygon. The parts of the lines inside the polygon are colored green and the outside parts are colored gray.

You can understand the method by reading all the comments along the code.

To compile it: g++ main.cpp -o <name of the object> -lGL -lglut -Wall

Below some screenshots of the program running properly and its pathological case:

![alt tag](https://github.com/netolcc06/Rays/blob/master/ex01.png)

![alt tag](https://github.com/netolcc06/Rays/blob/master/ex02.png)

![alt tag](https://github.com/netolcc06/Rays/blob/master/ex03.png)

![alt tag](https://github.com/netolcc06/Rays/blob/master/ex04.png)

My solution has one flaw. When a line intersects a polygon right at one of its corners without getting inside of the polygon itself, it will change the color line anyway. See it below:

![alt tag](https://github.com/netolcc06/Rays/blob/master/pat_case.png)


