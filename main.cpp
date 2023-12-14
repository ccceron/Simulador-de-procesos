#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdio>
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

string askForFileName(){
    string fileName;
    cout << "Write program file name (Without extension): ";
    cin >> fileName;
    return fileName;
}

string getFilePath(const string& txtName){
    string filePath = "../Programs/" + txtName + ".txt";
    return filePath;
}

int getEditModeAction(){
    int action = 0;
    system("cls");
    cout << endl << "*======================================================*" << endl;
    cout << "*       Edition mode menu (Program edition mode)       *" << endl;
    cout << "*            Select an option (Number only)            *" << endl;
    cout << "*======================================================*" << endl;
    cout << endl << "      1 > Print program file actions" << endl;
    cout << "      2 > Edit specific line" << endl;
    cout << "      3 > Add a line" << endl;
    cout << "      4 > Remove a line" << endl;
    cout << "      0 > Exit" << endl;
    cout << endl << "*======================================================*" << endl;
    cin >> action;
    if ((action == 1) or (action == 2) or (action == 3) or (action == 4)){
        return action;
    }
    return 0;
}

void printProgramFile(const string& filePath){
    ifstream inputFile;
    inputFile.open(filePath);
    if (inputFile.is_open()) {
        string line;

        system("cls");
        cout << endl;
        int i = 0;
        // Read and print each line in the file
        while (getline(inputFile, line)) {
            cout << i+1 << "> \t|"<< line << endl;
            i++;
        }

        // Close the file when done
        inputFile.close();
        cout << endl << "Program file printed successfully. \nContinue to stop watching." << endl;
    } else {
        cerr << "Error opening file: '" << filePath << "'." << endl;
    }

}

void editSpecificLine(const string& filename, int lineNumber, const string& newContent) {
    // Read the entire file into a vector of strings
    ifstream inputFile(getFilePath(filename));
    cout << "Filepath: " + getFilePath(filename) << endl;
    printProgramFile(getFilePath(filename));
    vector<string> lines;

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();
    } else {
        cerr << "Error opening file '" << filename << "'." << endl;
        return;
    }

    // Check if the line number is valid
    if (lineNumber <= 0 || lineNumber > static_cast<int>(lines.size())) {
        cerr << "Invalid line number." << endl;
        return;
    }

    // Edit the specific line
    lines[lineNumber - 1] = newContent;

    // Write the modified content back to the file
    ofstream outputFile(getFilePath(filename));
    if (outputFile.is_open()) {
        for (const auto& line : lines) {
            outputFile << line << endl;
        }
        outputFile.close();
        cout << "Line " << lineNumber << " successfully edited." << endl;
    } else {
        cerr << "Error opening file '" << filename << "' for writing." << endl;
    }
}

string askMemoryRef(){
    string memoryRef;
    cout << "Memory ref: ";
    cin >> memoryRef;
    return memoryRef;
}

int askMemoryValue(){
    int memoryValue;
    cout << "Memory value: ";
    cin >> memoryValue;
    return memoryValue;
}

string getNewContentLine(){
    string contentString;
    int instructionType;
    bool lineSelectedAndReady = false;
    string memoryRef;
    string memoryValue;
    while (!lineSelectedAndReady){
        system("cls");
        contentString = "";
        cout << endl << "*======================================================*" << endl;
        cout << "*              Quick program creation menu             *" << endl;
        cout << "*       (List of instructions for the simulator)       *" << endl;
        cout << "*            Select an option (Number only)            *" << endl;
        cout << "*======================================================*" << endl<< endl;
        cout << "\t1> SET\t\t2> LOAD\t\t3> ADDITION\n\t4> INCREMENT\t5> DECREMENT\t6> STORE\n\t7> SHOW\t\t8> PAUSE\t9> END" << endl << endl;
        cout << "\t\t\t0> Exit" << endl << endl;
        cout << "*======================================================*" << endl;
        cin >> instructionType;
        switch (instructionType) {
            case 0:
                break;
            case 1:
                memoryRef = askMemoryRef();
                memoryValue = to_string(askMemoryValue());
                contentString = "SET ";
                contentString += memoryRef;
                contentString += " " + memoryValue + " NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 2:
                memoryRef = askMemoryRef();
                contentString = "LDR ";
                contentString += memoryRef;
                contentString += " NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 3:
                memoryRef = askMemoryRef();
                contentString = "ADD ";
                contentString += memoryRef;
                contentString += " NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 4:
                memoryRef = askMemoryRef();
                contentString = "INC ";
                contentString += memoryRef;
                contentString += " NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 5:
                cout << "DEC";
                memoryRef = askMemoryRef();
                contentString = "DEC ";
                contentString += memoryRef;
                contentString += " NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 6:
                memoryRef = askMemoryRef();
                contentString = "STR ";
                contentString += memoryRef;
                contentString += " NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 7:
                memoryRef = askMemoryRef();
                contentString = "SHW ";
                contentString += memoryRef;
                contentString += " NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 8:
                contentString = "PAUSE NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            case 9:
                contentString = "END NULL NULL NULL";
                lineSelectedAndReady = true;
                break;
            default:
                cout << "Incorrect option selected, please try again." << endl;
                break;
        }
    }
    memoryRef = "";
    memoryValue = "";
    return contentString;
}

void addLineToFile(const string& filename, const string& lineToAdd) {
    // Open the file in append mode
    ofstream outputFile(getFilePath(filename), ios::app);

    if (outputFile.is_open()) {
        // Write the new line to the end of the file
        outputFile << lineToAdd << endl;
        outputFile.close();
        cout << "Line added to file '" << getFilePath(filename) << "'." << endl;
    } else {
        cerr << "Error opening file '" << getFilePath(filename) << "' for writing." << endl;
    }
}

void removeLineFromFile(const string& filename, int lineNumberToRemove) {
    // Read the entire file into a vector of strings
    ifstream inputFile(getFilePath(filename));
    vector<string> lines;

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();
    } else {
        cerr << "Error opening file '" << filename << "'." << endl;
        return;
    }

    // Check if the line number is valid
    if (lineNumberToRemove <= 0 || lineNumberToRemove > static_cast<int>(lines.size())) {
        cerr << "Invalid line number." << endl;
        return;
    }

    // Remove the specified line from the vector
    lines.erase(lines.begin() + lineNumberToRemove - 1);

    // Write the modified content back to the file
    ofstream outputFile(getFilePath(filename));
    if (outputFile.is_open()) {
        for (const auto& line : lines) {
            outputFile << line << endl;
        }
        outputFile.close();
        cout << "Line " << lineNumberToRemove << " successfully removed." << endl;
    } else {
        cerr << "Error opening file '" << filename << "' for writing." << endl;
    }
}

void editionMode(){
    string txt_file;
    bool execution = true;
    bool editingProgramMode = true;
    ifstream inputFile;
    ofstream outputFile;
    int lineNumber;
    string newContent;

    while(execution){
        int menu_option;
        system("cls");
        cout << endl << "*======================================================*" << endl;
        cout << "*       Edition mode menu (Program edition mode)       *" << endl;
        cout << "*            Select an option (Number only)            *" << endl;
        cout << "*======================================================*" << endl;
        if (!txt_file.empty()){
            cout << "           Currently editing: " << txt_file << endl;
            cout << "*======================================================*" << endl;
            cout << endl << "      1 > Load new program file" << endl;
            cout << "      2 > Create new program" << endl;
            cout << "      3 > Delete a program" << endl;
            cout << "      4 > Edit program file" << endl;
        } else {
            cout << endl << "      1 > Load program file" << endl;
            cout << "      2 > Create new program" << endl;
            cout << "      3 > Delete a program" << endl;
        }
        cout << "      0 > Exit" << endl;
        cout << endl << "*======================================================*" << endl;
        cout << "Option: ";
        cin >> menu_option;

        switch(menu_option){
            case 0:
                system("cls");
                cout << "Exiting program edition mode" << endl;
                execution = false;
                system("pause");
                system("cls");
                break;
            case 1:
                if(inputFile.is_open()){
                    cout << "Closing old file: " << txt_file << endl;
                    inputFile.close();
                }
                txt_file = askForFileName();
                inputFile.open(getFilePath(txt_file));
                if (!inputFile.is_open()) {
                    cerr << "Error opening file." << endl;
                    txt_file = "";
                    system("pause");
                    system("cls");
                    break;
                }
                break;
            case 2:
                txt_file = askForFileName();
                outputFile.open(getFilePath(txt_file));
                if (!outputFile.is_open()){
                    cerr << "Error creating program" << endl;
                    system("pause");
                    system("cls");
                    break;
                } else {
                    cout << "Successfully creating program if it does not exist." << endl;
                    outputFile.close();
                    system("pause");
                }
                break;
            case 3:
                txt_file = askForFileName();
                if (remove(getFilePath(txt_file).c_str()) == 0){
                    cout << "File '" << getFilePath(txt_file) << "' successfully deleted." << endl;
                    system("pause");
                } else {
                    cerr << "File not found" << endl;
                    system("pause");
                }
                txt_file = "";
                break;
            case 4:
                while(editingProgramMode){
                    if (!txt_file.empty()) {
                        int whatToEdit = getEditModeAction();
                        string filePath = getFilePath(txt_file);
                        // 1 > Print program file actions
                        // 2 > Edit specific line
                        // 3 > Add a line
                        // 4 > Remove a line
                        switch (whatToEdit) {
                            case 0:
                                cout << "Exiting the edition of the file: '" + filePath + "'." << endl;
                                editingProgramMode = false;
                                break;
                            case 1:
                                if (!txt_file.empty()){
                                    printProgramFile(getFilePath(txt_file));
                                } else {
                                    txt_file = askForFileName();
                                    printProgramFile(getFilePath(txt_file));
                                }
                                break;
                            case 2:
                                cout << "Enter line number to be edited: ";
                                cin >> lineNumber;
                                cout << "Enter new content" << endl;
                                newContent = getNewContentLine();
                                editSpecificLine(txt_file,lineNumber,newContent);
                                break;
                            case 3:
                                addLineToFile(txt_file,getNewContentLine());
                                break;
                            case 4:
                                cout << "What line you want to delete? (Number): ";
                                cin >> lineNumber;
                                removeLineFromFile(txt_file, lineNumber);
                                break;
                            default:
                                break;
                        }
                        system("pause");
                    }
                }
                break;
            default:
                break;
        }
    }
    // Just in case close (At this point the file should have been already closed)
    if(inputFile.is_open()){
        cout << "Check for closing the file before exiting the menu" << endl;
        outputFile.close();
        inputFile.close();
        system("pause");
        system("cls");
    }
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
    system("cls");
    cout << "Processes "<< " Burst time "
         << " Waiting time " << " Turn around time\n";
    // Calculate total waiting time and total turn
    // around time
    for (int i=0; i<n; i++)
    {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        cout << processes[i] << "\t\t" << bt[i] <<"\t\t" << wt[i] <<"\t\t " << tat[i] <<endl;
    }
    cout << "Average waiting time = " << (float)total_wt / (float)n;
    cout << "\nAverage turn around time = " << (float)total_tat / (float)n;
    cout << endl;
    system("pause");
    system("cls");
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
        cout << "*            Select an option (Number only)            *" << endl;
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
                editionMode();
                break;
            default:
                break;
        }
    }
    return 0;
};
