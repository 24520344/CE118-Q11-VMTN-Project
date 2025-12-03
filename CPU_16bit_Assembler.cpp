#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm> 
#include <bitset>
#include <fstream>   
#include <iomanip>

using namespace std;

// --- 1. Cấu trúc dữ liệu ---
enum InstType { RRR, RRI, RI };

struct Instruction {
    string name;
    InstType type;
    string opcode; // 3 bit
    string funct;  // 4 bit (RRR)

    Instruction() : name(""), type(RRR), opcode(""), funct("") {}
    Instruction(string n, InstType t, string o, string f) 
        : name(n), type(t), opcode(o), funct(f) {}
};

map<string, string> registers;
map<string, Instruction> instructionSet;

// --- 2. Hàm khởi tạo dữ liệu ---
void initInstructionSet() {
    registers["R0"] = "000"; registers["R1"] = "001";
    registers["R2"] = "010"; registers["R3"] = "011";
    registers["R4"] = "100"; registers["R5"] = "101";
    registers["R6"] = "110"; registers["R7"] = "111";

    // --- RRR (Op: 000) ---
    instructionSet["ADD"]  = Instruction("ADD",  RRR, "000", "0000");
    instructionSet["INC"]  = Instruction("INC",  RRR, "000", "0001");
    instructionSet["SUB"]  = Instruction("SUB",  RRR, "000", "0010");
    instructionSet["DEC"]  = Instruction("DEC",  RRR, "000", "0011");
    instructionSet["AND"]  = Instruction("AND",  RRR, "000", "0100");
    instructionSet["OR"]   = Instruction("OR",   RRR, "000", "0101");
    instructionSet["NAND"] = Instruction("NAND", RRR, "000", "0110");
    instructionSet["XOR"]  = Instruction("XOR",  RRR, "000", "0111");
    instructionSet["SHFL"] = Instruction("SHFL", RRR, "000", "1000");
    instructionSet["SHFR"] = Instruction("SHFR", RRR, "000", "1001");
    instructionSet["SLL"]  = Instruction("SLL",  RRR, "000", "1010");
    instructionSet["SRR"]  = Instruction("SRR",  RRR, "000", "1011");
    instructionSet["SRA"]  = Instruction("SRA",  RRR, "000", "1100");
    instructionSet["MUL"]  = Instruction("MUL",  RRR, "000", "1101");
    instructionSet["SLT"]  = Instruction("SLT",  RRR, "000", "1110");
    instructionSet["SGT"]  = Instruction("SGT",  RRR, "000", "1111");

    // --- RRI (Op: 001, 010, 011) ---
    instructionSet["ADDI"] = Instruction("ADDI", RRI, "001", "");
    instructionSet["SUBI"] = Instruction("SUBI", RRI, "010", "");
    instructionSet["BEQ"]  = Instruction("BEQ",  RRI, "011", "");

    // --- RI (Op: 100 - 111) ---
    instructionSet["LW"]   = Instruction("LW",   RI,  "100", "");
    instructionSet["SW"]   = Instruction("SW",   RI,  "101", "");
    instructionSet["JNZ"]  = Instruction("JNZ",  RI,  "110", "");
    instructionSet["LI"]   = Instruction("LI",   RI,  "111", "");
}

// --- 3. Các hàm tiện ích ---

string toUpperStr(string str) {
    string res = str;
    for (size_t i = 0; i < res.length(); ++i) res[i] = toupper(res[i]);
    return res;
}

string cleanString(string s) {
    replace(s.begin(), s.end(), ',', ' ');
    return s;
}

string intToBinary(int number, int bits) {
    if (bits == 7) return bitset<7>(number).to_string();
    if (bits == 10) return bitset<10>(number).to_string();
    return "";
}

string binaryToHex(string binStr) {
    if (binStr.length() != 16) return "ERROR";
    unsigned long val = stoul(binStr, nullptr, 2);
    stringstream ss;
    ss << hex << uppercase << setfill('0') << setw(4) << val;
    return ss.str();
}

// --- 4. HÀM XỬ LÝ MÃ GIẢ (PRE-PROCESSING) ---
// Hàm này sẽ biến đổi lệnh giả thành lệnh thật trước khi xử lý
string preprocessPseudo(string line) {
    string upperLine = toUpperStr(line);
    stringstream ss(cleanString(upperLine));
    string op;
    ss >> op;

    vector<string> args;
    string temp;
    while (ss >> temp) args.push_back(temp);

    // 1. NOT Rd, Rs -> NAND Rd, Rs, Rs
    if (op == "NOT" && args.size() == 2) {
        return "NAND " + args[0] + ", " + args[1] + ", " + args[1];
    }

    // 2. MOV Rd, Rs -> AND Rd, Rs, Rs
    if (op == "MOV" && args.size() == 2) {
        return "AND " + args[0] + ", " + args[1] + ", " + args[1];
    }

    // 3. NOP -> ADDI R1, R1, 0
    if (op == "NOP") {
        return "ADDI R1, R1, 0";
    }

    // 4. JMP Imm -> BEQ R0, R0, Imm
    if (op == "JMP" && args.size() == 1) {
        return "BEQ R0, R0, " + args[0];
    }

    // Nếu không phải mã giả, trả về dòng gốc
    return line; 
}

// --- 5. Hàm xử lý logic chính ---
string processInstruction(string line) {
    if (line.empty()) return "";
    
    // Bước 1: Chuẩn hóa
    string upperLine = toUpperStr(line);
    stringstream ss(cleanString(upperLine));
    
    string opcodeName;
    ss >> opcodeName;

    if (instructionSet.find(opcodeName) == instructionSet.end()) return "Error: Unknown Opcode";

    Instruction inst = instructionSet[opcodeName];
    vector<string> args;
    string temp;
    while (ss >> temp) args.push_back(temp);

    string binaryOutput = "";

    // Xử lý RRR: Op(3) Rs(3) Rt(3) Rd(3) Funct(4)
    if (inst.type == RRR) {
        string rd = "000", rs = "000", rt = "000";
        // 2 tham số (INC Rd, Rs) -> Rd=Arg0, Rs=Arg1, Rt=0
        if (args.size() == 2) {
            if (registers.count(args[0])) rd = registers[args[0]];
            if (registers.count(args[1])) rs = registers[args[1]];
        } 
        // 3 tham số (ADD Rd, Rs, Rt) -> Rd=Arg0, Rs=Arg1, Rt=Arg2
        else if (args.size() == 3) {
            if (registers.count(args[0])) rd = registers[args[0]];
            if (registers.count(args[1])) rs = registers[args[1]];
            if (registers.count(args[2])) rt = registers[args[2]];
        }
        binaryOutput = inst.opcode + rs + rt + rd + inst.funct;
    } 
    // Xử lý RRI: Op(3) Rs(3) Rt(3) Imm(7)
    // Assembly: ADDI Rs, Rt, Imm -> Rs=Arg0, Rt=Arg1
    else if (inst.type == RRI) {
        string rs = "000", rt = "000", imm = "0000000";
        if (args.size() >= 3) {
            if (registers.count(args[0])) rs = registers[args[0]];
            if (registers.count(args[1])) rt = registers[args[1]];
            try { imm = intToBinary(stoi(args[2]), 7); } catch(...) {}
        }
        binaryOutput = inst.opcode + rs + rt + imm;
    } 
    // Xử lý RI: Op(3) Rs(3) Imm(10)
    else if (inst.type == RI) {
        string rs = "000", imm = "0000000000";
        if (args.size() >= 2) {
            if (registers.count(args[0])) rs = registers[args[0]];
            try { imm = intToBinary(stoi(args[1]), 10); } catch(...) {}
        }
        binaryOutput = inst.opcode + rs + imm;
    }
    
    return binaryOutput;
}

// --- 6. Main ---
int main() {
    initInstructionSet();

    ifstream inputFile("input.asm");
    if (!inputFile.is_open()) {
        cerr << "Loi: Khong tim thay 'input.asm'!" << endl;
        return 1;
    }

    ofstream hexFile("program.hex");
    if (!hexFile.is_open()) return 1;

    cout << "Dang xu ly 'input.asm' voi ma gia..." << endl;
    cout << "STT | Lenh Goc (hoac Dich) | Binary           | Hex" << endl;
    cout << "----|----------------------|------------------|-----" << endl;

    string line;
    int lineCount = 0;
    
    while (getline(inputFile, line)) {
        if (line.find_first_not_of(" \t\r\n") == string::npos) continue;

        // BƯỚC QUAN TRỌNG: Chuyển mã giả thành mã thật
        string realInstruction = preprocessPseudo(line);

        // Xử lý mã thật
        string binCode = processInstruction(realInstruction);
        string hexCode = (binCode.substr(0, 5) == "Error") ? "ERR" : binaryToHex(binCode);

        if (hexCode != "ERR") {
            cout << left << setw(4) << ++lineCount << "| " 
                 << setw(21) << line // In dòng gốc để dễ theo dõi
                 << "| " << binCode << " | " << hexCode << endl;
            hexFile << hexCode << endl;
        } else {
            cout << left << setw(4) << ++lineCount << "| " 
                 << setw(21) << line << "| LOI CU PHAP" << endl;
        }
    }

    inputFile.close();
    hexFile.close();
    cout << "----------------------------------------------------" << endl;
    cout << "Xong! 'program.hex' da duoc cap nhat." << endl;

    return 0;
}   