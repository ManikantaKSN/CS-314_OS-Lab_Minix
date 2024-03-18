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
    vector<int> mem;
    queue<int> queue_fifo;
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
        auto it = std::find(mem.begin(), mem.end(), page_accesses[i]);
        if (it != mem.end()) no_of_page_hits++;
        else {
            no_of_page_faults++;
            if(queue_fifo.size() < psize) {
                mem.push_back(page_accesses[i]);
                queue_fifo.push(page_accesses[i]);
            }
            else{
                mem.erase(std::remove(mem.begin(), mem.end(), queue_fifo.front()), mem.end());
                queue_fifo.pop();
                queue_fifo.push(page_accesses[i]);
                mem.push_back(page_accesses[i]);
            }
        }
    }
    cout << "No. of Page Hits: " << no_of_page_hits << endl;
    cout << "No. of Page Faults: " << no_of_page_faults << endl;
    cout << "Percentage of page faults: " << (float)no_of_page_faults/(float)page_accesses.size() << endl;
    file.close();
    return 0;
}