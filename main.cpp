#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h"
#include "behaviortree_cpp_v3/action_node.h"
#include <iostream>
#include <chrono>
#include <thread>

// Simple sleep action for v3
class SleepAction : public BT::SyncActionNode
{
public:
    SleepAction(const std::string& name, const BT::NodeConfiguration& config)
        : BT::SyncActionNode(name, config)
    {}

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<int>("msec") };
    }

    BT::NodeStatus tick() override
    {
        int msec = 0;
        if (!getInput("msec", msec))
        {
             throw BT::RuntimeError("missing required input [msec]");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
        return BT::NodeStatus::SUCCESS;
    }
};

class DummyAction : public BT::SyncActionNode
{
public:
    DummyAction(const std::string& name, const BT::NodeConfiguration& config)
        : BT::SyncActionNode(name, config)
    {}

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("message") };
    }

    BT::NodeStatus tick() override
    {
        std::string msg;
        if (!getInput("message", msg))
        {
            throw BT::RuntimeError("missing required input [message]");
        }
        std::cout << "DummyAction: " << msg << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

static const char* xml_text = R"(
 <root main_tree_to_execute = "MainTree">
     <BehaviorTree ID="MainTree">
        <Sequence>
            <DummyAction message="Hello"/>
            <DummyAction message="World"/>
            <Sleep msec="1000"/>
        </Sequence>
     </BehaviorTree>
 </root>
 )";

int main()
{
    BT::BehaviorTreeFactory factory;

    factory.registerNodeType<DummyAction>("DummyAction");
    factory.registerNodeType<SleepAction>("Sleep");

    auto tree = factory.createTreeFromText(xml_text);

    // Connect to Groot (v1) via ZMQ
    // Default ports are usually used by the publisher
    BT::PublisherZMQ publisher(tree);

    std::cout << "BehaviorTree v3 started. Connect with Groot to visualize." << std::endl;

    while(true)
    {
        tree.tickRoot();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
