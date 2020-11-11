#ifndef QT_ADAPT_START_QT_H
#define QT_ADAPT_START_QT_H

namespace base{ struct Base; }

namespace uiadapter::qt
{
int startQt(base::Base& base, int& argc, char**& argv);
} // namespace uiadapter::qt

#endif