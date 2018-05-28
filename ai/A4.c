#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define N 4
typedef struct state{
	int matrix[4][4];
	struct state* parent;
	int cost;
	int level;

	int x;
	int y;

}STATE;

typedef struct node{
	STATE* state;
	struct node *next;
}node;

int count_node=0;
int count_closed=0;
void printmatrix(int a[][4])
{
    int i,j;
    for(i=0;i<4;i++)
    {
            for(j=0;j<4;j++)
        {
            printf("%d ",a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void printpath(STATE* head)
{
    if(head==NULL)
    {
        return ;
    }
        else{
            printpath(head->parent);
            printmatrix(head->matrix);
        }


}
int  matrix_check(int A[][4],int B[][4])
{
int i,j,f=0;
		for(i=0;i<4;i++){
			for(j=0;j<4;j++){
				if(A[i][j]!=B[i][j]){
				return 1;}
	}}
			return 0;

}
int check_mat(node *temp,STATE *S)
{
    node* open;
    open=temp;
	while(open!=NULL){
		int i,j,f=0;
		for(i=0;i<4;i++){
			for(j=0;j<4;j++){
				if(S->matrix[i][j]!=open->state->matrix[i][j]){
					f=1;
					break;}}
			if(f==1)
				break;
			}
		if(f==0 ){
			if(S->cost+S->level < open->state->cost+open->state->level)
				open->state=S;

			return 0; }
		open=open->next;
	}
	return 1;


}
//Priority queue functions
			node* push(STATE *S,node *open,node* te)
			{
				node *temp, *t;
				temp = (node *)malloc(sizeof(node));
				temp->state=S;
				temp->next=NULL;
				printf("Pushing..\n");



				if(check_mat(te,S)){
				if(open==NULL){

                    open = temp;
					//printmatrix(S->matrix);

					count_node++; //-------------------------------------------------------------------------------------------------count_node

				}
				else if((open->state)->cost+open->state->level > S->cost+S->level)
				{
					temp->next=open;
					open=temp;
					//printmatrix(S->matrix);

					count_node++;	 //-------------------------------------------------------------------------------------------------count_node
				}

				else
				{
					int f=0;
				  //  printf("condition 2nd in push \n");
					t=open;
					while(t->next!=NULL && (t->next->state->cost+t->next->state->level > S->cost+S->level) )
						t=t->next;


						if(t->next==NULL){
							t->next=temp;
						}
						else{
						temp->next=t->next;
						t->next=temp;
						}
				//	printmatrix(S->matrix);
					//printf("Cost:------------%d\n\n",S->cost);
					//printf("In rest nodes...\n");
						count_node++;     //-------------------------------------------------------------------------------------------------count_node


				}}

				return open;
			}


			STATE* del(node *open) //remove elements
			{
				STATE* t;//open pointer indicates the least cost node present in open
				if(open!=NULL)
				{
				t=open->state;
				return t;
				}
				else
				return NULL;
				//IN case of empty queue send an empty node
			}

int compute_cost(int mat[][4],int final[][4]){
	int count=0;
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(mat[i][j]&& mat[i][j]!=final[i][j])
				count++;
	return count;
}
void copy_matrix(int A[][4],int B[][4],int x,int y,int x1,int y1,int n)
{
	int temp,i,j;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			A[i][j]=B[i][j];
	temp=A[x][y];
	A[x][y]=A[x1][y1];
	A[x1][y1]=temp;
}
STATE* createState(int mat[][4],STATE* parent,int level,int x,int y,int x1,int y1,int final[][4])
{
	STATE *temp=(STATE*)malloc(sizeof(STATE));
	copy_matrix(temp->matrix,mat,x,y,x1,y1,4);



	temp->parent=parent;
	temp->cost=compute_cost(mat,final);
	temp->level=level;
	temp->x=x1;
	temp->y=y1;
/*	if(temp->parent->matrix!=NULL)
	{printf("----------------------------\n\n");
	printf("%d %d %d %d \n",x,y,x1,y1);
	printmatrix(temp->matrix);
	printmatrix(temp->parent->matrix);
	printf("----------------------------\n\n");}
*/
	return temp;
}
int isSafe(int x, int y)
{
    return (x >= 0 && x < 4 && y >= 0 && y < 4);
}
int check(STATE* closed,int a,int b)
{
     if(closed->parent==NULL)
        return 1;
	else if((closed->parent->x==a) && (closed->parent->y==b))
    {
       //  printf("state1\n");
		return 0;
    }
    else{
            //printf("%d closed",)
     //  printf("state2\n");
	return 1;
    }
}

void solve(int init[][4],int final[][4])
{
	int pi=3,pj=3;
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(init[i][j]==0){
			pi=i;
			pj=j;
		break;}


	STATE* pq=createState(init,NULL,0,pi,pj,pi,pj,final);
	node* open=(node*)malloc(sizeof(node));
	open=NULL;
	node *t;
	t=open;
	open=push(pq,open,t);

	int row[] = { 1, 0, -1, 0 };
	int col[] = { 0, -1, 0, 1 };

	while(open!=NULL ){

		STATE* closed = del(open);
		count_closed++;
		open=open->next;

		// if closed is an answer node
		if (closed->cost == 0)
		{
		    printpath(closed);
		    return;
		}


		// do for each child of closed
		// max 4 children for a node
		for (i = 0; i < 4; i++)
		{
		    //printf("%d \n",closed->x);
		   if (isSafe(closed->x + row[i], closed->y + col[i]) && check(closed, closed->x + row[i],closed->y + col[i]))
		    {
	   // create a child node and calculate
		        // its cost
		        STATE* child = createState(closed->matrix,closed, closed->level + 1, closed->x,closed->y, closed->x + row[i],closed->y + col[i],final);
		        child->cost = compute_cost(child->matrix, final);

		        // Add child to list of live nodes
		        open=push(child,open,t);

		    }
		}

	}
}



int count_inverse_pairs(int arr[])
{
    int i,j,inv_count = 0;
    for (i = 0; i < N * N - 1; i++)
    {
        for (j = i + 1; j < N * N; j++)
        {

            if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
        }
    }
    return inv_count;
}

int findXPosition(int input_matrix[N][N])
{
    int i,j;
    for (i = N - 1; i >= 0; i--)
        for (j = N - 1; j >= 0; j--)
            if (input_matrix[i][j] == 0)
                return N - i;
}

bool check_sol_mat(int input_matrix[N][N])
{
    int invCount = count_inverse_pairs((int*)input_matrix);


    if (N & 1)
        return !(invCount & 1);

    else
    {
        int pos = findXPosition(input_matrix);
        if (pos & 1)
            return !(invCount & 1);
        else
            return invCount & 1;
    }
}
int main()
{
int final[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
//int initial[4][4]={{0,1,2,3},{5,6,7,4},{9,10,11,8},{13,14,15,12}};
int initial[4][4]={{5,1,7,3},{9,2,11,4},{13,6,15,8},{0,10,14,12}};
//int initial[4][4]={{3,9,1,15},{14,11,4,6},{13,0,10,12},{2,7,8,5}};//Not solvable
//int initial[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{0,13,14,15}};;
//int initial[4][4]={{1,2,3,4},{5,6,7,0},{9,10,11,12},{8,13,14,15}};//Infinite time

printf("The path is:\n");
   if(check_sol_mat(initial)){

	solve(initial,final);
	printf("Total nodes:%d Closed_Node:%d\n",count_node,count_closed);
    }
    else{
        printf("Not solvable");
    }
return 0;
}
