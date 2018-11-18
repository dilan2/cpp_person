#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#define FILENAME "person.json"

using namespace rapidjson;
using namespace std;

void saveToFiles(const map<string, vector<string>> *obj) {
  for (auto const &x : (*obj)) {
    ofstream outFile("../emails/" + x.first + ".txt");
    for (const auto &e : x.second)
      outFile << e << "\n";
  }
}

void putFileDataInVector(const string path, vector<Document *> *data) {
  std::ifstream file("../person.json");

  if (file.is_open()) {
    string line;
    Document *l = NULL;
    while (getline(file, line)) {
      l = new Document();
      l->Parse(line.c_str());
      data->push_back(l);
    }
    file.close();
  }
}

map<string, vector<string>> parseVectorToMap(const vector<Document *> *data) {

  map<string, vector<string>> obj;
  for (int i = 0; i < data->size(); i++) {
    Document *doc = (*data)[i];
    string currentConf = doc->FindMember("confName")->value.GetString();
    for (rapidjson::SizeType k = 0; k < (*doc)["emails"].Size(); k++) {
      const Value &c = (*doc)["emails"][k];
      obj[currentConf].push_back(c.GetString());
    }
  }

  return obj;
}

int main() {
  std::ifstream file("../person.json");

  vector<Document *> data;
  putFileDataInVector("../person", &data);

  map<string, vector<string>> obj = parseVectorToMap(&data);
  saveToFiles(&obj);

  return 0;
}
