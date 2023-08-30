#include "selector.h"

Selector::Selector()
{

}

Status Selector::update(float deltaTime) {
    for (std::shared_ptr<BTNode> child: m_children) {
        Status childStatus = child->update(deltaTime);
        if (childStatus == Status::RUNNING) {
            if (m_lastRunning != nullptr)
                m_lastRunning->reset();
            m_lastRunning = child;
            return Status::RUNNING;
        }
        else if (childStatus == Status::SUCCESS) {
            return Status::SUCCESS;
        }
    }
    m_lastRunning = nullptr;
    return Status::FAILURE;
}

void Selector::reset() {
    for (std::shared_ptr<BTNode> child: m_children) {
        child->reset();
    }
}
