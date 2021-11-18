#ifndef UTIL_CALLBACK_SIGNAL_H
#define UTIL_CALLBACK_SIGNAL_H

#include <functional>
#include <vector>

#define CB_SIGNAL(cbName, ...) \
public:                    \
    using cbName##Cb = std::function<void(__VA_ARGS__)>; \
    void on##cbName(cbName##Cb cb) { m_##cbName##s.push_back(cb); } \
protected: \
    template<class ...Types> void emit##cbName(Types&&... args) { for(auto& cb : m_##cbName##s) cb(std::forward<Types>(args)...); } \
private: \
    std::vector<cbName##Cb> m_##cbName##s; \
public:


#endif