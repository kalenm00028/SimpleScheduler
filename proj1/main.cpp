#include <iostream>
#include <string>
#include <queue>
#include <list>

using namespace std;

const int freePoolMax = 10;
int freePool = freePoolMax;
int id = 0;

class job {

    public:
        int job_id;
        string job_description;
        int n_procs;
        int n_ticks;
        job(int id, string descrip, int procs, int ticks) {
            job_id = id;
            job_description = descrip;
            n_procs = procs;
            n_ticks = ticks;
        }

};

class scheduler {

    public:

        struct proc_comparator {
            bool operator()(job job1, job job2) {
                return job1.n_procs > job2.n_procs;
            }
        };

        struct tick_comparator {
            bool operator() (job job1, job job2) {
                return job1.n_ticks > job2.n_ticks;
            }
        };

        priority_queue<job, vector<job>, tick_comparator> waitQueue;
        list<job> runQueue;

        void newJob(job job) {
            if (job.n_procs < freePoolMax) {
                waitQueue.push(job);
                cout << "Adding job " << job.job_id << " to wait queue" << endl;
                cout << endl;
            }
            else {
                cout << "There are not enough available resources for this job" << endl;
                cout << endl;
            }
        }

        void moveToRunQueue() {
            if (waitQueue.top().n_procs < freePool+1) {
                runQueue.push_back(waitQueue.top());
                waitQueue.pop();
                freePool = freePool - waitQueue.top().n_procs;
                cout << "Moving job " << waitQueue.top().job_id << " to run queue" << endl;
                cout << endl;
            }
            else {
                cout << "Nothing moved to run queue" << endl;
                cout << endl;
            }
        }

        bool runQueueEmpty() {
            if (runQueue.empty()) {
                return true;
            }
            else {
                return false;
            }
        }

        void tick() {
            char newjob;
            jobinput:
            cout << "Would you like to enter a new job? (y/n)" << endl;
            cin >> newjob;
            while (newjob == 'y') {
                string user_descrip;
                int user_nprocs;
                int user_ticks;
                cout << "Job description? (no spaces)" << endl;
                cin >> user_descrip;
                cout << "Number of processors?" << endl;
                cin >> user_nprocs;
                cout << "Number of ticks?" << endl;
                cin >> user_ticks;
                cout << endl;
                newJob(job(id, user_descrip, user_nprocs, user_ticks));
                id++;
                goto jobinput;
            }
            cout << endl;
            cout << "TICK" << endl;
            cout << endl;
            cout << "Current run queue: " << endl;
            cout << "---------------------" << endl;
            if (!runQueue.empty()){
                for (list<job>::iterator it=runQueue.begin(); it!=runQueue.end(); it++) {
                    (*it).n_ticks = (*it).n_ticks-1;
                    if ((*it).n_ticks == 0) {
                        freePool = freePool + (*it).n_procs;
                        cout << "Job " << (*it).job_id << " finished" << endl;
                        it = runQueue.erase(it);
                    }
                    cout << "Job " << (*it).job_id << ": " << endl;
                    cout << "   Description: " << (*it).job_description << endl;
                    cout << "   Processors: " << (*it).n_procs << endl;
                    cout << "   Remaining ticks: " << (*it).n_ticks << endl;
                }
            }
            cout << "---------------------" << endl;
            cout << endl;
            if (!waitQueue.empty()) {
                moveToRunQueue();
            }
        }
};


int main() {

    scheduler newScheduler;

    newScheduler.tick();

    while (!newScheduler.runQueueEmpty()) {
        newScheduler.tick();
    }

    return 0;

}
