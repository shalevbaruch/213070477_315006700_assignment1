import math
import sys


def readVectors(fileName):
    with open(fileName) as f:
        vectors = [[float(v) for v in line.strip().split(",")] for line in f]
    return vectors


def distance(vectorA, vectorB):
    return math.sqrt(sum((a - b) ** 2 for (a, b) in zip(vectorA, vectorB)))


def findClusterIndex(centroidsArray, vector):
    return min(range(len(centroidsArray)), key=lambda i: distance(centroidsArray[i], vector))


def KMeansAlgorithm(datapoints_arr, k, iteration):
    epsilon=0.001
    i = 0
    centroidsArray = datapoints_arr[:k]
    num_of_coordinates = len(centroidsArray[0])
    while i < iteration:
        datapoints_num_in_centroids = [0 for i in range (k)]
        newCentroidsArray = [[0] * num_of_coordinates for _ in range(k)]
        for vector in datapoints_arr:
            index = findClusterIndex(centroidsArray, vector)
            for j in range(num_of_coordinates):
                newCentroidsArray[index][j] += vector[j]
            datapoints_num_in_centroids[index] += 1

        for m in range(k):
            centroid = newCentroidsArray[m]
            if datapoints_num_in_centroids[m] == 0:
                newCentroidsArray[m] = centroidsArray[m]
            else:
                newCentroidsArray[m] = [coordinate / datapoints_num_in_centroids[m] for coordinate in centroid]
        
        to_exit = all(distance(a, b) < epsilon for (a, b) in zip(centroidsArray, newCentroidsArray))
        centroidsArray = newCentroidsArray
        if to_exit:
            break
        i += 1

    print_result(centroidsArray)


def main(): 
    user_input = check_input()
    if user_input == 1:
        return 1
    else:
        k,iterations, filename = user_input
    
    datapoints_arr = readVectors(filename)
    N = len(datapoints_arr)
    if k<=1 or k>=N:
        print("Invalid number of clusters!")
        return 1
    
    KMeansAlgorithm(datapoints_arr, k, iterations)
    
    return 0


def check_input():
    try:
        k = int(sys.argv[1])
    except:
        print("Invalid number of clusters!")
        return 1
    num_of_arguments = len(sys.argv)
    if num_of_arguments == 4:
        try:
            iterations = int(sys.argv[2])
        except:
            print("Invalid maximum iteration!")
            return 1
        filename = sys.argv[3]
        if (iterations <= 1 or iterations >= 1000):
            print("Invalid maximum iteration!")
            return 1
        
    else:
        filename = sys.argv[2]
        iterations = 200

    return [k, iterations, filename]


def print_result(centroidsArray):
    for centroid in centroidsArray:
        line = ",".join("%.4f" % coord for coord in centroid)
        print(line)
    print("")



if __name__ == "__main__":
    main()
