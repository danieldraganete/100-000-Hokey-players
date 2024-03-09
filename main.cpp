#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <cstdlib>
#include <ctime>

// Structure to represent a hockey player
struct HockeyPlayer {
    int id;
    std::string name;
    int jersey;
    std::string teamName;
};

// Class LRUCache to implement Least Recently Used cache for HockeyPlayer
class LRUCache {
private:
    std::list<HockeyPlayer> cacheList; // Doubly linked list to maintain order of access
    std::unordered_map<int, std::list<HockeyPlayer>::iterator> cacheMap; // Hashmap for O(1) access to cache elements
    const int capacity; // Capacity of the cache

public:
    // Constructor to initialize the cache with given capacity
    LRUCache(int capacity) : capacity(capacity) {}

    // Method to retrieve a player from the cache or file
    HockeyPlayer* getPlayer(int id) {
        // Check if player is present in cache
        if (cacheMap.find(id) != cacheMap.end()) {
            // If player is in cache, move it to the front of the list (most recently used)
            auto it = cacheMap[id];
            cacheList.splice(cacheList.begin(), cacheList, it);
            return &(*it); // Return pointer to the player
        } else {
            // If player is not in cache, read from file
            HockeyPlayer player;
            std::ifstream file("hockey_players.txt");
            if (!file) {
                std::cerr << "Error: Unable to open file." << std::endl;
                return nullptr;
            }
            // Skip first line (header)
            std::string line;
            std::getline(file, line);
            // Search for the player in the file
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                if (iss >> player.id >> player.name >> player.jersey >> player.teamName) {
                    if (player.id == id) {
                        // If player found, add it to the cache
                        cacheList.push_front(player);
                        cacheMap[id] = cacheList.begin();
                        file.close();
                        // If cache size exceeds capacity, remove least recently used player
                        if (cacheList.size() > capacity) {
                            int idToRemove = cacheList.back().id;
                            cacheMap.erase(idToRemove);
                            cacheList.pop_back();
                        }
                        return &(*cacheMap[id]); // Return pointer to the player
                    }
                }
            }
            file.close();
            return nullptr; // Player not found in file
        }
    }
};

// Function to generate a file with 100,000 random hockey players
void generatePlayersFile() {
    std::ofstream file("hockey_players.txt");
    if (!file) {
        std::cerr << "Error: Unable to create file." << std::endl;
        return;
    }
    file << "id name jersey teamName" << std::endl; // Write header to file
    // Generate and write random hockey players to the file
    for (int i = 1; i <= 100000; ++i) {
        std::string name = "Player" + std::to_string(i);
        int jersey = rand() % 99 + 1;
        std::string teamName = "Team" + std::to_string(rand() % 1000 + 1);
        file << i << " " << name << " " << jersey << " " << teamName << std::endl;
    }
    file.close(); // Close the file
}

int main() {
    generatePlayersFile(); // Generate the file containing random hockey players

    LRUCache cache(10); // Create LRUCache object with capacity for 10 players

    // Test getPlayer method for players with ids from 1 to 15
    for (int i = 1; i <= 15; ++i) {
        HockeyPlayer* player = cache.getPlayer(i); // Get player from cache or file
        if (player != nullptr) {
            std::cout << "Player found with id " << i << ": " << player->name << std::endl;
        } else {
            std::cout << "Player with id " << i << " not found." << std::endl;
        }
    }

    return 0;
}
