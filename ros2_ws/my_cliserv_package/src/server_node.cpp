#include "my_cliserv_package_interfaces/srv/add.hpp"
#include "rclcpp/rclcpp.hpp"

#include <memory>

void add(const std::shared_ptr<my_cliserv_package_interfaces::srv::Add::Request>
             request,
         std::shared_ptr<my_cliserv_package_interfaces::srv::Add::Response>
             response) {
  response->sum = request->a + request->b;
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"),
              "Incoming request\na: %ld"
              " b: %ld",
              request->a, request->b);
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending back response: [%ld]",
              (long int)response->sum);
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  std::shared_ptr<rclcpp::Node> node =
      rclcpp::Node::make_shared("add_two_ints_server");
  rclcpp::Service<my_cliserv_package_interfaces::srv::Add>::SharedPtr service =
      node->create_service<my_cliserv_package_interfaces::srv::Add>(
          "add_two_ints", &add);
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to add two ints.");
  rclcpp::spin(node);
  rclcpp::shutdown();
}