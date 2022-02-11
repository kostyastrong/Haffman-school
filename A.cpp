#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <queue>
#include <string>

struct Node {
    Node* l = nullptr, *r = nullptr;
    char symbol = '#';
    int val;
    Node(Node* _l, Node* _r) {
        val = _l->val + _r->val;
        l = _l;
        r = _r;
    }
    Node(char _a, int _val = -1) {
        val = _val;
        symbol = _a;
    }

    bool leaf() const {
        return l == nullptr;
    }

    Node() {}
};

struct comp{
    bool operator()(const Node*a, const Node*b) {
        return a->val > b->val;  // boolshit imho
    }
};

class HaffNode{
public:
    Node* root_;
    std::unordered_map<char, std::string> codes;


    void gener(Node* cur, std::string code) {
        if (cur->leaf()) {
            codes[cur->symbol] = code;
            if (code == "") {
                codes[cur->symbol] = "0";
            }
        } else {
            gener(cur->l, code + '0');
            gener(cur->r, code + '1');
        }
    }
    HaffNode(std::string& input) {
        std::unordered_map<char, int> met;
        for (char& i : input) {
            met[i] += 1;
        }

        std::priority_queue<Node*, std::vector<Node*>, comp> el;
        for (auto& [dc, di] : met) {
            el.push(new Node(dc, di));
        }

        while (el.size() > 1) {
            Node*l = el.top();
            el.pop();
            Node*r = el.top();
            el.pop();
            Node* add = new Node(l, r);
            el.push(add);
        }

        root_ = el.top();
        gener(root_, "");
    }

    HaffNode(int _, const std::string& fileName) {
        std::ifstream stream;
        stream.open(fileName);
        if (! stream) {
            std::cout << "no file to create a Haffman tree";
            exit(0);
        }
        root_ = readNode(stream);
        gener(root_, "");
    }

    void writeTree(const std::string& fileName) {
        std::ofstream* out = new std::ofstream;
        out->open(fileName);  // maybe with , std::ofstream::out | std::ofstream::trunc
        writeNode(out, root_);
        out->close();
        delete out;
    }

    void eraseTree(Node* cur) {
        if (cur->leaf()) {
            delete cur;
        } else {
            eraseTree(cur->l);
            eraseTree(cur->r);
        }
    }
private:
    Node* readNode(std::ifstream& stream) {
        std::string input;
        std::getline(stream, input);
        if (input[0] == '1') {
            Node* cur = new Node;
            cur->l = readNode(stream);
            cur->r = readNode(stream);
            return cur;
        } else {
            char letter = '\n';
            if (input.size() > 2) {
                letter = input[2];
            } else {
                std::getline(stream, input);
            }
            return new Node(letter);
        }
    }

    void writeNode(std::ofstream* out, const Node* cur) {
        if (!cur->leaf()) {
            *out << "1\n";
            writeNode(out, cur->l);
            writeNode(out, cur->r);
        } else {
            (*out) << "0 " << cur->symbol << "\n";
        }
    }
};

void archiveString(std::string input, const std::string& fileNameString, const std::string& fileNameTree) {
    std::cout << "Please, buy winRAR license\n";
    HaffNode* tree = new HaffNode(input);
    std::string out;
    tree->writeTree(fileNameTree);
    tree->eraseTree(tree->root_);
    tree = new HaffNode(1, fileNameTree);
    for (char i : input) {
        out += tree->codes[i];
    }
    std::ofstream myFile;
    myFile.open(fileNameString);
    myFile << out;

    myFile.close();
}

std::string unarchive(const std::string& decSin, const std::string& fileTree) {
    HaffNode* tree = new HaffNode(0, fileTree);
    Node* cur = tree->root_;
    int ind = 0;
    std::string decSout = "";
    while (ind < decSin.size()) {
        while (! cur->leaf()) {
            if (decSin[ind] == '0') {
                cur = cur->l;
            } else {
                cur = cur->r;
            }
            ++ind;
        }
        if (cur == tree->root_) {
            ++ind;
        }
        decSout += cur->symbol;
        cur = tree->root_;
    }

    return decSout;
}

std::string readString(const std::string& fileName) {
    std::string out;
//    std::freopen("message.txt", "r", stdin);
//    std::cin >> out;
    std::ifstream stream;
    stream.open(fileName);
    std::getline(stream, out);
    return out;
}

std::string readFile(const std::string& fileName) {
    std::string out;
    std::ifstream stream;
    stream.open(fileName);
    std::string now;
    while (std::getline(stream, now)) {
        out += now + '\n';
    }
    return out;
}
signed main() {
    std::string fileString = "message.txt";
    std::string outCrypted = "cryptedMessage.txt";
    std::string tree = "tree.txt";
    archiveString(readFile(fileString), outCrypted, tree);
    std::cout << unarchive(readString(outCrypted), tree);
}
