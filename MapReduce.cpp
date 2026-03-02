#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

class MapReduce {
    private:
        std::map<std::string, std::vector<std::string>> intermediate;
        std::map<std::string, std::string> result;

        std::string filePath;



    public:
        MapReduce(std::string filePath){
            this->filePath = filePath;
        }

        virtual void map(std::string key, std::string value);

        virtual void reduce(std::string key, std::vector<std::string> values);

        void emitIntermediate(std::string key, std::string value){
            intermediate[key].push_back(value);
        }

        void emit(std::string key, std::string value){
            result[key] = value;
        }

        void execute(){
            std::ifstream file(filePath);
            std::string line;
            while(std::getline(file, line)){
                map(line, line);
            }
            file.close();

            for(auto it : intermediate){
                reduce(it.first, it.second);
            }

        }
};