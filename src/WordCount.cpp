#include "mapreduce/MapReduce.hpp"
#include "mapreduce/SequentialExecutor.hpp"
#include "mapreduce/Emitter.hpp"
#include <string>

class WordCounter: public MapReduce<std::string, int> {
    public:
        void map(std::string key, std::string value) override{
            std::string word;
            for(char c : value){
                if(c == ' '){
                   emitIntermediate(word, 1);
                    word = "";
                } else {
                    word += c;
                }
            }
            if(word != ""){
                emitIntermediate(word, 1);
            }
        }

        void reduce(std::string key, std::vector<int> values) override{
            emit(key, values.size());
        }
};

int main(){
    WordCounter wc;
    SequentialExecutor<std::string, int> executor(wc);
    executor.execute("../resources/pg-being_ernest.txt");
    executor.showResult();
}