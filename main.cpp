#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#define FILENAME "person.json"

using namespace rapidjson;
using namespace std;

struct Single
{

  public:
    Single(string conf, vector<string> emals) : conf(conf)
    {
        this->emails = emails;
    };

  private:
    string conf;
    vector<string> emails;
};

int main()
{
    // 1. Parse a JSON string into DOM.
    // const char *json = "{\"project\":\"rapidjson\",\"stars\":10}";
    // Document d;
    // d.Parse(json);

    // // 2. Modify it by DOM.
    // Value &s = d["stars"];
    // s.SetInt(s.GetInt() + 1);

    // // 3. Stringify the DOM
    // StringBuffer buffer;
    // Writer<StringBuffer> writer(buffer);
    // d.Accept(writer);

    // // Output {"project":"rapidjson","stars":11}
    // std::cout << buffer.GetString() << std::endl;

    std::ifstream file("../person.json");

    vector<Document *> data;

    map<string, vector<string>> obj;
    if (file.is_open())
    {
        std::string line;
        Document *l = NULL;
        while (getline(file, line))
        {
            l = new Document();
            (*l).Parse(line.c_str());
            data.push_back(l);

            // using printf() in all tests for consistency
            //   printf("%s", line.c_str());
        }
        file.close();
        StringBuffer buffer2;
        Writer<StringBuffer> writer2(buffer2);
        (*data[0]).Accept(writer2);
        // printf("%s", buffer2.GetString());
        // printf("%s", (*data[1]).GetString());

        Document *document = data[0];
        // static const char *kTypeNames[] =
        //     {"Null", "False", "True", "Object", "Array", "String", "Number"};
        // for (Value::ConstMemberIterator itr = (*data[0]).MemberBegin();
        //      itr != (*data[0]).MemberEnd(); ++itr)
        // {
        //     printf("Type of member %s is %s\n",
        //            itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        // }

        Value::ConstMemberIterator itr = document->FindMember("tag");
        if (itr != document->MemberEnd())
            printf("%s\n", itr->value.GetString());

        vector<Single *> singles;
        Single *s = NULL;

        for (int i = 0; i < data.size(); i++)
        {
            Document *doc = data[i];
            string currentConf = doc->FindMember("confName")->value.GetString();
            // for (Value::ConstValueIterator itr = (*doc)["emails"].Begin(); itr != (*doc)["emails"].End(); ++itr)
            //     printf("%d ", itr->GetInt());
            // s = new Single(doc->FindMember("confName")->value.GetString(), doc->FindMember("emails"));

            for (rapidjson::SizeType k = 0; k < (*doc)["emails"].Size(); k++)
            {
                const Value &c = (*doc)["emails"][k];
                obj[currentConf].push_back(c.GetString());
                // printf("%s \n", c.GetString());
            }
        }
    }

    for (auto const &x : obj)
    {
        std::cout << x.first // string (key)
                  << ':'
                  << x.second[0] // string's value
                  << std::endl;

        ofstream outFile("../emails/" + x.first + ".txt");
        // the important part
        for (const auto &e : x.second)
            outFile << e << "\n";
    }
    return 0;
}
