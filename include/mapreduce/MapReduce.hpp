#pragma once
#include "Emitter.hpp"
#include <string>
#include <vector>

template <typename K, typename V>
class MapReduce {
private:
    Emitter<K,V>* emitter = nullptr;
public:
    virtual ~MapReduce() = default;

    void setEmitter(Emitter<K,V>* emitter){
        this->emitter = emitter;
    }

    void emitIntermediate(K key, V value){
        if(emitter){
            emitter->emitIntermediate(key, value);
        }
    }
    void emit(K key, V value){
        if(emitter){
            emitter->emit(key, value);
        }
    }

    virtual void map(std::string key, std::string value) = 0;
    virtual void reduce(K key, std::vector<V> values) = 0;
};