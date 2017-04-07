#include "ocrmodule.h"
#include <stdexcept>
#include <leptonica/allheaders.h>

LeptonicaOCRModule::LeptonicaOCRModule(const std::string& lang) : m_language{lang}, m_ocrApi{new tesseract::TessBaseAPI}
{
    initOcrApi();
}

LeptonicaOCRModule::LeptonicaOCRModule(const LeptonicaOCRModule& oth) : m_language{oth.m_language}, m_ocrApi{new tesseract::TessBaseAPI}
{
    initOcrApi();
}

LeptonicaOCRModule& LeptonicaOCRModule::operator=(const LeptonicaOCRModule& oth)
{
    if (this != &oth)
    {
        tesseract::TessBaseAPI* tmpApi {new tesseract::TessBaseAPI};
        if (tmpApi->Init(nullptr, oth.m_language.c_str()))
        {
            throw std::runtime_error("Could not initialize tesseract API.");
        }
        deinitOcrApi();
        m_language = oth.m_language;
        m_ocrApi.reset(tmpApi);
    }
    return *this;
}

LeptonicaOCRModule::~LeptonicaOCRModule()
{
    deinitOcrApi();
}

const std::string LeptonicaOCRModule::processImage(const std::string &filepath, const Config* config) const
{
    Pix* originalImage = pixRead(filepath.data());
    Pix* binarizedImage = preprocessImage(originalImage, config);

    m_ocrApi->SetImage(binarizedImage);
    char* text = m_ocrApi->GetUTF8Text();
    if (text == nullptr)
    {
        pixDestroy(&binarizedImage);
        pixDestroy(&originalImage);
        return std::string{};
    }
    pixDisplay(binarizedImage, binarizedImage->w, binarizedImage->h);
    std::string ret{text};

    delete[] text;
    pixDestroy(&binarizedImage);
    pixDestroy(&originalImage);
    return ret;
}

void LeptonicaOCRModule::initOcrApi()
{
    if (m_ocrApi->Init(nullptr, m_language.c_str()))
    {
        throw std::runtime_error("Could not initialize tesseract API.");
    }
}

void LeptonicaOCRModule::deinitOcrApi()
{
    if (m_ocrApi)
    {
        m_ocrApi->End();
    }
}

Pix* LeptonicaOCRModule::preprocessImage(Pix *originalImage, const Config* /* unused */) const
{
    uint8_t tileSize = 10;
    uint8_t fgThreshold = 120;
    uint8_t bgThreshold = 255;
    uint8_t kernel = 5;

    Pixa* pixa = pixaCreate(5);
    pixSaveTiled(originalImage, pixa, 1.0, 1, 50, 32);

    int depth = pixGetDepth(originalImage);
    Pix* grayscaleImage = nullptr;
    if (depth == 32)
    {
        grayscaleImage = pixConvertRGBToGray(originalImage, 0.2, 0.7, 0.1);  // Standard weights for R, G and B
    }
    else
    {
        grayscaleImage = pixConvertTo8(originalImage, 0);
    }

    Pix* backgroundNormImage = pixBackgroundNorm(grayscaleImage, nullptr, nullptr, tileSize,
                                                 tileSize, fgThreshold, 50, bgThreshold, kernel, kernel);
    Pix* binarizedImage = pixThresholdToBinary(backgroundNormImage, 180);
    pixSaveTiled(binarizedImage, pixa, 1.0, 1, 50, 32);

    pixDestroy(&backgroundNormImage);
    pixDestroy(&grayscaleImage);
    pixaDestroy(&pixa);
    return binarizedImage;
}

// TODO: Add this to the preprocessImage() function:
//    if (config != nullptr)
//    {
//        const Config& cfg = *config;
//        for (auto&& keyValue : cfg)
//        {
//            const char* key = keyValue.first.data();
//            if (strcmp(key, ConfigTileSize))
//            {
//                tileSize = static_cast<uint8_t>(keyValue.second);
//            }
//            else if (strcmp(key, ConfigForegroundThreshold))
//            {
//                fgThreshold = static_cast<uint8_t>(keyValue.second);
//            }
//            else if (strcmp(key, ConfigBackgroundThreshold))
//            {
//                bgThreshold = static_cast<uint8_t>(keyValue.second);
//            }
//            else if (strcmp(key, ConfigKernel))
//            {
//                kernel = static_cast<uint8_t>(keyValue.second);
//            }
//        }
//    }
