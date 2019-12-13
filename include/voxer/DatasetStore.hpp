#pragma once
#include <array>
#include <map>
#include <memory>
#include <simdjson/jsonparser.h>
#include <string>
#include <voxer/Dataset.hpp>
#include <voxer/scene/SceneDataset.hpp>

namespace voxer {

class DatasetStore {
  using TimestepLookUpTable = std::vector<uint32_t>;
  using VariableLookUpTable = std::map<std::string, TimestepLookUpTable>;

public:
  void load_from_file(const std::string &filepath);
  void load_from_json(const char *json, uint32_t size);
  auto get(const std::string &name, const std::string &variable = "",
           uint32_t timestep = 0) const -> const voxer::Dataset &;
  auto get(const SceneDataset &scene_dataset) const -> const voxer::Dataset &;
  [[nodiscard]] auto print() const -> std::string;

private:
  simdjson::ParsedJson pj;
  std::vector<voxer::Dataset> datasets;
  std::map<std::string, VariableLookUpTable> lookup_table;
};

} // namespace voxer
