#include "voxer/encoders/Encoder.hpp"
#define TJE_IMPLEMENTATION
#include "third_party/tiny_jpeg.h"
#include "voxer/utils/Debugger.hpp"
#include <string>
#include <vector>

using namespace std;
using ospcommon::vec2ui;

static Debugger debug("encoder");

void _encode(void *context, void *data, int size) {
  auto buf = (vector<unsigned char> *)context;
  auto res = (unsigned char *)data;
  for (auto i = 0; i < size; i++) {
    buf->push_back(*(res + i));
  }
}

vector<unsigned char> Encoder::encode(vector<unsigned char> &data, vec2ui dim,
                                      string format, bool isRGBA) {
  if (format != "JPEG") {
    throw string("format not suppported!");
  }

  auto start = chrono::steady_clock::now();

  vector<unsigned char> img;
  auto numComponents = isRGBA ? 4 : 3;
  img.reserve(dim.x * dim.y * numComponents);
  auto quality = dim.x == 64 ? 1 : 3;
  tje_encode_with_func(_encode, &img, quality, dim.x, dim.y, numComponents,
                       data.data());

  const auto delta = chrono::duration_cast<chrono::milliseconds>(
      chrono::steady_clock::now() - start);
  debug.log(to_string(delta.count()) + " ms");

  return img;
}