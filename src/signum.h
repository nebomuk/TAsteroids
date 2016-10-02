#ifndef SIGNUM_H
#define SIGNUM_H

#include <QtGlobal>

inline int signum(qreal value)
{
	return qFuzzyCompare(value,0.0) ? 0 : qRound(value/qAbs(value));
}

#endif // SIGNUM_H
