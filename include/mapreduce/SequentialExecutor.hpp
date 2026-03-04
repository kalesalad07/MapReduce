#pragma once
#include "MapReduce.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <filesystem>


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

    std::vector<std::string> getFilePaths(const std::string& filePath){
        std::vector<std::string> paths;

        if(std::filesystem::exists(filePath) && std::filesystem::is_directory(filePath)){
            for(const auto& entry: std::filesystem::directory_iterator(filePath)){
                if(entry.is_regular_file()){
                    paths.push_back(entry.path().string());
                }
            }
        }
        return paths;
    }

    void execute(const std::string& filePath){
        auto files = getFilePaths(filePath);
        
        for(auto fileName: files){
            std::ifstream file(fileName);
            if(!file.is_open()){
                std::cerr << "Error opening file: " << fileName << std::endl;
                return;
            }
            //Map Phase
            std::string line;
            while(std::getline(file,line)){
                mr->map(fileName, line);
            }
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