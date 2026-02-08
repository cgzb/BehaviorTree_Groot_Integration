#pragma once

#include "behaviortree_cpp/action_node.h"
#include <iostream>

class DummyAction : public BT::SyncActionNode
{
public:
    DummyAction(const std::string& name, const BT::NodeConfig& config)
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
