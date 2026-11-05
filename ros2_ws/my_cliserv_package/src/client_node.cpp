#include "my_cliserv_package_interfaces/srv/add.hpp"
#include "rclcpp/rclcpp.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  if (argc != 3) {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),
                 "Usage: add_two_ints_client X Y");
    rclcpp::shutdown();
    return 1;
  }

  auto node = rclcpp::Node::make_shared("add_two_ints_client");

  using Add = my_cliserv_package_interfaces::srv::Add;

  auto client = node->create_client<Add>("add_two_ints");

  auto request = std::make_shared<Add::Request>();
  request->a = std::atoll(argv[1]);
  request->b = std::atoll(argv[2]);

  // Warten, bis der Service verfügbar ist
  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),
                   "Interrupted while waiting for service.");
      rclcpp::shutdown();
      return 1;
    }

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
                "Service not available, waiting again...");
  }

  // Anfrage senden
  auto result = client->async_send_request(request);

  // Auf Antwort warten
  if (rclcpp::spin_until_future_complete(node, result) ==
      rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %ld",
                static_cast<long>(result.get()->sum));
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),
                 "Failed to call service add_two_ints");
  }

  rclcpp::shutdown();
  return 0;
}
