#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"
#include "behaviortree_cpp/actions/sleep_node.h"
#include "dummy_nodes.h"
#include <chrono>
#include <thread>

static const char* xml_text = R"(
 <root BTCPP_format="4" >
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
    // factory.registerNodeType<BT::SleepNode>("Sleep");
    // Note: SleepNode is registered by default in BehaviorTree.CPP v4.

    auto tree = factory.createTreeFromText(xml_text);

    // Connect to Groot2
    // Default port is 1667
    BT::Groot2Publisher publisher(tree);

    std::cout << "BehaviorTree started. Connect with Groot2 to visualize." << std::endl;

    while(true)
    {
        tree.tickOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
