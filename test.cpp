#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class todoList {
private:
    fstream myFile;
    string fileName;
    vector<string> lines;

public:
    todoList(string name) : fileName(name) {
        myFile.open(name, ios::app);
        if (!myFile) {
            cout << "File cannot be opened: " << fileName << endl;
        }
        else {
            cout << "File opened successfully" << endl;
            load_lines();
        }
    }

    void load_lines() {
        ifstream read_file(fileName);
        if (!read_file) {
            cout << "Error opening file" << endl;
            return;
        }
        lines.clear();
        string line;
        while (getline(read_file, line)) {
            lines.push_back(line);
        }
        read_file.close();
    }

    void write(string text) {
        if (myFile.is_open()) {
            string act = text + " - undone";
            myFile << act << endl;
            lines.push_back(act);
            cout << "Data wrote correctly" << endl;
        }
        else {
            cout << "File isn't open" << endl;
        }
    }

    string read() {
        load_lines();
        string content;
        int line_number = 1;
        for (const auto& line : lines) {
            content += to_string(line_number++) + ". " + line + "\n";
        }
        return content;
    }

    void deleteActivity() {
        load_lines();
        int line_number;
        cout << "Line to delete: ";
        cin >> line_number;

        if (line_number > lines.size() || line_number <= 0) {
            cout << "Invalid line number" << endl;
            return;
        }

        ofstream write_file(fileName);
        if (write_file.fail()) {
            cout << "Error opening file" << endl;
            return;
        }

        line_number--;
        lines.erase(lines.begin() + line_number);

        for (const auto& line : lines) {
            write_file << line << endl;
        }

        write_file.close();
        cout << "Line deleted successfully" << endl;
    }

    void change_activity() {
        load_lines();
        int line_number;
        cout << "Line to change: ";
        cin >> line_number;

        if (line_number > lines.size() || line_number <= 0) {
            cout << "Invalid line number" << endl;
            return;
        }

        string changed_line;
        cout << "Change text to: ";
        cin.ignore();
        getline(cin, changed_line);

        line_number--;
        size_t pos = lines[line_number].find(" - ");
        if (pos != string::npos) {
            lines[line_number] = changed_line + lines[line_number].substr(pos);
        }
        else {
            lines[line_number] = changed_line + " - undone";
        }

        ofstream write_file(fileName);
        if (write_file.fail()) {
            cout << "Error opening file" << endl;
            return;
        }

        for (const auto& line : lines) {
            write_file << line << endl;
        }

        write_file.close();
        cout << "Line changed successfully" << endl;
    }

    void change_to_done() {
        load_lines();
        cout << "Which activity do you want to mark as done? ";
        int line_done;
        cin >> line_done;
        if (line_done > lines.size() || line_done <= 0) {
            cout << "Invalid line number" << endl;
            return;
        }

        line_done--;
        size_t pos = lines[line_done].find(" - undone");
        if (pos != string::npos) {
            lines[line_done].replace(pos, 9, " - done");
        }
        else {
            cout << "This activity is already marked as done." << endl;
            return;
        }

        ofstream write_file(fileName);
        if (write_file.fail()) {
            cout << "Error opening file" << endl;
            return;
        }

        for (const auto& line : lines) {
            write_file << line << endl;
        }

        write_file.close();
        cout << "Activity marked as done successfully" << endl;
    }

    ~todoList() {
        if (myFile.is_open()) {
            myFile.close();
        }
    }
};

int todoListApp();

int main() {
    todoListApp();
    return 0;
}

int todoListApp() {
    cout << "It's your todo list app" << endl;
    todoList list("todolist.txt");
    int option;
    string activity;
    while (true) {
        cout << "\nSelect your option: \n 1. Quit \n 2. Add your activity \n 3. Read your activities\n 4. Delete activity\n 5. Change activity\n 6. Mark activity as done" << endl;
        cin >> option;
        cout << " " << endl;
        switch (option) {
        case 1:
            return 0;
        case 2:
            cout << "Write thing to do: ";
            cin.ignore();
            getline(cin, activity);
            list.write(activity);
            break;
        case 3:
            cout << list.read();
            break;
        case 4:
            list.deleteActivity();
            break;
        case 5:
            list.change_activity();
            break;
        case 6:
            list.change_to_done();
            break;
        default:
            cout << "Invalid option" << endl;
            break;
        }
    }
}
