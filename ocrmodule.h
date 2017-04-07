#ifndef LEPTONICA_OCRMODULE_H
#define LEPTONICA_OCRMODULE_H

#include "iocr.h"
#include <string>
#include <memory>
#include <tesseract/baseapi.h>

struct Pix;  // Forward declaration

class LeptonicaOCRModule : public IOCR
{
public:
    explicit LeptonicaOCRModule(const std::string& lang);
    LeptonicaOCRModule(const LeptonicaOCRModule& oth);
    LeptonicaOCRModule(LeptonicaOCRModule&& oth) = default;
    LeptonicaOCRModule& operator=(const LeptonicaOCRModule& oth);
    LeptonicaOCRModule& operator=(LeptonicaOCRModule&& oth) = default;
    ~LeptonicaOCRModule() override;

    void changeLanguage(const std::string& lang) override;
    const std::string processImage(const std::string& filepath, const Config* config) const override;

private:
    std::string m_language;
    std::unique_ptr<tesseract::TessBaseAPI> m_ocrApi;

    void initOcrApi(const std::string& lang);
    void deinitOcrApi();
    Pix* preprocessImage(Pix* originalImage, const Config* config) const;
};

constexpr char ConfigTileSize[] = "TileSize";
constexpr char ConfigForegroundThreshold[] = "FgThreshold";
constexpr char ConfigBackgroundThreshold[] = "BgThreshold";
constexpr char ConfigKernel[] = "Kernel";

#endif // OCRMODULE_H
