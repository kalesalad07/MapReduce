#pragma once

template<typename K, typename V>
class Emitter {
public:
    virtual ~Emitter() = default;

    virtual void emitIntermediate(K key, V value) = 0;
    virtual void emit(K key, V value) = 0;
};