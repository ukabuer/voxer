#pragma once
#include "service/Service.hpp"
#include <string>
#include <voxer/DatasetStore.hpp>

class DatasetService final : public Service {
public:
  void on_message(const char *message, uint32_t size) final;

  [[nodiscard]] auto get_path() const -> std::string final {
    return "/datasets";
  }

  voxer::DatasetStore *m_datasets = nullptr;

private:
  void load_dataset(const char *json, uint32_t size);
};