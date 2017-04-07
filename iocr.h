#ifndef IOCR_H
#define IOCR_H

#include <string>
#include <unordered_map>

using Config = std::unordered_map<std::string, double>;

class IOCR
{
public:
    virtual const std::string processImage(const std::string& filepath, const Config* config) const = 0;
};

#endif // IOCR_H
