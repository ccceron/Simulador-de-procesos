#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

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
}

int get_position_value(const string& position){
    int position_value = 0;
    for (int i = 0; i < 8; ++i) {
        if (position == positions[i].mem_reference){
            position_value = positions[i].mem_value;
            break;
        }
    }
    return position_value;
}

void clean_memory(){
    for (int & i : memory_sim) {
        i = 0;
    }
    acc = 0;
};

void set(const string& position, int value, const string& add_arg_two = "", const string& add_arg_three = "") {
    int position_value = get_position_value(position);
    memory_sim[position_value - 1] = value;
}

void ldr(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
    int position_value = get_position_value(position);
    acc = memory_sim[position_value-1];
}


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
}

void inc(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
    int position_value = get_position_value(position);
    memory_sim[position_value-1] = memory_sim[position_value-1] + 1;
}

void dec(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = ""){
    int position_value = get_position_value(position);
    memory_sim[position_value-1] = memory_sim[position_value-1] - 1;
}

void str(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = "") {
    int position_value = get_position_value(position);
    memory_sim[position_value - 1] = acc;
}

void shw(const string& position, const string& add_arg_two = "", const string& add_arg_three = "", const string& add_arg_four = "") {
    if (position == "D1" || position == "D2" || position == "D3" || position == "D4" || position == "D5" || position == "D6" || position == "D7" || position == "D8") {
        int position_value = get_position_value(position);
        cout << "Show: " << memory_sim[position_value - 1] << endl;
    } else if (position == "ACC") {
        cout << "ACC: " << acc << endl;
    }
}


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
}

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
        cout << "      0 > Exit" << endl;
        cout << endl << "*======================================================*" << endl;
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
                break;

            default:
                break;
        }
    }
    return 0;
}
