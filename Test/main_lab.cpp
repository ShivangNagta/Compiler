#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

// TOken : Value;




void Tokenizer(string &code, vector<pair<string, string>> &tokens){

    // string comment = "";
    bool comment = false;

    for (int i = 0; i < code.size(); ++i){
        char str = code[i];
        // cout << str << "\n";

        //Check for #
        // if (str == '\n') cout << "yes";

        if (code.substr(i, 3) == "def") tokens.push_back({"DEF","def"});

        if (comment == true){
            if (str == '\n'){
                str = '\0';
                comment = false;
                
            } 
            tokens[tokens.size() - 1].second.push_back(str);
        }

        if (str == '#'){
            comment = true;
            tokens.push_back({"COMMENT", "#"});
        }

        if ( str == '+' || str == '-' || str == '<' || str == '>' || str == '=' || str == '(' || str == ')' ){
            string s(1, str);
            tokens.push_back({"OPERATOR", s});
        }
        



    }
    
}




string read_file_contents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return "";
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    
    return buffer.str();
}

int main() {
    const string filename = "input_code.txt";
    string code = read_file_contents(filename);

    vector<pair<string, string>> tokens;

    Tokenizer(code, tokens);


    cout << tokens.size() << endl;

    for (int i = 0; i < tokens.size(); ++i){
          cout << tokens[i].first << " : "<< tokens[i].second << endl;
    } 
}











