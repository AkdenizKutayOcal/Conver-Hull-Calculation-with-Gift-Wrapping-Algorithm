#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct {            //Point struct with x and y coordinates
    double x,y;
}Point;

enum orientation{                           //This was a little bit unnecessary and I could do the same thing with int values while checking Clockwise condition
    counterWise, clockWise,coolinear        //However I wanted to learn how to implement and use typedef in such cases.
};
typedef enum orientation orientation;

void printArray(Point points[], int size);          //Explanations are at below.
Point leftpoint(Point points[], int size);
orientation isCounter(Point A,Point B,Point C);
void wrap(Point points[],Point *wrapped,int size,Point left);
int findIndex(Point points[], int size,Point p);


int main()
{
    FILE * inp;

    puts("Enter name of the file: ");       //gets the file name from user
    char str[50];
    gets(str);
    inp = fopen(str, "r");                  //Opens file to read

    if(!inp){
        printf("Could not read the file %s",str);   //Checks to read if not error message occurs
        exit(0);
    }
    double temparr[100];
    int status = 0;
    int SIZE = 0;
    int elements = 0;

    while(status!=-1){                                              //While reading the file for the first time I find the number of elements in the file

        status = fscanf(inp,"%lf",&temparr[elements]);              //Since Points just have x and y coordinates, when we divide that value to 2 we will find the number of points
        elements++;
    }
    SIZE = elements/2;
    fclose(inp);                                                    //Then I reopen the file. This time to read the values and create Points

    inp = fopen(str, "r");

    Point left;                                                 //Left Most point
    double matrix[SIZE][2];                                     //Double dimensional array where I keep Point coordinates while reading them from the file
    Point points[SIZE];                                         //Point array where we store points
    Point *wrapped = (Point*)malloc(SIZE*sizeof(Point));       //Is used to store Points that will be returned


    for(int r=0; r<SIZE;r++){

        for(int c=0 ;c<2;c++ ){
            fscanf(inp, "%lf", &matrix[r][c]);      //Scanning Point coordinates from the file
        }
    }
    fclose(inp);

    for(int r=0; r<SIZE;r++){                       //The loop that creates Points

        Point point ={0,0};
        point.x=matrix[r][0];                       //It sets Point's x and y values from the double dim array
        point.y = matrix[r][1];
        points[r]= point;                           //And adds them to the Point array
    }

    left = leftpoint(points,SIZE);
    puts("Convex hull is being computed....");
    puts("Polygon points are:");
    wrap(points,wrapped,SIZE,left);

    return 0;
}
////////////////////////////////////////////////
//This function basically prints the Point array
////////////////////////////////////////////////
void printArray(Point points[], int size){

    for(int i=0;i<size;i++){

        printf("%0.2f %0.2f\n",points[i].x,points[i].y);

    }
}
/////////////////////////////////////////////////////////////////
//This function finds the left most point of the given point list
////////////////////////////////////////////////////////////////
Point leftpoint(Point points[],int size){

    Point left = {points[0].x,points[0].y};
    double min = points[0].x;                       //It searches for the smallest x value and sets it to min

    for(int i=1;i<size;i++){

        if(points[i].x<min){
           min = points[i].x;
           left.x = min;
           left.y = points[i].y;                    //At the end it returns left Point which has the smallest x value
        }
        else{
            continue;
        }
    }
    return left;
}

////////////////////////////////////////////////////////////
//isCounter Function returns whether given 3 points
//are CounterClock wise,Clock wise or coolinear.
///////////////////////////////////////////////////////////
orientation isCounter(Point A,Point B,Point C){

    orientation result;

    double a = ((B.y-A.y)*(C.x-B.x))- ((B.x-A.x)*(C.y-B.y));    //Calculates the orientation

    if(a<0){
        result = counterWise;
    }                                                           //And assigns the value according to that value
    else if((int)a==0){
        result = coolinear;
    }
    else if(a>0){
        result = clockWise;
    }
    return result;
}

/////////////////////////////////////////////////////////////
//wrap Function finds ConvexHull. Takes points array,
// wrapped array that will return final points, size,
//and Left point as parameters.If there are more than 3 points
//It finds convexHull and returns the Points.
/////////////////////////////////////////////////////////////
void wrap(Point points[],Point *wrapped,int size,Point left){

    if(size>2){

        int index = findIndex(points,size,left);        //Finds the index of Leftmost point
        int p = index,q;                                //p is the first and q is the second point
        int k = 0;                                      //k is the int value which will be increased while we are adding new points
        do{

            wrapped[k] = points[p] ;                    //First it adds left most to the array, that in every loop it assigns new p point

            q = (p+1)%size;                             //Assigning q value while it is not more than size

            for(int i=0;i<size-1;i++){

                if(isCounter(points[p],points[i],points[q])==counterWise){      //This loop finds a point which is clockwise to all other points

                    q = i;                                                      //And that assigns that point to q
                }
            }

           k++;
           p = q;                                         //At the end of every loop we set p as q so that we can add q to the list and continue finding other points
        }while(p != index);                               //This loop continues until it reaches start point

        printArray(wrapped,k);                            //At the end, it prints the array
        puts("");
    }
    else{
        printf("There must be at least 3 points");
        exit(0);
    }

}

///////////////////////////////////////////////////////////////////
//This function finds the index of given point in given point array
//Where I used it at wrap function
///////////////////////////////////////////////////////////////////
int findIndex(Point points[], int size,Point p){

    int index = 0;
    for(int i=0;i<size;i++){

        if(p.x == points[i].x && p.y==points[i].y){
            index = i;
            break;
        }
        else{
            continue;
        }
    }
    return index;
}

