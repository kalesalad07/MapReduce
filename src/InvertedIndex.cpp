#include "mapreduce/MapReduce.hpp"
#include "mapreduce/SequentialExecutor.hpp"
#include <string>
#include <vector>
#include <set>
#include <algorithm>

class InvertedIndex: public MapReduce<std::string, std::string> {
    public:
        void map(std::string key, std::string value) override{
            std::string word;
            std::set<std::string> uniqueWord;
            for(char c: value){
                if(c == ' '){
                    if(uniqueWord.find(word) == uniqueWord.end()){
                        emitIntermediate(word, key);
                    }else{
                        uniqueWord.insert(word);
                    }
                    
                    word = "";
                }
                else{
                    word += c;
                }
            }
            if(word != ""){
                if(uniqueWord.find(word) == uniqueWord.end()){
                    emitIntermediate(word, key);
                }
            }
        }

        void reduce(std::string key, std::vector<std::string> values) override{
            std::sort(values.begin(), values.end());
            for(auto it: values) emit(key, it);            
        }
};

int main(){
    InvertedIndex ii;
    SequentialExecutor<std::string, std::string> executor(ii);
    executor.execute("../resources/word_count");
    executor.showResult();
}