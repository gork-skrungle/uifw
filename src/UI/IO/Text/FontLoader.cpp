#include "FontLoader.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

using namespace ui;
using json = nlohmann::json;

FontData FontLoader::loadFont(const char *imagePath, const char *jsonPath)
{
  FontData fontData;

  // TODO: Load image
  // ...

  // Load JSON
  load_JSON_data(jsonPath, &fontData);

  return fontData;
}

void FontLoader::load_JSON_data(const char *jsonPath, FontData *fontData)
{
  std::ifstream jsonFile(jsonPath);

  if (!jsonFile.is_open()) {
    UI_LOG_MSG("Failed to open JSON file at '%s'", jsonPath);
    return;
  }

  json data = json::parse(jsonFile);

  // Atlas data
  const auto &fontAtlasData = data["atlas"];

  const auto &fontAtlasType = fontAtlasData["type"].get<std::string>();
  const auto &distanceRange = fontAtlasData["distanceRange"].get<uint16_t>();
  const auto &distanceRangeMiddle =
      fontAtlasData["distanceRangeMiddle"].get<uint16_t>();
  const auto &size = fontAtlasData["size"].get<float>();
  const auto &width = fontAtlasData["width"].get<uint16_t>();
  const auto &height = fontAtlasData["height"].get<uint16_t>();
  const auto &yOrigin = fontAtlasData["yOrigin"].get<std::string>();

  FontAtlasType fontAtlasTypeEnum;

  if (fontAtlasType == "msdf") {
    fontAtlasTypeEnum = FontAtlasType_MSDF;
  } else {
    fontAtlasTypeEnum = FontAtlasType_Unknown;
  }

  FontYOrigin yOriginEnum;

  if (yOrigin == "bottom") {
    yOriginEnum = FontYOrigin_Bottom;
  } else if (yOrigin == "middle") {
    yOriginEnum = FontYOrigin_Middle;
  } else {
    yOriginEnum = FontYOrigin_Top;
  }

  fontData->atlas = {
    .type = fontAtlasTypeEnum,
    .distanceRange = distanceRange,
    .distanceRangeMiddle = distanceRangeMiddle,
    .size = size,
    .atlasDimensions = {
      .x = width,
      .y = height,
    },
    .yOrigin = yOriginEnum,
  };

  // Metrics data
  const auto &fontMetricsData = data["metrics"];

  const auto &emSize = fontMetricsData["emSize"].get<uint16_t>();
  const auto &lineHeight = fontMetricsData["lineHeight"].get<float>();
  const auto &ascender = fontMetricsData["ascender"].get<float>();
  const auto &descender = fontMetricsData["descender"].get<float>();
  const auto &underlineY = fontMetricsData["underlineY"].get<float>();
  const auto &underlineThickness =
      fontMetricsData["underlineThickness"].get<float>();

  fontData->metrics = {
    .emSize = emSize,
    .lineHeight = lineHeight,
    .ascender = ascender,
    .descender = descender,
    .underlineY = underlineY,
    .underlineThickness = underlineThickness,
  };

  // Glyph data
  const auto &glyphData = data["glyphs"];
  fontData->glyphs.resize(glyphData.size());

  for (const auto &glyph : glyphData) {
    const auto &index = glyph["index"].get<uint16_t>();
    const auto &advance = glyph["advance"].get<float>();

    if (!glyph.contains("planeBounds") || glyph["planeBounds"].is_null() ||
        !glyph.contains("atlasBounds") || glyph["atlasBounds"].is_null()) {
      continue;
    }

    const auto &planeBounds = glyph["planeBounds"];
    const auto &atlasBounds = glyph["atlasBounds"];

    fontData->glyphs[index] = {
      .advance = advance,
      .planeBounds = {
        .left = planeBounds["left"].get<float>(),
        .bottom = planeBounds["bottom"].get<float>(),
        .right = planeBounds["right"].get<float>(),
        .top = planeBounds["top"].get<float>(),
      },
      .atlasBounds = {
        .left = atlasBounds["left"].get<float>(),
        .bottom = atlasBounds["bottom"].get<float>(),
        .right = atlasBounds["right"].get<float>(),
        .top = atlasBounds["top"].get<float>(),
      },
    };
  }
}