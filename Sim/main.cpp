#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cstring>
#include <cstdlib>

using namespace std;

int memory_sim[] = {0,0,0,0,0,0,0,0};
int acc = 0;

struct position {
    string mem_reference;
    int mem_value;
};

vector<position>positions {
    {"D1", 1},
    {"D2", 2},
    {"D3", 3},
    {"D4", 4},
    {"D5", 5},
    {"D6", 6},
    {"D7", 7},
    {"D8", 8}
};

void debug(){
    cout << "Printing Memory: " << endl;
    for (int i = 0; i < 8; ++i) {
        cout << "D" << i+1 << ": " << memory_sim[i] << endl;
    }
    cout << "Printing ACC" << endl;
    cout << acc << endl;
};

int get_position_value(const string& position){
    int position_value = 0;
    for (int i = 0; i < 8; ++i) {
        if (position == positions[i].mem_reference){
            position_value = positions[i].mem_value;
            break;
        }
    }
    return position_value;
};

void clean_memory(){
    for (int & i : memory_sim) {
        i = 0;
    }
    acc = 0;
};

void set(const string& position, int value, const string& add_arg_two = "", const string& add_arg_three = "") {
    int position_value = get_position_value(position);
    memory_sim[position_value - 1] = value;
};

void ldr(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
    int position_value = get_position_value(position);
    acc = memory_sim[position_value-1];
};


void add(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = "") {
    int position_value = get_position_value(position);
    int result = acc;  // Use a temporary variable to accumulate the result

    if (!add_arg_two.empty()) {
        int add_arg_two_value = get_position_value(add_arg_two);
        result += memory_sim[add_arg_two_value - 1];
    } else if (!add_arg_three.empty()) {
        int add_arg_three_value = get_position_value(add_arg_three);
        result += memory_sim[add_arg_three_value - 1];
        if (!add_arg_four.empty()) {
            int add_arg_four_value = get_position_value(add_arg_four);
            memory_sim[add_arg_four_value - 1] = result;
        }
    } else {
        result += memory_sim[position_value - 1];
    }

    acc = result;  // Update acc at the end
};

void inc(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
    int position_value = get_position_value(position);
    memory_sim[position_value-1] = memory_sim[position_value-1] + 1;
};

void dec(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
    int position_value = get_position_value(position);
    memory_sim[position_value-1] = memory_sim[position_value-1] - 1;
};

void str(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = "") {
    int position_value = get_position_value(position);
    memory_sim[position_value - 1] = acc;
};

void shw(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = "") {
    if (position == "D1" || position == "D2" || position == "D3" || position == "D4" || position == "D5" || position == "D6" || position == "D7" || position == "D8") {
        int position_value = get_position_value(position);
        cout << "Show: " << memory_sim[position_value - 1] << endl;
    } else if (position == "ACC") {
        cout << "ACC: " << acc << endl;
    }
};


void pause(const string& add_arg_one , const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
}

void end(const string& add_arg_one , const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
    clean_memory();
}


// Function to process a generic command with variable arguments
void process_command(const string& command, const string& arg1, const string& arg2, const string& arg3, const string& arg4) {
    if (command == "SET") {
        set(arg1, stoi(arg2), arg3, arg4);
    } else if (command == "LDR") {
        ldr(arg1, arg2, arg3, arg4);
    } else if (command == "ADD") {
        add(arg1, arg2, arg3, arg4);
    } else if (command == "INC") {
        inc(arg1, arg2, arg3, arg4);
    } else if (command == "DEC") {
        dec(arg1, arg2, arg3, arg4);
    } else if (command == "STR") {
        str(arg1, arg2, arg3, arg4);
    } else if (command == "SHW") {
        shw(arg1, arg2, arg3, arg4);
    } else if (command == "PAUSE") {
        pause(arg1, arg2, arg3, arg4);
    } else if (command == "END") {
        end(arg1, arg2, arg3, arg4);
    } else {
        cerr << "Unknown command: " << command << endl;
    }
};

void load_program(const string& txt_file){
    string file_path = "../Programs/" + txt_file + ".txt";
    ifstream inputFile(file_path);

    if (!inputFile.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string command, arg1, arg2, arg3, arg4;

        if (iss >> command >> arg1 >> arg2 >> arg3) {
            // Handle "NULL" values by reading them and doing nothing
            if (arg1 == "NULL") arg1 = "";
            if (arg2 == "NULL") arg2 = "";
            if (arg3 == "NULL") arg3 = "";

            // Call the appropriate function based on the command
            process_command(command, arg1, arg2, arg3, "");
        } else {
            cerr << "Error parsing line: " << line << endl;
        }
    }

    inputFile.close();
};

int countLines(const string& filename) {
    string file_path = "../Programs/" + filename + ".txt";
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << ".txt" << std::endl;
        return -1; // Return -1 to indicate an error
    }

    int lineCount = 0;
    string line;
    while (getline(file, line)) {
        ++lineCount;
    }

    file.close();
    return lineCount;
}

/**
 * Start of the (SJF) SHORTEST JOB FIRST Scheduling Algorithm
 *
 */

//Implementation fo SHORTEST JOB FIRST(Preemptive) Using C++


typedef  struct proccess
{
    int at,bt,ct,ta,wt,btt;
    string pro_id;

}Schedule;

bool compare(Schedule a,Schedule b)
{
    return a.at<b.at;

    /* This Schedule will always return TRUE
    if above condition comes*/
}
bool compare2(Schedule a,Schedule b)
{
    return a.bt<b.bt;

    /* This Schedule will always return TRUE
    if above condition comes*/
}

void shortestJobFirst(int processes, const int burstTimes[], string processNames[])
{
    Schedule pro[20];
    //An array of Processes
    int n,i,j,pcom;
    //n = number of processes, i= iteration variable

    n = processes;

    for(i=0;i<n;i++)
    {
        pro[i].pro_id = processNames[i]; // Process ID
        pro[i].at = i; // Process Arrival Time
        pro[i].bt = burstTimes[i]; // Process Burst Time
        pro[i].btt=pro[i].bt;
    }

    sort(pro,pro+n,compare);

    /*sort is a predefined funcion  defined in algorithm.h header file,
    it will sort the processes according to their arrival time*/

    i=0;
    pcom=0;
    while(pcom<n)
    {
        for(j=0;j<n;j++)
        {
            if(pro[j].at>i)
                break;
        }

        sort(pro,pro+j,compare2);

        /*sort is a predefined funcion  defined in algorithm.h header file,
    it will sort the processes according to their burst time*/

        if(j>0)
        {

            for(j=0;j<n;j++)
            {
                if(pro[j].bt!=0)
                    break;
            }
            if(pro[j].at>i)

            {
                i=pro[j].at;

            }
            pro[j].ct=i+1;
            pro[j].bt--;
        }
        i++;
        pcom=0;
        for(j=0;j<n;j++)
        {
            if(pro[j].bt==0)
                pcom++;
        }
    }

    system("cls");
    cout<<"ProID\tAtime\tBtime\tCtime\tTtime\tWtime\n";

    for(i=0;i<n;i++)
    {
        pro[i].ta=pro[i].ct-pro[i].at;
        pro[i].wt=pro[i].ta-pro[i].btt;

        /*Printing the Process id, arrival time, burst time,
        completion time, turn around time, waiting time*/

        cout<<pro[i].pro_id<<"\t"<<pro[i].at<<"\t"<<pro[i].btt<<"\t"<<pro[i].ct<<"\t"<<pro[i].ta<<"\t"<<pro[i].wt;
        cout<<endl;
    }
    system("pause");
    system("cls");
}


/**
 * End of the (SJF) SHORTEST JOB FIRST Scheduling Algorithm
 */

/**
 * Start of the (FCFS) FIRST COME FIRST SERVE Scheduling Algorithm
 *
 */

void firstComeFirstServe(int processes, const int burstTimes[], string processNames[])
{   int n,bt[20],wt[20],tat[20],avwt=0,avtat=0,i,j;
    n = processes;

    for (int k = 0; k < processes; ++k) {
        bt[k] = burstTimes[k];
    }

    wt[0]=0;    //waiting time for first process is 0
    //calculating waiting time
    for(i=1;i<n;i++)
    {
        wt[i]=0;
        for(j=0;j<i;j++)
            wt[i]+=bt[j];
    }
    system("cls");
    cout<<"\nProcess\t\tBurst Time\tWaiting Time\tTurnaround Time";
    //calculating turnaround time
    for(i=0;i<n;i++)
    {
        tat[i]=bt[i]+wt[i];
        avwt+=wt[i];
        avtat+=tat[i];
        cout<<"\n"<<processNames[i]<<"\t\t"<<bt[i]<<"\t\t"<<wt[i]<<"\t\t"<<tat[i];
    }
    avwt/=i;
    avtat/=i;
    cout<<"\n\nAverage Waiting Time: "<<avwt;
    cout<<"\nAverage Turnaround Time: "<<avtat<<"\n";
    system("pause");
    system("cls");
}

/**
 * End of the (FCFS) FIRST COME FIRST SERVE Scheduling Algorithm
 */

/**
 * Start of the (RR) ROUND ROBIN Scheduling Algorithm
 */

// Function to find the waiting time for all
// processes
void findWaitingTime(string processes[], int n,
                     int bt[], int wt[], int quantum)
{
    // Make a copy of burst times bt[] to store remaining
    // burst times.
    int rem_bt[n];
    for (int i = 0 ; i < n ; i++)
        rem_bt[i] = bt[i];
    int t = 0; // Current time
    // Keep traversing processes in round robin manner
    // until all of them are not done.
    while (true)
    {
        bool done = true;
        // Traverse all processes one by one repeatedly
        for (int i = 0 ; i < n; i++)
        {
            // If burst time of a process is greater than 0
            // then only need to process further
            if (rem_bt[i] > 0)
            {
                done = false; // There is a pending process
                if (rem_bt[i] > quantum)
                {
                    // Increase the value of t i.e. shows
                    // how much time a process has been processed
                    t += quantum;
                    // Decrease the burst_time of current process
                    // by quantum
                    rem_bt[i] -= quantum;
                }
                    // If burst time is smaller than or equal to
                    // quantum. Last cycle for this process
                else
                {
                    // Increase the value of t i.e. shows
                    // how much time a process has been processed
                    t = t + rem_bt[i];
                    // Waiting time is current time minus time
                    // used by this process
                    wt[i] = t - bt[i];
                    // As the process gets fully executed
                    // make its remaining burst time = 0
                    rem_bt[i] = 0;
                }
            }
        }
        // If all processes are done
        if (done)
            break;
    }
}
// Function to calculate turn around time
void findTurnAroundTime(string processes[], int n,
                        int bt[], int wt[], int tat[])
{
    // calculating turnaround time by adding
    // bt[i] + wt[i]
    for (int i = 0; i < n ; i++)
        tat[i] = bt[i] + wt[i];
}
// Function to calculate average time
void findavgTime(string processes[], int n, int bt[],
                 int quantum)
{
    int wt[n], tat[n], total_wt = 0, total_tat = 0;
    // Function to find waiting time of all processes
    findWaitingTime(processes, n, bt, wt, quantum);
    // Function to find turn around time for all processes
    findTurnAroundTime(processes, n, bt, wt, tat);
    // Display processes along with all details
    cout << "Processes "<< " Burst time "
         << " Waiting time " << " Turn around time\n";
    // Calculate total waiting time and total turn
    // around time
    for (int i=0; i<n; i++)
    {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        cout << " " << i+1 << "\t\t" << bt[i] <<"\t "
             << wt[i] <<"\t\t " << tat[i] <<endl;
    }
    cout << "Average waiting time = "
         << (float)total_wt / (float)n;
    cout << "\nAverage turn around time = "
         << (float)total_tat / (float)n;
}
void roundRobin(int processes, const int burstTimes[], string processNames[])
{
    int burst_time[20];
    string processesNames[20];

    // process id's
    for (int i = 0; i < processes; ++i) {
        processesNames[i] = processNames[i];
    }
    int n = processes;

    // Burst time of all processes
    for (int i = 0; i < processes; ++i) {
        burst_time[i] = burstTimes[i];
    }

    // Time quantum
    int quantum = 2;
    findavgTime(processesNames, n, burst_time, quantum);
}

/**
 * End of the (RR) ROUND ROBIN Scheduling Algorithm
 */

void algorithmMenu(){
    int menu_option;
    string processNames[20];
    int processBurstTime[20];
    int processes;

    cout << endl << "*======================================================*" << endl << endl;
    cout << "      1 > First Come First Serve (FCFS)" << endl;
    cout << "      2 > Shortest job first (SJF)" << endl;
    cout << "      3 > Round Robin (RR)" << endl;
    cout << "      5 > Multilevel Queue (MLQ) [IN PROGRESS]" << endl;
    cout << "      6 > Multi Level Feedback Queue (MLFQ) [IN PROGRESS]" << endl;
    cout << "      0 > Go back" << endl;
    cout << endl << "*======================================================*" << endl;
    cout << "Option: ";
    cin >> menu_option;

    switch(menu_option){
        case 0:
            cout << "Going back to the main menu" << endl;
            break;
        case 1:
            cout << "How many processes are going to be executed? (20 Max): ";
            cin >> processes;
            for (int i = 0; i < processes; ++i) {
                cout << "Process #"<< i << " name: ";
                cin >> processNames[i];
                // Simulating the bursttime or execution time by how many lines there are to be executed in a program
                processBurstTime[i] = countLines(processNames[i]);
            }
            firstComeFirstServe(processes, processBurstTime, processNames);
            break;
        case 2:
            cout << "How many processes are going to be executed? (20 Max): ";
            cin >> processes;
            for (int i = 0; i < processes; ++i) {
                cout << "Process #"<< i << " name: ";
                cin >> processNames[i];
                // Simulating the bursttime or execution time by how many lines there are to be executed in a program
                processBurstTime[i] = countLines(processNames[i]);
            }
            shortestJobFirst(processes, processBurstTime, processNames);
            break;
        case 3:
            cout << "How many processes are going to be executed? (20 Max): ";
            cin >> processes;
            for (int i = 0; i < processes; ++i) {
                cout << "Process #"<< i << " name: ";
                cin >> processNames[i];
                // Simulating the bursttime or execution time by how many lines there are to be executed in a program
                processBurstTime[i] = countLines(processNames[i]);
            }
            roundRobin(processes, processBurstTime, processNames);
            break;
        default:
            break;
    }
}

int main() {
    string txt_file;
    bool execution = true;
    while(execution){
        int menu_option;
        cout << endl << "*======================================================*" << endl;
        cout << "*           Select and option (Number only)            *" << endl;
        cout << "*======================================================*" << endl;
        cout << endl << "      1 > Load program file" << endl;
        cout << "      2 > Enter algorithm test menu" << endl;
        cout << "      3 > Program edition mode" << endl;
        cout << "      0 > Exit" << endl;
        cout << endl << "*======================================================*" << endl;
        cout << "Option: ";
        cin >> menu_option;

        switch(menu_option){
            case 0:
                cout << "Exiting program" << endl;
                execution = false;
                break;
            case 1:
                cout << "Enter the file name: ";
                cin >> txt_file;
                load_program(txt_file);
                // cout << "Lines: " << countLines(txt_file) << endl;
                break;
            case 2:
                algorithmMenu();
                break;
            case 3:
                cout << "[COMING]";
                break;
            default:
                break;
        }
    }
    return 0;
};
