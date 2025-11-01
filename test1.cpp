#include<iostream> 
#include<string> 
#include<vector> // to use vetor container for storing students
#include<fstream> // For read/write data in file;
#include<sstream> // Read and Analyzing data and converting it into a useful format
#include<algorithm>
#include<iomanip>

using namespace std;

const string SUBJECTS[5] = { "OOPS", "DSA", "Python", "Maths", "QAPD-1"};

struct Student
{
    int roll;
    string name;
    int marks[5];
    double percentage;
    int total;
    char grade;

    Student() : roll(0), name(""), total(0), percentage(0.0), grade('F'){
        for(int i=0; i<5; ++i){
            marks[i]=0;
        }
    }
};

const string DATA_FILE = "students.txt";

double CalculateResult(Student &s) {
    int total = 0;
    for (int i = 0; i < 5; i++)
        total += s.marks[i];
    s.total = total;
    s.percentage = total/5.0;
    if (s.percentage >= 90) s.grade = 'A';
    else if (s.percentage >= 75) s.grade = 'B';
    else if (s.percentage >= 60) s.grade = 'C';
    else if (s.percentage >= 40) s.grade = 'D';
    else s.grade = 'F';
}
void displayStudent(const Student &s) {
    cout << "Roll No: " << s.roll << endl;
    cout << "Name   : " << s.name << endl;
    cout << "Marks  : ";
    for (int i = 0; i < 5; ++i){
        cout << SUBJECTS[i] << "=" << s.marks[i] << "  ";
    }
    cout << "Total  : " << s.total << endl;
    cout << fixed << setprecision(2);
    cout << "Percent: " << s.percentage << "%" << endl;
    cout << "Grade  : " << s.grade << endl << endl;
    cout.unsetf(ios::fixed);
}

// Find student by roll number
vector<Student>::iterator findStudent(vector<Student> &students, int roll) {
    return find_if(students.begin(), students.end(), [roll](const Student &s){ return s.roll == roll; });
}

// Save students to file
void saveData(const vector<Student> &students) {
    ofstream fout(DATA_FILE);
    if (!fout.is_open()) {
        cout << "File not open.\n";
        return;
    }
    for (const auto &s : students) {
        fout << s.roll << "| " << s.name << "| ";
        for (int i = 0; i < 5; ++i) fout << s.marks[i] << " ";
        fout << endl;
    }
    fout.close();
}

// Load students from file
void loadData(vector<Student> &students) {
    students.clear();
    ifstream fin(DATA_FILE);
    if (!fin.is_open()) return;

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Student s;
        ss >> s.roll;
        ss.ignore();
        getline(ss, s.name, '|');
        for (int i = 0; i < 5; ++i) ss >> s.marks[i];
        CalculateResult(s);
        students.push_back(s);
    }
    fin.close();
}

void TeacherMenu(vector<Student> &students){
    string password;
    cout<<"Enter password: ";
    cin>>password;
    if(password != "teacher@123"){
        cout<<"Wrong Password";
        return;
    }
    while (true){
        cout<<"\n---Teacher Menu---\n";
        cout<<"1. Add Student\n2. Update Student\n3. List all Student\n";
        cout<<"4. Search Student\n5. Save changes\n6. Return to Main Menu\noption: ";
        int opt;
        cin>>opt;

        if(opt==1){
            char choice = 'Y';
            while (choice == 'y'||choice == 'Y'){
                Student s;
                cout<<"Enter Roll no: ";
                cin>>s.roll;
                cin.ignore();
                cout<<"Enter Name: ";
                getline(cin,s.name);
                cout<<"Enter marks of subjects:\n ";
                for(int i=0;i<5;i++){
                    cout<<SUBJECTS[i]<<": ";
                    cin>>s.marks[i];
                }
                CalculateResult(s);
                students.push_back(s);
                cout<<"want to add another student? (y/n): ";
                cin>>choice;
            }
        }
        else if(opt==2){
            int roll;
            cout<<"Enter roll no: ";
            cin>>roll;
            auto it = findStudent(students,roll);
            if(it == students.end()){
                cout<<"Student not in list.";
            }
            else{
                cin.ignore();
                cout<<"Enter new Name: ";
                getline(cin, it->name);
                cout<<"Enter marks:\n ";
                for(int i=0; i<5; i++){
                    cout<<SUBJECTS[i]<<": ";
                    cin>>it->marks[i];
                }
            }
            CalculateResult(*it);
            cout<<"Update Complete!\n";
        }
        else if(opt==3){
            cout << left << setw(8) << "Roll" << setw(20) << "Name" 
                << setw(8) << "OOPS" << setw(8) << "DSA" 
                << setw(8) << "Python" << setw(8) << "Maths"
                << setw(10) << "QAPD-1" << setw(10) << "Percent" 
                << setw(8) << "Grade" << '\n';
            cout << string(88, '-') << '\n';

            for (const auto &s : students) {
                cout << left << setw(8) << s.roll << setw(20) << s.name
                    << setw(8) << s.marks[0] << setw(8) << s.marks[1] 
                    << setw(8) << s.marks[2] << setw(8) << s.marks[3] 
                    << setw(10) << s.marks[4] << setw(10) << fixed << setprecision(2) << s.percentage
                    << setw(8) << s.grade << '\n';
            }

        }
    else if(opt==4){
        int roll;
        cout<<"Enter roll no: ";
        cin >> roll;
        auto it = findStudent(students, roll);
        if(it==students.end()){
            cout<<"Student not in the list";
        }
        else{
            displayStudent(*it);
        }
    }
    else if(opt==5){
        saveData(students);
        cout<<"Data Saved";
        break;
    }
    else if(opt==6){
        break;
    }
    else{
        cout<<"Error";
    }
    }    
}
void StudentMenu(const vector <Student> & students){
    int roll;
    cout<<"Enter your roll no: ";
    cin>>roll;
    auto it = find_if(students.begin(), students.end(),[roll](const Student &s){return s.roll == roll;});
    if (it==students.end()){
        cout<<"Error\n";
    }
    else{
        displayStudent(*it);
    }
}

int main(){
    vector<Student>students;
    loadData(students);

    while(true){
        cout<<"\n---Student Result Management---\n";
        cout<<"1. Teacher\n";
        cout<<"2. Student\n";
        cout<<"3. Exit\n";
        cout<<"option: ";
        int role;
        cin>>role;
        if(role==1){
            TeacherMenu(students);
        }
        else if(role==2){
            StudentMenu(students);
        }
        else if(role==3){
            break;
        }
        else{
            cout<<"Error";
        }
    }
    return 0;
}