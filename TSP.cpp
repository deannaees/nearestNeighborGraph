#include "TSP.hpp"
#include <climits>

/**
 * Displays the edges and total distance of the tour.
 * Each edge is printed in the format: "EDGE start_id -> end_id | WEIGHT: weight".
 */
void TSP::Tour::display() const {
  for (size_t i = 1; i < path.size(); i++) {
    std::cout << "EDGE " << path[i-1].id << " -> " << path[i].id << " | WEIGHT : " << weights[i] << std::endl;
  }
  std::cout << "TOTAL DISTANCE: " << total_distance << std::endl;
}

/**
 * Reads a .tsp file and constructs a list of cities as nodes.
 * The file should have a section labeled "NODE_COORD_SECTION" followed by lines with the format: ID x-coordinate y-coordinate.
 * 
 * @param filename The path to the TSP file.
 * @return A list of `Node` objects representing cities and their coordinates.
 * @throws std::runtime_error If the file cannot be read or parsed.
 * 
 * @pre The file specified by `filename` exists and follows the TSP format.
 */
std::list<Node> TSP::constructCities(const std::string& filename) {
  // Read past metadata
  std::ifstream fin(filename);
  if (fin.fail()) {
    std::cerr << "ERROR: Could not read file: " << filename << std::endl;
    throw std::runtime_error("Failed to read file. Terminating.");
  }

  std::string line;
  do { std::getline(fin, line); }
  while (line.find("NODE_COORD_SECTION"));

  // Read data from file into Node list "cities"
  std::list<Node> cities;
  size_t ID;
  double x, y;
  while (!fin.eof()){
    if (!(fin >> ID >> x >> y)) break;
    cities.push_back(Node(ID, x, y));
  }
  return cities;
}
/**
 * Constructs a tour using the nearest neighbor heuristic for the traveling salesperson problem (TSP).
 * Starting from the city of the given ID, it iteratively visits the nearest unvisited city and returns to the start.
 *
 * @param cities A list of `Node` objects representing the cities to be visited.
 * @param start_id The unique identifier of the starting city.
 * @return A `TSP::Tour` object representing the path, edge weights, and total distance of the computed tour.
 * 
 * @pre `start_id` must be a valid city ID within the range of IDs in `cities`.
 * @note The ith weight in `weights` is the distance traveled from the previous (i-1) city to the current i city (represented by the ith city in the returned Tour.path)
 *       As such, the first weight will ALWAYS equal 0, since there is no edge from the start city to itself
 *
 */
  TSP::Tour TSP::nearestNeighbor(std::list<Node> cities, const size_t& start_id)
  {
    
    Tour tour;
     tour.total_distance = 0;
  //   //iterable variable  -> it 
      auto it2 = cities.begin();
      for(auto it = cities.begin(); it!= cities.end(); ++it){
        if((*it).id == start_id){
            it2 = it;
        }//end of setting starting point
      }

  //   traverse through list -> ieach time remove cities ; starting city ; traverse and remove the starting city from list; traverse sthrough list to find shortesst distance and store in another iterator(so we can do delete easily)
  // before we erase the starting city, push it into the tour? right ..
    tour.path.push_back(*it2);
    tour.weights.push_back(0);
    //we have no distance since we are the beginning
    int shortest = INT_MAX;
    int curr_distance;
    auto it_currCity = it2;

    Node curr_city  = *it2;
    cities.erase(it_currCity);

    while( !cities.empty()){ //know we are traversing through 
    //         //starting point is removed 

          //curr_city = *it_currCity;


         

          //remove the rest according to their distance
          //distance --> int to keep track ;
          //finding highest curr distance to keep track , and keep calculting from city curr at to the neighbors and after that add shortest city distance ; reset the distance at end bc then we go to another city 
          for(auto it3 = cities.begin(); it3!=cities.end(); ++it3){
              curr_distance = (curr_city).distance(*it3);
            
              //we check if the curr_distance is > then the previous iterator;
              //if its not, then we add to the curr_distance

              if(shortest > curr_distance){
                  shortest = curr_distance;
                  it_currCity = it3;
              }
          } 
          // curr_city = *it_currCity;
          // it_currCity = it_currCity;

          tour.path.push_back(*it_currCity);
          tour.weights.push_back(shortest);
          tour.total_distance += shortest;
         
          shortest = INT_MAX;
          curr_city = *it_currCity;
          cities.erase(it_currCity);
          // cities.erase(it_currCity);


    
    }//so now we have removed the cities and we should be empty 



    int start_city_again = tour.path.front().distance(*it_currCity);
    tour.path.push_back(tour.path.front());
    tour.weights.push_back(start_city_again);
    tour.total_distance += start_city_again;

    return tour;
  }