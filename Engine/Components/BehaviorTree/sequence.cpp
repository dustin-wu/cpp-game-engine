#include "sequence.h"

Sequence::Sequence()
{

}

Status Sequence::update(float deltaTime) {
    for (std::shared_ptr<BTNode> child: m_children) {
        if (m_lastRunning != nullptr && child != m_lastRunning) {
            continue; // resume from previous running child
        }

        Status childStatus = child->update(deltaTime);
        if (childStatus == Status::RUNNING) {
            m_lastRunning = child;
            return Status::RUNNING;
        }
        else if (childStatus == Status::FAILURE) {
            return Status::FAILURE;
        }
    }

    if (m_lastRunning != nullptr)
        m_lastRunning->reset();
    m_lastRunning = nullptr;
    return Status::SUCCESS;
}

void Sequence::reset() {
    for (std::shared_ptr<BTNode> child: m_children) {
        child->reset();
    }
}
