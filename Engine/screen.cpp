#include "screen.h"

void Screen::setApplication(std::shared_ptr<Application> application) {
    m_application = application;
}
