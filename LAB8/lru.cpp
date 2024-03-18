#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
using namespace std;

int main(int argc, char const *argv[])
{
	if (argc != 5) {
		cout << "Usage: ./fifo <vsize> <psize> <ssize> <filename>" << endl;
		return 1;
	}

    vector<int> page_accesses;
    vector<int> lru;
	int vsize = atoi(argv[1]);
	int psize = atoi(argv[2]);
	int ssize = atoi(argv[3]);
    int no_of_page_hits = 0;
    int no_of_page_faults = 0;

    fstream file;
    file.open(argv[4], ios::in);
    char line[256];
    file.getline(line, 256);
	stringstream ss(line);
	int n;
	while (ss >> n){
		page_accesses.push_back(n);
	}
    if (ssize < vsize-psize){
        cerr << "Error: Require more blocks in the swap space than it is available" << endl;
        exit(1);
    }

    for(int i=0; i<page_accesses.size(); i++){
        if (page_accesses[i] > vsize) {
            cout << "Error: VPN can't be greater than no. of virtual pages" << endl;
            exit(1);
        }
        auto it = std::find(lru.begin(), lru.end(), page_accesses[i]);
        if (it != lru.end()) { //found
            no_of_page_hits++;
            lru.erase(it);
            lru.push_back(page_accesses[i]);
        }
        else {
            no_of_page_faults++;
            if(lru.size() < psize) {
                lru.push_back(page_accesses[i]);
            }
            else{
                lru.erase(lru.begin());
                lru.push_back(page_accesses[i]);
            }
        }
    }
    cout << "No. of Page Hits: " << no_of_page_hits << endl;
    cout << "No. of Page Faults: " << no_of_page_faults << endl;
    cout << "Percentage of page fault: " << (float)no_of_page_faults/(float)page_accesses.size() << endl;
    file.close();
    return 0;
}