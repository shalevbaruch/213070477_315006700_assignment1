#define _GNU_SOURCE
#include <stdlib.h>
#include<stdio.h>


int chr_until_comma(const char * str);
int initialize(double *** arr_ptr, int * num_of_coordinates_ptr, int * N_ptr);
void enter_datapoint(double *arr, char* line);
int min_centroid(double ** centroid_arr, double * datapoint, int num_of_coordinates, int K);
double euclidean_distance(double *datapoint1, double *datapoint2,  int num_of_coordinates);
int contains_only_digits(char * word);
int check_input(int argc, char* argv[], int* iterations_pointer, int* K_pointer);
int initialize_centroids_arr(double*** centroids_arr_pointer, int K, int num_of_iterations,double** datapoints_arr);
int kmeans_algoritm(int iterations, double** centroids_arr, double** datapoints_arr,int num_of_datapoints, int num_of_coordinates, int K);
void print_result(int iterations, double** centroids_arr, double** new_centroids_arr, int num_of_coordinates, int K);


int initialize(double *** arr_ptr, int * num_of_coordinates_ptr, int * N_ptr) {
    char *line = NULL;
    size_t len = 0;
    size_t nread;
    size_t i;
    int num_of_coordinates = 1;
    int capacity = 0;
    int num_of_datapoints = 0;
    double** arr = NULL;

    while ((nread = getline(&line, &len, stdin)) != (size_t)-1){
        if (num_of_datapoints == 0){
        for (i = 0; i < nread; i++) {

            if (line[i] == ',') { /* if we find a comma, increment the count */
                num_of_coordinates++;
            }
        }
        }
        
        if (num_of_datapoints == capacity){
            if (capacity == 0){
                capacity = 128;
            }
            capacity = capacity * 2;
            arr = (double**)realloc(arr, capacity * sizeof(double*));
            if (arr == NULL) {
                return 0;
            }
        }
        arr[num_of_datapoints] = (double*)malloc(num_of_coordinates * sizeof(double));
        if (arr[num_of_datapoints] == NULL) {
            return 0;
        }
        enter_datapoint(arr[num_of_datapoints], line);
        num_of_datapoints++;
    }
    free(line);
    *N_ptr = num_of_datapoints;
    *num_of_coordinates_ptr = num_of_coordinates;
    *arr_ptr = arr;
    return 1; 
}



int main(int argc, char * argv[]) {
    int num_of_coordinates;
    int num_of_datapoints;
    double** datapoints_arr = NULL;
    double** centroids_arr;
    int K;
    int iterations;

    if(!check_input(argc,argv, &iterations, &K)){
        return 1;
    }
    /* initialize datapoints_arr,num_of_coordinates,num_of_datapoints  */
    if (!initialize(&datapoints_arr, &num_of_coordinates, &num_of_datapoints)){
        printf("An Error Has Occurred");
        return 1;
    }
    if (K >= num_of_datapoints){
         printf("Invalid number of clusters!");
         return 1;
    }

    if(!initialize_centroids_arr(&centroids_arr, K, num_of_coordinates, datapoints_arr)){
        printf("An Error Has Occurred");
        return 1;
    }


    if(!kmeans_algoritm(iterations, centroids_arr, datapoints_arr, num_of_datapoints, num_of_coordinates, K)){
        printf("An Error Has Occurred");
        return 1;
    }
    return 0;
}



int chr_until_comma(const char * str){
    int i = 0;
    if (str == NULL){
        return 0;
    }
    while (str[i] != '\0' && str[i] != ','){
        i++;
    }
    return i;
}


void enter_datapoint(double *arr, char* line){
    int i = 0;
    int pos = 0;
    while (sscanf(line + pos, "%lf", &arr[i]) == 1) {
        pos += chr_until_comma(line + pos) + 1;
        if ((line[pos-1]) == '\0'){
            break;
        }
        i++;
    }
}

int min_centroid(double ** centroid_arr, double * datapoint, int num_of_coordinates, int K){
    double tmp_min_distance;
    double min_distance = euclidean_distance(centroid_arr[0], datapoint, num_of_coordinates);
    int index_of_min_centroid = 0;
    int i;
    for (i = 1; i < K; i++){
        tmp_min_distance = euclidean_distance(centroid_arr[i], datapoint, num_of_coordinates);
        if (tmp_min_distance < min_distance){
            min_distance = tmp_min_distance;
            index_of_min_centroid = i;
        }
    }
    return index_of_min_centroid;
}


double euclidean_distance(double *datapoint1, double *datapoint2, int num_of_coordinates){
   double sum = 0;
   int i;
   for (i = 0; i < num_of_coordinates; i++){
       sum += (datapoint1[i] - datapoint2[i]) * (datapoint1[i] - datapoint2[i]);
   }
   return sum;
}


int contains_only_digits(char * word){
    int i = 0;
    int num_of_dots = 0;
    int is_only_digits = 1;
    while (word[i] != '\0'){
        if ((word[i] < '0' || word[i] > '9')){
            if (word[i] == '.'){
                if (num_of_dots == 0){
                    num_of_dots++;
                }
                else{
                    is_only_digits = 0;
                } 
            }
            else{
                is_only_digits = 0;
            }
            
        }
        i++;
    }
    return is_only_digits;
}


int check_input(int argc, char* argv[], int* iterations_pointer, int* K_pointer){
    double check_iterations;
    double check_K;

    if (argc < 2 || argc > 3){
            printf("An Error Has Occurred");
            return 0;
        }
    if (argc == 2){
    *iterations_pointer = 200;
    }
    else{
        if (!contains_only_digits(argv[2])){
        printf("Invalid maximum iteration!");
        return 0;
    }
    check_iterations = strtod(argv[2], NULL);
        if ((check_iterations == (int)check_iterations)  &&  (1<(int)check_iterations) && ((int)check_iterations<1000)){
            *iterations_pointer = (int)check_iterations;
        }
        else{
            printf("Invalid maximum iteration!");
            return 0;
        }
    }
    if (!contains_only_digits(argv[1])){
        printf("Invalid number of clusters!");
        return 0;
    }
    check_K = strtod(argv[1], NULL);
    if ((check_K == (int)check_K) && (int)check_K > 1){
        *K_pointer = (int)check_K;
    }
    else{
        printf("Invalid number of clusters!");
        return 0;
    }
    return 1;
}



int initialize_centroids_arr(double*** centroids_arr_pointer, int K, int num_of_coordinates, double** datapoints_arr){
    double** centroids_arr = (double**) malloc(K * sizeof (double *));
    int i;
    int j;

    if (centroids_arr == NULL){
        return 0;
    }
    for(i=0; i < K; i++){
        centroids_arr[i] = (double*) malloc(num_of_coordinates * sizeof(double));
        if (centroids_arr[i] == NULL){
            return 0;
        }
        for (j = 0; j < num_of_coordinates; j++){
            centroids_arr[i][j] = datapoints_arr[i][j];
        }
    }
    *centroids_arr_pointer = centroids_arr;
    return 1;
}


int kmeans_algoritm(int iterations, double** centroids_arr, double** datapoints_arr,int num_of_datapoints, int num_of_coordinates, int K){
    int i = 0;
    int j = 0;
    int m = 0;
    int smaller_than_epsilon = 1;
    double epsilon = 0.001;
    double ** new_centroids_arr;
    int index_of_centroid;
    int *datapoints_num_in_centroids;

    datapoints_num_in_centroids = calloc(K, sizeof(int));
    if (datapoints_num_in_centroids == NULL){
        return 0;
    }
    while (iterations){
        
        new_centroids_arr = (double**) malloc(K * sizeof (double *));
        if (new_centroids_arr == NULL){
            return 0;
        }
        for(i=0; i < K; i++){
            new_centroids_arr[i] = (double*) calloc(num_of_coordinates,sizeof(double));
            if (new_centroids_arr[i] == NULL){
                return 0;
            }
        }
        for (i=0; i < num_of_datapoints; i++){
            index_of_centroid = min_centroid(centroids_arr, datapoints_arr[i], num_of_coordinates, K);
            for (j=0; j < num_of_coordinates; j++){
                new_centroids_arr[index_of_centroid][j] += datapoints_arr[i][j];
            }
            datapoints_num_in_centroids[index_of_centroid] += 1;
        }

        for (i=0; i < K; i++){
            for (j=0; j < num_of_coordinates; j++){
                if (datapoints_num_in_centroids[i] != 0){
                    new_centroids_arr[i][j] = new_centroids_arr[i][j] / datapoints_num_in_centroids[i];                  
                } else {
                    new_centroids_arr[i][j] = centroids_arr[i][j];
                }
            }
        }

        for (i=0; i < K; i++){
            if (euclidean_distance(new_centroids_arr[i], centroids_arr[i], num_of_coordinates) >= epsilon){
               smaller_than_epsilon = 0;
               for(m=0; m < K; m++){
                datapoints_num_in_centroids[m] = 0;
               }
               for(j = 0; j < K; j++){
                free(centroids_arr[j]);
            }
            free(centroids_arr);
            centroids_arr = new_centroids_arr;
            new_centroids_arr = NULL;
               break;
            }
        }
        if (smaller_than_epsilon){
            break;
        }
        smaller_than_epsilon = 1;
        iterations--;
    }
    print_result(iterations, centroids_arr, new_centroids_arr, num_of_coordinates, K);
    if (iterations > 0){
        for(i = 0; i < K; i++){
                free(centroids_arr[i]);
                free(new_centroids_arr[i]);
            }
            free(centroids_arr);
            free(new_centroids_arr);
    }   
    else{
        for(i=0; i < K; i++){
            free(centroids_arr[i]);
        }
        free(centroids_arr);
    }

    free(datapoints_num_in_centroids);
    for (i = 0; i < num_of_datapoints; i++) {
        free(datapoints_arr[i]);
    }
    free(datapoints_arr);
    return 1;
}

void print_result(int iterations, double** centroids_arr, double** new_centroids_arr, int num_of_coordinates, int K){
 int i;
 int j;
 if (iterations > 0){
    for (i=0; i < K; i++){
        for(j=0; j < num_of_coordinates; j++){
            printf("%.4f", new_centroids_arr[i][j]);
            if (j < num_of_coordinates - 1){
                printf(",");}
            
        }
        printf("\n");
    }
    printf("\n");
 }
 else{
    for (i=0; i < K; i++){
            for(j=0; j < num_of_coordinates; j++){
                printf("%.4f", centroids_arr[i][j]);
                if (j < num_of_coordinates - 1){
                    printf(",");
                }
                
            }
            printf("\n");
        }
        printf("\n");
 }
}