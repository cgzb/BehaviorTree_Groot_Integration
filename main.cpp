#include <QApplication>
#include <thread>
#include <chrono>
#include <iostream>
#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h"
#include "behaviortree_cpp_v3/action_node.h"
#include "custom_window.h"

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

void behaviorTreeThread()
{
    BT::BehaviorTreeFactory factory;

    factory.registerNodeType<DummyAction>("DummyAction");
    factory.registerNodeType<SleepAction>("Sleep");

    auto tree = factory.createTreeFromText(xml_text);

    // Connect to Groot (v1) via ZMQ
    // The publisher must be kept alive while the tree is running.
    // The ZMQ publisher opens a server socket.
    // Groot will connect to this socket.
    BT::PublisherZMQ publisher(tree);

    std::cout << "BehaviorTree v3 thread started." << std::endl;

    while(true)
    {
        tree.tickRoot();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Start Behavior Tree in a separate thread
    // Note: ZMQ publisher might not be thread safe if accessed from another thread
    // but here the publisher and the tree tick are in the same thread (bt_thread).
    // The Qt loop is in the main thread.
    // Groot (the client) connects via socket, so it's inter-process or inter-thread safe via ZMQ.
    std::thread bt_thread(behaviorTreeThread);
    bt_thread.detach();

    // Create and show the main window with Groot embedded
    CustomWindow window;
    window.show();

    std::cout << "Qt Application started. Groot embedded." << std::endl;

    return app.exec();
}
