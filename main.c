#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#include<stdlib.h>

struct edge{
	int id_e;
	int edge_length;
	struct vertex *start,*end;
};

struct vertex{
	int id_v;
	struct list_pointer *edges;
};

struct vertex_pointer{
	struct vertex *head;
};

struct list_node{
	struct edge *edge;
	struct list_node *next;
};

struct list_pointer{
	struct list_node *head;
};

struct table{
	int size;
	int *pointer;
};

bool add_to_table(struct table *table, int data);
void print_table(struct table *table);
void clear_table(struct table *table);
bool check_in_table(struct table *table, int id);

struct vertex * add_vertex(int id_v);
bool add_edge(struct vertex *start_vertex, struct vertex *end_vertex, int id_e, int edge_length);
bool check_vertex(struct vertex *vertex, int id);
struct vertex * recursive_search_vertex(struct vertex *vertex, int id, struct table *table);
struct vertex * search_vertex(struct vertex *vertex, int id, struct table *table);
void print_graph(struct vertex *vertex);

int check_matrix_size(struct vertex *vertex, struct table *s_table);
int *** create_matrix(int ***matrix, int size_of_matrix);
void print_matrix(int ***matrix, int size_of_matrix);
void matrix_search_vertex(struct vertex *vertex, struct table *table, int ***matrix);
void matrix_vertex(struct vertex *vertex, struct table *table, int ***matrix);


bool add_to_table(struct table *table, int data){
	if(table->size == 0){
		table->pointer = (int*)calloc(1, sizeof(int));
		if(!table->pointer)
			return false;

		table->size = 1;
		table->pointer[0] = data;
		return true;
	}

	for(int i=0;i<table->size;i++){
		if(table->pointer[i] == data)
			return true;
	}

	table->pointer = (int*)realloc(table->pointer,sizeof(int)*(table->size+1));
	if(!table->pointer)
		return false;

	table->pointer[table->size] = 1;
	table->size += 1;
	return true;
}

void print_table(struct table *table){
	printf("\nPrint table:");
	for(int i=0;i<=table->size;i++){
		printf("\n %d: %d",i,table->pointer[i]);
	}
}

void clear_table(struct table *table){
	free(table->pointer);
	table->pointer = NULL;
	table->size=0;
}

bool check_in_table(struct table *table, int id){
	for(int i=0;i<table->size;i++){
		if(table->pointer[i] == id)
			return true;
	}
	return false;
}


struct vertex * add_vertex(int id_v){
	struct vertex *new_vertex = (struct vertex *)malloc(sizeof(struct vertex));
	if(!new_vertex)
		return NULL;
	new_vertex->id_v = id_v;
	new_vertex->edges = NULL;
	return new_vertex;
}

bool add_edge(struct vertex *start_vertex, struct vertex *end_vertex, int id_e, int edge_length){
	if(!start_vertex)
		return false;
	
	if(!end_vertex)
		return false;

	struct edge *new_edge = (struct edge *)malloc(sizeof(struct edge));
	if(!new_edge)
		return false;

	struct list_node *new_node = (struct list_node *)malloc(sizeof(struct list_node));		
	if(!new_node)
		return false;

	new_edge->id_e = id_e;
	new_edge->edge_length = edge_length;
	new_edge->start = start_vertex;
	new_edge->end = end_vertex;

	new_node->edge = new_edge;
	new_node->next = NULL;

	struct list_pointer *new_list = (struct list_pointer *)malloc(sizeof(struct list_pointer));
	if(!new_list)
		return false;

	if(NULL != start_vertex->edges){
		new_list->head = start_vertex->edges->head;
		for(;NULL != new_list->head->next; new_list->head = new_list->head->next){}
		new_list->head->next = new_node;
		return true;
	}
	else{
		start_vertex->edges = new_list;
		start_vertex->edges->head = new_node;
		return true;
	}	
}

bool check_vertex(struct vertex *vertex, int id){
	if(!vertex)
		return false;

	if(vertex->id_v == id){
		return true;
	}

	else
		return false;
}

struct vertex * recursive_search_vertex(struct vertex *vertex, int id, struct table *table){
	if(false == check_in_table(table,vertex->id_v)){
		add_to_table(table, vertex->id_v);

		if(check_vertex(vertex, id)){
			return vertex;
		}

		if(vertex->edges != NULL){
			struct vertex *new_vertex = (struct vertex *)malloc(sizeof(struct vertex));
			if(!new_vertex)
				return NULL;

			struct list_pointer *new_list = (struct list_pointer *)malloc(sizeof(struct list_pointer));
			if(!new_list)
				return NULL;

			new_list->head = vertex->edges->head;
			for(new_list;NULL != new_list->head; new_list->head = new_list->head->next){
				new_vertex = recursive_search_vertex(new_list->head->edge->end, id, table);
				if(NULL != new_vertex)
					return new_vertex;
			}
		}
	}
}

struct vertex * search_vertex(struct vertex *vertex, int id, struct table *table){
	clear_table(table);
	return recursive_search_vertex(vertex, id, table);
}

void print_graph(struct vertex *vertex){
	if(!vertex)
		return;

	if(vertex->edges != NULL){
		struct list_pointer *new_list = (struct list_pointer *)malloc(sizeof(struct list_pointer));
		if(!new_list)
			return;

		new_list->head = vertex->edges->head;
		for(new_list;NULL != new_list->head; new_list->head = new_list->head->next){
			printf("\nSTART_VERTEX_ID: %d END_VERTEX_ID: %d ID_E: %d EDGE_LENGTH: %d", vertex->id_v, new_list->head->edge->end->id_v, new_list->head->edge->id_e, new_list->head->edge->edge_length);
			print_graph(new_list->head->edge->end);
		}
	}
}

int check_matrix_size(struct vertex *vertex, struct table *s_table){
	
	struct table table = {0,NULL};
	int act = 0;
	int result = 0;
	
	do{
		if(NULL != search_vertex(vertex, act, s_table)){
			if(false != add_to_table(&table, act)){
				act += 1;
				result += 1;
			}
		}
		else
			break;

	}while(act > -1);
	return result;
}

int *** create_matrix(int ***matrix, int size_of_matrix){
	matrix = (int***)malloc(sizeof(int**)*size_of_matrix);
	if(size_of_matrix == -1){
		printf("\nadd_to_table return false");
		return NULL;
	}
	if(size_of_matrix == 0){
		printf("\ncheck_matrix_size return 0");
		return NULL;
	}
	
	for(int i=0;i<size_of_matrix;i++){
		matrix[i] = (int**)malloc(sizeof(int*)*size_of_matrix);
		for(int j=0;j<size_of_matrix;j++){
			matrix[i][j] = (int*)malloc(sizeof(int)*size_of_matrix);
			for(int k=0;k<size_of_matrix;k++)
				matrix[i][j][k] = -1;
		}

	}
	return matrix;
}

void print_matrix(int ***matrix, int size_of_matrix){
	// for(int i=0;i<size_of_matrix;i++){
	for(int i=0;i<1;i++){
		printf("\nMatrix A%d",i);
		for(int j=0;j<size_of_matrix;j++){
			printf("\n");
			for(int k=0;k<size_of_matrix;k++){
				if(matrix[i][j][k] > -1)
					printf(" ");
				printf(" %d", matrix[i][j][k]);
			}

		}
	}
}

void matrix_search_vertex(struct vertex *vertex, struct table *table, int ***matrix){
	if(false == check_in_table(table,vertex->id_v)){
		add_to_table(table, vertex->id_v);

		if(vertex->edges != NULL){
			struct vertex *new_vertex = (struct vertex *)malloc(sizeof(struct vertex));
			if(!new_vertex)
				return;

			struct list_pointer *new_list = (struct list_pointer *)malloc(sizeof(struct list_pointer));
			if(!new_list)
				return;

			new_list->head = vertex->edges->head;
			for(new_list;NULL != new_list->head; new_list->head = new_list->head->next){
				matrix[0][vertex->id_v][new_list->head->edge->end->id_v] = new_list->head->edge->edge_length;
				matrix_search_vertex(new_list->head->edge->end, table, matrix);
			}
		}
	}
}

void matrix_vertex(struct vertex *vertex, struct table *table, int ***matrix){
	clear_table(table);
	matrix_search_vertex(vertex, table, matrix);
}

int main(){
   	int c_time;
	struct table s_table = {0,NULL};

	//Graph1.png representation
	struct vertex_pointer vertex;
	vertex.head = add_vertex(0);
	add_edge(search_vertex(vertex.head, 0, &s_table), add_vertex(1), 0, 2);
	add_edge(search_vertex(vertex.head, 0, &s_table), add_vertex(2), 1, 2);
	add_edge(search_vertex(vertex.head, 0, &s_table), add_vertex(3), 2, 3);
	add_edge(search_vertex(vertex.head, 1, &s_table), add_vertex(4), 3, 5);
	add_edge(search_vertex(vertex.head, 1, &s_table), add_vertex(5), 4, 4);
	add_edge(search_vertex(vertex.head, 0, &s_table), search_vertex(vertex.head, 5, &s_table), 5, 7);
	add_edge(search_vertex(vertex.head, 2, &s_table), add_vertex(6), 6, 5);
	add_edge(search_vertex(vertex.head, 3, &s_table), search_vertex(vertex.head, 6, &s_table), 7, 5);
	add_edge(search_vertex(vertex.head, 6, &s_table), add_vertex(7), 8, 3);

	int ***matrix;
	int size_of_matrix = check_matrix_size(vertex.head, &s_table);
	matrix = create_matrix(matrix, size_of_matrix);

	print_matrix(matrix, size_of_matrix);

	matrix_vertex(vertex.head, &s_table, matrix);

	print_matrix(matrix, size_of_matrix);

	c_time = clock();
	printf("\nCzas wykonania: %f", (float)c_time/CLOCKS_PER_SEC);
	
	return 0; 
}
