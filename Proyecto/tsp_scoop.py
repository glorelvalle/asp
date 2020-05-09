# Lloro
import random, operator, time, itertools, math
import numpy
import array

from functools import partial

from deap import algorithms, base, creator, tools

from scoop import futures


City = complex # Constructor for new cities, e.g. City(300, 400)

def total_distance(tour):
    "The total distance between each pair of consecutive cities in the tour."
    return sum(distance(tour[i], tour[i-1])
               for i in range(len(tour)))

def exact_TSP(cities):
    "Generate all possible tours of the cities and choose the shortest one."
    return shortest(alltours(cities))

def shortest(tours):
    "Return the tour with the minimum total distance."
    return min(tours, key=total_distance)

def distance(A, B):
    "The Euclidean distance between two cities."
    return abs(A - B)

def generate_cities(n):
    "Make a set of n cities, each with random coordinates."
    return set(City(random.randrange(10, 890),
                    random.randrange(10, 590))
               for c in range(n))

def all_non_redundant_tours(cities):
    "Return a list of tours, each a permutation of cities, but each one starting with the same city."
    start = first(cities)
    return [[start] + list(tour)
            for tour in itertools.permutations(cities - {start})]

def first(collection):
    "Start iterating over collection, and return the first element."
    for x in collection: return x

def exact_non_redundant_TSP(cities):
    "Generate all possible tours of the cities and choose the shortest one."
    return shortest(all_non_redundant_tours(cities))

def greedy_TSP(cities):
    "At each step, visit the nearest neighbor that is still unvisited."
    start = first(cities)
    tour = [start]
    unvisited = cities - {start}
    while unvisited:
        C = nearest_neighbor(tour[-1], unvisited)
        tour.append(C)
        unvisited.remove(C)
    return tour

def nearest_neighbor(A, cities):
    "Find the city in cities that is nearest to city A."
    return min(cities, key=lambda x: distance(x, A))


num_cities = 30
cities = generate_cities(num_cities)

toolbox = base.Toolbox()

creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
creator.create("Individual", list, fitness=creator.FitnessMin)
#creator.create("Individual", array.array, typecode='b', fitness=creator.FitnessMax)

toolbox.register("indices", numpy.random.permutation, len(cities))
toolbox.register("individual", tools.initIterate, creator.Individual, toolbox.indices)
toolbox.register("population", tools.initRepeat, list, toolbox.individual)
toolbox.register("mate", tools.cxOrdered)
toolbox.register("mutate", tools.mutShuffleIndexes, indpb=0.05)

def create_tour(individual):
    return [list(cities)[e] for e in individual]

def evaluation(individual):
    '''Evaluates an individual by converting it into
    a list of cities and passing that list to total_distance'''
    return (total_distance(create_tour(individual)),)

toolbox.register("evaluate", evaluation)

toolbox.register("select", tools.selTournament, tournsize=3)

#toolbox.register("map", futures.map)


#pop = toolbox.population(n=100)

#%%time
#result, log = algorithms.eaSimple(pop, toolbox, cxpb=0.8, mutpb=0.2, ngen=400, verbose=False)

#best_individual = tools.selBest(result, k=1)[0]
#print('Fitness of the best individual: ', evaluation(best_individual)[0])
def main():
    random.seed(4)
    N_ISLES = 5
    FREQ = 5

    islands = [toolbox.population(n=100) for i in range(N_ISLES)]

    toolbox.unregister("indices")
    toolbox.unregister("individual")
    toolbox.unregister("population")

    toolbox.register("alg_scoop", algorithms.eaSimple, toolbox=toolbox, cxpb=0.8, mutpb=0.2, ngen=5, verbose=False)

    start_time = time.time()
    for i in range (0, 400, FREQ):
        results = futures.map(toolbox.alg_scoop, islands)
        islands = [pop for pop, logbook in results]
        tools.migRing(islands, 15, tools.selBest)

    print("--- %s seconds ---" % (time.time() - start_time))
    return islands

if __name__ == "__main__":
    print(main())
