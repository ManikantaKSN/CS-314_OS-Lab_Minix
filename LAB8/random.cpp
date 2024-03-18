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
    vector<int> random;
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

    srand(getpid());
    for(int i=0; i<page_accesses.size(); i++){
        if (page_accesses[i] > vsize) {
            cerr << "Error: VPN can't be greater than no. of virtual pages" << endl;
            exit(1);
        }

        auto it = std::find(random.begin(), random.end(), page_accesses[i]);
        if (it != random.end()) no_of_page_hits++;
        else {
            no_of_page_faults++;
            if(random.size() < psize) {
                random.push_back(page_accesses[i]);
            }
            else{
                int remove = rand() % psize;
                random.erase(random.begin() + remove);
                random.push_back(page_accesses[i]);
            }
        }
    }
    
    cout << "No. of Page Hits: " << no_of_page_hits << endl;
    cout << "No. of Page Faults: " << no_of_page_faults << endl;
    cout << "Percentage of page fault: " << (float)no_of_page_faults/(float)page_accesses.size() << endl;
    file.close();
    return 0;
}