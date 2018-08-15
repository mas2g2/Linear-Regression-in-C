#include <stdio.h>
#include <stdlib.h>
/*
 * This Statistics Library contains functions for allocating memory in a list for 
 * storing data in a list data structure as well as functions that can give us 
 * a comprehensive statistical analysis on at most two sets of numbers.
*/
typedef struct node{	
	double key;
	struct node *next;
}Node;

typedef struct{
	Node *head,*tail;
	int length;
}List;

//Allocates memory for list object
List *list(){
	List *p = (List*)malloc(sizeof(List));
	p->head = p->tail = NULL;
	p->length = 0;
	return p;	
}

//Allocates memory for nodes in list
Node *node(double key){
	Node *p = (Node*)malloc(sizeof(Node));
	p->key = key;
	p->next = NULL;
	return p;
}

//Adds item to the begining of the list
List *push(List* ulist,double key){
	Node *p = node(key);
	ulist->length++;
	if(!ulist->head){
		ulist->head = ulist->tail = p;
		return ulist;
	}
	else{
		p->next = ulist->head;
		ulist->head = p;
		return ulist;
	}
}

//Deletes first item in the list
void pop(List *ulist){
	if(!ulist){
		printf("--error : list is empty--\n");
		return;
	}
	else if(ulist->head == ulist->tail && ulist->head->next == NULL){
		ulist->length--;
		printf("deleting item %.2f ...\n",ulist->head->key);
		ulist->head = ulist->tail = NULL;
		ulist = NULL;
	}
	else{
		ulist->length--;
		printf("deleting item %.2f ...\n",ulist->head->key);
		Node *p = ulist->head;
		ulist->head = ulist->head->next;
		free(p);
		return;
	}
}

//Prints items stored in the list
void printList(List *list){
	Node *p = NULL;
	for(p = list->head; p != NULL; p = p->next){
		if(p->next != NULL){
			printf("%.2f ",p->key);
		}		
		else{
			printf("%.2f\n",p->key);
		}
	}
}

//Prints amount of items in the list
int len(List *list){
	return list->length;
}

//Checks if lists contain the same amount of items
int compare_length(List *ulist1, List *ulist2){
	if(ulist1->length == ulist2->length)return 1;
	else return 0;
}
void readFile(List *xlist,List *ylist){
	Node *p = xlist->head, *q = ylist->head;
	FILE *fptr = NULL;
	char filename[] = "dji.txt";
	char ch;
	double v1,v2;
	fptr = fopen(filename,"r");
	if(fptr){
		while(!feof(fptr)){
			fscanf(fptr,"%lf %lf",&v1,&v2);
			xlist = push(xlist,v1);
			ylist = push(ylist,v2);
		}	
	}
	else{
		printf("File could not be read.\n");
	}
}
//Calculates mean of list
double mean(List *list){
	Node *p = NULL;
	double total = 0;
	for(p = list->head; p != NULL; p = p->next){
		total += p->key;		
	}
	return total/(double)list->length;
}

//Sorts list in ascending order
void sort(List *ulist){
	Node *head = ulist->head, *i, *j, *next_i;
	for(i = head; i != NULL && i->next !=NULL ; i = i->next){
		Node *min;
		min = i;
		for(j = i->next; j != NULL; j = j->next){
			if(j->key < min->key)min=j;
		}
		if(min != i){
			int temp = min->key;
			min->key = i->key;
			i->key = temp;
		}
	}
	ulist->head = head;	
}

//Gets median value of the list
double median(List *list){
	sort(list);
	int index = 0;
	int medianIndex = (list->length + 1)/2 - 1;
	Node *current = list->head;
	while(current != NULL && index != medianIndex){
		current = current->next;
		index++;
	}
	if(list->length%2 != 0)return current->key;
	else{
		double median = current->key + current->next->key;
		median = median/2;
		return median;
	}
}

//Gets mode of list
double mode(List *list){
	int modeCount = 0;
	double mode =0;
	Node *p,*q;
	for(p = list->head; p != NULL; p = p->next){
		int count = 0;
		for(q = list->head; q != NULL; q = q->next){
			if(q->key == p->key)count++;
		}
		if(count > modeCount){
			modeCount = count;
			mode = p->key;
		}
	}
	return mode;
}

//Gets variance from a set of numbers
double var(List *list){
	double var = 0;
	double avg = mean(list);
	Node *current = NULL;
	for(current = list->head; current != NULL; current = current->next){
		var += current->key*current->key - 2*current->key*avg + avg*avg;
	}
	var = var/(list->length - 1);
	return var;
}

//Calculates square root of number
double sqrt(double num){
	double  x = num;
	double y = 1;
	double a = 0.00000000001;
	while((x-y) > a){
		x = (x+y)/2;
		y = num/x;
	}
	return x;
}

//Calculates standard deviation from a set of numbers
double std(List *list){
	double std = sqrt(var(list));
	return std;
}

//Calculates sum of numbers stored in the list
double sum(List *list){
	double sum = 0;
	Node *p = NULL;
	for(p = list->head; p != NULL; p = p->next){
		sum += p->key;
	}
	return sum;
}

//Calculates sum of each item in a list squared
double sqrd_sum(List* list){
	double sqrd_sum = 0;
	Node *p = NULL;
	for(p = list->head; p != NULL; p = p->next){
		sqrd_sum += p->key * p->key;
	}
	return sqrd_sum;
}

//Calculates correlation coefficient
double corr_coeff(List *ylist, List *xlist){
	if(compare_length(ylist,xlist) != 1){
	        printf("List must be of the same length.\n");
	        return 0;
        }
	else{
		double xy_sum = 0;
		Node *p = ylist->head, *q = xlist->head;
		while(p){
			xy_sum += p->key * q->key;
			p = p->next;
			q = q->next;
		}
		int length = len(ylist);
		double numerator = length*xy_sum - sum(xlist)*sum(ylist);
		double sqrd_denominator = (length*sqrd_sum(xlist) - sum(xlist)*sum(xlist))*(length*sqrd_sum(ylist)-sum(ylist)*sum(ylist));
		double denominator = sqrt(sqrd_denominator);
		double corr_coeff = numerator/denominator;
		return corr_coeff;
	}
}

//Calculates slope on a list
double slope(List *ylist, List *xlist){
	if(compare_length(ylist,xlist) != 1){
		printf("List must be of the same length.\n");
		return 0;
	}
	else{
		double y_mean = mean(ylist);
		double x_mean = mean(xlist);
		double xy_product = 0;
		double ss_xy = 0;
		double ss_xx = 0;
		Node *p = ylist->head, *q = xlist->head;
		while(p){
			xy_product = p->key * q->key;
			ss_xy += xy_product - len(xlist)*y_mean*x_mean;
			ss_xx += q->key*q->key - len(xlist)*x_mean*x_mean;
			p = p->next;
			q = q->next;
		}
		//double ss_xy = xy_sum - len(xlist)*x_mean*y_mean;
		//double ss_xx = sqrd_sum(xlist) - len(xlist)*x_mean*x_mean;
		return ss_xy/ss_xx;
	}
}

//Calculates y-intercept of datapoints
double intercept(List *ylist, List *xlist){
	if(compare_length(ylist,xlist) != 1){
		printf("List must be of the same length.\n");
		return 0;
	}
	else{
		double intercept = mean(ylist) - slope(ylist,xlist)*mean(xlist);
		return intercept;
	}
}

//Prints linear equation that can predict future values based on existing data points
void linearRegression(List *ylist, List *xlist){
	if(compare_length(ylist,xlist) != 1){
	        printf("List must be of the same length.\n");
	        return;
       	}
	printf("Linear equation : y = %lfx + %lf\n",slope(ylist,xlist),intercept(ylist,xlist));
}

//Predicts future values of inputs based on current data points available
double linearPrediction(double input,double slope,double intercept){
	return slope*input + intercept;
}
//cost function or  one half mean squared error function for calculating residual error
double half_mse(double slope, double intercept,List *ylist, List *xlist){
	Node *p = ylist->head;
	Node *q = xlist->head;
	double sqrd_error = 0;
	double error = 0;
	while(p){
		error = p->key - linearPrediction(q->key,slope,intercept);
		sqrd_error += error*error;
		p = p->next;
		q = q->next;
	}
	double cost = sqrd_error*0.5;
	cost = cost/len(ylist);
	return cost;
}

//Updates slope
double update_slope(List *ylist, List *xlist,double slope,double intercept,double learning_rate){
	double slope_derivative = 0;
	double m = slope;
	Node *p = xlist->head, *q = ylist->head;
	while(p){
		// Calculates partial derivative of MSE with respect to slope
		// dMSE/dm = -2x[y - (mx + b)]
		slope_derivative += -2*p->key*(q->key - linearPrediction(q->key,slope,intercept));
		p = p->next;
		q = q->next;
	}

	m -= learning_rate*slope_derivative/(float)len(xlist);
	
	return m;
}	

//Updates intercept
double update_intercept(List *ylist, List *xlist,double slope, double intercept,double learning_rate){
	double b = intercept, intercept_derivative = 0;
	Node *p = ylist->head, *q = xlist->head;
	while(p){
		intercept_derivative += -2*(p->key - linearPrediction(q->key,slope,intercept));
		p = p->next;
		q = q->next;
	}
	b -= learning_rate*intercept_derivative/(float)len(xlist);
	
	return b;
}

//Gradient Descent
//Returns slope and intercept of line needed for smallest error
double* grad_desc(List *ylist,List *xlist,double slope, double intercept, double learning_rate){
	double m = slope,error,previous_error = 0,prev_m,prev_b,b = intercept;
	error = half_mse(m,b,ylist,xlist);
	previous_error = error;
	printf("Slope Intercept Error\n");
	while(previous_error >= error){
		printf("%.4lf %.4lf %.4lf \n",m,b,error);
		prev_m = m;
		prev_b = b;
		previous_error = error;
		m = update_slope(ylist,xlist,m,b,learning_rate);
		b = update_intercept(ylist,xlist,m,b,learning_rate);
		error = half_mse(m,b,ylist,xlist);
		
	}
	double *param = (double*)malloc(sizeof(double)*2);
	param[0] = prev_m;
	param[1] = prev_b;
	return param;
}
