#ifndef LEPTONICA_OCRMODULE_H
#define LEPTONICA_OCRMODULE_H

#include "iocr.h"
#include <string>

namespace tesseract
{
    class TessBaseAPI;  // Forward declaration
}
struct Pix;  // Forward declaration

class LeptonicaOCRModule : public IOCR
{
public:
    LeptonicaOCRModule(const std::string& lang);
    LeptonicaOCRModule(const LeptonicaOCRModule&) = default;
    LeptonicaOCRModule(LeptonicaOCRModule&&) = default;
    LeptonicaOCRModule& operator=(const LeptonicaOCRModule&) = default;
    LeptonicaOCRModule& operator=(LeptonicaOCRModule&&) = default;
    ~LeptonicaOCRModule() override;

    const std::string processImage(const std::string& filepath, const Config* config) const override;

private:
    tesseract::TessBaseAPI* m_ocrApi {nullptr};

    Pix* preprocessImage(Pix* originalImage, const Config* config) const;
};

constexpr char ConfigTileSize[] = "TileSize";
constexpr char ConfigForegroundThreshold[] = "FgThreshold";
constexpr char ConfigBackgroundThreshold[] = "BgThreshold";
constexpr char ConfigKernel[] = "Kernel";

#endif // OCRMODULE_H
