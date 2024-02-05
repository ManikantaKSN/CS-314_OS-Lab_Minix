#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

int time_count = 0;
void selection(vector<vector<int>> &processQueue);

int main(int argc, char* argv[]){
    vector<vector<int>> process_list;
    vector<int> prc;
    vector<vector<int>> cpu_q;
    vector<vector<int>> io_q;
    vector<int> performance_vec {0, 0, 0};
    map<int, int> exit_time;
    map<int, int> cpu_burst_time;
    map<int, int> response_time;
    float avgPr = 0;

    ifstream inpt;
    string inpt_st;
    inpt.open(argv[1]);
    while(inpt >> inpt_st){
        if (inpt_st[0]=='<') continue;
        prc.push_back(stoi(inpt_st));
        if (inpt_st == "-1"){
            process_list.push_back(prc);
            prc.clear();
        }
    }
    inpt.close();

    for (int i = 0; i < process_list.size(); i++){
        int sum = 0;
        for (int j = 1; j < process_list[i].size() - 1; j++){
            sum += process_list[i][j];
        }
        cpu_q.push_back({i, process_list[i][0], process_list[i][1], 2, sum});
        for (int j = 1; j < process_list[i].size(); j+=2)
        {
            cpu_burst_time[i] += process_list[i][j];
        }
    }
    selection(cpu_q);
    int done = 0;

    while (done != process_list.size()) {
    if (!cpu_q.empty()) {
        selection(cpu_q);

        vector<int> currCpuRunning = cpu_q.front();
        int id = currCpuRunning[0];
        int arr_time = currCpuRunning[1];
        int burst_time = currCpuRunning[2];
        int step = currCpuRunning[3];
        int total_burst_time = currCpuRunning[4];

        if (arr_time <= time_count) {
            if (response_time.find(id) == response_time.end()) {
                response_time[id] = time_count - process_list[id][0];
            }

            if (burst_time == 0) {
                if (process_list[id][step] == -1) {
                    exit_time[id] = time_count;
                    done += 1;
                } else {
                    vector<int> newIo = {id, time_count, process_list[id][step], step + 1, total_burst_time - process_list[id][step - 1]};
                    io_q.push_back(newIo);
                }

                cpu_q.erase(cpu_q.begin());
                if (!cpu_q.empty()) {
                    if (response_time.find(id) == response_time.end()) {
                        response_time[cpu_q[0][0]] = time_count - process_list[cpu_q[0][0]][0];
                    }
                    cpu_q[0][2] -= 1;
                }
            } else {
                cpu_q[0][2] -= 1;
            }
        }
    }

    if (!io_q.empty()) {
        vector<int> currIoRunning = io_q.front();
        int ioIndex = currIoRunning[0];
        int io_arr_time = currIoRunning[1];
        int io_burst_time = currIoRunning[2];
        int ioStep = currIoRunning[3];
        int ioTotalBurst_time = currIoRunning[4];

        if (io_arr_time <= time_count) {
            if (io_burst_time == 0) {
                vector<int> newCpu = {ioIndex, time_count, process_list[ioIndex][ioStep], ioStep + 1, ioTotalBurst_time - process_list[ioIndex][ioStep - 1]};
                cpu_q.push_back(newCpu);

                if (cpu_q.size() == 1) {
                    cpu_q[0][2] -= 1;
                }
                io_q.erase(io_q.begin());
                if (!io_q.empty()) {
                    io_q[0][2] -= 1;
                }
            } else {
                io_q[0][2] -= 1;
            }
        }
    }
    time_count += 1;
}

    cout<<"Process\t Arrival time\t Turnaround time\t Waiting time\t Response time\t Penalty Ratio\t"<<endl;
    cout<<"-------\t ------------\t ---------------\t ------------\t -------------\t -------------\t"<<endl;

    for(int i = 0; i < process_list.size(); i++)
    {
        int turnaround_time = exit_time[i] - process_list[i][0];
        int waiting_time = turnaround_time - cpu_burst_time[i];
        int total_burst = 0;
        for (int j = 1; j < process_list[i].size() - 1; j++)
        {
            total_burst += process_list[i][j];
        }
        performance_vec[0]+= response_time[i];
        float penaltyRatio = (float)(waiting_time + total_burst)  / total_burst;
        performance_vec[1]+= waiting_time;
        avgPr+= penaltyRatio;
        performance_vec[2]+= turnaround_time;
        cout<<"      "<<i<<"\t\t"<<process_list[i][0]<<"\t\t"<<turnaround_time<<"\t\t"<<waiting_time<<"\t\t"<<response_time[i]<<"\t\t"<<penaltyRatio<<"\t"<<endl;
    }
    cout<<"Results:\n";
    cout<<"--------\n";
    cout<<"Avg Response time = "<< (float)performance_vec[0]/process_list.size()<<"\n";
    cout<<"Avg Waiting time = "<< (float)performance_vec[1]/process_list.size()<<"\n";
    cout<<"Avg Turnaround time = "<< (float)performance_vec[2]/process_list.size()<<"\n";
    cout<<"Avg Penalty ratio = "<<(float)(avgPr)/(process_list.size())<<"\n";
    cout<<"System Throughput = "<<((float)process_list.size())/(time_count - 1)<<"\n";
    return 0;
}

void selection(vector<vector<int>> &prcq)
{
  vector<vector<int>> prv, nxt;
  int i=0;
  while(i<prcq.size()){
    if(prcq[i][1] <= time_count){
      prv.push_back(prcq[i]);
    }
    else{
      nxt.push_back(prcq[i]);
    }
    i++;
  }
  sort(prv.begin(), prv.end(), [](const vector<int>& v1, const vector<int>& v2) {
    return v1[2] < v2[2]; });
  sort(nxt.begin(), nxt.end(), [](const vector<int>& v1, const vector<int>& v2) {
    return v1[2] < v2[2]; });
  prcq.clear();
  copy(prv.begin(), prv.end(), back_inserter(prcq));
  copy(nxt.begin(), nxt.end(), back_inserter(prcq));
}