# K-Means Clustering Algorithm
This repository contains an implementation of the K-Means Clustering Algorithm, a popular unsupervised machine learning technique used for partitioning a dataset into K clusters. The algorithm aims to minimize the sum of squared distances between data points and their corresponding cluster centroids.

## Problem Description
The task is to implement the K-Means Algorithm in both Python and C, and apply it to a given dataset. The input dataset contains N data points, each with M coordinates. The program should partition the data points into K clusters and output the centroids of these clusters.

## Usage
The program can be run from the command line with the following arguments:
#### Python:
python kmeans.py <K> [<iterations>] <input_file>

<K>: The number of clusters (required).

<iterations>: The maximum number of iterations (optional, default is 200).

<input_file>: The path to the input file containing the dataset (required).

#### C:
compile the program with: gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans

run the program with: ./kmeans <K> [<iterations>]  < <input_file>

<K>: The number of clusters (required).

<iterations>: The maximum number of iterations (optional, default is 200).

<input_file>: The path to the input file containing the dataset (required).

#### Note: there is a redirection symbol between  [<iterations>] and <input_file> - it has to be there!

## Input File Format
The input file should contain one data point per line, with coordinates separated by commas. For example:

1.2,3.4,5.6
7.8,9.1,2.3
4.5,6.7,8.9
## Output Format
The program outputs the K centroids, one per line, with their coordinates separated by commas and formatted to 4 decimal places. For example:

1.2000,3.4000,5.6000
7.8000,9.1000,2.3000
4.5000,6.7000,8.9000
Algorithm
The K-Means Algorithm implemented in this repository follows these steps:

Initialize the centroids by selecting the first K data points from the dataset.
Assign each data point to the nearest centroid.
Update the centroids by computing the mean of all data points assigned to each centroid.
Repeat steps 2 and 3 until convergence (i.e., centroids do not change significantly) or the maximum number of iterations is reached.
