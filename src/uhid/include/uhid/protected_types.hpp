#pragma once
#include <functional>
#include <atomic>
#include <inputtino/input.hpp>
#include <optional>
#include <mutex>
#include <thread>
#include <uhid/ps5.hpp>
#include <uhid/uhid.hpp>

namespace inputtino {
struct PS5JoypadState {
  std::shared_ptr<uhid::Device> dev;
  int gadget_fd = -1;
  int gadget_slot = -1;
  /**
   * This will be the MAC address of the device
   *
   * IMPORTANT: this needs to be unique for each virtual device,
   * otherwise the kernel driver will return an error:
   * "Duplicate device found for MAC address XX:XX:XX:XX"
   *
   * We also use this information internally to unique match a device with the
   * /dev/input/devXX files; see get_nodes()
   */
  unsigned char mac_address[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  uint16_t vendor_id;

  uhid::dualsense_input_report current_state = {};
  uint8_t last_touch_id = 0;

  std::optional<std::function<void(int, int)>> on_rumble = std::nullopt;
  std::mutex haptics_mutex;
  std::function<void(const unsigned char *, std::size_t)> on_haptics;
  std::optional<std::function<void(int, int, int)>> on_led = std::nullopt;
  std::optional<std::function<void(const PS5Joypad::TriggerEffect &)>> on_trigger_effect = std::nullopt;
  uint32_t last_left_trigger_event = 0;
  uint32_t last_right_trigger_event = 0;

  std::atomic_bool stop_repeat_thread {false};
  bool is_bluetooth = true;
  std::thread gadget_thread;
};
} // namespace inputtino
