//Group: 10
//Names: Dustin Hengel, Madison Williams, Bradley Worthen
//Class: CS3050
//Assignment: Final

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum states{Empty=0,Wall,Robot1,Robot2};//Used for state of node

typedef struct Node{
    int* adjacent;//Adjacent to Node
    int size; 
    int capacity;//Vector Parameters
    enum states state;//State of Node
    int visited;//1 for Robot1, 2 for Robot2, 3 for Both
}Node;


typedef struct Graph{
    Node* nodes;//Holds Nodes in Graph
    int nodeSize;
    int nodeCapacity;//Vector Parameters
    int* lineCharacters;//Characters per Line in Input
    int lineSize;
    int lineCapacity;//Vector Parameters
    int robot1;//Location of Robot1
    int robot2;//Location of Robot2
    int endpoint1;//Endpoint for Robot1
    int endpoint2;//Endpoint for Robot2
}Graph;

typedef struct StackNode{
    int value1;
    int value2;
    struct StackNode* next;
}StackNode;

Graph buildGraph(FILE*);
Graph createNode(Graph, const char,const int);
Graph addEdges(Graph,const int,const int);
void printAdjacencyList(const Graph);
void printRoom(Graph);
void freeMemory(Graph);
int depthFirstSearch(Graph,const int);
void push(StackNode**,const int,const int);
StackNode pop(StackNode**);
int stackEmpty(StackNode** stack);
void printPaths(Graph graph);
void freeStack(StackNode** stack);
int isAdjacent(Graph graph, int robot1, int robot2);
void printPaths(Graph graph);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Arguments should be <executable> <input file>\n");
        exit(0);
    }//Checks for # Arguments

    FILE* file = fopen(argv[1], "r");//Opens 2nd Argument as File
    if(file==NULL)
    {
        printf("File failed to Open\nArguments should be <executable> <input file>");
        exit(0);
    }//If File Failed to Open
    
    Graph graph = buildGraph(file);
    
    if(fclose(file)!=0){
        printf("\nFile failed to close\n");
        exit(0);
    }//If File Failed to Close
    
    int moveCount=0;
    moveCount=depthFirstSearch(graph,0);
    //printAdjacencyList(graph);
    //printRoom(graph);
    //printPaths(graph);
    freeMemory(graph);
    
    
    printf("Would you like to see full animation? \nEstimated Time: %.3f Minutes\nEnter 1 for \"Yes\" or 0 for \"No\": ",(float)moveCount/60/60);
    int input=1;
    do
    {
        if(input!=1&&input!=0)
        {
            puts("Invalid Input, Please Enter Again");
        }//Checks Invalid Input
        scanf("%d",&input);
    }//Do While Incorrect Input
    while(input!=1&&input!=0);
    
    if(input==1)
    {
        file = fopen(argv[1], "r");//Opens 2nd Argument as File
        if(file==NULL)
        {
            printf("File failed to Open\nArguments should be <executable> <input file>");
            exit(0);
        }//If File Failed to Open

        graph = buildGraph(file);

        if(fclose(file)!=0){
            printf("\nFile failed to close\n");
            exit(0);
        }//If File Failed to Close


        moveCount=depthFirstSearch(graph,1);
        //printAdjacencyList(graph);
        //printRoom(graph);
        //printPaths(graph);
        freeMemory(graph);
    }//If Yes, Redo
    return 0;
}


Graph buildGraph(FILE* file)
{
    Graph graph;
    graph.nodes=NULL;
    graph.nodeSize=0;
    graph.nodeCapacity=0;
    graph.lineCharacters=NULL;
    graph.lineSize=0;
    graph.lineCapacity=0;
    graph.robot1=-1;
    graph.robot2=-1;
    graph.endpoint1=-1;
    graph.endpoint2=-1;//Initial values for Graph

 
    int charCount=0;//count Characters in Line before Newline
    char inputChar;//Read from File
    
    for(inputChar = fgetc(file); inputChar!=EOF; inputChar=fgetc(file))
    {
        if(inputChar=='\n')
        {
            if(graph.lineCapacity==0&&graph.lineCharacters==NULL)
            {
                graph.lineCapacity=1;
                graph.lineCharacters=malloc(sizeof(int)*graph.lineCapacity);
                if(graph.lineCharacters==NULL)
                {
                    printf("\nMalloc Error\n");
                    freeMemory(graph);
                    exit(0);
                }//Check for Failed Malloc
            }//If First Line
            
            if(graph.lineCapacity==graph.lineSize)
            {
                graph.lineCapacity=graph.lineCapacity*2;
                graph.lineCharacters=realloc(graph.lineCharacters,sizeof(int)*graph.lineCapacity);
                if(graph.lineCharacters==NULL)
                {
                    printf("\nRealloc Error\n");
                    freeMemory(graph);
                    exit(0);
                }//Check for Failed Realloc
            }//If Need to Expand Lines
            
            graph.lineCharacters[graph.lineSize]=charCount;//Adds Characters in Line to Corresponding Index
            graph.lineSize++;
            charCount=0;
        }//If Newline
        else
        {            
            charCount++;
            if(graph.nodeCapacity==0&&graph.nodes==NULL)
            {
                graph.nodeCapacity=2;
                graph.nodes=malloc(sizeof(Node)*graph.nodeCapacity);
                if(graph.nodes==NULL)
                {
                    printf("\nMalloc Error\n");
                    freeMemory(graph);
                    
                    exit(0);
                }//Check for Failed Malloc
            }//If First Node
            
            if(graph.nodeCapacity==graph.nodeSize+1)
            {
                graph.nodeCapacity=graph.nodeCapacity*2;
                graph.nodes=realloc(graph.nodes,sizeof(Node)*graph.nodeCapacity);  
                if(graph.nodes==NULL)
                {
                    printf("\nRealloc Error\n");
                    freeMemory(graph);
                    exit(0);
                }//Check for Failed Realloc
            }//If Need to Expand Nodes
   

            graph=createNode(graph,inputChar,charCount);//Adds Node to Graph
            if(graph.nodeSize==-1)
            {
                if(fclose(file)!=0){
                printf("\nFile failed to close\n");
                exit(0);
                }//If File Failed to Close
                exit(0);
            }

        }//Else Character
    }//For Characters in Input
    
    if(graph.robot1==-1||graph.robot2==-1||graph.endpoint1==-1||graph.endpoint2==-1)
    {
        puts("Missing At least 1 Robot or Endpoint");
        freeMemory(graph);
        if(fclose(file)!=0){
            printf("\nFile failed to close\n");
            exit(0);
        }//If File Failed to Close
        exit(0);
    }//If Graph Missing Robots or Endpoint
    
    return graph;//Returns graph
}


Graph createNode(Graph graph, const char inputChar,const int charCount)
{
    graph.nodeSize++;//Increments Number of Nodes
    
    graph.nodes[graph.nodeSize].adjacent=NULL;
    graph.nodes[graph.nodeSize].size=0;
    graph.nodes[graph.nodeSize].capacity=0;
    graph.nodes[graph.nodeSize].visited=0;//Sets Default Values
    
    if(inputChar==' ')
    {
        graph.nodes[graph.nodeSize].state=Empty;
    }//If Empty
    else if(inputChar=='#')
    {
        graph.nodes[graph.nodeSize].state=Wall;
    }//If Wall
    else if(inputChar=='S')
    {
        graph.nodes[graph.nodeSize].state=Robot1;
        graph.robot1=graph.nodeSize;//Sets Location of First Robot
    }//If Robot1
    else if(inputChar=='F')
    {
        graph.nodes[graph.nodeSize].state=Robot2;
        graph.robot2=graph.nodeSize;//Sets Location of Second Robot
    }//If Robot2
    else if(inputChar=='E')
    {
        graph.nodes[graph.nodeSize].state=Empty;
        graph.endpoint1=graph.nodeSize;//Sets Location of First Endpoint
    }//If Endpoint1
    else if(inputChar=='L')
    {
        graph.nodes[graph.nodeSize].state=Empty;
        graph.endpoint2=graph.nodeSize;//Sets Location of Second Endpoint
    }//If Endpoint2
    else
    {
        puts("Invalid Character");
        freeMemory(graph);
        graph.nodeSize=-1;
        return graph;
    }
    //Sets State and Location of Robots/Endpoints from Input Character
    
    if(graph.nodes[graph.nodeSize].state!=Wall)
    {
        if(graph.lineSize==0&&graph.nodeSize==1)
        {
            //Do Nothing
        }//If First Node
        else if(graph.lineSize==0)
        {
            if(graph.nodes[graph.nodeSize-1].state!=Wall)
            {
                graph=addEdges(graph,graph.nodeSize,graph.nodeSize-1);
            }//If Node to Left is Empty
        }//If First Row
        else if(charCount==1)
        {
            if(graph.nodes[graph.nodeSize-graph.lineCharacters[graph.lineSize-1]].state!=Wall)
            {
                graph=addEdges(graph,graph.nodeSize,graph.nodeSize-graph.lineCharacters[graph.lineSize-1]);
            }//If Node Above is Empty

            if(graph.nodes[graph.nodeSize-graph.lineCharacters[graph.lineSize-1]+1].state!=Wall)
            {
                graph=addEdges(graph,graph.nodeSize,graph.nodeSize-graph.lineCharacters[graph.lineSize-1]+1);
            }//If Node Above and to the Right is Empty
        }//If First Column
        else
        {
            if(graph.nodes[graph.nodeSize-1].state!=Wall)
            {
                graph=addEdges(graph,graph.nodeSize,graph.nodeSize-1);
            }//If Node to Left is Empty

            if(graph.nodes[graph.nodeSize-graph.lineCharacters[graph.lineSize-1]].state!=Wall)
            {
                graph=addEdges(graph,graph.nodeSize,graph.nodeSize-graph.lineCharacters[graph.lineSize-1]);
            }//If Node Above is Empty

            if(graph.nodes[graph.nodeSize-graph.lineCharacters[graph.lineSize-1]+1].state!=Wall)
            {
                graph=addEdges(graph,graph.nodeSize,graph.nodeSize-graph.lineCharacters[graph.lineSize-1]+1);
            }//If Node Above and to the Right is Empty

            if(graph.nodes[graph.nodeSize-graph.lineCharacters[graph.lineSize-1]-1].state!=Wall)
            {
                graph=addEdges(graph,graph.nodeSize,graph.nodeSize-graph.lineCharacters[graph.lineSize-1]-1);
            }//If Node Above and to the Left is Empty
        }//Else Non-Edge Case
    }//Adds Edges if Not Wall
    return graph;//Returns graph
}

Graph addEdges(Graph graph,const int vertex1,const int vertex2)
{   
    if(graph.nodes[vertex1].capacity==0&&graph.nodes[vertex1].adjacent==NULL)
    {
        graph.nodes[vertex1].capacity=1;
        graph.nodes[vertex1].adjacent=malloc(sizeof(int)*graph.nodes[vertex1].capacity);
        if(graph.nodes[vertex1].adjacent==NULL)
            {
                printf("\nMalloc Error\n");
                freeMemory(graph);
                exit(0);
            }//Check for Failed Malloc
    }//If First Adjacent
    
    if(graph.nodes[vertex1].capacity==graph.nodes[vertex1].size)
    {
        graph.nodes[vertex1].capacity=graph.nodes[vertex1].capacity*2;
        graph.nodes[vertex1].adjacent=realloc(graph.nodes[vertex1].adjacent,sizeof(int)*graph.nodes[vertex1].capacity);
        if(graph.nodes[vertex1].adjacent==NULL)
            {
                printf("\nRealloc Error\n");
                freeMemory(graph);
                exit(0);
            }//Check for Failed Realloc
    }//If Needs to Expand Adjacency List
    
    graph.nodes[vertex1].adjacent[graph.nodes[vertex1].size]=vertex2;
    graph.nodes[vertex1].size++;
    //Adds Edge from Vertex1 to Vertex2
    
    
    if(graph.nodes[vertex2].capacity==0&&graph.nodes[vertex2].adjacent==NULL)
    {
        graph.nodes[vertex2].capacity=1;
        graph.nodes[vertex2].adjacent=malloc(sizeof(int)*graph.nodes[vertex2].capacity);
        if(graph.nodes[vertex2].adjacent==NULL)
        {
            printf("\nMalloc Error\n");
            freeMemory(graph);
            exit(0);
        }//Check for Failed Malloc
    }//If First Adjacent
    
    if(graph.nodes[vertex2].capacity==graph.nodes[vertex2].size)
    {
        graph.nodes[vertex2].capacity=graph.nodes[vertex2].capacity*2;
        graph.nodes[vertex2].adjacent=realloc(graph.nodes[vertex2].adjacent,sizeof(int)*graph.nodes[vertex2].capacity);
        if(graph.nodes[vertex2].adjacent==NULL)
        {
            printf("\nRealloc Error\n");
            freeMemory(graph);
            exit(0);
        }//Check for Failed Realloc
    }//If Needs to Expand Adjacency List
    

    graph.nodes[vertex2].adjacent[graph.nodes[vertex2].size]=vertex1;
    graph.nodes[vertex2].size++;
    //Adds Edge from Vertex2 to Vertex1
    return graph;//Returns graph
}

void printAdjacencyList(const Graph graph)
{

    int nodeIterations,adjIterations;
    printf("State: # Adjacent Vertices: Node #: Adjacent Vertices:\n");
    for(nodeIterations=1;nodeIterations<=graph.nodeSize;nodeIterations++)
    {
        printf("%d %d %d: ",graph.nodes[nodeIterations].state,graph.nodes[nodeIterations].size,nodeIterations);
        for(adjIterations=0;adjIterations<graph.nodes[nodeIterations].size;adjIterations++)
        {
            printf("%d -",graph.nodes[nodeIterations].adjacent[adjIterations]);
        }//For Adjacent to Node
        puts("");
    }//For Nodes
}

void printRoom(Graph graph)
{

    system("clear");//Clears Screen
    int nodeCount=0;
    int i=0, j=0;
    for(i=0;i<graph.lineSize; i++ )
    {
        for(j=1; j<graph.lineCharacters[i]+1; j++)
        {
            if(graph.nodes[j+nodeCount].state==Robot1)//robot1
            {
                printf("%c", 'S');
            }

            else if(graph.nodes[j+nodeCount].state==Robot2)//robot2
            {
                printf("%c", 'F');
            }
            else if(j+nodeCount==graph.endpoint1)
            {
                printf("%c", 'E');//Endpoint 1
            }
            else if(j+nodeCount==graph.endpoint2)
            {
                printf("%c", 'L');//Endpoint2
            }
            else if(graph.nodes[j+nodeCount].state==Empty)//space
            {
                printf("%c", ' ');
            }
            else if(graph.nodes[j+nodeCount].state==Wall)//wall
            {
                printf("%c", '#');
            }
            

        }//Nested For Loops print Stuff
        printf("\n");
        nodeCount+=graph.lineCharacters[i];

    }
}

void freeMemory(Graph graph)
{
    int nodeIterations;
    for(nodeIterations=1;nodeIterations<=graph.nodeSize;nodeIterations++)
    {
        free(graph.nodes[nodeIterations].adjacent);//Free each Node's AdjList
    } 
    free(graph.lineCharacters);//Free Line Characters Array
    free(graph.nodes);//Free Nodes
}

int depthFirstSearch(Graph graph,const int display)
{
    if(isAdjacent(graph,graph.robot1,graph.robot2)==1)
    {   
        puts("Robots Initially Adjacent Error");
        freeMemory(graph);
        exit(0);
        return 0;
    }//Tests if Robts Initially Adjacent
    

    
    int** visitedArray;
    visitedArray=malloc(sizeof(int*)*(graph.nodeSize+1));
    if(visitedArray==NULL)
    {
        printf("\nMalloc Error\n");
        freeMemory(graph);
        exit(0);
    }//Check for Failed Malloc
    int outsideIterations,insideIterations,iterations;
    
    for(outsideIterations=1;outsideIterations<=graph.nodeSize;outsideIterations++)
    {
        visitedArray[outsideIterations]=malloc(sizeof(int)*(graph.nodeSize+1));
        if(visitedArray[outsideIterations]==NULL)
        {
            printf("\nMalloc Error\n");
            freeMemory(graph);
            exit(0);
        }//Check for Failed Malloc
    }//Array for Combinations of Nodes and Visited or Not
    
    for(outsideIterations=1;outsideIterations<=graph.nodeSize;outsideIterations++)
    {
        for(insideIterations=1;insideIterations<=graph.nodeSize;insideIterations++)
        {
            visitedArray[insideIterations][outsideIterations]=0;
        }
    }//Initializes 2D Array to 0
    
    
    
    
    
    
    
    StackNode** stack=malloc(sizeof(StackNode*));
    *stack=NULL;
    
    push(stack,graph.robot1,graph.robot2);//Pushes Initial Node
    if(*stack==NULL)
    {
        freeMemory(graph);
        exit(0);
    }//If Push Failed
    
    
    StackNode poppedNode,tempNode;
    tempNode.value1=graph.robot1;
    tempNode.value2=graph.robot2;
    
    //printRoom(graph);
    
    int count=0;
    while(stackEmpty(stack)==0)
    {
        graph.nodes[tempNode.value1].state=Empty;
        graph.nodes[tempNode.value2].state=Empty;
        poppedNode=pop(stack);
        //printf("Popping %d,%d\n",poppedNode.value1,poppedNode.value2);
        graph.nodes[poppedNode.value1].state=Robot1;
        graph.nodes[poppedNode.value2].state=Robot2;
        tempNode=poppedNode;//Updates Robot Locations
        
        if(graph.nodes[poppedNode.value1].visited==0)
        {
            graph.nodes[poppedNode.value1].visited=1;
        }
        if(graph.nodes[poppedNode.value1].visited==2)
        {
            graph.nodes[poppedNode.value1].visited=3;
        }
        if(graph.nodes[poppedNode.value2].visited==0)
        {
            graph.nodes[poppedNode.value2].visited=2;
        }
        if(graph.nodes[poppedNode.value2].visited==1)
        {
            graph.nodes[poppedNode.value2].visited=3;
        }//Updates Visited Status by Node
        
        
        if(poppedNode.value1==graph.endpoint1&&poppedNode.value2==graph.endpoint2)
        {
            printPaths(graph);
            puts("Robots Successfully Reached Endpoints");
            for(iterations=1;iterations<=graph.nodeSize;iterations++)
            {
                free(visitedArray[iterations]);
            }
            free(visitedArray);//Frees 2D Array
            freeStack(stack);//Frees Stack
            return count;//Returns Moves Count
        }//If Successful

        
        if(visitedArray[poppedNode.value1][poppedNode.value2]==0)
        {
            visitedArray[poppedNode.value1][poppedNode.value2]=1;
        }//If not visited, make visited
        
        if(poppedNode.value2!=graph.endpoint2)
        {
            for(iterations=0;iterations<graph.nodes[poppedNode.value1].size;iterations++)
            {
                if(visitedArray[graph.nodes[poppedNode.value1].adjacent[iterations]][poppedNode.value2]==0)
                {
                    if(isAdjacent(graph,graph.nodes[poppedNode.value1].adjacent[iterations],poppedNode.value2)==0)
                    {
                        push(stack,graph.nodes[poppedNode.value1].adjacent[iterations],poppedNode.value2);
                        //printf("Pushing %d,%d\n",graph.nodes[poppedNode.value1].adjacent[iterations],poppedNode.value2);
                        if(*stack==NULL)
                        {
                            freeMemory(graph);
                            exit(0);
                        }//If Push Failed
                    }//If Not Adjacent to Eachother
                }//If Not Visited   
            }//For Adjacent to Robot1

            for(iterations=0;iterations<graph.nodes[poppedNode.value2].size;iterations++)
            {
                if(visitedArray[poppedNode.value1][graph.nodes[poppedNode.value2].adjacent[iterations]]==0)
                {
                    if(isAdjacent(graph,poppedNode.value1,graph.nodes[poppedNode.value2].adjacent[iterations])==0)
                    {
                        push(stack,poppedNode.value1,graph.nodes[poppedNode.value2].adjacent[iterations]);
                        //printf("Pushing %d,%d\n",poppedNode.value1,graph.nodes[poppedNode.value2].adjacent[iterations]);
                        if(*stack==NULL)
                        {
                            freeMemory(graph);
                            exit(0);
                        }//If Push Failed
                    }//If Not Adjacent to Eachother
                }//If Not Visited   
            }//For Adjacent to Robot2
        }//If Robot2 Not Finished
        else
        {
            for(iterations=0;iterations<graph.nodes[poppedNode.value2].size;iterations++)
            {
                if(visitedArray[poppedNode.value1][graph.nodes[poppedNode.value2].adjacent[iterations]]==0)
                {
                    if(isAdjacent(graph,poppedNode.value1,graph.nodes[poppedNode.value2].adjacent[iterations])==0)
                    {
                        push(stack,poppedNode.value1,graph.nodes[poppedNode.value2].adjacent[iterations]);
                        //printf("Pushing %d,%d\n",poppedNode.value1,graph.nodes[poppedNode.value2].adjacent[iterations]);
                        if(*stack==NULL)
                        {
                            freeMemory(graph);
                            exit(0);
                        }//If Push Failed
                    }//If Not Adjacent to Eachother
                }//If Not Visited   
            }//For Adjacent to Robot2
            
            for(iterations=0;iterations<graph.nodes[poppedNode.value1].size;iterations++)
            {
                if(visitedArray[graph.nodes[poppedNode.value1].adjacent[iterations]][poppedNode.value2]==0)
                {
                    if(isAdjacent(graph,graph.nodes[poppedNode.value1].adjacent[iterations],poppedNode.value2)==0)
                    {
                        push(stack,graph.nodes[poppedNode.value1].adjacent[iterations],poppedNode.value2);
                        //printf("Pushing %d,%d\n",graph.nodes[poppedNode.value1].adjacent[iterations],poppedNode.value2);
                        if(*stack==NULL)
                        {
                            freeMemory(graph);
                            exit(0);
                        }//If Push Failed
                    }//If Not Adjacent to Eachother
                }//If Not Visited   
            }//For Adjacent to Robot1
            
        }//Robot2 Finished
        count++;
        if(display==1)
        {
            printPaths(graph);
        }//If printing paths
    }//While Stack Not Empty
    printPaths(graph);
    puts("Unable to Reach Endpoints");

    
    for(iterations=1;iterations<=graph.nodeSize;iterations++)
    {
        free(visitedArray[iterations]);
    }
    free(visitedArray);//Frees 2D Array
    free(stack);
   
    
    return count;//Returns Moves Count
}

void push(StackNode** stack,const int value1,const int value2)
{
    StackNode* Temp;//Temp Node
    if(*stack==NULL)
    {
        *stack=malloc(sizeof(StackNode));
        if(*stack==NULL)
        {
            printf("\nMalloc Error\n");
            freeStack(stack);
            exit(0);
        }//Check for Failed Malloc
        (*stack)->value1=value1;
        (*stack)->value2=value2;
        (*stack)->next=NULL;//Sets Values in Node
        return;//Returns Nothing
    }//If stack Empty
    else
    {
        Temp=malloc(sizeof(StackNode));
        if(Temp==NULL)
        {
            printf("\nMalloc Error\n");
            freeStack(stack);
            exit(0);
        }//Check for Failed Malloc
        Temp->value1=value1;
        Temp->value2=value2;
        Temp->next=*stack;//Sets Values in Node
        *stack=Temp;
        return;//Returns Nothing
    }//Else stack Not Empty
    return;//Returns Nothing if Errored
}

StackNode pop(StackNode** stack)
{
    StackNode returnNode;//Node to Return
    StackNode* tempNode;//Temp Node
    if((*stack)!=NULL)
    {
        returnNode.value1=(*stack)->value1;
        returnNode.value2=(*stack)->value2;
        tempNode=(*stack)->next;
        free(*stack);
        *stack=tempNode;
        return returnNode;
    }//If Queue Exists
    returnNode.value1=-1;
    returnNode.value2=-1;
    return returnNode;//Returns -1s if Empty
}

int stackEmpty(StackNode** stack)
{
    if(*stack==NULL)
    {
        return 1;
    }//If Empty Return 1
    else
    {
        return 0;
    }//Else Return 0
}

void freeStack(StackNode** stack ){
    StackNode* temp = *stack;
    while((*stack)->next!=NULL)
    {
        temp=*stack;
        *stack=(*stack)->next;
        free(temp);
    }
    free(*stack);
    free(stack);
    return;
}

int isAdjacent(Graph graph, int robot1, int robot2)
{
    int adjIterations=0;

    for(adjIterations=0; adjIterations<graph.nodes[robot1].size; adjIterations++)
    {
        
        if(graph.nodes[robot1].adjacent[adjIterations]==robot2)
        {
            return 1;
        }/*Robots are adjacent*/
    }
        return 0; /*Robots are not adjacent*/
}

void printPaths(Graph graph)
{
    system("clear");//Clears Screen
    int nodeCount=0;
    int i=0, j=0;
    for(i=0;i<graph.lineSize; i++ )
    {
        for(j=1; j<graph.lineCharacters[i]+1; j++)
        {
            if(graph.nodes[j+nodeCount].state==Wall)//wall
            {
                printf("%c", '#');
            }
            else if(graph.nodes[j+nodeCount].state==Robot1)//robot1
            {
                printf("%c", 'S');
            }
            else if(graph.nodes[j+nodeCount].state==Robot2)//robot2
            {
                printf("%c", 'F');
            }         
            else if(j+nodeCount==graph.endpoint1)
            {
                printf("%c", 'E');//Endpoint 1
            }
            else if(j+nodeCount==graph.endpoint2)
            {
                printf("%c", 'L');//Endpoint2
            }
            else if(graph.nodes[j+nodeCount].state==Empty)//space
            {   
                if(graph.nodes[j+nodeCount].visited==3 )
                {
                    printf("%c", '=');
                }//If this node was visited by both robots then indicate this with =
                else if(graph.nodes[j+nodeCount].visited==1)
                {

                    printf("%c", '-');
                }//If this node was visited by robot 1 only then indicate this with -
                else if(graph.nodes[j+nodeCount].visited==2)
                {

                    printf("%c", '~');
                }//If this node was visited by robot 2 only then indicate this with ~
                else
                {
                    printf("%c", ' ');
                }//If this node was not visited then indicate this with ' '
            }

        }
        printf("\n");
        nodeCount+=graph.lineCharacters[i];

    }
}