#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QObject>

#define QCPTEST(t) t t##_inst; QTest::qExec(&t##_inst);

class TestRunner : public QObject
{
  Q_OBJECT
public:
  explicit TestRunner() : QObject(0) {}
public slots:
  void run();
};

#endif // TESTRUNNER_H
