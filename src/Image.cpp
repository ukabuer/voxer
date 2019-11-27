#include <voxer/Image.hpp>
#define TJE_IMPLEMENTATION
#include "third_party/tiny_jpeg.h"
#include "utils/Debugger.hpp"
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace voxer {
  
static Debugger debug("encoder");

static void _encode(void *context, void *data, int size) {
  auto buf = reinterpret_cast<vector<unsigned char> *>(context);
  auto res = (unsigned char *)data;
  for (auto i = 0; i < size; i++) {
    buf->push_back(*(res + i));
  }
}

Image Image::encode(uint8_t *data, uint32_t width, uint32_t height,
                    uint8_t channels, Image::Format format,
                    Image::Quality quality) {
  if (format != Image::Format::JPEG) {
    throw domain_error("format not suppported!");
  }

  auto start = chrono::steady_clock::now();

  Image image{width, height, channels, format};
  image.data.reserve(width * height * channels);

  auto qualityValue = static_cast<int>(quality);
  assert(qualityValue >= 0 && qualityValue <= 3);

  tje_encode_with_func(_encode, &image.data, qualityValue, width, height,
                       channels, data);

  const auto delta = chrono::duration_cast<chrono::milliseconds>(
      chrono::steady_clock::now() - start);
  debug.log(to_string(delta.count()) + " ms");

  return image;
}

} // namespace voxer