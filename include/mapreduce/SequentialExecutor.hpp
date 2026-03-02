#pragma once
#include "MapReduce.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

template <typename K, typename V>
class SequentialEmitter : public Emitter<K,V> {
    public:
    std::map<K, std::vector<V>> intermediate;
    std::map<K,V> result;

    void emitIntermediate(K key, V value) override {
        intermediate[key].push_back(value);
    }
    void emit(K key, V value) override {
        result[key] = value;
    }
};


template <typename K, typename V>
class SequentialExecutor{
public:
    SequentialEmitter<K,V> emitter;
    MapReduce<K,V>* mr;

    SequentialExecutor(MapReduce<K,V>& mr){
        this->mr = &mr;
        mr.setEmitter(&emitter);
    }

    void execute(const std::string& filePath){        
        std::ifstream file(filePath);
        if(!file.is_open()){
            std::cerr << "Error opening file: " << filePath << std::endl;
            return;
        }

        //Map Phase
        std::string line;
        while(std::getline(file,line)){
            mr->map(filePath, line);
        }

        //Reduce Phase
        for(auto const& it: emitter.intermediate){
            mr->reduce(it.first, it.second);
        }
    }

    void showResult(){
        for(auto it : emitter.result){
            std::cout << it.first << ": " << it.second << std::endl;
        }
    }
};