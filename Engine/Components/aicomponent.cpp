#include "aicomponent.h"

AIComponent::AIComponent()
{

}

void AIComponent::setBehaviorTree(std::shared_ptr<BTNode> behaviorTree) {
    m_behaviorTree = behaviorTree;
}


std::shared_ptr<BTNode> AIComponent::getBehaviorTree() {
    return m_behaviorTree;
}
