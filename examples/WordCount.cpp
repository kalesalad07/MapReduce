#include "mapreduce/MapReduce.hpp"
#include <string>

class WordCounter: public MapReduce {
    public:
        WordCounter(std::string filePath): MapReduce(filePath){}

        void map(std::string key, std::string value) override{
            std::string word;
            for(char c : value){
                if(c == ' '){
                    emitIntermediate(word, "1");
                    word = "";
                } else {
                    word += c;
                }
            }
            if(word != ""){
                emitIntermediate(word, "1");
            }
        }

        void reduce(std::string key, std::vector<std::string> values) override{
            emit(key, std::to_string(values.size()));
        }
};

int main(){
    WordCounter wc("../resources/pg-being_ernest.txt");
    wc.execute();
    wc.showResult();
    return 0;
}