#include "geometry_msgs/msg/point_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

#include <chrono>

using namespace std::chrono_literals;

class SimpleTfListener : public rclcpp::Node {
public:
  SimpleTfListener()
      : Node("my_tf_listener") {

    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener1_ = std::make_unique<tf2_ros::TransformListener>(*tf_buffer_);
    point_publisher_ = this->create_publisher<geometry_msgs::msg::PointStamped>("/Points", 10);

    // call tf-data every 10ms
    timer_ = this->create_wall_timer(
        100ms, std::bind(&SimpleTfListener::timer_callback, this));
  }

private:
  void timer_callback() {
    geometry_msgs::msg::PointStamped p;
    // p.header.stamp = this->get_clock()->now();
    p.header.frame_id = "frame_2";

    p.point.x = 1;
    p.point.y = 0;
    p.point.z = 0;

    auto t = tf_buffer_->transform(p,"world");

    RCLCPP_INFO(this->get_logger(), "Transform X: %lf",
                t.point.x);
    RCLCPP_INFO(this->get_logger(), "Transform Y: %lf",
                t.point.y);
    RCLCPP_INFO(this->get_logger(), "Transform Z: %lf",
                t.point.z);
    RCLCPP_INFO(this->get_logger(), "--------------------");

    point_publisher_->publish(t);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  std::unique_ptr<tf2_ros::TransformListener> tf_listener1_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_publisher_;

};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SimpleTfListener>());
  rclcpp::shutdown();
  return 0;
}