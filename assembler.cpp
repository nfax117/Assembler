#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <map>
#include <sstream>
using namespace std;

// 3 types: label, instruction, data
// label will always start with number or lowercase and end with ":"
// label will either have instruction or data contained in lines after until next label
// need to store label name with address in map in case it is called later
// instruction will always start with uppercase letter
// data will always start with .word

// struct should have: opcode, adressing mode, operand


// PSEUDOCODE
/*
address_counter = 0  //How to increment counter if it goes 009->00A

map<string, string> Instructions  
map<string, string> AddressingModes
map<string, int> Labels

while read each line:

    if line begins with label: // if (islower(str[0]) || isdigit(str[0]))

        
        label_str = the label before ":"
        Labels[label_str] = address_counter
        continue


    else if line begins with instruction:  // (isupper(str[0]))

        first_str = first argument  //which is the instruction

        if second argument in AddressingModes:  //there is an addressing mode (3 arguments)

            second_str = second argument
            third_str = third argument

            if third argument in Labels:  //third argument in label

                for (i=0; i<6-(Labels[third_str].length() + AddressingModes[second_str].length() + Instructions[first_str].length()); i++)
                    zero_str = zero_str + "0";

                output_str = zero_str + Instructions[first_str] + AddressingModes[second_str] + Labels[third_str]

            else if third argument starts with "$"  //third argument has $ in front


            else if third argument doesnt have symbol before  //third argument has no symbol

                for (i=0; i<6-(third_str.length() + AddressingModes[second_str].length() + Instructions[first_str].length()); i++)
                    zero_str = zero_str + "0";

                output_str = zero_str + Instructions[first_str] + AddressingModes[second_str] + third_str
            

        else if there isnt an addressing mode (aka direct):  //there isnt an addressing mode (2 arguments)

            second_str = operand argument

            if second_str in Labels:

            
            else if second_str begins with "$"


            else if second_str doesnt have symbol before

            for (i=0; i<5-(second_str.length() + Instructions[first_str].length()); i++)
                zero_str = zero_str + "0";

            output_str = zero_str + Instructions[first_str] + "2" + second_str

        UPDATE MEMORY ADDRESS IN CORRECT FORMAT!!! into address_counter


    else if line begins with .word: //  if (str[0] == '.')

        Add to memory address (address_counter) in correct format!!!

        temp_str = argument after ".word"

        for (i=0; i<6-temp_str.length(); i++)
            zero_str = zero_str + "0";
        
        output_str = zero_str + temp_str
        Update memory address by one unit into address_counter
        continue
*/

//need to make dictionaries for addressing mode types and instruction types

typedef map<string, string> Dict1;

void assembly(const char *input_path, const char *output_path)
{

    Dict1 Instructions;
    Instructions["LDA"] = "00";
    Instructions["LDX"] = "01";
    Instructions["LDS"] = "02";
    Instructions["LDF"] = "03";
    Instructions["STA"] = "04";
    Instructions["STX"] = "05";
    Instructions["STS"] = "06";
    Instructions["STF"] = "07";
    Instructions["PSH"] = "08";
    Instructions["POP"] = "09";
    Instructions["CLR"] = "0A";
    Instructions["SET"] = "0B";
    Instructions["PSHA"] = "FFF010";
    Instructions["PSHX"] = "FFF011";
    Instructions["PSHF"] = "FFF012";
    Instructions["POPA"] = "FFF013";
    Instructions["POPX"] = "FFF014";
    Instructions["POPF"] = "FFF015";
    Instructions["ADA"] = "10";
    Instructions["ADX"] = "11";
    Instructions["ADS"] = "12";
    Instructions["ADF"] = "13";
    Instructions["SBA"] = "14";
    Instructions["SBX"] = "15";
    Instructions["SBS"] = "16";
    Instructions["SBF"] = "17";
    Instructions["MUL"] = "18";
    Instructions["DIV"] = "19";
    Instructions["MOD"] = "1A";
    Instructions["CMA"] = "20";
    Instructions["CMX"] = "21";
    Instructions["CMS"] = "22";
    Instructions["CMF"] = "23";
    Instructions["SHRA"] = "FFF022";
    Instructions["SHLA"] = "FFF023";
    Instructions["JSR"] = "41";
    Instructions["RTN"] = "FFF040";
    Instructions["JEQ"] = "48";
    Instructions["JLT"] = "4A";
    Instructions["JGE"] = "4B";
    Instructions["NOP"] = "FFF038";
    Instructions["HLT"] = "FFFFFF";

    Dict1 AddressingModes;
    AddressingModes["#"] = "0";
    AddressingModes["+"] = "4";
    AddressingModes["*"] = "6";
    AddressingModes["&"] = "8";
    AddressingModes["# !"] = "1";
    AddressingModes["!"] = "3";
    AddressingModes["+ !"] = "5";
    AddressingModes["* !"] = "7";
    AddressingModes["& !"] = "9";

    Dict1 InstructionsSpec;
    InstructionsSpec["STA"] = "04";
    InstructionsSpec["STX"] = "05";
    InstructionsSpec["STS"] = "06";
    InstructionsSpec["STF"] = "07";
    InstructionsSpec["POP"] = "09";
    InstructionsSpec["CLR"] = "0A";
    InstructionsSpec["SET"] = "0B";
    InstructionsSpec["JSR"] = "41";
    InstructionsSpec["JEQ"] = "48";
    InstructionsSpec["JLT"] = "4A";
    InstructionsSpec["JGE"] = "4B";

    Dict1 InstructionsSpec2;
    InstructionsSpec2["PSHA"] = "FFF010";
    InstructionsSpec2["PSHX"] = "FFF011";
    InstructionsSpec2["PSHF"] = "FFF012";
    InstructionsSpec2["POPA"] = "FFF013";
    InstructionsSpec2["POPX"] = "FFF014";
    InstructionsSpec2["POPF"] = "FFF015";
    InstructionsSpec2["SHRA"] = "FFF022";
    InstructionsSpec2["SHLA"] = "FFF023";
    InstructionsSpec2["RTN"] = "FFF040";
    InstructionsSpec2["NOP"] = "FFF038";
    InstructionsSpec2["HLT"] = "FFFFFF";

    Dict1 Labels;

    ifstream input_txt(input_path);

    string line;

    string address = "000";
    int counter = 0;
    
    while (getline(input_txt, line))
    {

        if (islower(line[0]) || isdigit(line[0])){  //if the line is a label, need to put this on a separate while loop
            
            int str_length = line.length();
            string label_str = line;
            label_str.erase(str_length-1);
            //cout << "label: " << label_str << endl;

            if (Labels.count(label_str) == 1){

                cerr << "ASSEMBLER ERROR: label defined multiple times." << endl;
                return;

            }

            Labels[label_str] = address;
            continue;

        }

        else {  //increment address one unit

            counter = counter + 1;
            int num = counter;
            string remainder;
            string zero1_str = "";
            string key = "0123456789ABCDEF";
            while (num > 0){
                remainder = key[num % 16] + remainder;
                num = num/16;
            }
            int x = 3 - remainder.length();
            for (int i=0; i<x; i++){
                zero1_str = zero1_str + "0";
            }
            remainder = zero1_str + remainder;
            address = remainder;
            continue;

        }

    }
    input_txt.close();

    ifstream input_txt2(input_path);
    ofstream output_txt(output_path);

    while (getline(input_txt2, line))
    {
        string zero_str;
        string output_str;
        string first_str, second_str, third_str, fourth_str;

        //cout << "Line read:" << line << endl;

        if (islower(first_str[0]) || isdigit(first_str[0])){  //if the line is a label, need to put this on a separate while loop

            continue;

        }

        istringstream w(line);
        w >> first_str >> second_str >> third_str >> fourth_str;

        if (fourth_str != ""){  //if there are 4 arguments, adressing mode has 2, need to combine them

            second_str = second_str + " " + third_str;
            third_str = fourth_str;
        }

        if (isupper(first_str[0]))  //if the line is an instruction
        {
            //cout << "Line has instruction" << endl;

            if (second_str == ""){  //if there is only an instruction in the line

                //cout << "Only Instruction: " << first_str << endl;

                if (Instructions.count(first_str) == 1){  //the instruction is valid
                    output_str = Instructions[first_str];
                    output_txt << output_str << endl;
                    continue;
                }
                else {  //will never happen
                    cout << "Invalid Instruction" << endl;
                }
            }

            else if (AddressingModes.count(second_str) == 1){  //If the second argument is an addressing mode (therefore 3 arguments)

                //cout << "Line has instruction and addressing mode" << endl;

                if (InstructionsSpec.count(first_str) == 1 && AddressingModes[second_str] == "1"){  //if instruction uses unsupported addressing mode

                    cerr << "ASSEMBLER ERROR: instruction using unsupported addressing mode." << endl;
                    return;

                }

                if (InstructionsSpec.count(first_str) == 1 && AddressingModes[second_str] == "0"){  //if instruction uses unsupported addressing mode

                    cerr << "ASSEMBLER ERROR: instruction using unsupported addressing mode." << endl;
                    return;

                }

                if (InstructionsSpec2.count(first_str) == 1){

                    cerr << "ASSEMBLER ERROR: instruction using unsupported addressing mode." << endl;
                    return;

                }

                if (isdigit(third_str[0])){  //if last argument is just a number

                    int num = stoi(third_str);
                    string remainder;
                    string key = "0123456789ABCDEF";
                    while (num > 0){
                        remainder = key[num % 16] + remainder;
                        num = num/16;
                    }
                    int x = 6 - (remainder.length() + AddressingModes[second_str].length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + AddressingModes[second_str] + zero_str + remainder;
                    //cout << "Output: " << output_str << endl;
                    output_txt << output_str << endl;
                    continue;

                }

                if (isupper(third_str[0])){  //if last argument is just a letter (e.g. A-F)

                    int x = 6 - (third_str.length() + AddressingModes[second_str].length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + AddressingModes[second_str] + zero_str + third_str;
                    output_txt << output_str << endl;
                    continue;

                }

                if (third_str[0] == '$'){  //if the third argument begins with "$"

                    third_str.erase(0, 1);
                    int x = 6 - (third_str.length() + AddressingModes[second_str].length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + AddressingModes[second_str] + zero_str + third_str;
                    //cout << "Output: " << output_str << endl;
                    output_txt << output_str << endl;
                    continue;

                }

                if (Labels.count(third_str) == 1){  //if the third argument is a label

                    //cout << "Line has instruction, addressing mode, and label" << endl;

                    int x = 6 - (Labels[third_str].length() + AddressingModes[second_str].length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + AddressingModes[second_str] + zero_str + Labels[third_str];
                    //cout << "Output: " << output_str << endl;
                    output_txt << output_str << endl;
                    continue;

                }

                else {  //if the third argument is a label that has not been defined

                    cerr << "ASSEMBLER ERROR: undefined label." << endl;
                    return;
                    
                }

            }

            else if (third_str == ""){  //if there are only two arguments (direct addressing)

                //cout << "Only two arguments" << endl;

                if (isdigit(second_str[0])){  //if the last argument is just a number
                    
                    int num = stoi(second_str);
                    string remainder;
                    string key = "0123456789ABCDEF";
                    while (num > 0){
                        remainder = key[num % 16] + remainder;
                        num = num/16;
                    }
                    int x = 5 - (remainder.length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + "2" + zero_str + remainder;
                    //cout << "Output: " << output_str << endl;
                    output_txt << output_str << endl;
                    continue;

                }

                if (isupper(second_str[0])){  //if last argument is just a letter (e.g. A-F)

                    int x = 5 - (second_str.length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + "2" + zero_str + second_str;
                    output_txt << output_str << endl;
                    continue;

                }

                if (second_str[0] == '$'){  //if the second argument begins with "$"

                    second_str.erase(0, 1);
                    //cout << "Erased $: " << second_str << endl;
                    int x = 5 - (second_str.length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + "2" + zero_str + second_str;
                    //cout << "Output: " << output_str << endl;
                    output_txt << output_str << endl;
                    continue;

                }

                if (Labels.count(second_str) == 1){  //if the second argument is a label

                    //cout << "Only two arguments, second one is label" << endl;

                    int x = 5 - (Labels[second_str].length() + Instructions[first_str].length());
                    for (int i=0; i<x; i++){
                        zero_str = zero_str + "0";
                    }
                    output_str = Instructions[first_str] + "2" + zero_str + Labels[second_str];
                    //cout << "Output: " << output_str << endl;
                    output_txt << output_str << endl;
                    continue;

                }

                else {  //if the second argument has a label that has not been defined

                    cerr << "ASSEMBLER ERROR: undefined label." << endl;
                    return;
                    
                }

            }

            else {  //there is no second argument in AddressingModes map but still 3 arguments

                cout << "Error: potentially no addressing mode found" << endl;
                //cout << "First_str: " << first_str << "   " << "Second_str: " << second_str << "   " << "third_str: " << third_str << endl;
                continue;

            }

        }


        else if (first_str[0] == '.')  //if the line begins with .word
        {
            //cout << "Line is a variable" << endl;
            int x = 6-second_str.length();
            for (int i=0; i<x; i++){
                zero_str = zero_str + "0";
            }
            output_str = zero_str + second_str;
            //cout << "Output: " << output_str << endl;
            output_txt << output_str << endl;
            continue;

        }

    }

    input_txt2.close();
    output_txt.close();

}


int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        cout << "Please follow the correct format.\n";
        return 0;
    }
    assembly(argv[1], argv[2]);

}